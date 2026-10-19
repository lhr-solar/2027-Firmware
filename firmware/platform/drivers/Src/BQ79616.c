#include "BQ79616.h"
#include <stdbool.h>
#include <string.h>        // memcpy
#include "FreeRTOS.h"
#include "task.h"          // vTaskDelay

// Most RX bytes a flush will drop: one full stack read's worth
#define BQ_FLUSH_MAX_BYTES  (BQ_MAX_FRAME_LEN * BQ_MAX_RESPONSE_FRAMES)

// Driver state. The s_ prefix marks file-scope statics.
static bq_init_struct_t s_cfg;
static CRC_HandleTypeDef s_hcrc;
static TIM_HandleTypeDef s_htim;
static bool s_initialised = false;

static bq_status_t gpio_clk_enable(GPIO_TypeDef *port)
{
    if (port == GPIOA)      { __HAL_RCC_GPIOA_CLK_ENABLE(); }
    else if (port == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); }
    else if (port == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); }
    else if (port == GPIOD) { __HAL_RCC_GPIOD_CLK_ENABLE(); }
    else if (port == GPIOE) { __HAL_RCC_GPIOE_CLK_ENABLE(); }
    else if (port == GPIOF) { __HAL_RCC_GPIOF_CLK_ENABLE(); }
    else if (port == GPIOG) { __HAL_RCC_GPIOG_CLK_ENABLE(); }
    else return BQ_ERR_PARAM;

    return BQ_OK;
}

static bq_status_t wake_tim_clk_enable(TIM_TypeDef *tim)
{
    if (tim == TIM2)      { __HAL_RCC_TIM2_CLK_ENABLE(); }
    else if (tim == TIM3) { __HAL_RCC_TIM3_CLK_ENABLE(); }
    else if (tim == TIM4) { __HAL_RCC_TIM4_CLK_ENABLE(); }
    else if (tim == TIM5) { __HAL_RCC_TIM5_CLK_ENABLE(); }
    else return BQ_ERR_PARAM;

    return BQ_OK;
}

// Request types, shared by bq_send(), bq_write() and bq_read()
static inline bool is_write_req(bq_req_t req)
{
    return req == BQ_SINGLE_WRITE || req == BQ_STACK_WRITE ||
           req == BQ_BROAD_WRITE  || req == BQ_BROAD_WRITE_REV;
}

static inline bool is_read_req(bq_req_t req)
{
    return req == BQ_SINGLE_READ || req == BQ_STACK_READ || req == BQ_BROAD_READ;
}

static inline bool is_single_req(bq_req_t req)
{
    return req == BQ_SINGLE_READ || req == BQ_SINGLE_WRITE;
}

// Addresses a single read/write can reach: the bridge (0) or a BQ79616 (1..num_devices)
static inline bool dev_exists(uint8_t dev_addr)
{
    return dev_addr <= s_cfg.num_devices;
}

const char *bq_status_str(bq_status_t status)
{
    switch (status) {
        case BQ_OK:           return "BQ_OK";
        case BQ_ERR_PARAM:    return "BQ_ERR_PARAM";
        case BQ_ERR_NOT_INIT: return "BQ_ERR_NOT_INIT";
        case BQ_ERR_HAL:      return "BQ_ERR_HAL";
        case BQ_ERR_BUSY:     return "BQ_ERR_BUSY";
        case BQ_ERR_UART:     return "BQ_ERR_UART";
        case BQ_ERR_TIMEOUT:  return "BQ_ERR_TIMEOUT";
        case BQ_ERR_CRC:      return "BQ_ERR_CRC";
        case BQ_ERR_FRAME:    return "BQ_ERR_FRAME";
        case BQ_ERR_VERIFY:   return "BQ_ERR_VERIFY";
        case BQ_ERR_ADC:      return "BQ_ERR_ADC";
        default:              return "BQ_ERR_UNKNOWN";
    }
}

/* Hardware CRC unit, set up for the BQ frame CRC: CRC-16-IBM (poly 0x8005, init 0xFFFF,
 * reflected in and out), BQ79600 §7.3.2.1.1.1 p17 */
static bq_status_t crc_init(void)
{
    __HAL_RCC_CRC_CLK_ENABLE();

    s_hcrc.Instance                     = CRC;
    s_hcrc.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
    s_hcrc.Init.GeneratingPolynomial    = 0x8005U;
    s_hcrc.Init.CRCLength               = CRC_POLYLENGTH_16B;
    s_hcrc.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;
    s_hcrc.Init.InitValue               = 0xFFFFU;
    s_hcrc.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_BYTE;
    s_hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
    s_hcrc.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

    if (HAL_CRC_Init(&s_hcrc) != HAL_OK) {
        return BQ_ERR_HAL;
    }

    return BQ_OK;
}

/* CRC over len bytes. Running it across a whole frame including its CRC
 * gives 0 when the frame is intact (BQ79600 §7.3.2.1.1.2 p17). */
static uint16_t bq_crc16(const uint8_t *buf, uint32_t len)
{
    /* With FORMAT_BYTES, len is in bytes despite the uint32_t* signature */
    return (uint16_t)HAL_CRC_Calculate(&s_hcrc, (uint32_t *)buf, len);
}

/* Drop any stale bytes (noise, the rest of a failed response) so the next response frame
 * starts at its INIT byte. Bounded, so a line that keeps receiving can't hang the caller. */
static void bq_rx_flush(void)
{
    uint8_t discard;

    for (uint16_t i = 0U; i < BQ_FLUSH_MAX_BYTES; i++) {
        if (uart_recv(s_cfg.huart, &discard, 1U, 0U) != UART_OK) {
            break;
        }
    }
}

/* WAKE ping generator. While a ping is sent, tx_pin is switched to this timer channel, which
 * drives the low pulse in hardware: one-pulse mode, PWM mode 2, active low. */
static bq_status_t wake_tim_init(void)
{
    TIM_OC_InitTypeDef oc = {0};
    uint32_t tim_clk = HAL_RCC_GetPCLK1Freq();
    bq_status_t status = wake_tim_clk_enable(s_cfg.wake_tim);

    if (status != BQ_OK) {
        return status;
    }

    /* TODO: the timer clock is changing from 80 MHz to 96 MHz. The prescaler below is worked out
     * from the clock at bq_init() time (79 at 80 MHz, 95 at 96 MHz, both exact), so the ping
     * should stay BQ_WAKE_PING_US long, but check it once the change lands: the new clock must be
     * set up before bq_init(), HAL_RCC_GetPCLK1Freq() must report it, and scope the ping on tx_pin. */

    /* APB1 timer kernel clock is 2x PCLK1 unless the APB1 prescaler is 1 */
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_HCLK_DIV1) {
        tim_clk *= 2U;
    }
    // the prescaler can't make 1 MHz from this clock
    if (tim_clk < BQ_WAKE_TIM_HZ || (tim_clk / BQ_WAKE_TIM_HZ) > 0x10000U) {
        return BQ_ERR_HAL;
    }

    s_htim.Instance               = s_cfg.wake_tim;
    s_htim.Init.Prescaler         = (tim_clk / BQ_WAKE_TIM_HZ) - 1U;
    s_htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
    s_htim.Init.Period            = BQ_WAKE_PING_US;   /* ARR, with CCR = 1 gives BQ_WAKE_PING_US ticks low */
    s_htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    s_htim.Init.RepetitionCounter = 0U;
    s_htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_OnePulse_Init(&s_htim, TIM_OPMODE_SINGLE) != HAL_OK) {
        return BQ_ERR_HAL;
    }

    oc.OCMode     = TIM_OCMODE_PWM2;
    oc.Pulse      = 1U;     /* CCR: one tick of delay so CNT = 0 is the idle-high state */
    oc.OCPolarity = TIM_OCPOLARITY_LOW;
    oc.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&s_htim, &oc, s_cfg.wake_tim_channel) != HAL_OK) {
        return BQ_ERR_HAL;
    }

    /* Enable the channel output now, while tx_pin is still routed elsewhere,
     * so it is already driving high by the time the pin is switched to it. */
    TIM_CCxChannelCmd(s_htim.Instance, s_cfg.wake_tim_channel, TIM_CCx_ENABLE);

    return BQ_OK;
}

bq_status_t bq_init(const bq_init_struct_t init) {
    GPIO_InitTypeDef gpio = {0};
    bq_status_t status;

    if (init.huart == NULL || init.tx_rx_port == NULL || init.fault_port == NULL || init.wake_tim == NULL) {
        return BQ_ERR_PARAM;
    }
    /* TIM2..TIM5 only have channels 1-4. The HAL's TIM_CHANNEL_x values are register offsets
     * (0x0, 0x4, 0x8, 0xC), not 1..4, so compare against the constants themselves. */
    if (init.wake_tim_channel != TIM_CHANNEL_1 && init.wake_tim_channel != TIM_CHANNEL_2 &&
        init.wake_tim_channel != TIM_CHANNEL_3 && init.wake_tim_channel != TIM_CHANNEL_4) {
        return BQ_ERR_PARAM;
    }
    /* Callers size frame arrays with BQ_MAX_RESPONSE_FRAMES, and bq_wake's stack wait scales with it */
    if (init.num_devices > BQ_MAX_DEVICES) {
        return BQ_ERR_PARAM;
    }

    s_initialised = false;
    s_cfg = init;

    status = gpio_clk_enable(s_cfg.tx_rx_port);
    if (status == BQ_OK) {
        status = gpio_clk_enable(s_cfg.fault_port);
    }
    if (status == BQ_OK) {
        status = crc_init();
    }
    if (status == BQ_OK) {
        status = wake_tim_init();
    }
    if (status != BQ_OK) {
        return status;
    }

    /* TX starts as a GPIO driven high (idle). It's only handed to the UART
     * once the UART is enabled and idling high too, so the BQ's RX never sees
     * a low glitch that could read as a ping or COMM CLEAR.
     * GPIO speed only affects output edges: parked here TX just holds a level, so LOW is
     * enough; once it carries 1 Mbps UART data it gets HIGH below. */
    HAL_GPIO_WritePin(s_cfg.tx_rx_port, s_cfg.tx_pin, GPIO_PIN_SET);
    gpio.Pin   = s_cfg.tx_pin;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(s_cfg.tx_rx_port, &gpio);

    /* RX. Pull-up because the BQ's TX is high-impedance in SHUTDOWN - a
     * floating RX manufactures framing errors that look like real faults. */
    gpio.Pin       = s_cfg.rx_pin;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_PULLUP;
    gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = s_cfg.uart_af;
    HAL_GPIO_Init(s_cfg.tx_rx_port, &gpio);

    /* NFAULT is open-drain, active low, and pulled up externally (BQ79600 §7.3.3.2.1 p31,
     * §8.2.1.2.1 p55). The internal pull-up is a backup, so a missing or disconnected external
     * pull-up reads as "no fault" instead of floating; it's harmless alongside the external one. */
    gpio.Pin       = s_cfg.fault_pin;
    gpio.Mode      = GPIO_MODE_INPUT;
    gpio.Pull      = GPIO_PULLUP;
    gpio.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0U;
    HAL_GPIO_Init(s_cfg.fault_port, &gpio);

    s_cfg.huart->Init.BaudRate     = BQ_UART_BAUD;
    s_cfg.huart->Init.WordLength   = UART_WORDLENGTH_8B;
    s_cfg.huart->Init.StopBits     = UART_STOPBITS_1;
    s_cfg.huart->Init.Parity       = UART_PARITY_NONE;
    s_cfg.huart->Init.Mode         = UART_MODE_TX_RX;
    s_cfg.huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    s_cfg.huart->Init.OverSampling = UART_OVERSAMPLING_16;

    /* With overrun detection on, an ORE aborts HAL's interrupt reception and
     * nothing in UART.c restarts it, so RX would go dead for good. A lost
     * byte already shows up as a CRC or timeout error on the frame. */
    s_cfg.huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
    s_cfg.huart->AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;

    if (uart_init(s_cfg.huart) != UART_OK) {
        return BQ_ERR_HAL;
    }

    /* UART is enabled and idling high, hand TX over to it */
    gpio.Pin       = s_cfg.tx_pin;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = s_cfg.uart_af;
    HAL_GPIO_Init(s_cfg.tx_rx_port, &gpio);

    s_initialised = true;
    return BQ_OK;
}

// Build a command frame (INIT, [DEV ADR], REG ADR, DATA, CRC) from msg and send it.
// msg->data holds msg->len (1-8) DATA bytes. BQ79600 §7.3.2.1.1 p15-16
static bq_status_t bq_send(const bq_msg_frame_t *msg)
{
    // Every byte up to byte_num is written before it's sent, so frame needs no zeroing
    uint8_t frame[BQ_MAX_CMD_LEN];
    uint16_t byte_num = 0U;

    // Callers check len too; this keeps frame from ever overrunning
    if (msg->len == 0U || msg->len > BQ_MAX_WRITE_LEN) {
        return BQ_ERR_PARAM;
    }

    // INIT: command frame, request type, DATA length - 1
    frame[byte_num] = (uint8_t)(0x80U | ((uint8_t)msg->req << 4) | (msg->len - 1U));
    byte_num++;

    // only single reads/writes carry a device address
    if (is_single_req(msg->req)) {
        frame[byte_num] = msg->dev_addr;
        byte_num++;
    }

    // register address, MSB first
    frame[byte_num] = (uint8_t)(msg->reg_addr >> 8);
    byte_num++;
    frame[byte_num] = (uint8_t)(msg->reg_addr & 0xFFU);
    byte_num++;

    for (uint8_t i = 0U; i < msg->len; i++) {
        frame[byte_num] = msg->data[i];
        byte_num++;
    }

    uint16_t crc = bq_crc16(frame, byte_num);
    frame[byte_num] = (uint8_t)(crc & 0xFFU);    /* CRC LSB first */
    byte_num++;
    frame[byte_num] = (uint8_t)(crc >> 8);
    byte_num++;

    if (uart_send(s_cfg.huart, frame, byte_num, pdMS_TO_TICKS(BQ_RESP_TIMEOUT_MS)) != UART_OK) {
        return BQ_ERR_UART;
    }

    return BQ_OK;
}

bq_status_t bq_write(const bq_msg_frame_t *msg)
{
    if (!s_initialised) {
        return BQ_ERR_NOT_INIT;
    }
    if (msg == NULL || msg->data == NULL || msg->len == 0U || msg->len > BQ_MAX_WRITE_LEN) {
        return BQ_ERR_PARAM;
    }
    if (!is_write_req(msg->req)) {
        return BQ_ERR_PARAM;
    }
    if (msg->req == BQ_SINGLE_WRITE && !dev_exists(msg->dev_addr)) {
        return BQ_ERR_PARAM;
    }

    return bq_send(msg);
}

bq_status_t bq_read(const bq_msg_frame_t *msg, bq_response_frame_t *frames, uint8_t max_frames)
{
    static uint8_t resp[BQ_MAX_FRAME_LEN];
    uint8_t num_frames;

    if (!s_initialised) {
        return BQ_ERR_NOT_INIT;
    }
    if (msg == NULL || frames == NULL || msg->len == 0U || msg->len > BQ_MAX_DATA_LEN) {
        return BQ_ERR_PARAM;
    }
    if (!is_read_req(msg->req)) {
        return BQ_ERR_PARAM;
    }
    if (msg->req == BQ_SINGLE_READ && !dev_exists(msg->dev_addr)) {
        return BQ_ERR_PARAM;
    }

    const uint16_t resp_len = (uint16_t)msg->len + BQ_FRAME_OVERHEAD;

    // one frame per responding device
    switch (msg->req) {
        case BQ_SINGLE_READ:
            num_frames = 1U;
            break;
        case BQ_STACK_READ:
            num_frames = s_cfg.num_devices;
            break;
        case BQ_BROAD_READ:
            num_frames = s_cfg.num_devices + 1U;
            break;
        default: return BQ_ERR_PARAM;
    }
    // check before sending, so a too-small array never leaves unread replies on the line
    if (num_frames == 0U || num_frames > max_frames) {
        return BQ_ERR_PARAM;
    }

    /* Read command DATA byte is the number of bytes to return, minus one */
    const uint8_t count = msg->len - 1U;
    const bq_msg_frame_t cmd = {
        .req      = msg->req,
        .dev_addr = msg->dev_addr,
        .reg_addr = msg->reg_addr,
        .data     = &count,
        .len      = 1U,
    };
    bq_rx_flush();
    bq_status_t status = bq_send(&cmd);

    if (status != BQ_OK) {
        return status;
    }

    // one response frame per device, top of stack first
    for (uint8_t dev = 0U; dev < num_frames; dev++) {

        if (uart_recv(s_cfg.huart, resp, resp_len, pdMS_TO_TICKS(BQ_RESP_TIMEOUT_MS)) != UART_OK) {
            return BQ_ERR_TIMEOUT;
        }

        if (bq_crc16(resp, resp_len) != 0U) {
            return BQ_ERR_CRC;
        }

        // response INIT is the byte count - 1, then DEV ADR and the register address we asked for
        if (resp[0] != count) {
            return BQ_ERR_FRAME;
        }
        if (msg->req == BQ_SINGLE_READ && resp[1] != msg->dev_addr) {
            return BQ_ERR_FRAME;
        }
        if (resp[2] != (uint8_t)(msg->reg_addr >> 8) || resp[3] != (uint8_t)(msg->reg_addr & 0xFFU)) {
            return BQ_ERR_FRAME;
        }

        // keep who answered and what they sent
        frames[dev].dev_addr = resp[1];
        memcpy(frames[dev].data, &resp[4], msg->len);
    }

    return BQ_OK;
}

// WAKE ping: hold the BQ79600's RX low for BQ_WAKE_PING_US, then release it high (BQ79600 §7.3.1.2 p12)
bq_status_t bq_wake(void) {
    GPIO_InitTypeDef gpio = {0};
    bq_status_t status;

    if (!s_initialised) {
        return BQ_ERR_NOT_INIT;
    }

    /* Don't cut off a frame. HAL only returns gState to READY at transmit
     * complete, i.e. after the last stop bit has left the pin. */
    if (s_cfg.huart->gState != HAL_UART_STATE_READY) {
        return BQ_ERR_BUSY;
    }

    // Hand tx_pin to the timer
    gpio.Pin       = s_cfg.tx_pin;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = s_cfg.wake_tim_af;
    HAL_GPIO_Init(s_cfg.tx_rx_port, &gpio);

    /* Fire. From here the timer times the ping on its own and clears CEN
     * when it is done. */
    s_htim.Instance->CR1 |= TIM_CR1_CEN;

    /* The ping, then the bridge's startup. +1 tick: vTaskDelay(N) can return up to a tick early. */
    vTaskDelay(pdMS_TO_TICKS(((BQ_WAKE_PING_US + 999U) / 1000U) + BQ_WAKE_SETTLE_MS) + 1U);

    // Hand tx_pin back to the UART, which idles high
    gpio.Alternate = s_cfg.uart_af;
    HAL_GPIO_Init(s_cfg.tx_rx_port, &gpio);

    const uint8_t send_wake = BQ_REG_CONTROL1_SEND_WAKE;
    const bq_msg_frame_t wake_stack = {
        .req      = BQ_SINGLE_WRITE,
        .dev_addr = 0U,                 // the BQ79600 bridge
        .reg_addr = BQ_REG_CONTROL1,
        .data     = &send_wake,
        .len      = 1U,
    };
    status = bq_write(&wake_stack);
    if (status != BQ_OK) {
        return status;
    }

    // Each BQ79616 takes up to tSU(WAKE_SHUT) to wake and forward the tone to the next one
    vTaskDelay(pdMS_TO_TICKS((uint32_t)s_cfg.num_devices * BQ_WAKE_PER_DEV_MS) + 1U);

    return BQ_OK;
}

// One-byte write
static bq_status_t bq_write_byte(bq_req_t req, uint8_t dev_addr, uint16_t reg_addr, uint8_t value)
{
    const bq_msg_frame_t msg = {
        .req      = req,
        .dev_addr = dev_addr,
        .reg_addr = reg_addr,
        .data     = &value,
        .len      = 1U,
    };
    return bq_write(&msg);
}

// Auto-addressing, BQ79600 Table 7-9 steps 3-10 p27-28. Steps 1-2 are bq_wake().
bq_status_t bq_auto_address(void)
{
    static bq_response_frame_t frames[BQ_MAX_RESPONSE_FRAMES];
    bq_msg_frame_t msg;
    bq_status_t status;

    if (!s_initialised) {
        return BQ_ERR_NOT_INIT;
    }
    if (s_cfg.num_devices == 0U) {
        return BQ_ERR_PARAM;
    }

    // step 3: sync the stack's DLLs in the write direction, one dummy write per register
    for (uint16_t reg = BQ_REG_OTP_ECC_DATAIN1; reg <= BQ_REG_OTP_ECC_DATAIN8; reg++) {
        status = bq_write_byte(BQ_STACK_WRITE, 0U, reg, 0x00U);
        if (status != BQ_OK) {
            return status;
        }
    }

    // step 4: every device keeps the next DIR0_ADDR it sees and doesn't forward it
    status = bq_write_byte(BQ_BROAD_WRITE, 0U, BQ_REG_CONTROL1, BQ_REG_CONTROL1_ADDR_WR);
    if (status != BQ_OK) {
        return status;
    }

    // step 5: 0 lands on the bridge, 1..num_devices on the BQ79616s in chain order
    for (uint8_t addr = 0U; addr <= s_cfg.num_devices; addr++) {
        status = bq_write_byte(BQ_BROAD_WRITE, 0U, BQ_REG_DIR0_ADDR, addr);
        if (status != BQ_OK) {
            return status;
        }
    }

    // step 6: every BQ79616 is a stack device. The bridge is the base by default.
    status = bq_write_byte(BQ_BROAD_WRITE, 0U, BQ_REG_COMM_CTRL, BQ_REG_COMM_CTRL_STACK_DEV);
    if (status != BQ_OK) {
        return status;
    }

    // step 7: the last one is also top of stack, so it starts the response chain
    status = bq_write_byte(BQ_SINGLE_WRITE, s_cfg.num_devices, BQ_REG_COMM_CTRL,
                           BQ_REG_COMM_CTRL_STACK_DEV | BQ_REG_COMM_CTRL_TOP_STACK);
    if (status != BQ_OK) {
        return status;
    }

    /* step 8: sync the DLLs in the read direction, one dummy read per register. The replies can
     * be garbled while the DLLs lock, so their status is ignored; bq_read() flushes whatever is
     * left over before its next command. */
    msg = (bq_msg_frame_t){ .req = BQ_STACK_READ, .len = 1U };
    for (uint16_t reg = BQ_REG_OTP_ECC_DATAIN1; reg <= BQ_REG_OTP_ECC_DATAIN8; reg++) {
        msg.reg_addr = reg;
        (void)bq_read(&msg, frames, BQ_MAX_RESPONSE_FRAMES);
    }

    // step 9: each BQ79616 reports the address it took, top of stack first
    msg.reg_addr = BQ_REG_DIR0_ADDR;
    status = bq_read(&msg, frames, BQ_MAX_RESPONSE_FRAMES);
    if (status != BQ_OK) {
        return status;
    }
    for (uint8_t i = 0U; i < s_cfg.num_devices; i++) {
        const uint8_t expected = s_cfg.num_devices - i;
        if (frames[i].dev_addr != expected || (frames[i].data[0] & BQ_MAX_DEV_ADDR) != expected) {
            return BQ_ERR_VERIFY;
        }
    }

    // step 10: the bridge still answers as device 0, with DEV_CONF1 at its reset value
    msg = (bq_msg_frame_t){
        .req      = BQ_SINGLE_READ,
        .dev_addr = 0U,
        .reg_addr = BQ_REG_DEV_CONF1,
        .len      = 1U,
    };
    status = bq_read(&msg, frames, BQ_MAX_RESPONSE_FRAMES);
    if (status != BQ_OK) {
        return status;
    }
    if (frames[0].data[0] != BQ_REG_DEV_CONF1_RESET) {
        return BQ_ERR_VERIFY;
    }

    return BQ_OK;
}

/* Stack-write one byte to every BQ79616, then stack-read it back and check every device took it */
static bq_status_t bq_stack_write_verify(uint16_t reg_addr, uint8_t value)
{
    static bq_response_frame_t frames[BQ_MAX_RESPONSE_FRAMES];
    const bq_msg_frame_t read = {
        .req      = BQ_STACK_READ,
        .reg_addr = reg_addr,
        .len      = 1U,
    };
    bq_status_t status = bq_write_byte(BQ_STACK_WRITE, 0U, reg_addr, value);

    if (status != BQ_OK) {
        return status;
    }
    status = bq_read(&read, frames, BQ_MAX_RESPONSE_FRAMES);
    if (status != BQ_OK) {
        return status;
    }
    for (uint8_t i = 0U; i < s_cfg.num_devices; i++) {
        if (frames[i].data[0] != value) {
            return BQ_ERR_VERIFY;
        }
    }

    return BQ_OK;
}

bq_status_t bq_config(bq_comm_timeout_t comm_timeout)
{
    bq_status_t status;

    if (!s_initialised) {
        return BQ_ERR_NOT_INIT;
    }
    if (s_cfg.num_devices == 0U || (uint32_t)comm_timeout > BQ_REG_COMM_TIMEOUT_CONF_CTL_TIME_MASK) {
        return BQ_ERR_PARAM;
    }

    // number of cells in series, then let the analog front end settle for the new count
    status = bq_stack_write_verify(BQ_REG_ACTIVE_CELL, BQ_REG_ACTIVE_CELL_NUM(BQ_NUM_CELLS));
    if (status != BQ_OK) {
        return status;
    }
    vTaskDelay(pdMS_TO_TICKS(BQ_AFE_SETTLE_MS) + 1U);

    /* Long comm timeout: after comm_timeout without valid communication, go to SHUTDOWN.
     * CTS_TIME = 0 leaves the short timeout off. */
    status = bq_stack_write_verify(BQ_REG_COMM_TIMEOUT_CONF,
                                   BQ_REG_COMM_TIMEOUT_CONF_CTL_ACT_SHUTDOWN | (uint8_t)comm_timeout);
    if (status != BQ_OK) {
        return status;
    }

    /* GPIO1-8 as ADC-only thermistor inputs: measured by the main ADC, but the OT/UT comparators
     * stay off, so a missing thermistor can't raise a fault. Each register holds two GPIOs; the
     * upper bits (GPIO_CONF1's FAULT_IN_EN and SPI_EN, spares elsewhere) stay 0. */
    for (uint16_t reg = BQ_REG_GPIO_CONF1; reg <= BQ_REG_GPIO_CONF4; reg++) {
        status = bq_stack_write_verify(reg, BQ_REG_GPIO_CONF_PAIR(BQ_GPIO_MODE_ADC_ONLY, BQ_GPIO_MODE_ADC_ONLY));
        if (status != BQ_OK) {
            return status;
        }
    }

    /* TSREF is the 5 V bias at the top of each thermistor divider; it's off at reset. SEND_HW_RESET,
     * the only other bit in CONTROL2, stays 0. */
    status = bq_stack_write_verify(BQ_REG_CONTROL2, BQ_REG_CONTROL2_TSREF_EN);
    if (status != BQ_OK) {
        return status;
    }
    vTaskDelay(pdMS_TO_TICKS(BQ_TSREF_SETTLE_MS) + 1U);

    return BQ_OK;
}

/* Converts one VCELL result (big-endian two's complement, 190.73 uV/LSB) to mV, rounded.
 * Returns false for 0x8000, which means the cell has never been converted. A negative reading
 * becomes 0 mV, since mv is unsigned. */
static bool vcell_to_mv(const uint8_t hi_lo[2], uint16_t *mv)
{
    const int16_t raw = (int16_t)(((uint16_t)hi_lo[0] << 8) | hi_lo[1]);

    if (raw == BQ_ADC_RESULT_INVALID) {
        return false;
    }
    /* |raw| <= 32767, so raw * 19073 fits in int32 */
    const int32_t rounded = ((int32_t)raw * BQ_VCELL_UV_PER_LSB_X100 + 50000) / 100000;
    *mv = (rounded < 0) ? 0U : (uint16_t)rounded;
    return true;
}

bq_status_t bq_read_cell_voltages(uint16_t cell_voltages_mv[BQ_MAX_DEVICES][BQ_NUM_CELLS])
{
    static bq_response_frame_t frames[BQ_MAX_RESPONSE_FRAMES];
    bq_msg_frame_t msg;
    bq_status_t status;

    if (!s_initialised) {
        return BQ_ERR_NOT_INIT;
    }
    // bq_init() capped num_devices at BQ_MAX_DEVICES, so every device has a row
    if (cell_voltages_mv == NULL || s_cfg.num_devices == 0U) {
        return BQ_ERR_PARAM;
    }

    /* One-shot main ADC run on every BQ79616: 8 round robins, then it stops and holds the results */
    status = bq_write_byte(BQ_STACK_WRITE, 0U, BQ_REG_ADC_CTRL1,
                           BQ_REG_ADC_CTRL1_MAIN_MODE_SINGLE | BQ_REG_ADC_CTRL1_MAIN_GO);
    if (status != BQ_OK) {
        return status;
    }

    /* Wait out the whole single run (8 x tMAIN_ADC_CYCLE). +1 tick: vTaskDelay(N) can return early.
     * Nothing here confirms the run finished: DRDY_MAIN_ADC isn't read, so if it hasn't, the VCELL
     * registers still hold the previous run's results. Only a cell that has never been converted
     * since reset (0x8000) is caught, below. */
    vTaskDelay(pdMS_TO_TICKS(BQ_ADC_SINGLE_RUN_MS) + 1U);

    /* One stack read from VCELL<BQ_NUM_CELLS>_HI up to VCELL1_LO. The registers run from the
     * highest cell down, 2 bytes each. */
    msg = (bq_msg_frame_t){
        .req      = BQ_STACK_READ,
        .reg_addr = BQ_REG_VCELL_HI(BQ_NUM_CELLS),
        .len      = 2U * BQ_NUM_CELLS,
    };
    status = bq_read(&msg, frames, BQ_MAX_RESPONSE_FRAMES);
    if (status != BQ_OK) {
        return status;
    }

    for (uint8_t i = 0U; i < s_cfg.num_devices; i++) {
        const uint8_t dev = frames[i].dev_addr;     // top of stack comes first
        if (dev == 0U || dev > s_cfg.num_devices) {
            return BQ_ERR_FRAME;
        }
        for (uint8_t cell = 1U; cell <= BQ_NUM_CELLS; cell++) {
            if (!vcell_to_mv(&frames[i].data[2U * (BQ_NUM_CELLS - cell)],
                             &cell_voltages_mv[dev - 1U][cell - 1U])) {
                return BQ_ERR_ADC;
            }
        }
    }

    return BQ_OK;
}
