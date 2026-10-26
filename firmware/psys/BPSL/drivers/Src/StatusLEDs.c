#include "pinDefs.h"
#include "StatusLEDs.h"

// RGB pins, indexed by rgb_color_t
static const gpio_pin_t rgb_pins[RGB_NUM_COLORS] = {
    [RGB_BLUE] = {RGB_LED_BLUE_PORT, RGB_LED_BLUE_PIN},
    [RGB_RED] = {RGB_LED_RED_PORT, RGB_LED_RED_PIN},
    [RGB_GREEN] = {RGB_LED_GREEN_PORT, RGB_LED_GREEN_PIN},
};

static rgb_color_t heartbeat_color = RGB_GREEN;

// Enables the clock for a GPIO port.
static led_status_t prvEnableGpioClock(GPIO_TypeDef* port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else {
        return LED_ERROR;
    }
    return LED_OK;
}

led_status_t StatusLEDs_Init(void) {
    for (int i = 0; i < RGB_NUM_COLORS; i++) {
        if (prvEnableGpioClock(rgb_pins[i].port) != LED_OK) {
            return LED_ERROR;
        }
    }

    // set the output level before switching the pins to outputs, so no LED flashes on
    StatusLEDs_Clear();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    for (int i = 0; i < RGB_NUM_COLORS; i++) {
        GPIO_InitStruct.Pin = rgb_pins[i].pin;
        HAL_GPIO_Init(rgb_pins[i].port, &GPIO_InitStruct);
    }

    return LED_OK;
}

void StatusLEDs_Clear(void) {
    for (int i = 0; i < RGB_NUM_COLORS; i++) {
        HAL_GPIO_WritePin(rgb_pins[i].port, rgb_pins[i].pin, GPIO_PIN_RESET);
    }
}

void StatusLEDs_SetHeartbeat(bool state) {
    HAL_GPIO_WritePin(rgb_pins[heartbeat_color].port, rgb_pins[heartbeat_color].pin,
                      state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void StatusLEDs_ToggleHeartbeat(void) {
    HAL_GPIO_TogglePin(rgb_pins[heartbeat_color].port, rgb_pins[heartbeat_color].pin);
}

led_status_t StatusLEDs_SetHeartbeatColor(rgb_color_t color) {
    // make sure color is in range
    if ((color < 0) || (color >= RGB_NUM_COLORS)) {
        return LED_ERROR;
    }

    // turn off the old color so it doesn't stay lit
    StatusLEDs_Clear();
    heartbeat_color = color;

    return LED_OK;
}
