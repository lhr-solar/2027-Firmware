// Initialize clock for heartbeat LED port
#include "stm32xx_hal.h"
#include "fake.h"

void Heartbeat_Clock_Init() {
    #ifdef yeetpc
    __HAL_RCC_GPIOA_CLK_ENABLE();
    #endif
}
