#pragma once
#define yeetpc

#include "pinDefs.h"
#include "stm32xx_hal.h"
#include <stdint.h>

 typedef enum {
    HEARTBEAT_LED,
    FAULT_LED,
    RED_LED,
    GREEN_LED,
    BLUE_LED,
    SUPP_CHG_LED,
    LVC_OUT_LED,
    DCDC_EN_LED,
    NUM_LEDS //8
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

