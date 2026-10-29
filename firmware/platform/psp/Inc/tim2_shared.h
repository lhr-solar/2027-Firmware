/**
 * @file tim2_shared.h
 * @brief Shared TIM2 time base, so several modules can each own TIM2 channels.
 *
 * TIM2's counter, prescaler and period are shared by all four channels, so one
 * module (this one) owns them, along with the only TIM2 HAL handle. Each user
 * claims the channels it needs and sets up only those: its own pin, output
 * compare mode, polarity and compare value.
 *
 * Timing:
 *  - PWM period is TIM2_SHARED_PERIOD_TICKS (ARR = 4095), so duty has 12-bit
 *    resolution.
 *  - The prescaler is picked at init from the APB1 timer clock to get as close
 *    to TIM2_SHARED_PWM_FREQ_HZ as possible. With the platform's 80 MHz clock
 *    that is PSC = 0, so the PWM runs at 80 MHz / 4096 = 19.53 kHz.
 *  - Every TIM2 channel user gets this same frequency. There is no API to
 *    change PSC or ARR after init.
 *
 * Call order:
 *   1. tim2_shared_init(). Safe to call more than once; later calls do nothing.
 *   2. tim2_shared_claim_channel() for each channel you use.
 *   3. Set up your pin, then HAL_TIM_PWM_ConfigChannel() and HAL_TIM_PWM_Start()
 *      on tim2_shared_get_handle(), only for your channels.
 *
 * Init and claim can run before or after the scheduler starts, but not from an
 * ISR. They use a FreeRTOS critical section.
 *
 * Pins are set up by each channel user, not here. There are no TIM2 MSP
 * callbacks, so the pin lives next to the code that owns the channel.
 *
 * Example: a second module using the free CH4 on PB11 (AF1):
 * @code
 *   if (tim2_shared_init() != TIM2_SHARED_OK ||
 *       tim2_shared_claim_channel(TIM_CHANNEL_4) != TIM2_SHARED_OK) {
 *       // TIM2 failed to start, or CH4 is already taken
 *   }
 *   // PB11: GPIO_MODE_AF_PP, Alternate = GPIO_AF1_TIM2, via HAL_GPIO_Init
 *   TIM_OC_InitTypeDef oc = {0};
 *   oc.OCMode = TIM_OCMODE_PWM1;
 *   oc.OCPolarity = TIM_OCPOLARITY_HIGH;
 *   HAL_TIM_PWM_ConfigChannel(tim2_shared_get_handle(), &oc, TIM_CHANNEL_4);
 *   HAL_TIM_PWM_Start(tim2_shared_get_handle(), TIM_CHANNEL_4);
 *   __HAL_TIM_SET_COMPARE(tim2_shared_get_handle(), TIM_CHANNEL_4,
 *                         (tim2_shared_get_period() + 1U) / 2U); // 50% duty
 * @endcode
 */

#pragma once

#include <stdint.h>

#include "stm32xx_hal.h"

typedef enum {
    TIM2_SHARED_OK,    // success
    TIM2_SHARED_ERROR, // HAL failure, invalid channel, or not initialized
    TIM2_SHARED_BUSY   // channel already claimed
} tim2_shared_status_t;

#define TIM2_SHARED_PWM_FREQ_HZ  20000U // target; see the timing notes above
#define TIM2_SHARED_PERIOD_TICKS 4096U  // ARR = 4095, 12-bit duty

/**
 * @brief Enable TIM2 and start its counter with the shared PSC and ARR.
 *
 * Only the first call configures the timer. Later calls return TIM2_SHARED_OK
 * without touching it.
 *
 * @return TIM2_SHARED_OK, or TIM2_SHARED_ERROR if HAL_TIM_PWM_Init fails.
 */
tim2_shared_status_t tim2_shared_init(void);

/**
 * @brief Claim one TIM2 channel for the caller.
 *
 * @param channel  TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 or TIM_CHANNEL_4.
 * @return TIM2_SHARED_OK, TIM2_SHARED_BUSY if another user has it, or
 *         TIM2_SHARED_ERROR for an invalid channel or before tim2_shared_init().
 */
tim2_shared_status_t tim2_shared_claim_channel(uint32_t channel);

/**
 * @brief The TIM2 handle, for ConfigChannel, Start and __HAL_TIM_SET_COMPARE on
 *        claimed channels.
 *
 * @return The handle, or NULL before tim2_shared_init() succeeds.
 */
TIM_HandleTypeDef* tim2_shared_get_handle(void);

/**
 * @brief The auto-reload value (ARR). A compare value of ARR + 1 is 100% duty.
 *
 * @return ARR, or 0 before tim2_shared_init() succeeds.
 */
uint32_t tim2_shared_get_period(void);
