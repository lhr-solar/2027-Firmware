#pragma once
#include "stm32xx_hal.h"

/**
 *  @brief Enables GPIO clock port [A - G]
 *   
 *  @param port: GPIO_TypeDef* of the port (GPIOx) to init 
 *  @return HAL_ERROR if invalid port, HAL_OK otherwise
*/
HAL_StatusTypeDef enable_gpio_clock(const GPIO_TypeDef* port);