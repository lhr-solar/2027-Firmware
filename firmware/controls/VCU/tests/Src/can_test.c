/* STM32G473 external FDCAN loopback and physical TX waveform test.
 * Sends and validates four Classic CAN frames with IDs 0x001 and 0x003.
 * PD4 blinks during repeated successful transmissions; PD4 stays high on failure.
 * External loopback drives FDCAN_TX. CANH/CANL only toggle if a powered, enabled
 * CAN transceiver is connected to the selected FDCAN pins.
 * No second CAN node is required to acknowledge external-loopback frames.
 * Assumes SystemClock_Config() and the HAL FDCAN driver are linked by the platform.
 */
#include "stm32xx_hal.h"
#include <stdint.h>

#if !defined(STM32G473xx)
#error "This test is for STM32G473xx."
#endif
#if !defined(FDCAN1)
#error "FDCAN1 is not defined for this MCU."
#endif

#define TEST_LED_PORT GPIOD
#define TEST_LED_PIN  GPIO_PIN_4
#define TEST_TIMEOUT_MS 100U
#define TEST_FRAME_PERIOD_MS 100U

/* Example pin mapping: PB8 = FDCAN1_RX, PB9 = FDCAN1_TX, AF9.
 * IMPORTANT: change these pins if your actual VCU board uses different
 * FDCAN1 pins, and make sure the transceiver is powered and enabled. */
#define TEST_CAN_GPIO_PORT GPIOB
#define TEST_CAN_RX_PIN GPIO_PIN_8
#define TEST_CAN_TX_PIN GPIO_PIN_9
#define TEST_CAN_GPIO_AF GPIO_AF9_FDCAN1

static FDCAN_HandleTypeDef hfdcan1;

static void led_init(void)
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = TEST_LED_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TEST_LED_PORT, &gpio);
    HAL_GPIO_WritePin(TEST_LED_PORT, TEST_LED_PIN, GPIO_PIN_RESET);
}

static void test_fail(void)
{
    /* A solid LED indicates failure (use debugger to locate the call site). */
    HAL_GPIO_WritePin(TEST_LED_PORT, TEST_LED_PIN, GPIO_PIN_SET);
    while (1) { }
}

/* HAL_FDCAN_Init() calls this hook to enable peripheral clock. */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan->Instance == FDCAN1) {
        __HAL_RCC_FDCAN_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef gpio = {0};
        gpio.Pin = TEST_CAN_RX_PIN | TEST_CAN_TX_PIN;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Pull = GPIO_NOPULL;
        gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = TEST_CAN_GPIO_AF;
        HAL_GPIO_Init(TEST_CAN_GPIO_PORT, &gpio);
    }
}

static void fdcan_init(void)
{
    hfdcan1.Instance = FDCAN1;
    hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan1.Init.Mode = FDCAN_MODE_EXTERNAL_LOOPBACK;
    hfdcan1.Init.AutoRetransmission = ENABLE;
    hfdcan1.Init.TransmitPause = DISABLE;
    hfdcan1.Init.ProtocolException = DISABLE;

    /* At an 80 MHz FDCAN kernel clock: 80MHz / (20 * (1+13+2)) = 250 kbit/s.
     * If your FDCAN kernel clock isn't 80 MHz, adjust the timing accordingly.
     */
    hfdcan1.Init.NominalPrescaler = 20;
    hfdcan1.Init.NominalSyncJumpWidth = 1;
    hfdcan1.Init.NominalTimeSeg1 = 13;
    hfdcan1.Init.NominalTimeSeg2 = 2;
    hfdcan1.Init.DataPrescaler = 1;
    hfdcan1.Init.DataSyncJumpWidth = 1;
    hfdcan1.Init.DataTimeSeg1 = 1;
    hfdcan1.Init.DataTimeSeg2 = 1;
    hfdcan1.Init.StdFiltersNbr = 1;
    hfdcan1.Init.ExtFiltersNbr = 0;
    hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK) test_fail();

    /* One standard-ID mask filter accepts all standard IDs into FIFO0. */
    FDCAN_FilterTypeDef filter = {0};
    filter.IdType = FDCAN_STANDARD_ID;
    filter.FilterIndex = 0;
    filter.FilterType = FDCAN_FILTER_MASK;
    filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter.FilterID1 = 0;
    filter.FilterID2 = 0;
    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &filter) != HAL_OK) test_fail();
    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,
                                     FDCAN_REJECT, FDCAN_REJECT,
                                     FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK) test_fail();
    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) test_fail();
}

static void send_and_check(uint32_t id, uint8_t first_byte)
{
    FDCAN_TxHeaderTypeDef tx = {0};
    FDCAN_RxHeaderTypeDef rx = {0};
    uint8_t tx_data[8] = {first_byte, 0};
    uint8_t rx_data[8] = {0};

    tx.Identifier = id;
    tx.IdType = FDCAN_STANDARD_ID;
    tx.TxFrameType = FDCAN_DATA_FRAME;
    tx.DataLength = FDCAN_DLC_BYTES_2;
    tx.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx.BitRateSwitch = FDCAN_BRS_OFF;
    tx.FDFormat = FDCAN_CLASSIC_CAN;
    tx.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx.MessageMarker = 0;

    uint32_t start = HAL_GetTick();
    while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0U) {
        if ((HAL_GetTick() - start) > TEST_TIMEOUT_MS) test_fail();
    }
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx, tx_data) != HAL_OK) test_fail();

    start = HAL_GetTick();
    while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) == 0U) {
        if ((HAL_GetTick() - start) > TEST_TIMEOUT_MS) test_fail();
    }
    if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rx, rx_data) != HAL_OK) test_fail();
    if (rx.IdType != FDCAN_STANDARD_ID ||
        rx.Identifier != id ||
        rx.RxFrameType != FDCAN_DATA_FRAME ||
        rx.DataLength != FDCAN_DLC_BYTES_2 ||
        rx_data[0] != first_byte || rx_data[1] != 0U) {
        test_fail();
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config(); /* Provided by your G473 platform. */
    led_init();
    fdcan_init();

    /* Repeat bursts so an oscilloscope/CAN analyzer can see activity.
     * Blinking confirms the FDCAN controller self-test only; it does not
     * prove the transceiver or CANH/CANL traces are functional.
     */
    uint32_t burst_count = 0;
    while (1) {
        send_and_check(0x001, 0x01);
        send_and_check(0x001, 0x02);
        send_and_check(0x003, 0x03);
        send_and_check(0x003, 0x04);

        if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) != 0U) test_fail();
        if (++burst_count == 5U) {
            HAL_GPIO_TogglePin(TEST_LED_PORT, TEST_LED_PIN);
            burst_count = 0;
        }
        HAL_Delay(TEST_FRAME_PERIOD_MS);
    }
}