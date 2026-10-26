#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "led.h"
#include "include_me_please.h"

int main() {
    HAL_Init();
    SystemClock_Config();

    LEDs_Init(); // enable clocks and init all status LEDs

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef hv_dcdc_config = {
        .Pin = HV_DCDC_EN_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
    };

    HAL_GPIO_Init(HV_DCDC_EN_PORT, &hv_dcdc_config);

    HAL_GPIO_WritePin(HV_DCDC_EN_PORT, HV_DCDC_EN_PIN, GPIO_PIN_SET);

    while (1) {
        statusLEDs_toggle(HEARTBEAT_LED);
        HAL_Delay(500);
    }
}
