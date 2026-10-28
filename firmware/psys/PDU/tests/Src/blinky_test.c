#include "stm32xx_hal.h"

#define LED_PIN GPIO_PIN_0
#define LED_PORT GPIOA

// Initialize clock for heartbeat LED port
// HAL_StatusTypeDef enable_gpio_clock(const GPIO_TypeDef* port) {
// PR #53
// }

int main(){
    HAL_Init();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };
    
    enable_gpio_clock(LED_PORT); // enable clock for LED_PORT
    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config

    while(1){
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(500);
    }

    return 0;
}
