#pragma once
/**
 * @file common.h
 * @brief Board-wide BPSL functions and shared includes.
 */

#include <stdbool.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

/**
 * @brief Handles software errors during initialization. Blinks the heartbeat
 *        LED red, 500 ms on / 500 ms off, forever.
 * @note  Only call before the scheduler starts; the busy-wait never yields.
 *        Interrupts stay enabled.
 */
void Error_Handler(void);

/**
 * @brief Configures the system clock: 8 MHz crystal (HSE) -> 96 MHz, with the
 *        PLL's Q output at 48 MHz for USB.
 * @note  Call after HAL_Init(), which enables the PWR clock this needs.
 */
void SystemClock_Config(void);
