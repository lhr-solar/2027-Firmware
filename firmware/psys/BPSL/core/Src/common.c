#include "stm32g4xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"

// how long the error LED stays on, then off
static const uint32_t ERROR_BLINK_DELAY_MS = 500;

// busy-waits using the CPU cycle counter; correct at any clock speed
// (SystemCoreClock tracks it) and needs no scheduler or HAL tick
static void prvBusyDelay(uint32_t delay_ms) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // enable the DWT unit
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // start the cycle counter

    const uint32_t start = DWT->CYCCNT;
    const uint32_t cycles = (SystemCoreClock / 1000U) * delay_ms;
    while ((DWT->CYCCNT - start) < cycles) {
        // unsigned subtraction handles counter wraparound
    }
}

// for software errors; only called during initialization
void Error_Handler(void) {
    (void)StatusLEDs_Init();
    (void)StatusLEDs_SetHeartbeatColor(RGB_RED);

    while (true) {
        StatusLEDs_SetHeartbeat(true);
        prvBusyDelay(ERROR_BLINK_DELAY_MS);
        StatusLEDs_SetHeartbeat(false);
        prvBusyDelay(ERROR_BLINK_DELAY_MS);
    }
}

// TODO: Fix clock once Aarav makes changes
// 8 MHz crystal (HSE) -> 80 MHz system clock
// SYSCLK = HSE / PLLM * PLLN / PLLR = 8 MHz / 1 * 20 / 2 = 80 MHz
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }

    /** Start the external crystal and the PLL
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2; // system clock output
    // P and Q outputs aren't used yet, but must still hold valid values
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Run the CPU and buses from the PLL at 80 MHz
     */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    // 2 flash wait states are correct for 80 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}
