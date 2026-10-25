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
 * It starts by showing red, green, then blue for COLOR_CHECK_MS each so every
 * channel can be checked by eye. The race then runs for TEST_DURATION_MS, then the writers stop and the LED shows
 * the result. While it runs the LED looks mostly red, since the red writer
 * never sleeps and overwrites the others within microseconds.
 *
 * Pass:  solid green after the run.
 * Fail:  solid white (a mixed colour or a stuck mutex). Can happen early.
 * Crash: still red after the run, meaning the checker never finished.
 *
 * The result is shown on the LED instead of over UART because the usual printf
 * UART (USART2) shares PA2 with the green channel.
 */

#define RGB_TIMEOUT_MS   50U
#define CHECK_PERIOD_MS  1U
#define FAIL_SETTLE_MS   20U
#define TEST_DURATION_MS 10000U
#define COLOR_CHECK_MS   500U
#define TASK_STACK_WORDS (configMINIMAL_STACK_SIZE * 2)

typedef struct {
    rgb_led_color_t color;
    TickType_t delay_ticks; // 0 = never block
} writer_args_t;

static TIM_HandleTypeDef htim_rgb;
static rgb_led_handle_t rgb_led;
static volatile bool failed = false;
static volatile bool started = false; // set by the checker after the colour check
static volatile bool done = false;    // set by the checker when the run ends

// The lowest-priority writer never blocks, so it is almost always the one
// mid-write when a higher-priority writer or the checker wakes up. The others
// preempt it whenever their delay ends.
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
    if (lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b) {
        return false;
    }
    return true;
}

// Keeps setting one pure colour until the run ends or a failure is flagged. A timeout on the
// mutex counts as a failure, since nobody should hold it for 50 ms.
static void writer_task(void* argument) {
    const writer_args_t* args = argument;

    while (!started) {
        vTaskDelay(1);
    }
    while (!failed && !done) {
        if (rgb_led_set(&rgb_led, args->color, pdMS_TO_TICKS(RGB_TIMEOUT_MS)) != RGB_LED_OK) {
            failed = true;
            break;
        }
        if (args->delay_ticks != 0U) {
            vTaskDelay(args->delay_ticks);
        }
    }
    vTaskSuspend(NULL);
}

// Highest priority, so it can wake while a writer is partway through a set.
// Fails if the colour read back is anything but off or one writer's colour.
// After TEST_DURATION_MS it stops the writers and shows green (pass) or white.
static void checker_task(void* argument) {
    (void)argument;

    // Colour check: show each writer's colour so a dead channel is easy to spot.
    // The writers are held back until this finishes.
    for (uint32_t i = 0; i < NUM_WRITERS && !failed; i++) {
        if (rgb_led_set(&rgb_led, writer_args[i].color, pdMS_TO_TICKS(RGB_TIMEOUT_MS)) !=
            RGB_LED_OK) {
            failed = true;
        }
        vTaskDelay(pdMS_TO_TICKS(COLOR_CHECK_MS));
    }
    started = true;

    const TickType_t start = xTaskGetTickCount();

    while (!failed && (xTaskGetTickCount() - start) < pdMS_TO_TICKS(TEST_DURATION_MS)) {
        rgb_led_color_t seen;
        if (rgb_led_get(&rgb_led, &seen, pdMS_TO_TICKS(RGB_TIMEOUT_MS)) != RGB_LED_OK) {
            failed = true;
            break;
        }
        // RGB_LED_OFF is the value before any writer has run
        bool valid = is_color(seen, RGB_LED_OFF);
        for (uint32_t i = 0; i < NUM_WRITERS; i++) {
            if (is_color(seen, writer_args[i].color)) {
                valid = true;
            }
        }
        if (!valid) {
            failed = true;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(CHECK_PERIOD_MS));
    }

    // Let the writers see the flag and stop, then latch the result: green for
    // pass, white for fail. Write the registers directly: a failure may be a
    // stuck mutex, and waiting on it here would leave the LED frozen on a
    // colour instead of showing the result.
    done = true;
    vTaskDelay(pdMS_TO_TICKS(FAIL_SETTLE_MS));
    const uint32_t full = htim_rgb.Init.Period;
    uint32_t red_blue = 0U;
    if (failed) {
        red_blue = full;
    }
    taskENTER_CRITICAL();
    __HAL_TIM_SET_COMPARE(&htim_rgb, rgb_led.red_channel, red_blue);
    __HAL_TIM_SET_COMPARE(&htim_rgb, rgb_led.green_channel, full);
    __HAL_TIM_SET_COMPARE(&htim_rgb, rgb_led.blue_channel, red_blue);
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
