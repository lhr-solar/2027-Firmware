#include "stm32xx_hal.h"

/*
 * RGB LED PWM test
 *
 * All three channels sit on TIM2 at AF1, valid on both LQFP48 and LQFP100:
 *   PA0 -> TIM2_CH1 -> blue   (LQFP48 pin 8,  LQFP100 pin 20)
 *   PA1 -> TIM2_CH2 -> red    (LQFP48 pin 9,  LQFP100 pin 21)
 *   PA2 -> TIM2_CH3 -> green  (LQFP48 pin 10, LQFP100 pin 22)
 *
 * Common-cathode wiring: each LED anode is driven by its pin, all cathodes go
 * to GND. Driving a pin high turns that color on.
 *
 * Shows each ROYGBIV color for ~1 s, then hue cycles forever.
 */

#define RGB_PORT  GPIOA
#define RED_PIN   GPIO_PIN_1
#define GREEN_PIN GPIO_PIN_2
#define BLUE_PIN  GPIO_PIN_0

#define RED_CH   TIM_CHANNEL_2
#define GREEN_CH TIM_CHANNEL_3
#define BLUE_CH  TIM_CHANNEL_1

// Common-cathode LED: pin high = on, so duty 255 = full brightness.
// Set to 0 for a common-anode LED (pin low = on); the timer output polarity is
// then inverted so duty 255 still means full brightness.
#define RGB_COMMON_CATHODE 1

#define PWM_FREQ_HZ    1000U
#define PWM_RESOLUTION 256U // ARR = 255, so compare value == 8-bit color value

#define RGB_MAX     255U
#define HUE_DEGREES 360U
#define HUE_SECTOR  60U // degrees per hue sector

#define RAINBOW_STEP_MS  1000U
#define RAINBOW_PAUSE_MS 500U
#define HUE_STEP_MS      10U

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color_t;

// ROYGBIV
#define RGB_RED    ((rgb_color_t){255, 0, 0})
#define RGB_ORANGE ((rgb_color_t){255, 127, 0})
#define RGB_YELLOW ((rgb_color_t){255, 255, 0})
#define RGB_GREEN  ((rgb_color_t){0, 255, 0})
#define RGB_BLUE   ((rgb_color_t){0, 0, 255})
#define RGB_INDIGO ((rgb_color_t){75, 0, 130})
#define RGB_VIOLET ((rgb_color_t){148, 0, 211})

#define RGB_WHITE ((rgb_color_t){255, 255, 255})
#define RGB_OFF   ((rgb_color_t){0, 0, 0})

TIM_HandleTypeDef htim2;

void MX_TIM2_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);

// Set the LED to any color, each channel 0 (off) to 255 (full brightness)
void rgb_set(uint8_t red, uint8_t green, uint8_t blue) {
    __HAL_TIM_SET_COMPARE(&htim2, RED_CH, red);
    __HAL_TIM_SET_COMPARE(&htim2, GREEN_CH, green);
    __HAL_TIM_SET_COMPARE(&htim2, BLUE_CH, blue);
}

// Set the LED to a color struct, e.g. rgb_set_color(RGB_ORANGE)
void rgb_set_color(rgb_color_t color) {
    rgb_set(color.r, color.g, color.b);
}

// Fully saturated, full brightness color for a hue in degrees (0-359)
rgb_color_t hue_to_rgb(uint16_t hue) {
    hue %= HUE_DEGREES;
    uint8_t rise = (uint8_t)(((hue % HUE_SECTOR) * RGB_MAX) / HUE_SECTOR);
    uint8_t fall = (uint8_t)(RGB_MAX - rise);

    switch (hue / HUE_SECTOR) {
        case 0:
            return (rgb_color_t){RGB_MAX, rise, 0};
        case 1:
            return (rgb_color_t){fall, RGB_MAX, 0};
        case 2:
            return (rgb_color_t){0, RGB_MAX, rise};
        case 3:
            return (rgb_color_t){0, fall, RGB_MAX};
        case 4:
            return (rgb_color_t){rise, 0, RGB_MAX};
        default:
            return (rgb_color_t){RGB_MAX, 0, fall};
    }
}

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_TIM2_Init();

    if (HAL_TIM_PWM_Start(&htim2, RED_CH) != HAL_OK ||
        HAL_TIM_PWM_Start(&htim2, GREEN_CH) != HAL_OK ||
        HAL_TIM_PWM_Start(&htim2, BLUE_CH) != HAL_OK) {
        Error_Handler();
    }

    // show each rainbow color once to check the channel -> pin mapping by eye
    rgb_color_t rainbow[] = {RGB_RED,  RGB_ORANGE, RGB_YELLOW, RGB_GREEN,
                             RGB_BLUE, RGB_INDIGO, RGB_VIOLET};
    for (uint32_t i = 0; i < sizeof(rainbow) / sizeof(rainbow[0]); i++) {
        rgb_set_color(rainbow[i]);
        HAL_Delay(RAINBOW_STEP_MS);
    }
    rgb_set_color(RGB_OFF);
    HAL_Delay(RAINBOW_PAUSE_MS);

    // ~3.6 s per trip around the color wheel
    uint16_t hue = 0;
    while (1) {
        rgb_set_color(hue_to_rgb(hue));
        hue = (hue + 1) % HUE_DEGREES;
        HAL_Delay(HUE_STEP_MS);
    }

    return 0;
}

// Timer kernel clock for APB1 timers: PCLK1, doubled when the APB1 prescaler is not 1
static uint32_t tim2_clock_hz(void) {
    uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_HCLK_DIV1) {
        pclk1 *= 2U;
    }
    return pclk1;
}

void MX_TIM2_Init(void) {
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    // computed at runtime so the PWM frequency doesn't depend on the HSE value
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = (tim2_clock_hz() / (PWM_RESOLUTION * PWM_FREQ_HZ)) - 1U;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = PWM_RESOLUTION - 1U;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
#if RGB_COMMON_CATHODE
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
#else
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
#endif
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, RED_CH) != HAL_OK ||
        HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, GREEN_CH) != HAL_OK ||
        HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, BLUE_CH) != HAL_OK) {
        Error_Handler();
    }

    HAL_TIM_MspPostInit(&htim2);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (timHandle->Instance == TIM2) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = RED_PIN | GREEN_PIN | BLUE_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(RGB_PORT, &GPIO_InitStruct);
    }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
        HAL_GPIO_DeInit(RGB_PORT, RED_PIN | GREEN_PIN | BLUE_PIN);
    }
}
