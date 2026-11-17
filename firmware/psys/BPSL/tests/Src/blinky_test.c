// Cycles the RGB heartbeat LED through red, green, and blue, 500 ms each.
#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"
#include "BPS_tasks.h"

#define BLINKY_COLOR_PERIOD pdMS_TO_TICKS(500)

// cycle through red, green, and blue
void Task_Test(void* pvParameters) {
    (void)pvParameters;
    rgb_color_t color = RGB_RED;

    while (true) {
        // can only fail if color is out of range, which would be a bug
        if (StatusLEDs_SetHeartbeatColor(color) != LED_OK) {
            configASSERT(pdFALSE);
        }

        StatusLEDs_SetHeartbeat(true);
        vTaskDelay(BLINKY_COLOR_PERIOD);

        // move to the next color, wrapping back to red after blue
        color = (color + 1) % RGB_NUM_COLORS;
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

    TaskHandle_t handle = TASK_CREATE(TEST, Test);
    configASSERT(handle != NULL);
    (void)handle;

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    Error_Handler();
}
