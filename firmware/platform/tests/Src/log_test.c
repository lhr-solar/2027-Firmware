#include "stm32xx_hal.h"
#include "log.h"

/* logging task */
StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];

#define BOARD_NAME "jensen_huang"
// assign macros to CDC ports
#define CDC_PORT_A_NAME   "serial_a"
#define CDC_SERIAL_PORT_A 0
#define CDC_PORT_B_NAME   "serial_b"
#define CDC_SERIAL_PORT_B 1

/* Performs LOG calls */
void TxTask(void* argument) {
    while (1) {
        LOG(CDC_SERIAL_PORT_A, L_INFO, "touch ravi for me!\r\n");
        vTaskDelay(pdMS_TO_TICKS(500));

        LOG(CDC_SERIAL_PORT_B, L_INFO, "again!\r\n");
        vTaskDelay(pdMS_TO_TICKS(500));

        LOG(CDC_SERIAL_PORT_B, L_ERROR, "[REDACTED]\r\n");
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
        // .levels = LOG_BIT(L_INFO) | LOG_BIT(L_WARN),
        .board_name = BOARD_NAME,
        // in order of cdc port 0 and 1
        .port_names = {CDC_PORT_A_NAME, CDC_PORT_B_NAME},
    };
    // init logging structs, also creates USB polling task
    log_init(&log_cfg);

    xTaskCreateStatic(TxTask, "TX", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2,
                      txTaskStack, &txTaskBuffer);

    vTaskStartScheduler();

    while (1) {
        // ur chopped
    }
}
