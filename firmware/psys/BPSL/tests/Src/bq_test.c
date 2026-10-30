// Inits, wakes, auto-addresses and configures the BQ79600 + BQ79616 chain, then reads the cell
// voltages every BQ_TEST_READ_PERIOD_MS. The heartbeat LED (green) is on only while cell 1 of
// BQ79616 device 1 reads BQ_TEST_CELL1_MIN_MV..BQ_TEST_CELL1_MAX_MV, and off otherwise, including
// after any driver error.
// Progress and readings are printed on the USB serial port (the board shows up as two serial ports,
// e.g. /dev/ttyACM0 and /dev/ttyACM1; printf writes to both). printf only sends to a port with a
// terminal open, so anything printed before you connect is lost; the readings repeat every loop.
// Also in GDB: `p bq_test_fail_step`, `p bq_test_fail_status`, `p bq_test_cell1_mv`.
#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"
#include "pinDefs.h"
#include "BQ79616.h"
#include "printf.h"
#include "task.h"
#include "tusb.h"

// Task configuration
#define BQ_TEST_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 4) // printf needs the extra room
#define BQ_TEST_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

// USB task: above the test task, so printed data goes out promptly. tud_task() blocks until there's
// a USB event, so it doesn't starve the test task.
#define USB_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define USB_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)

// The USB interrupt hands events to tud_task() through FreeRTOS, so it must be at or below
// configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY's urgency (a numerically higher priority)
#define USB_IRQ_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)

#define BQ_TEST_NUM_DEVICES    1U   // BQ79616s behind the BQ79600 bridge
#define BQ_TEST_READ_PERIOD_MS 500U // how often cell 1 is read and the LED updated

// LED on only while cell 1 reads within these, inclusive
#define BQ_TEST_CELL1_MIN_MV 3000U
#define BQ_TEST_CELL1_MAX_MV 3300U

// The comm timeout must outlast the read period, or the stack shuts down between reads
#define BQ_TEST_COMM_TIMEOUT BQ_COMM_TIMEOUT_2S
_Static_assert(BQ_TEST_READ_PERIOD_MS < 2000U,
               "BQ_TEST_COMM_TIMEOUT would shut the stack down between reads");

// One printed line. Each loop prints two lines of about 60 bytes: TinyUSB holds 64 bytes per port
// plus one 64-byte packet in flight, and drops what doesn't fit until the USB task sends it.
#define BQ_TEST_LINE_LEN 80U

// Static task buffers
static StaticTask_t xBqTestTaskBuffer;
static StackType_t xBqTestStack[BQ_TEST_TASK_STACK_SIZE];
static StaticTask_t xUsbTaskBuffer;
static StackType_t xUsbStack[USB_TASK_STACK_SIZE];

static PCD_HandleTypeDef hpcd_USB_FS;

// Where the test stopped, for GDB
typedef enum {
    BQ_TEST_NONE = 0,
    BQ_TEST_INIT,
    BQ_TEST_WAKE,
    BQ_TEST_AUTO_ADDRESS,
    BQ_TEST_CONFIG,
    BQ_TEST_READ,      // bq_read_volt_temp()
    BQ_TEST_GET_VOLTS, // bq_get_volts_mv()
    BQ_TEST_GET_TEMPS, // bq_get_temps_mc()
} bq_test_step_t;

volatile bq_test_step_t bq_test_fail_step = BQ_TEST_NONE;
volatile bq_status_t bq_test_fail_status = BQ_OK; // driver error behind the failure
volatile uint16_t bq_test_cell1_mv = 0U;          // last cell 1 reading

// Turns the LED off, prints and records where and why for GDB, and stops the test
static void fail(bq_test_step_t step, const char* what, bq_status_t status) {
    StatusLEDs_SetHeartbeat(false);
    printf("FAIL: %s returned %s\r\n", what, bq_status_str(status));
    bq_test_fail_status = status;
    bq_test_fail_step = step;
    vTaskSuspend(NULL);
}

// Prints the cell voltages in mV on one line, and whether cell 1 is in range (LED on)
static void print_cells(const uint16_t cell_mv[BQ_NUM_CELLS], bool led_on) {
    char line[BQ_TEST_LINE_LEN];
    int len = snprintf(line, sizeof(line), "cells mV:");

    for (uint8_t cell = 0U; cell < BQ_NUM_CELLS && len > 0 && (size_t)len < sizeof(line); cell++) {
        len += snprintf(&line[len], sizeof(line) - (size_t)len, " %u", cell_mv[cell]);
    }
    printf("%s  LED %s\r\n", line, led_on ? "on" : "off");
}

// Prints the GPIO (thermistor) temperatures on one line, in C to 0.1 C, or "--" for a GPIO with no
// valid thermistor reading
static void print_temps(const int32_t temp_mc[BQ_NUM_GPIO]) {
    char line[BQ_TEST_LINE_LEN];
    int len = snprintf(line, sizeof(line), "temps C:");

    for (uint8_t gpio = 0U; gpio < BQ_NUM_GPIO && len > 0 && (size_t)len < sizeof(line); gpio++) {
        const int32_t mc = temp_mc[gpio];
        if (mc == BQ_TEMP_INVALID_MC) {
            len += snprintf(&line[len], sizeof(line) - (size_t)len, " --");
        } else {
            // mC split into degrees and tenths, so no floating point is needed
            const uint32_t mag = (mc < 0) ? (uint32_t)(-(int64_t)mc) : (uint32_t)mc;
            len +=
                snprintf(&line[len], sizeof(line) - (size_t)len, " %s%lu.%lu", (mc < 0) ? "-" : "",
                         (unsigned long)(mag / 1000U), (unsigned long)((mag % 1000U) / 100U));
        }
    }
    printf("%s\r\n", line);
}

static void vBqTestTask(void* pvParameters) {
    (void)pvParameters;
    const bq_init_struct_t bq_cfg = {
        .tx_pin = BQ79600_UART_TX_PIN,
        .rx_pin = BQ79600_UART_RX_PIN,
        .tx_rx_port = BQ79600_UART_PORT,
        .fault_pin = BQ79600_NFAULT_PIN,
        .fault_port = BQ79600_NFAULT_PORT,
        .huart = husart3,
        .uart_af = BQ79600_UART_AF,
        .wake_tim = BQ79600_WAKE_TIM,
        .wake_tim_channel = BQ79600_WAKE_TIM_CHANNEL,
        .wake_tim_af = BQ79600_WAKE_TIM_AF,
        .wake_tim_complementary = BQ79600_WAKE_TIM_COMPLEMENTARY,
        .num_devices = BQ_TEST_NUM_DEVICES,
    };
    uint16_t cell_mv[BQ_NUM_CELLS];
    int32_t temp_mc[BQ_NUM_GPIO];
    bq_status_t status;

    // can only fail if the color is out of range, which would be a bug
    if (StatusLEDs_SetHeartbeatColor(RGB_GREEN) != LED_OK) {
        configASSERT(pdFALSE);
    }

    printf("\r\nBQ test start, %u BQ79616(s)\r\n", BQ_TEST_NUM_DEVICES);

    status = bq_init(bq_cfg);
    if (status != BQ_OK) {
        fail(BQ_TEST_INIT, "bq_init", status);
    }
    printf("bq_init OK\r\n");

    status = bq_wake();
    if (status != BQ_OK) {
        fail(BQ_TEST_WAKE, "bq_wake", status);
    }
    printf("bq_wake OK\r\n");

    status = bq_auto_address();
    if (status != BQ_OK) {
        fail(BQ_TEST_AUTO_ADDRESS, "bq_auto_address", status);
    }
    printf("bq_auto_address OK\r\n");

    status = bq_config(BQ_TEST_COMM_TIMEOUT);
    if (status != BQ_OK) {
        fail(BQ_TEST_CONFIG, "bq_config", status);
    }
    printf("bq_config OK, cell 1 LED range %u-%u mV\r\n", BQ_TEST_CELL1_MIN_MV,
           BQ_TEST_CELL1_MAX_MV);

    // Reading every period also keeps the comm timeout from shutting the stack down
    while (true) {
        status = bq_read_volt_temp();
        if (status != BQ_OK) {
            fail(BQ_TEST_READ, "bq_read_volt_temp", status);
        }

        status = bq_get_volts_mv(1U, cell_mv);
        if (status != BQ_OK) {
            fail(BQ_TEST_GET_VOLTS, "bq_get_volts_mv", status);
        }

        status = bq_get_temps_mc(1U, temp_mc);
        if (status != BQ_OK) {
            fail(BQ_TEST_GET_TEMPS, "bq_get_temps_mc", status);
        }

        bq_test_cell1_mv = cell_mv[0];
        const bool led_on =
            cell_mv[0] >= BQ_TEST_CELL1_MIN_MV && cell_mv[0] <= BQ_TEST_CELL1_MAX_MV;
        StatusLEDs_SetHeartbeat(led_on);

        print_cells(cell_mv, led_on);
        print_temps(temp_mc);

        vTaskDelay(pdMS_TO_TICKS(BQ_TEST_READ_PERIOD_MS));
    }
}

// Runs TinyUSB's device stack: enumeration, and sending what printf queues
static void vUsbTask(void* pvParameters) {
    (void)pvParameters;

    while (true) {
        tud_task(); // blocks until the USB interrupt queues an event
    }
}

// Clock and interrupts for the USB peripheral; HAL_PCD_Init() calls this
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd) {
    RCC_PeriphCLKInitTypeDef clk = {0};

    if (hpcd->Instance != USB) {
        return;
    }

    // 48 MHz from the PLL's Q output, see SystemClock_Config()
    clk.PeriphClockSelection = RCC_PERIPHCLK_USB;
    clk.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&clk) != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_USB_CLK_ENABLE();

    // TinyUSB's handlers for these are in platform/common/Src/tusb_port.c
    HAL_NVIC_SetPriority(USB_HP_IRQn, USB_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(USB_HP_IRQn);
    HAL_NVIC_SetPriority(USB_LP_IRQn, USB_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(USB_LP_IRQn);
}

// USB full-speed device on PA11/PA12, as in platform/tests/Src/tusb_cdc_test.c
static void usb_init(void) {
    hpcd_USB_FS.Instance = USB;
    hpcd_USB_FS.Init.dev_endpoints = 8;
    hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_USB_FS.Init.Sof_enable = DISABLE;
    hpcd_USB_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_FS.Init.lpm_enable = DISABLE;
    hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
    if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK) {
        Error_Handler();
    }
}

int main(void) {
    if (HAL_Init() != HAL_OK) {
        Error_Handler();
    }

    SystemClock_Config();

    if (StatusLEDs_Init() != LED_OK) {
        Error_Handler();
    }

    // USB peripheral first: printf_init() starts TinyUSB on it
    usb_init();
    if (!printf_init()) {
        Error_Handler();
    }

    TaskHandle_t handle = xTaskCreateStatic(vUsbTask, "USB", USB_TASK_STACK_SIZE, NULL,
                                            USB_TASK_PRIORITY, xUsbStack, &xUsbTaskBuffer);
    configASSERT(handle != NULL);

    handle = xTaskCreateStatic(vBqTestTask, "BQ Test", BQ_TEST_TASK_STACK_SIZE, NULL,
                               BQ_TEST_TASK_PRIORITY, xBqTestStack, &xBqTestTaskBuffer);
    configASSERT(handle != NULL);
    (void)handle;

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    Error_Handler();
}
