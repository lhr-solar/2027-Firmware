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
} gpio_pin_t;

// RGB status LED (Everlight 19-337/R6GHBHC-A01/2T), plain GPIO outputs, no AF
// RGB LED Blue     -- PA0, LQFP100 pin 20
#define RGB_LED_BLUE_PIN  GPIO_PIN_0
#define RGB_LED_BLUE_PORT GPIOA

// RGB LED Red      -- PA1, LQFP100 pin 21
#define RGB_LED_RED_PIN  GPIO_PIN_1
#define RGB_LED_RED_PORT GPIOA

// RGB LED Green    -- PA2, LQFP100 pin 22
#define RGB_LED_GREEN_PIN  GPIO_PIN_2
#define RGB_LED_GREEN_PORT GPIOA

// HV+ Contactor    -- PE1
#define HV_PLUS_CTRL_PIN  GPIO_PIN_1
#define HV_PLUS_CTRL_PORT GPIOE

// HV- Contactor    -- PE4
#define HV_MINUS_CTRL_PIN  GPIO_PIN_4
#define HV_MINUS_CTRL_PORT GPIOE

// Motor Contactor  -- PC2
#define MOTOR_CTRL_PIN  GPIO_PIN_2
#define MOTOR_CTRL_PORT GPIOC

// HV+ Sense        -- PB9
#define HV_PLUS_SENSE_PIN  GPIO_PIN_9
#define HV_PLUS_SENSE_PORT GPIOB

// HV- Sense        -- PE2
#define HV_MINUS_SENSE_PIN  GPIO_PIN_2
#define HV_MINUS_SENSE_PORT GPIOE

// Motor Sense      -- PC0
#define MOTOR_SENSE_PIN  GPIO_PIN_0
#define MOTOR_SENSE_PORT GPIOC

// ESTOP 1 Sense    -- PD2
#define ESTOP_1_SENSE_PIN  GPIO_PIN_2
#define ESTOP_1_SENSE_PORT GPIOD

// ESTOP 2 Sense    -- PD3
#define ESTOP_2_SENSE_PIN  GPIO_PIN_3
#define ESTOP_2_SENSE_PORT GPIOD

// ESTOP 3 Sense    -- PD4
#define ESTOP_3_SENSE_PIN  GPIO_PIN_4
#define ESTOP_3_SENSE_PORT GPIOD
