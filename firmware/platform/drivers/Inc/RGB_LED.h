/**
 * @file RGB_LED.h
 * @brief Driver for a common-cathode RGB LED on three PWM channels of one timer.
 *
 * Call order:
 *   1. rgb_led_init()  - before or after the scheduler starts
 *   2. rgb_led_set() / rgb_led_get()  - from tasks only
 *
 * Each color channel is 0-255 and goes through a gamma 2.2 table onto a
 * 12-bit, ~20 kHz PWM, so equal steps look like equal changes in brightness.
 * A mutex in the handle keeps tasks from interleaving their register writes.
 *
 * Hardware notes:
 *  - Don't use the RGB LED timer for anything else.
 *  - Only APB1 timers with three or more channels are supported (TIM2-TIM5).
 *  - Clocks and GPIO are set up inside rgb_led_init(). The driver doesn't
 *    define any HAL_TIM_*Msp* callbacks, so it won't clash with a board that
 *    defines its own.
 *  - The default pins (PA0-PA2, TIM2 AF1) are the same on both MCUs we use:
 *    STM32G473CET6 (LQFP48) and STM32G473VET6 (LQFP100).
 *  - rgb_led_set() and rgb_led_get() take a mutex and must not be called from
 *    an ISR.
 */

#pragma once

#include <stdint.h>

#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef enum {
    RGB_LED_OK,          // success
    RGB_LED_INVALID_PTR, // a pointer argument was NULL
    RGB_LED_ERROR,       // unsupported timer or HAL failure
    RGB_LED_BUSY         // mutex not obtained before the timeout
} rgb_led_status_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_led_color_t;

#define RGB_LED_RED    ((rgb_led_color_t){255, 0, 0})
#define RGB_LED_ORANGE ((rgb_led_color_t){255, 127, 0})
#define RGB_LED_YELLOW ((rgb_led_color_t){255, 255, 0})
#define RGB_LED_GREEN  ((rgb_led_color_t){0, 255, 0})
#define RGB_LED_BLUE   ((rgb_led_color_t){0, 0, 255})
#define RGB_LED_INDIGO ((rgb_led_color_t){75, 0, 130})
#define RGB_LED_VIOLET ((rgb_led_color_t){148, 0, 211})
#define RGB_LED_WHITE  ((rgb_led_color_t){255, 255, 255})
#define RGB_LED_OFF    ((rgb_led_color_t){0, 0, 0})

// Which timer, channels and pins the LED is wired to. All three pins must be
// on the same port and use the same alternate function.
typedef struct {
    TIM_TypeDef* timer;
    uint32_t red_channel;
    uint32_t green_channel;
    uint32_t blue_channel;
    GPIO_TypeDef* port;
    uint16_t red_pin;
    uint16_t green_pin;
    uint16_t blue_pin;
    uint8_t alternate; // GPIO_AFx_TIMy
} rgb_led_config_t;

#define RGB_LED_DEFAULT_TIMER     TIM2
#define RGB_LED_DEFAULT_RED_CH    TIM_CHANNEL_2
#define RGB_LED_DEFAULT_GREEN_CH  TIM_CHANNEL_3
#define RGB_LED_DEFAULT_BLUE_CH   TIM_CHANNEL_1
#define RGB_LED_DEFAULT_PORT      GPIOA
#define RGB_LED_DEFAULT_RED_PIN   GPIO_PIN_1
#define RGB_LED_DEFAULT_GREEN_PIN GPIO_PIN_2
#define RGB_LED_DEFAULT_BLUE_PIN  GPIO_PIN_0
#define RGB_LED_DEFAULT_AF        GPIO_AF1_TIM2

#define RGB_LED_DEFAULT_CONFIG                     \
    ((rgb_led_config_t){                           \
        .timer = RGB_LED_DEFAULT_TIMER,            \
        .red_channel = RGB_LED_DEFAULT_RED_CH,     \
        .green_channel = RGB_LED_DEFAULT_GREEN_CH, \
        .blue_channel = RGB_LED_DEFAULT_BLUE_CH,   \
        .port = RGB_LED_DEFAULT_PORT,              \
        .red_pin = RGB_LED_DEFAULT_RED_PIN,        \
        .green_pin = RGB_LED_DEFAULT_GREEN_PIN,    \
        .blue_pin = RGB_LED_DEFAULT_BLUE_PIN,      \
        .alternate = RGB_LED_DEFAULT_AF,           \
    })

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t red_channel;
    uint32_t green_channel;
    uint32_t blue_channel;
    rgb_led_color_t color;      // last color written, guarded by mutex
    SemaphoreHandle_t mutex;    // serializes access to the timer's compare registers
    StaticSemaphore_t mutexBuf; // static storage for mutex
} rgb_led_handle_t;

/**
 * @brief Set up the timer, pins and mutex, and start PWM with the LED off.
 *
 * @param led   Handle to fill in. Must outlive every other call that uses it.
 * @param htim  Timer handle the driver will own. Its Instance is set from cfg.
 * @param cfg   Wiring; RGB_LED_DEFAULT_CONFIG for the standard PA0-PA2 layout.
 * @return RGB_LED_OK, RGB_LED_INVALID_PTR, or RGB_LED_ERROR for an unsupported
 *         timer or a HAL failure.
 */
rgb_led_status_t rgb_led_init(rgb_led_handle_t* led, TIM_HandleTypeDef* htim,
                              const rgb_led_config_t* cfg);

/**
 * @brief Change the LED color. Task context only.
 *
 * @param led      Initialized handle.
 * @param color    Brightness per channel, 0 = off, 255 = full.
 * @param timeout  Ticks to wait for the mutex.
 * @return RGB_LED_OK, RGB_LED_INVALID_PTR (NULL or uninitialized handle), or
 *         RGB_LED_BUSY on timeout.
 */
rgb_led_status_t rgb_led_set(rgb_led_handle_t* led, rgb_led_color_t color, TickType_t timeout);

/**
 * @brief Read the color last written with rgb_led_set(). Task context only.
 *
 * @param led      Initialized handle.
 * @param color    Receives the color.
 * @param timeout  Ticks to wait for the mutex.
 * @return RGB_LED_OK, RGB_LED_INVALID_PTR (NULL or uninitialized handle), or
 *         RGB_LED_BUSY on timeout.
 */
rgb_led_status_t rgb_led_get(rgb_led_handle_t* led, rgb_led_color_t* color, TickType_t timeout);

/**
 * @brief Color at a point on a color wheel. Doesn't touch hardware.
 *
 * @param phase  Position on the wheel; 256 steps make one full turn and it wraps.
 * @return Red, green and blue as three sine waves 120 degrees apart.
 */
rgb_led_color_t rgb_led_wheel(uint8_t phase);
