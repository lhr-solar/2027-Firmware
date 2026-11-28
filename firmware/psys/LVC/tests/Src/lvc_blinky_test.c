#include "led.h"

int main() {
    HAL_Init();
    SystemClock_Config();

    LEDs_Init();

    for (int j = 0; j < NUM_LEDS; j++) {
        statusLEDs_write(j, ON);
        HAL_Delay(500);
        statusLEDs_write(j, OFF);
        HAL_Delay(500);
    }
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            statusLEDs_toggle(j);
        }
        HAL_Delay(500);
    }
    while (1) {

    }
}