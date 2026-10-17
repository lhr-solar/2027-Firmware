// Cycles the RGB heartbeat LED through red, green, and blue, 500 ms each.
#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"

// Task configuration
#define BLINKY_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINKY_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define BLINKY_COLOR_PERIOD    pdMS_TO_TICKS(500)

// Static task buffers
static StaticTask_t xBlinkyTaskBuffer;
static StackType_t xBlinkyStack[BLINKY_TASK_STACK_SIZE];

// cycle through red, green, and blue
static void vBlinkyTask(void* pvParameters) {
    (void)pvParameters;
    rgb_color_t color = RGB_RED;

    while (true) {
        // can only fail if color is out of range, which would be a bug
        led_state_t status = StatusLEDs_SetHeartbeatColor(color);
        configASSERT(status == LED_OK);
        (void)status; // avoids an unused-variable warning if configASSERT is compiled out

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

    TaskHandle_t handle = xTaskCreateStatic(vBlinkyTask, "Blinky", BLINKY_TASK_STACK_SIZE, NULL,
                                            BLINKY_TASK_PRIORITY, xBlinkyStack, &xBlinkyTaskBuffer);
    configASSERT(handle != NULL);
    (void)handle;

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    Error_Handler();
}
