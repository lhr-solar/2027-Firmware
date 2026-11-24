#include "stm32xx_hal.h"
#include "log.h"

/* logging task */
StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

#define BOARD_NAME "jensen_huang"
// assign macros to CDC ports
#define PORT_A_NAME   "serial_a"
#define SERIAL_PORT_A 0
#define PORT_B_NAME   "serial_b"
#define SERIAL_PORT_B 1

/* Polls for USB events, should be highest prio */
static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}

/* Performs LOG calls */
void TxTask(void* argument) {
    while (1) {
        LOG(SERIAL_PORT_A, L_INFO, "touch ravi for me!\r\n");
        vTaskDelay(pdMS_TO_TICKS(500));

        LOG(SERIAL_PORT_B, L_INFO, "again!\r\n");
        vTaskDelay(pdMS_TO_TICKS(500));

        LOG(SERIAL_PORT_B, L_ERROR, "[REDACTED]\r\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    // USB Init
    MX_USB_PCD_Init();
    log_init_t log_cfg = {
        // bitmasks OR'ed to select logging levels
        .levels = LOG_BIT(L_INFO) | LOG_BIT(L_WARN),
        .board_name = BOARD_NAME,
        // in order of cdc port 0 and 1
        .port_names = {PORT_A_NAME, PORT_B_NAME},
    };
    // tusb must be up before the poll task runs, tud_task() returns immediately otherwise
    log_init(&log_cfg);

    xTaskCreateStatic(poll_events, "Poll Event Queue", configMINIMAL_STACK_SIZE, NULL,
                      tskIDLE_PRIORITY + 3, pollEventsStack, &pollEventsTaskBuffer);

    xTaskCreateStatic(TxTask, "TX", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2,
                      txTaskStack, &txTaskBuffer);

    vTaskStartScheduler();

    while (1) {
        // ur chopped
    }
}
