/**
 * @file BQ79616.h
 * @brief Driver for a TI BQ79600 bridge and a daisy chain of BQ79616 battery monitors, over UART.
 *
 * @details
 * The MCU talks UART to the BQ79600. The BQ79600 forwards each command up the isolated COMH/COML
 * daisy chain to the BQ79616s and passes their responses back. This driver wakes the chain,
 * auto-addresses it, configures the BQ79616s and reads cell voltages; bq_read() and bq_write()
 * give raw register access for everything else (registers are in BQ79616_regs.h).
 *
 * Datasheets. Section and page numbers in this driver ("§x.y pN") refer to these revisions:
 *  - BQ79600-Q1, SLUSDS1A: https://www.ti.com/lit/ds/symlink/bq79600-q1.pdf
 *  - BQ79616,    SLUSF21A: https://www.ti.com/lit/ds/symlink/bq79616.pdf
 *
 * How to Use This Driver:
 *
 *  1. bq_init() sets up the CRC unit, wake timer, pins and UART. It does not talk to the chips.
 *
 *  2. bq_wake() wakes the bridge with a WAKE ping, then wakes the stack with a WAKE tone.
 *
 *  3. bq_auto_address() gives the bridge address 0 and the BQ79616s 1..num_devices.
 *
 *  4. bq_config() sets the cell count, comm timeout, GPIO mode and TSREF on every BQ79616.
 *
 *  5. bq_read_volt_temp() to take a new set of measurements, then bq_get_volts_mv() and
 *     bq_get_temps_mc() to fetch them per device. bq_read() and bq_write() as needed.
 *
 *  A WAKE resets the BQ79616s, so steps 3 and 4 must run again after every bq_wake().
 *
 * Usage Notes:
 *
 *  - Single task by design: the driver uses static buffers and the UART's single RX queue, with
 *    no lock. Only one task may call it; if a second task needs BQ data, route it through the
 *    owning task (e.g. a queue) rather than calling the driver directly.
 *
 *  - bq_wake(), bq_auto_address(), bq_config() and bq_read_volt_temp() block with
 *    vTaskDelay(), so they must run from a task.
 *
 *  - tx_pin and rx_pin share one port (tx_rx_port). tx_pin must also have a TIM2..TIM5 or TIM8
 *    channel, or a TIM8 complementary (CHxN) output, as an alternate function: that timer
 *    generates the WAKE ping, and the driver owns it.
 *
 *  - uart_init() calls HAL_UART_MspGPIOInit(). The psp default for USART3 also routes PC10/PC11
 *    to USART3, on top of the pins bq_init() sets. A board that uses PC10/PC11 for anything else
 *    must override HAL_UART_MspGPIOInit() so it leaves the BQ UART's pins to this driver. If the
 *    BQ is on PC10/PC11 itself, the default sets RX without its pull-up, so bq_init() sets RX
 *    again afterwards; TX is still handed to USART3 before the UART is enabled.
 *
 *  - bq_config() sets the long comm timeout the caller picks (bq_comm_timeout_t). If nothing
 *    talks to the stack for that long, the BQ79616s shut down and need bq_wake(),
 *    bq_auto_address() and bq_config() again, so poll the stack more often than the timeout.
 *
 * Not implemented yet (TODO):
 *
 *  - cell balancing
 *  - fault registers and the NFAULT pin (configured as an input, but never read)
 *  - re-check the WAKE ping length when the timer clock moves from 80 MHz to 96 MHz
 *    (see wake_tim_init() in BQ79616.c)
 *  - storing the configuration in the BQ79616s' OTP, so it survives a WAKE instead of being
 *    rewritten by bq_config() after every one
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "stm32xx_hal.h"   // GPIO_TypeDef, TIM_TypeDef, UART_HandleTypeDef
#include "UART.h"          // psp UART handles (husart3, ...)
#include "BQ79616_regs.h"

#define BQ_UART_BAUD        1000000U    /* BQ79600 UART default, BQ79600 §7.3.2.1.2.1 p18 */

#define BQ_WAKE_TIM_HZ      1000000U    /* wake timer counts in 1 us ticks */
#define BQ_WAKE_PING_US     2750U       /* middle of tHLD_WAKE = 2.5-3 ms, BQ79600 §6.6 p7 */
#define BQ_WAKE_SETTLE_MS   5U          /* covers tSU(WAKE_SHUT) = 3.5 ms max, BQ79600 §6.6 p7 */
#define BQ_WAKE_PER_DEV_MS  10U         /* BQ79616 tSU(WAKE_SHUT) max, per stack device, BQ79616 §5.6 p13 */

/* Sized for our current battery pack architecture, not limits of the chips */
#define BQ_MAX_DEVICES      4U          /* BQ79616s in the stack, not counting the BQ79600 bridge */
#define BQ_NUM_CELLS        8U          /* cells in series on each BQ79616 (the chip supports 6-16) */
#define BQ_NUM_GPIO         8U          /* GPIO1-8 on each BQ79616, one thermistor each (BQ79616 6.3.2.1.2.2 p25) */

_Static_assert(BQ_NUM_CELLS >= 6U && BQ_NUM_CELLS <= 16U, "BQ79616 supports 6S to 16S");

/* Frame limits, BQ79600 §7.3.2.1.1 p15-16 */
#define BQ_MAX_WRITE_LEN    8U          /* write DATA is 1-8 bytes (DATA_SIZE field is 3 bits) */
#define BQ_MAX_DATA_LEN     128U        /* read returns 1-128 bytes (RESPONSE_BYTE field is 7 bits) */
#define BQ_MAX_DEV_ADDR     0x3FU       /* device addresses are 6 bits */
#define BQ_FRAME_OVERHEAD   6U          /* INIT + DEV ADR + REG ADR (2) + CRC (2) */
#define BQ_MAX_FRAME_LEN    (BQ_FRAME_OVERHEAD + BQ_MAX_DATA_LEN)   /* longest response frame */
#define BQ_MAX_CMD_LEN      (BQ_FRAME_OVERHEAD + BQ_MAX_WRITE_LEN)  /* longest command frame */

/* Thermistor divider on each GPIO, BQ79616 Figure 6-6 p24: TSREF -> R1 -> GPIO -> (NTC || R2) -> GND.
 * These describe our board, not the chip. */
#define BQ_GPIO_PULLUP_OHMS     (3200U)     /* R1 */
#define BQ_GPIO_PULLDOWN_OHMS   (15000U)    /* R2, the board's own pull-down, in parallel with the NTC */
/* The thermistor is a Vishay NTCLE413E2103F520L: 10 k at 25 C (+-1 %), B25/85 = 3435 K. Its R-T
 * curve is the lookup table in BQ79616.c, from ntcle413.pdf (Vishay 29078) Table 3 p4. */

/* Above this the GPIO is treated as having no thermistor: just above the NTCLE413's 191 k at -40 C,
 * the bottom of its range (Table 3), so no real reading is mistaken for open. With nothing fitted,
 * ADC rounding and R1/R2 tolerance leave the computed value just under "infinite" rather than at it;
 * this still catches that with up to ~2.5 % R1/R2 error. Beyond that, a missing thermistor reads as
 * very cold and clamps to -40 C instead, which still trips an under-temperature limit. */
#define BQ_NTC_OPEN_OHMS        (250000U)

/* Stored for a GPIO whose reading can't be a thermistor: TSREF off, GPIO shorted to ground, or no
 * thermistor fitted (the node sits at R2's divider voltage) */
#define BQ_TEMP_INVALID_MC      INT32_MIN

#define BQ_RESP_TIMEOUT_MS  10U         /* max wait between response bytes (driver choice) */

// Result of every driver call. Anything other than BQ_OK says why it failed; bq_status_str() names it.
typedef enum {
    BQ_OK = 0,
    BQ_ERR_PARAM,       // invalid argument or request (NULL, bad length or type, no such device)
    BQ_ERR_NOT_INIT,    // bq_init() hasn't succeeded yet
    BQ_ERR_HAL,         // a peripheral (CRC, timer, UART) failed to start
    BQ_ERR_BUSY,        // the UART is still sending a frame
    BQ_ERR_UART,        // a command frame couldn't be queued for sending
    BQ_ERR_TIMEOUT,     // a response frame didn't arrive in time
    BQ_ERR_CRC,         // a response frame failed its CRC check
    BQ_ERR_FRAME,       // a response frame didn't match the command (length, device, register)
    BQ_ERR_VERIFY,      // a value read back isn't what was written or expected
    BQ_ERR_ADC,         // a cell result has never been converted
} bq_status_t;

typedef struct bq_init_struct_t {
    uint16_t tx_pin;
    uint16_t rx_pin;
    GPIO_TypeDef* tx_rx_port;
    uint16_t fault_pin;
    GPIO_TypeDef* fault_port;

    UART_HandleTypeDef* huart;      // psp UART handle wired to the BQ79600, e.g. husart3
    uint8_t uart_af;                // AF that routes tx/rx to huart, e.g. GPIO_AF7_USART3

    // The WAKE ping is generated by a timer channel output on tx_pin, so tx_pin
    // must have one of TIM2..TIM5's or TIM8's channels, or one of TIM8's
    // complementary (CHxN) outputs, as an alternate function.
    // e.g. PB10: TIM2, TIM_CHANNEL_3, GPIO_AF1_TIM2, wake_tim_complementary = false
    //      PC10: TIM8, TIM_CHANNEL_1, GPIO_AF4_TIM8, wake_tim_complementary = true (TIM8_CH1N)
    TIM_TypeDef* wake_tim;          // TIM2..TIM5 or TIM8
    uint32_t wake_tim_channel;      // TIM_CHANNEL_1..TIM_CHANNEL_4
    uint8_t wake_tim_af;
    bool wake_tim_complementary;    // tx_pin carries the channel's CHxN output, not CHx (TIM8 only)

    uint8_t num_devices;            // BQ79616s in the stack (0..BQ_MAX_DEVICES), not counting the bridge
} bq_init_struct_t;

/* Command frame REQ_TYPE (INIT byte bits 6:4), BQ79600 §7.3.2.1.1 p16 */
typedef enum {
    BQ_SINGLE_READ,
    BQ_SINGLE_WRITE,
    BQ_STACK_READ,
    BQ_STACK_WRITE,
    BQ_BROAD_READ,
    BQ_BROAD_WRITE,
    BQ_BROAD_WRITE_REV,
} bq_req_t;

// One device's response to a read: the DEV ADR it sent back and the register bytes
typedef struct bq_response_frame_t {
    uint8_t data[BQ_MAX_DATA_LEN];
    uint8_t dev_addr;
} bq_response_frame_t;

// Internal structure to store measurements
typedef struct bq_measurements_t {
    int32_t temp_mc[BQ_NUM_GPIO];       // milli-degC, signed (below 0 C is negative), or BQ_TEMP_INVALID_MC
    uint32_t timestamp_ms; 
    uint16_t volt_mv[BQ_NUM_CELLS];
    uint8_t dev_addr;
} bq_measurements_t; 

// A command to the BQ chain, passed to bq_write() and bq_read()
typedef struct bq_msg_frame_t {
    bq_req_t req;
    uint8_t dev_addr;               // only used for single read/write
    uint16_t reg_addr;              // first register
    const uint8_t *data;            // write: the bytes to write. read: unused, may be NULL
    uint8_t len;                    // write: bytes in data (1-8). read: bytes to read per device (1-128)
} bq_msg_frame_t;

/* Long comm timeout: how long the BQ79616s wait without valid communication before they shut
 * down. The values are COMM_TIMEOUT_CONF[CTL_TIME] codes, BQ79616 §6.5.4.3.6 p124. */
typedef enum {
    BQ_COMM_TIMEOUT_DISABLED = 0,   // never: the stack stays awake if the MCU stops talking
    BQ_COMM_TIMEOUT_100MS    = 1,
    BQ_COMM_TIMEOUT_2S       = 2,
    BQ_COMM_TIMEOUT_10S      = 3,
    BQ_COMM_TIMEOUT_1MIN     = 4,
    BQ_COMM_TIMEOUT_10MIN    = 5,
    BQ_COMM_TIMEOUT_30MIN    = 6,
    BQ_COMM_TIMEOUT_1HR      = 7,
} bq_comm_timeout_t;

// Largest number of frames a read can return: a broadcast read gets one from every BQ79616 plus the bridge
#define BQ_MAX_RESPONSE_FRAMES  (BQ_MAX_DEVICES + 1U)

/**
 * @brief Name of a status code, e.g. "BQ_ERR_TIMEOUT", for logs and prints.
 * @return A string constant; "BQ_ERR_UNKNOWN" for a value outside bq_status_t.
 */
const char *bq_status_str(bq_status_t status);

/**
 * @brief Sets up the CRC unit, wake timer, pins and UART for the BQ chain. Does not wake the chips.
 * @param init Pins, UART and wake timer to use, and the number of BQ79616s in the stack.
 * @return BQ_OK; BQ_ERR_PARAM if init is invalid (NULL handle, unsupported port or timer, bad
 *         timer channel, wake_tim_complementary on a timer other than TIM8,
 *         num_devices > BQ_MAX_DEVICES); BQ_ERR_HAL if a peripheral fails to start.
 */
bq_status_t bq_init(const bq_init_struct_t init);

/**
 * @brief Wakes the chain: a WAKE ping to the BQ79600, then CONTROL1[SEND_WAKE] so the bridge sends
 *        the WAKE tone up the stack.
 * @details Blocks for the ping, the bridge's startup and num_devices * BQ_WAKE_PER_DEV_MS for the
 *          stack, then clears the BQ79600's FAULT_SYS, which the wake's digital reset leaves set
 *          (DRST). A WAKE resets the BQ79616s, so run bq_auto_address() and bq_config() afterwards.
 *          Call from a task, after bq_init().
 * @return BQ_OK; BQ_ERR_NOT_INIT; BQ_ERR_BUSY if a UART frame is still sending; or the error from
 *         the SEND_WAKE or FAULT_RST write.
 */
bq_status_t bq_wake(void);

/**
 * @brief Sends a write command.
 * @param msg The command. msg->req must be a write type, msg->data holds msg->len (1-8) bytes, and
 *            a single write's msg->dev_addr must be 0 (the bridge) or 1..num_devices.
 * @return BQ_OK once the frame is queued for sending; BQ_ERR_NOT_INIT; BQ_ERR_PARAM if msg is
 *         invalid; BQ_ERR_UART if the frame can't be queued.
 */
bq_status_t bq_write(const bq_msg_frame_t *msg);

/**
 * @brief Sends a read command and collects the replies.
 * @param msg        The command. msg->req must be a read type and msg->len is the bytes to read per
 *                   device (1-128). A single read's msg->dev_addr must be 0..num_devices.
 * @param frames     Caller's array for the replies: 1 frame for a single read, num_devices for a
 *                   stack read (top of stack first), num_devices + 1 for a broadcast read (bridge
 *                   last). BQ_MAX_RESPONSE_FRAMES entries always fit.
 * @param max_frames Number of entries in frames.
 * @return BQ_OK; BQ_ERR_NOT_INIT; BQ_ERR_PARAM if msg is invalid or frames is too small (both
 *         checked before anything is sent); BQ_ERR_UART if the command can't be queued;
 *         BQ_ERR_TIMEOUT, BQ_ERR_CRC or BQ_ERR_FRAME for a bad reply. On an error the contents
 *         of frames are undefined.
 */
bq_status_t bq_read(const bq_msg_frame_t *msg, bq_response_frame_t *frames, uint8_t max_frames);

/**
 * @brief Auto-addresses the chain: address 0 for the BQ79600 and 1..num_devices for the BQ79616s,
 *        with the last one set as top of stack. Follows BQ79600 Table 7-9 p27-28.
 * @details Reads the addresses back to check them. Run right after bq_wake(), from a task.
 * @return BQ_OK; BQ_ERR_NOT_INIT; BQ_ERR_PARAM if num_devices is 0; BQ_ERR_VERIFY if the
 *         addresses or the bridge's DEV_CONF1 read back wrong; or the failing read/write's error.
 */
bq_status_t bq_auto_address(void);

/**
 * @brief Configures every BQ79616 with stack writes, then reads each register back from every device.
 * @param comm_timeout Long comm timeout: after this long without valid communication the BQ79616s
 *                     go to SHUTDOWN. Pick one longer than the interval the stack is polled at.
 * @details Sets:
 *          - ACTIVE_CELL: BQ_NUM_CELLS cells in series
 *          - COMM_TIMEOUT_CONF: long comm timeout of comm_timeout, then SHUTDOWN; short comm
 *            timeout off
 *          - GPIO_CONF1..4: GPIO1-8 as ADC-only thermistor inputs (no OT/UT comparators)
 *          - CONTROL2: TSREF on, to bias the thermistor dividers, then waits for it to settle
 *          - CUST_CRC_HI/LO: the new customer CRC, since the changes above alter it; otherwise
 *            FAULT_OTP[CUST_CRC] stays set (copied from each device's own computed CRC)
 *          Then it clears the faults a wake and this config are expected to leave: each BQ79616's
 *          FAULT_SYS[DRST], FAULT_COMM3[FCOMM_DET] and FAULT_OTP CRC flags, and the bridge's
 *          FCOMM_DET, so FAULT_SUMMARY and NFAULT only show real faults afterwards.
 *          These registers go back to their OTP values on a digital reset (including a WAKE), so
 *          run this after every bq_wake() and bq_auto_address(), from a task.
 * @return BQ_OK; BQ_ERR_NOT_INIT; BQ_ERR_PARAM if num_devices is 0 or comm_timeout isn't a
 *         bq_comm_timeout_t value; BQ_ERR_VERIFY if any device didn't take a value;
 *         BQ_ERR_TIMEOUT if a device never finishes a CRC check; or the failing read/write's error.
 */
bq_status_t bq_config(bq_comm_timeout_t comm_timeout);

/**
 * @brief Runs one single-shot main ADC run on every BQ79616 and stores the cell voltages and GPIO
 *        (thermistor) readings in the driver, for bq_get_volts_mv() and bq_get_temps_mc().
 * @details Integer maths only. Run after bq_config(), from a task. A negative cell reading is
 *          stored as 0 mV. If it fails, the getters return BQ_ERR_ADC until the next successful run,
 *          so a partly updated set is never handed out.
 * @return BQ_OK; BQ_ERR_NOT_INIT; BQ_ERR_PARAM if num_devices is 0; BQ_ERR_FRAME if a reply
 *         names a device outside the chain; BQ_ERR_ADC if a cell or GPIO has never been converted;
 *         or the failing read/write's error.
 */
bq_status_t bq_read_volt_temp(void);

/**
 * @brief Copies one BQ79616's cell voltages from the last bq_read_volt_temp().
 * @param dev_addr Device address, 1..num_devices (1 is the bottom of the stack).
 * @param volts_mv Filled with cells 1..BQ_NUM_CELLS as volts_mv[cell - 1], in mV.
 * @return BQ_OK; BQ_ERR_NOT_INIT; BQ_ERR_PARAM if volts_mv is NULL or dev_addr isn't in the chain;
 *         BQ_ERR_ADC if there's no complete set of readings yet.
 */
bq_status_t bq_get_volts_mv(uint8_t dev_addr, uint16_t volts_mv[BQ_NUM_CELLS]);

/**
 * @brief Copies one BQ79616's GPIO (thermistor) readings from the last bq_read_volt_temp().
 * @param dev_addr Device address, 1..num_devices (1 is the bottom of the stack).
 * @param temps_mc Filled with GPIO1..BQ_NUM_GPIO as temps_mc[gpio - 1], in milli-degC. A GPIO whose
 *                 reading can't be a thermistor (none fitted, shorted, TSREF off) is
 *                 BQ_TEMP_INVALID_MC. Readings outside the thermistor's -40..105 C table are
 *                 clamped to -40 C or 105 C (see ntc_ohms_to_mc() in BQ79616.c).
 * @return BQ_OK; BQ_ERR_NOT_INIT; BQ_ERR_PARAM if temps_mc is NULL or dev_addr isn't in the chain;
 *         BQ_ERR_ADC if there's no complete set of readings yet.
 */
bq_status_t bq_get_temps_mc(uint8_t dev_addr, int32_t temps_mc[BQ_NUM_GPIO]);
