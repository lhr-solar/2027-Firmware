#include "led.h"

int main() {
    HAL_Init();
    SystemClock_Config();

    LEDs_Init();

        for (int i = 0; i < 2; i++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            statusLEDs_toggle(j);
            HAL_Delay(500);
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            statusLEDs_toggle(j);
        }
    }
    while (1) {

    }
}