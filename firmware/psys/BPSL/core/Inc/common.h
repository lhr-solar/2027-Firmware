#pragma once
/**
 * @file common.h
 * @brief Board-wide BPSL functions.
 */

/**
 * @brief Handles unrecoverable software errors. Shows a solid red
 *        heartbeat LED and halts. Never returns.
 */
void Error_Handler(void);

/**
 * @brief Configures the system clock: 8 MHz crystal (HSE) -> 80 MHz.
 * @note  Call after HAL_Init(), which enables the PWR clock this needs.
 */
void SystemClock_Config(void);
