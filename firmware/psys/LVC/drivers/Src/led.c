#include "stm32xx_hal.h"
#include "led.h"
#include "pinDefs.h"

// indexed by status_leds_t
static const led_t leds[NUM_LEDS] = {
    // Low Voltage Carrier LEDs
    [HEARTBEAT_LED] = {HEARTBEAT_PIN, HEARTBEAT_PORT, OFF},
    [FAULT_LED]     = {FAULT_PIN,     FAULT_PORT,     OFF},
    // PSOM LEDs
    [RED_LED]       = {RGB_RED_PIN,   RGB_RED_PORT,   OFF},
    [GREEN_LED]     = {RGB_GREEN_PIN, RGB_GREEN_PORT, OFF},
    [BLUE_LED]      = {RGB_BLUE_PIN,  RGB_BLUE_PORT,  OFF},
};

void LEDs_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clocks (RGB on port A, heartbeat/fault on port E)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    for (int i = 0; i < NUM_LEDS; i++) {
        GPIO_InitStruct.Pin = leds[i].Pin;
        HAL_GPIO_Init(leds[i].Port, &GPIO_InitStruct);
        HAL_GPIO_WritePin(leds[i].Port, leds[i].Pin, GPIO_PIN_RESET);
    }
}

void statusLEDs_toggle(status_leds_t led)
{
    if (led >= NUM_LEDS) return;
    HAL_GPIO_TogglePin(leds[led].Port, leds[led].Pin);
}

void statusLEDs_write(status_leds_t led, pin_status_t newState)
{
    if (led >= NUM_LEDS) return;
    HAL_GPIO_WritePin(leds[led].Port, leds[led].Pin, newState == ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
