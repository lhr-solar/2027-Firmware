#include "PDU_Clock.h"


void PDU_Clock_Init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef periph = {0};
    
    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    // HSI16 for the core, HSI48 for USB (HSI16 is too inaccurate for USB;
    // HSI48 is trimmed by CRS against USB SOF, see PDU_USB.c)
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;   // PLL unused for now
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                       | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;   // 16 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;          // SPI1 runs off APB2 = 16 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }

    // Kernel clocks. FDCAN resets to HSE, which is off on this board.
    periph.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_FDCAN;
    periph.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48;
    periph.FdcanClockSelection  = RCC_FDCANCLKSOURCE_PCLK1;  // 16 MHz
    if (HAL_RCCEx_PeriphCLKConfig(&periph) != HAL_OK) {
        Error_Handler();
    }
}
