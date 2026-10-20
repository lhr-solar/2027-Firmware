#include "stm32xx_hal.h"

/*
    Enables GPIO clock port [A - G]
    port: GPIO_TypeDef* of the port (GPIOx) to init 
*/
HAL_StatusTypeDef enable_gpio_clock(const GPIO_TypeDef* port);