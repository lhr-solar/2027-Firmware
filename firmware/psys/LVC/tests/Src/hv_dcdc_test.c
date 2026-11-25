#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "led.h"
#include "include_me_please.h"

int main() {
    HAL_Init();
    SystemClock_Config();

    LEDs_Init(); // enable clocks and init all status LEDs

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef hv_dcdc_config = {
        .Pin = HV_DCDC_EN_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
    };

    GPIO_InitTypeDef heartbeat_config = {
        .Pin = HEARTBEAT_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
    };

    HAL_GPIO_Init(HV_DCDC_EN_PORT, &hv_dcdc_config);
    HAL_GPIO_Init(HEARTBEAT_PORT, &heartbeat_config);

    HAL_GPIO_WritePin(HV_DCDC_EN_PORT, HV_DCDC_EN_PIN, GPIO_PIN_SET);

    while (1) {
        HAL_GPIO_TogglePin(HEARTBEAT_PORT, HEARTBEAT_PIN);
        HAL_Delay(500);
    }
}