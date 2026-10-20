#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "RGB_LED.h"

/*
 * RGB LED PWM test, using the default wiring in RGB_LED.h:
 *   PA0 -> blue, PA1 -> red, PA2 -> green, common cathode to GND.
 *
 * Shows red, orange, yellow, green, blue, indigo, violet for 1 s each so the
 * channel -> pin mapping can be checked by eye, turns off for 0.5 s, then fades
 * around the colour wheel forever (~2.56 s per lap).
 */

#define RAINBOW_STEP_MS  1000U
#define RAINBOW_PAUSE_MS 500U
#define FADE_STEP_MS     10U

static TIM_HandleTypeDef htim_rgb;
static rgb_led_handle_t rgb_led;

static StaticTask_t rgb_task_buffer;
static StackType_t rgb_task_stack[configMINIMAL_STACK_SIZE * 2];

// Runs the whole test: rainbow check first, then the endless colour-wheel fade.
// vTaskDelayUntil keeps the fade steps evenly spaced even if a set call waits.
static void rgb_task(void* argument) {
    (void)argument;

    static const rgb_led_color_t rainbow[] = {RGB_LED_RED,   RGB_LED_ORANGE, RGB_LED_YELLOW,
                                              RGB_LED_GREEN, RGB_LED_BLUE,   RGB_LED_INDIGO,
                                              RGB_LED_VIOLET};
    for (uint32_t i = 0; i < sizeof(rainbow) / sizeof(rainbow[0]); i++) {
        rgb_led_set(&rgb_led, rainbow[i], portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(RAINBOW_STEP_MS));
    }
    rgb_led_set(&rgb_led, RGB_LED_OFF, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(RAINBOW_PAUSE_MS));

    uint8_t phase = 0U;
    TickType_t last_wake = xTaskGetTickCount();
    while (1) {
        rgb_led_set(&rgb_led, rgb_led_wheel(phase++), portMAX_DELAY);
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(FADE_STEP_MS));
    }
}

// Set up clocks and the LED, start the test task, then hand over to FreeRTOS.
int main(void) {
    HAL_Init();
    SystemClock_Config();

    const rgb_led_config_t cfg = RGB_LED_DEFAULT_CONFIG;
    if (rgb_led_init(&rgb_led, &htim_rgb, &cfg) != RGB_LED_OK) {
        Error_Handler();
    }

    xTaskCreateStatic(rgb_task, "RGB", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1,
                      rgb_task_stack, &rgb_task_buffer);

    vTaskStartScheduler();

    while (1) {
        // scheduler never returns
    }
    return 0;
}
