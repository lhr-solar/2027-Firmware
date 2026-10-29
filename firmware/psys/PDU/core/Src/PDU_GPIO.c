#include "PDU_GPIO.h"

void PDU_GPIO_Init(void)
{
    GPIO_InitTypeDef init = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Preload output levels before switching pins to output so nothing glitches.
    // CS is active low: start high so no HSS chip is selected (6 chips share MISO).
    HAL_GPIO_WritePin(HSS1_CS_PORT, HSS1_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HSS2_CS_PORT, HSS2_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HSS3_CS_PORT, HSS3_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HSS4_CS_PORT, HSS4_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HSS5_CS_PORT, HSS5_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HSS6_CS_PORT, HSS6_CS_PIN, GPIO_PIN_SET);

    // high forces LIMP_HOME state, low = normal SPI control (SLVSHR0 8.3.7)
    HAL_GPIO_WritePin(LIMP_HOME_PORT, LIMP_HOME_PIN, GPIO_PIN_RESET);
    // DI: low = OFF
    HAL_GPIO_WritePin(DI_MODE_PORT, DI_MODE_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_DIN_PORT, LED_DIN_PIN, GPIO_PIN_RESET);

    // Push-pull outputs
    init.Mode  = GPIO_MODE_OUTPUT_PP;
    init.Pull  = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;

    // PA: HSS2_CS, HSS6_CS, RGB LED
    init.Pin = HSS2_CS_PIN | HSS6_CS_PIN | LED_BLUE_PIN | LED_RED_PIN | LED_GREEN_PIN;
    HAL_GPIO_Init(GPIOA, &init);

    // PB: HSS1/3/4_CS, LIMP_HOME, DI_MODE, WS2812B data
    init.Pin = HSS1_CS_PIN | HSS3_CS_PIN | HSS4_CS_PIN
             | LIMP_HOME_PIN | DI_MODE_PIN | LED_DIN_PIN;
    HAL_GPIO_Init(GPIOB, &init);

    // PC: HSS5_CS 
    init.Pin = HSS5_CS_PIN;
    HAL_GPIO_Init(GPIOC, &init);

    // low = read faults
    init.Mode  = GPIO_MODE_INPUT;
    init.Pull  = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;

    init.Pin = HSS2_FLT_PIN | HSS3_FLT_PIN | HSS6_FLT_PIN;
    HAL_GPIO_Init(GPIOA, &init);

    init.Pin = HSS1_FLT_PIN | HSS4_FLT_PIN;
    HAL_GPIO_Init(GPIOB, &init);

    init.Pin = HSS5_FLT_PIN;
    HAL_GPIO_Init(GPIOC, &init);
}
