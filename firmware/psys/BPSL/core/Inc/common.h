#pragma once
/**
 * @file common.h
 * @brief Board-wide BPSL functions.
 */

/**
 * @brief Handles software errors. Blinks the heartbeat LED red,
 *        1 s on / 1 s off, forever.
 * @note  Interrupts stay enabled, and once the scheduler is running other tasks
 *        keep running. Not safe to call from an ISR.
 */
void Error_Handler(void);

/**
 * @brief Configures the system clock: 8 MHz crystal (HSE) -> 80 MHz.
 * @note  Call after HAL_Init(), which enables the PWR clock this needs.
 */
void SystemClock_Config(void);
