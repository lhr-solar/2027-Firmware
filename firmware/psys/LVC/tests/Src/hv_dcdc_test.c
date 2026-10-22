#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "led.h"
#include "include_me_please.h"
#include "pinDefs.h"

int main() {
    HAL_Init();
    SystemClock_Config();

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef hv_dcdc_config = {
        .Pin = HV_DCDC_EN_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL
    };

    HAL_GPIO_Init(HV_DCDC_EN_PORT, &hv_dcdc_config);

    HAL_GPIO_WritePin(HV_DCDC_EN_PORT, HV_DCDC_EN_PIN, GPIO_PIN_SET);

    while(1) {
        HAL_GPIO_TogglePin(HEARTBEAT_PORT, HEARTBEAT_PIN);
        HAL_Delay(500);
    }
}