#include "stm32xx_hal.h"
#include "PDU_Clock.h"
#include "PDU_GPIO.h"


int main(void)
{
    HAL_Init();
    PDU_Clock_Init();
    PDU_GPIO_Init();

    GPIO_InitTypeDef init = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    init.Pin   = SPI1_MISO_PIN;
    init.Mode  = GPIO_MODE_INPUT;
    init.Pull  = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SPI1_MISO_PORT, &init);

    while (1) {
        if (HAL_GPIO_ReadPin(SPI1_MISO_PORT, SPI1_MISO_PIN) == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
        }
    }
}
