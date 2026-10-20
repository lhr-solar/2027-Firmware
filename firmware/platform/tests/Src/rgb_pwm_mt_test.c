#include <stdbool.h>

#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "RGB_LED.h"

/*
 * RGB LED multi-task test: three tasks fight over one LED.
 *
 * Writers at three priorities each keep setting their own pure colour (red,
 * green, blue) and preempt each other. A checker at the highest priority reads
 * the colour back and confirms it is exactly one of the three. A mixed colour
 * would mean two writes interleaved; a timeout would mean the mutex got stuck.
 *
 * Pass: the LED keeps flickering between red, green and blue.
 * Fail: the LED goes solid white and stays that way.
 *
 * The result is shown on the LED instead of over UART because the usual printf
 * UART (USART2) shares PA2 with the green channel.
 */

#define RGB_TIMEOUT_MS   50U
#define CHECK_PERIOD_MS  1U
#define FAIL_SETTLE_MS   20U
#define TASK_STACK_WORDS (configMINIMAL_STACK_SIZE * 2)

typedef struct {
    rgb_led_color_t color;
    TickType_t delay_ticks; // 0 = never block, just yield
} writer_args_t;

static TIM_HandleTypeDef htim_rgb;
static rgb_led_handle_t rgb_led;
static volatile bool failed = false;

// The lowest-priority writer never blocks, so it is almost always the one
// mid-write when a higher-priority writer or the checker wakes up.
static const writer_args_t writer_args[] = {
    {RGB_LED_RED, 0},
    {RGB_LED_GREEN, 1},
    {RGB_LED_BLUE, 3},
};
#define NUM_WRITERS (sizeof(writer_args) / sizeof(writer_args[0]))

static StaticTask_t writer_buffers[NUM_WRITERS];
static StackType_t writer_stacks[NUM_WRITERS][TASK_STACK_WORDS];
static StaticTask_t checker_buffer;
static StackType_t checker_stack[TASK_STACK_WORDS];

// True when two colours match on all three channels.
static bool is_color(rgb_led_color_t lhs, rgb_led_color_t rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

// Keeps setting one pure colour until a failure is flagged. A timeout on the
// mutex counts as a failure, since nobody should hold it for 50 ms.
static void writer_task(void* argument) {
    const writer_args_t* args = argument;

    while (!failed) {
        if (rgb_led_set(&rgb_led, args->color, pdMS_TO_TICKS(RGB_TIMEOUT_MS)) != RGB_LED_OK) {
            failed = true;
            break;
        }
        if (args->delay_ticks == 0U) {
            taskYIELD();
        } else {
            vTaskDelay(args->delay_ticks);
        }
    }
    vTaskSuspend(NULL);
}

// Highest priority, so it can wake while a writer is partway through a set.
// Fails if the colour read back is anything but off or one writer's colour.
static void checker_task(void* argument) {
    (void)argument;

    while (!failed) {
        rgb_led_color_t seen;
        if (rgb_led_get(&rgb_led, &seen, pdMS_TO_TICKS(RGB_TIMEOUT_MS)) != RGB_LED_OK) {
            failed = true;
            break;
        }
        // RGB_LED_OFF is the value before any writer has run
        bool valid = is_color(seen, RGB_LED_OFF);
        for (uint32_t i = 0; i < NUM_WRITERS; i++) {
            valid = valid || is_color(seen, writer_args[i].color);
        }
        if (!valid) {
            failed = true;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(CHECK_PERIOD_MS));
    }

    // Let the writers see the flag and stop, then latch white. Write the
    // registers directly: the failure may be a stuck mutex, and waiting on it
    // here would leave the LED frozen on a colour instead of showing the fail.
    vTaskDelay(pdMS_TO_TICKS(FAIL_SETTLE_MS));
    taskENTER_CRITICAL();
    __HAL_TIM_SET_COMPARE(&htim_rgb, RGB_LED_DEFAULT_RED_CH, htim_rgb.Init.Period);
    __HAL_TIM_SET_COMPARE(&htim_rgb, RGB_LED_DEFAULT_GREEN_CH, htim_rgb.Init.Period);
    __HAL_TIM_SET_COMPARE(&htim_rgb, RGB_LED_DEFAULT_BLUE_CH, htim_rgb.Init.Period);
    taskEXIT_CRITICAL();
    vTaskSuspend(NULL);
}

// Set up the LED, start three writers at rising priorities and the checker
// above them all, then hand over to FreeRTOS.
int main(void) {
    HAL_Init();
    SystemClock_Config();

    const rgb_led_config_t cfg = RGB_LED_DEFAULT_CONFIG;
    if (rgb_led_init(&rgb_led, &htim_rgb, &cfg) != RGB_LED_OK) {
        Error_Handler();
    }

    for (uint32_t i = 0; i < NUM_WRITERS; i++) {
        xTaskCreateStatic(writer_task, "RGBW", TASK_STACK_WORDS, (void*)&writer_args[i],
                          tskIDLE_PRIORITY + 1 + i, writer_stacks[i], &writer_buffers[i]);
    }
    xTaskCreateStatic(checker_task, "RGBC", TASK_STACK_WORDS, NULL,
                      tskIDLE_PRIORITY + 1 + NUM_WRITERS, checker_stack, &checker_buffer);

    vTaskStartScheduler();

    while (1) {
        // scheduler never returns
    }
    return 0;
}
