#ifndef PDU_PINS_H
#define PDU_PINS_H

#include "stm32xx_hal.h"

// Pin map from the PDU schematic (sheet 1 top level, sheet 8 MCU)
// TODO: declare package -- STM32G473CET6 (LQFP48) or STM32G473VET6 (LQFP100).
// Every pin below exists on both and was checked against references/.

// HSS chip selects, one per TPS2HCS08 pair (active low, 10k pull-up on board)
#define HSS1_CS_PORT    GPIOB
#define HSS1_CS_PIN     GPIO_PIN_3
#define HSS2_CS_PORT    GPIOA
#define HSS2_CS_PIN     GPIO_PIN_9
#define HSS3_CS_PORT    GPIOB
#define HSS3_CS_PIN     GPIO_PIN_15
#define HSS4_CS_PORT    GPIOB
#define HSS4_CS_PIN     GPIO_PIN_5
#define HSS5_CS_PORT    GPIOC
#define HSS5_CS_PIN     GPIO_PIN_13
#define HSS6_CS_PORT    GPIOA
#define HSS6_CS_PIN     GPIO_PIN_4

// HSS fault / wake inputs (open drain, active low, 4.7k pull-up on board)
#define HSS1_FLT_PORT   GPIOB
#define HSS1_FLT_PIN    GPIO_PIN_4
#define HSS2_FLT_PORT   GPIOA
#define HSS2_FLT_PIN    GPIO_PIN_10
#define HSS3_FLT_PORT   GPIOA
#define HSS3_FLT_PIN    GPIO_PIN_8
#define HSS4_FLT_PORT   GPIOB
#define HSS4_FLT_PIN    GPIO_PIN_6
#define HSS5_FLT_PORT   GPIOC
#define HSS5_FLT_PIN    GPIO_PIN_14
#define HSS6_FLT_PORT   GPIOA
#define HSS6_FLT_PIN    GPIO_PIN_3

// Shared HSS control lines (go to every pair)
#define LIMP_HOME_PORT  GPIOB
#define LIMP_HOME_PIN   GPIO_PIN_0
#define DI_MODE_PORT    GPIOB
#define DI_MODE_PIN     GPIO_PIN_7

// SPI1 bus shared by all 6 HSS chips (AF5 on all three pins)
#define SPI1_SCK_PORT   GPIOA
#define SPI1_SCK_PIN    GPIO_PIN_5
#define SPI1_MISO_PORT  GPIOA
#define SPI1_MISO_PIN   GPIO_PIN_6
#define SPI1_MOSI_PORT  GPIOA
#define SPI1_MOSI_PIN   GPIO_PIN_7
#define SPI1_PINS_AF    GPIO_AF5_SPI1

// Status RGB LED D5 (active high)
#define LED_BLUE_PORT   GPIOA
#define LED_BLUE_PIN    GPIO_PIN_0
#define LED_RED_PORT    GPIOA
#define LED_RED_PIN     GPIO_PIN_1
#define LED_GREEN_PORT  GPIOA
#define LED_GREEN_PIN   GPIO_PIN_2

// WS2812B data line (plain GPIO for now, becomes timer PWM later)
#define LED_DIN_PORT    GPIOB
#define LED_DIN_PIN     GPIO_PIN_1

// USB FS: PA11 = D-, PA12 = D+. Fixed function, no GPIO/AF setup needed.

// TODO: FDCAN2 TX/RX -- not on PB5/PB6 (used by HSS4). Fill in from schematic.

#endif
