#pragma once
#define yeetpc

#include "pinDefs.h"
#include "stm32xx_hal.h"
#include "stdint.h"

void Heartbeat_Clock_Init();

/**
 * @brief Initializes all status LEDs
 * @note HEARTBEAT led should blink when called
 */

 typedef enum {
    HEARTBEAT_LED,
    FAULT_LED,
    RED_LED,
    GREEN_LED,
    BLUE_LED,
    NUM_LEDS
 } status_leds_t;

 typedef enum {
    OFF = GPIO_PIN_RESET,
    ON = GPIO_PIN_SET
 } pin_status_t;

  typedef struct {
   uint16_t Pin;
   GPIO_TypeDef *Port;
   pin_status_t status;
} led_t;

void LEDs_Init(void);
void statusLEDs_write(status_leds_t led, pin_status_t newState);
void statusLEDs_toggle(status_leds_t led);

