#pragma once
/**
 * @file pinDefs.h
 * @brief All pin, port, and channel definitions for BPSL.
 *
 * MCU: STM32G473VET6 (LQFP100). Every pin here has been checked against
 * references/stm32g473vet_lqfp100_alternate_functions.md.
 */

#include "stm32g4xx_hal.h"

/**
 * @brief Holds the port and pin number of a GPIO pin
 */
typedef struct {
    GPIO_TypeDef* port; // e.g., GPIOA
    uint16_t pin;       // e.g., GPIO_PIN_3
} GpioPin_t;

// RGB status LED (Everlight 19-337/R6GHBHC-A01/2T), plain GPIO outputs, no AF
// RGB LED Blue  -- PA0, LQFP100 pin 20
#define RGB_LED_BLUE_PIN GPIO_PIN_0
#define RGB_LED_BLUE_PORT GPIOA

// RGB LED Red   -- PA1, LQFP100 pin 21
#define RGB_LED_RED_PIN GPIO_PIN_1
#define RGB_LED_RED_PORT GPIOA

// RGB LED Green -- PA2, LQFP100 pin 22
#define RGB_LED_GREEN_PIN GPIO_PIN_2
#define RGB_LED_GREEN_PORT GPIOA

// Pin level that turns an RGB LED color on.
// TODO: confirm against the BPSL schematic. The 19-337 is three independent
// diodes (no shared anode or cathode), so the level depends on board wiring:
//   MCU pin -> resistor -> anode, cathode -> GND   => GPIO_PIN_SET   (active high)
//   3V3 -> anode, cathode -> resistor -> MCU pin   => GPIO_PIN_RESET (active low)
#define RGB_LED_ON_LEVEL GPIO_PIN_SET
#define RGB_LED_OFF_LEVEL ((RGB_LED_ON_LEVEL == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET)
