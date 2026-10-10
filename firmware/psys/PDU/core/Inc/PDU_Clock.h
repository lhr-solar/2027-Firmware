#ifndef PDU_CLOCK_H
#define PDU_CLOCK_H

#include "stm32xx_hal.h"

/**
 * SYSCLK = HSI 16 MHz, HCLK/PCLK1/PCLK2 = 16 MHz, flash latency 0.
 * HSI48 on for USB, FDCAN kernel clock = PCLK1.
 * Calls Error_Handler() on failure.
 */
void PDU_Clock_Init(void);

#endif
