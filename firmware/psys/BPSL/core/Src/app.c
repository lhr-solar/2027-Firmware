#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"

int main(void) {
    if (HAL_Init() != HAL_OK) {
        Error_Handler();
    }

    SystemClock_Config();

    if (StatusLEDs_Init() != LED_OK) {
        Error_Handler();
    }

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    Error_Handler();
}
