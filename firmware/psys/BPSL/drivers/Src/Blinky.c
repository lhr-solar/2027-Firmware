#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"
#include "Blinky.h"

// Task configuration
#define BLINKY_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINKY_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define BLINKY_COLOR_PERIOD pdMS_TO_TICKS(500)

// Static task buffers
static StaticTask_t xBlinkyTaskBuffer;
static StackType_t xBlinkyStack[BLINKY_TASK_STACK_SIZE];

// cycle through red, green, and blue
static void vBlinkyTask(void* pvParameters) {
    (void)pvParameters;
    rgb_color_t color = RGB_RED;

    while (true) {
        if (StatusLEDs_SetHeartbeatColor(color) != LED_OK) {
            Error_Handler();
        }
        StatusLEDs_SetHeartbeat(true);
        vTaskDelay(BLINKY_COLOR_PERIOD);

        // move to the next color, wrapping back to red after blue
        color = (color + 1) % RGB_NUM_COLORS;
    }
}

void Blinky_Init(void) {
    TaskHandle_t handle = xTaskCreateStatic(
        vBlinkyTask,
        "Blinky",
        BLINKY_TASK_STACK_SIZE,
        NULL,
        BLINKY_TASK_PRIORITY,
        xBlinkyStack,
        &xBlinkyTaskBuffer);

    if (handle == NULL) {
        Error_Handler();
    }
}
