#include "charger.h"

void Charger_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = CHARGE_EN_PIN;

    HAL_GPIO_WritePin(CHARGE_EN_PORT, CHARGE_EN_PIN, CHARGER_OFF);
    HAL_GPIO_Init(CHARGE_EN_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = CHG_CC_PIN;
    HAL_GPIO_Init(CHG_CC_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = CHG_PG_PIN;
    HAL_GPIO_Init(CHG_PG_PORT, &GPIO_InitStruct);
}

void charger_write(charger_status_t newState) {
    if (newState >= 2) return;
    HAL_GPIO_WritePin(CHARGE_EN_PORT, CHARGE_EN_PIN, newState);
}

void charger_readStatus(void) {

}