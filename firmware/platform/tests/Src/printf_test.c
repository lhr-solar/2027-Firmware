#include "stm32xx_hal.h"
#include "tusb_port.h"
#include "printf.h"

/* actual printf task */
StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}

void TxTask(void* argument) {
    while (1) {
        printf("touch ravi for me!\r\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_USB_PCD_Init();
    // tusb must be up before the poll task runs, tud_task() returns immediately otherwise
    printf_init();

    xTaskCreateStatic(poll_events, "Poll Event Queue", configMINIMAL_STACK_SIZE, NULL,
                      tskIDLE_PRIORITY + 3, pollEventsStack, &pollEventsTaskBuffer);

    xTaskCreateStatic(TxTask, "TX", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2,
                      txTaskStack, &txTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
