#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "led.h"

int main() {
    HAL_Init();
    SystemClock_Config();

    LEDs_Init();

    while (1) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < NUM_LEDS; j++) {
                statusLEDs_toggle(i);
                HAL_Delay(500);
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < NUM_LEDS; j++) {
                statusLEDs_toggle(i);
            }
        }
    }
}