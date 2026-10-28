// Inits, wakes, auto-addresses and configures the BQ79600 + BQ79616 chain, then reads the cell
// voltages every BQ_TEST_READ_PERIOD_MS. The heartbeat LED (green) is on only while cell 1 of
// BQ79616 device 1 reads BQ_TEST_CELL1_MIN_MV..BQ_TEST_CELL1_MAX_MV, and off otherwise, including
// after any driver error. BPSL has no debug output yet, so in GDB:
// `p bq_test_fail_step`, `p bq_test_fail_status`, `p bq_test_cell1_mv`.
#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"
#include "pinDefs.h"
#include "BQ79616.h"
#include "task.h"

// Task configuration
#define BQ_TEST_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define BQ_TEST_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

#define BQ_TEST_NUM_DEVICES    1U   // BQ79616s behind the BQ79600 bridge
#define BQ_TEST_READ_PERIOD_MS 500U // how often cell 1 is read and the LED updated

// LED on only while cell 1 reads within these, inclusive
#define BQ_TEST_CELL1_MIN_MV 3000U
#define BQ_TEST_CELL1_MAX_MV 3300U

// The comm timeout must outlast the read period, or the stack shuts down between reads
#define BQ_TEST_COMM_TIMEOUT BQ_COMM_TIMEOUT_2S
_Static_assert(BQ_TEST_READ_PERIOD_MS < 2000U,
               "BQ_TEST_COMM_TIMEOUT would shut the stack down between reads");

// Static task buffers
static StaticTask_t xBqTestTaskBuffer;
static StackType_t xBqTestStack[BQ_TEST_TASK_STACK_SIZE];

// Where the test stopped, for GDB
typedef enum {
    BQ_TEST_NONE = 0,
    BQ_TEST_INIT,
    BQ_TEST_WAKE,
    BQ_TEST_AUTO_ADDRESS,
    BQ_TEST_CONFIG,
    BQ_TEST_READ,      // bq_read_volt_temp()
    BQ_TEST_GET_VOLTS, // bq_get_volts_mv()
} bq_test_step_t;

volatile bq_test_step_t bq_test_fail_step = BQ_TEST_NONE;
volatile bq_status_t bq_test_fail_status = BQ_OK; // driver error behind the failure
volatile uint16_t bq_test_cell1_mv = 0U;          // last cell 1 reading

// Turns the LED off, records where and why for GDB, and stops the test
static void fail(bq_test_step_t step, bq_status_t status) {
    StatusLEDs_SetHeartbeat(false);
    bq_test_fail_status = status;
    bq_test_fail_step = step;
    vTaskSuspend(NULL);
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
    bq_status_t status;

    // can only fail if the color is out of range, which would be a bug
    if (StatusLEDs_SetHeartbeatColor(RGB_GREEN) != LED_OK) {
        configASSERT(pdFALSE);
    }

    status = bq_init(bq_cfg);
    if (status != BQ_OK) {
        fail(BQ_TEST_INIT, status);
    }

    status = bq_wake();
    if (status != BQ_OK) {
        fail(BQ_TEST_WAKE, status);
    }

    status = bq_auto_address();
    if (status != BQ_OK) {
        fail(BQ_TEST_AUTO_ADDRESS, status);
    }

    status = bq_config(BQ_TEST_COMM_TIMEOUT);
    if (status != BQ_OK) {
        fail(BQ_TEST_CONFIG, status);
    }

    // Reading every period also keeps the comm timeout from shutting the stack down
    while (true) {
        status = bq_read_volt_temp();
        if (status != BQ_OK) {
            fail(BQ_TEST_READ, status);
        }

        status = bq_get_volts_mv(1U, cell_mv);
        if (status != BQ_OK) {
            fail(BQ_TEST_GET_VOLTS, status);
        }

        bq_test_cell1_mv = cell_mv[0];
        StatusLEDs_SetHeartbeat(cell_mv[0] >= BQ_TEST_CELL1_MIN_MV &&
                                cell_mv[0] <= BQ_TEST_CELL1_MAX_MV);

        vTaskDelay(pdMS_TO_TICKS(BQ_TEST_READ_PERIOD_MS));
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

    TaskHandle_t handle =
        xTaskCreateStatic(vBqTestTask, "BQ Test", BQ_TEST_TASK_STACK_SIZE, NULL,
                          BQ_TEST_TASK_PRIORITY, xBqTestStack, &xBqTestTaskBuffer);
    configASSERT(handle != NULL);
    (void)handle;

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    Error_Handler();
}
