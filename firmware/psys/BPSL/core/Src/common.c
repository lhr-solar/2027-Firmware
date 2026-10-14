#include "stm32g4xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"

// for software errors
void Error_Handler(void)
{
    // stop interrupts and other tasks so nothing else changes the LED
    __disable_irq();

    // solid red heartbeat = software error
    // init again because the error may happen before main() initializes the LEDs.
    // A failed init can't be reported from here, so the result is ignored.
    (void)StatusLEDs_Init();
    (void)StatusLEDs_SetHeartbeatColor(RGB_RED);
    StatusLEDs_SetHeartbeat(true);

    while (true)
    {
    }
}

// 8 MHz crystal (HSE) -> 80 MHz system clock
// SYSCLK = HSE / PLLM * PLLN / PLLR = 8 MHz / 1 * 20 / 2 = 80 MHz
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

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
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Run the CPU and buses from the PLL at 80 MHz
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    // 2 flash wait states are correct for 80 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}
