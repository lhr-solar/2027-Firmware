#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "fake.h"
#include "include_me_please.h"

int main(){
    HAL_Init();
    SystemClock_Config();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };
    
    Heartbeat_Clock_Init(); // enable clock for LED_PORT
    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config

    while(1){
        #ifdef RUN
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        #endif
        HAL_Delay(150);
    }

    return 0;
}
