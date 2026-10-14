#pragma once
/**
 * @file StatusLEDs.h
 * @brief Driver for the BPSL RGB status (heartbeat) LED.
 *
 * The heartbeat LED shows one color at a time. Pins and the on/off pin level
 * come from pinDefs.h.
 *
 * Call order:
 *   StatusLEDs_Init() -> StatusLEDs_SetHeartbeatColor() -> StatusLEDs_SetHeartbeat()
 *                                                          / StatusLEDs_ToggleHeartbeat()
 *
 * Gotchas:
 *  - StatusLEDs_SetHeartbeatColor() turns the LED off. The new color only shows
 *    after the next StatusLEDs_SetHeartbeat(true) or StatusLEDs_ToggleHeartbeat().
 *  - Not thread-safe. Only one task should drive the LED at a time.
 *    Error_Handler() takes it over after disabling interrupts.
 */

#include <stdbool.h>

/** @brief Return status for StatusLEDs functions
 */
typedef enum {
    LED_OK,
    LED_ERR
} led_status_t;

/** @brief Colors for the RGB heartbeat LED
 */
typedef enum {
    RGB_RED,
    RGB_GREEN,
    RGB_BLUE,
    RGB_NUM_COLORS /**< Count of colors */
} rgb_color_t;

/** @brief Enables the GPIO clocks and configures the pins for all LEDs, leaving
 *         them off. Safe to call more than once.
 * @return LED_OK on success, LED_ERR if pinDefs.h uses a GPIO port this
 *         driver doesn't know how to clock.
 */
led_status_t StatusLEDs_Init(void);

/** @brief Turns off all LEDs.
 */
void StatusLEDs_Clear(void);

/** @brief Turns the heartbeat LED on or off, in its current color.
 * @param on true to turn the LED on, false to turn it off.
 */
void StatusLEDs_SetHeartbeat(bool on);

/** @brief Toggles the heartbeat LED, in its current color.
 */
void StatusLEDs_ToggleHeartbeat(void);

/** @brief Sets the color of the heartbeat LED and turns the LED off.
 * @param color The color to use (RGB_RED, RGB_GREEN, or RGB_BLUE).
 * @return      LED_OK on success, LED_ERR if the color is out of range.
 */
led_status_t StatusLEDs_SetHeartbeatColor(rgb_color_t color);
