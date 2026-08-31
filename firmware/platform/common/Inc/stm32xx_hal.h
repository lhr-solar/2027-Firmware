#ifndef STM32xx_HAL_H
#define STM32xx_HAL_H

/* Keeping this header for legacy code */

// STM32
#if defined(STM32G4xx)
    #include "stm32g4xx.h"
    #include "stm32g4xx_hal.h"
    #include "stm32g4xx_hal_conf.h"
#else
    #error "No valid STM32 series defined. Please define either STM32F4xx, STM32L4xx, or STM32G4xx."
#endif

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

// Standard Library
#include <stdint.h>
#include <stdbool.h>

// Init function prototypes
void Error_Handler(void);
void HAL_MspInit(void);
__weak void SystemClock_Config(void);

#endif /* STM32xs_HAL_H */