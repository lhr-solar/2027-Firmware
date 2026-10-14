// Cycles the RGB heartbeat LED through red, green, and blue, 500 ms each.
#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"
#include "Blinky.h"

int main(void) {
    if (HAL_Init() != HAL_OK) {
        Error_Handler();
    }

    SystemClock_Config();

    if (StatusLEDs_Init() != LED_OK) {
        Error_Handler();
    }

    Blinky_Init();

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    Error_Handler();
}
