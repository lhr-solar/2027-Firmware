#include "tim2_shared.h"

#include <stdbool.h>
#include <stddef.h>

#include "FreeRTOS.h"
#include "task.h"

#define TIM2_SHARED_CHANNEL_COUNT 4U

// The only TIM2 handle in the codebase.
static TIM_HandleTypeDef htim2_shared;
static bool initialized = false;
static uint8_t claimed_mask = 0U; // bit n set = channel n + 1 claimed

// Kernel clock of an APB1 timer. The hardware feeds timers PCLK1 when the APB1
// prescaler is 1, and 2 x PCLK1 otherwise (RM0440, RCC chapter), so a divided
// APB1 bus doesn't halve the timer clock.
static uint32_t apb1_timer_clock_hz(void) {
    uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_HCLK_DIV1) {
        pclk1 *= 2U;
    }
    return pclk1;
}

// See tim2_shared.h. The whole check-and-init runs in a critical section so two
// tasks calling it at once can't both configure the timer.
tim2_shared_status_t tim2_shared_init(void) {
    tim2_shared_status_t status = TIM2_SHARED_OK;

    taskENTER_CRITICAL();
    if (!initialized) {
        __HAL_RCC_TIM2_CLK_ENABLE();

        // A slow timer clock can make this 0; clamp so PSC can't wrap to
        // 0xFFFFFFFF (PWM then just runs below TIM2_SHARED_PWM_FREQ_HZ).
        uint32_t psc_div =
            apb1_timer_clock_hz() / (TIM2_SHARED_PERIOD_TICKS * TIM2_SHARED_PWM_FREQ_HZ);
        if (psc_div == 0U) {
            psc_div = 1U;
        }

        htim2_shared.Instance = TIM2;
        htim2_shared.Init.Prescaler = psc_div - 1U;
        htim2_shared.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim2_shared.Init.Period = TIM2_SHARED_PERIOD_TICKS - 1U;
        htim2_shared.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim2_shared.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
        if (HAL_TIM_PWM_Init(&htim2_shared) == HAL_OK) {
            initialized = true;
        } else {
            status = TIM2_SHARED_ERROR;
        }
    }
    taskEXIT_CRITICAL();

    return status;
}

// See tim2_shared.h. TIM_CHANNEL_1..4 are 0x0, 0x4, 0x8, 0xC, so channel >> 2
// is the bit index.
tim2_shared_status_t tim2_shared_claim_channel(uint32_t channel) {
    if ((channel & 0x3U) != 0U || (channel >> 2U) >= TIM2_SHARED_CHANNEL_COUNT) {
        return TIM2_SHARED_ERROR;
    }
    const uint8_t bit = (uint8_t)(1U << (channel >> 2U));
    tim2_shared_status_t status = TIM2_SHARED_OK;

    taskENTER_CRITICAL();
    if (!initialized) {
        status = TIM2_SHARED_ERROR;
    } else if ((claimed_mask & bit) != 0U) {
        status = TIM2_SHARED_BUSY;
    } else {
        claimed_mask |= bit;
    }
    taskEXIT_CRITICAL();

    return status;
}

// See tim2_shared.h.
TIM_HandleTypeDef* tim2_shared_get_handle(void) {
    if (!initialized) {
        return NULL;
    }
    return &htim2_shared;
}

// See tim2_shared.h.
uint32_t tim2_shared_get_period(void) {
    if (!initialized) {
        return 0U;
    }
    return htim2_shared.Init.Period;
}
