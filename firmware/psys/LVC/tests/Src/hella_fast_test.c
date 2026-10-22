#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "led.h"
#include "include_me_please.h"

int main(){
    HAL_Init();
    SystemClock_Config();

    LEDs_Init(); // enable clocks and init all status LEDs

    while(1){
        #ifdef RUN
        statusLEDs_toggle(HEARTBEAT_LED);
        #endif
        HAL_Delay(150);
    }

    return 0;
}
