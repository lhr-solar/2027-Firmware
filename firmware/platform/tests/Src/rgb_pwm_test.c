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
 * Shows each ROYGBIV color for ~1 s, then fades smoothly around the color
 * wheel forever.
 *
 * PWM is 12-bit (ARR = 4095) at ~20 kHz. Colors stay 0-255 at the API; rgb_set()
 * maps them through a gamma 2.2 table so the fade looks even to the eye. The
 * wheel is three sine waves 120 degrees apart, read from a 256-entry table.
 *
 * TIM2 also paces the fade: its update interrupt fires once per PWM period and
 * steps the wheel every fade_div periods (~FADE_STEP_MS). Writing the compare
 * registers there is glitch-free because they are preloaded and latch on the
 * next update event.
 *
 * Why TIM2_IRQHandler handles the update flag itself:
 * stm/stm32g4xx_hal_timebase_tim.c runs the HAL tick on TIM6 and defines
 * HAL_TIM_PeriodElapsedCallback() as a strong symbol that calls HAL_IncTick().
 *  - Defining HAL_TIM_PeriodElapsedCallback() here too is a duplicate symbol
 *    and fails to link.
 *  - Calling HAL_TIM_IRQHandler(&htim2) would route every TIM2 update through
 *    that same callback, bumping the HAL tick once per PWM period (~20 kHz
 *    instead of 1 kHz) and breaking HAL_Delay() and every HAL timeout.
 * So this file never defines that callback and never passes TIM2 to
 * HAL_TIM_IRQHandler().
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

#define PWM_FREQ_HZ    20000U
#define PWM_RESOLUTION 4096U // ARR = 4095, 12-bit duty

#define WHEEL_THIRD 85U // ~120 degrees of a 256-step wheel

#define RAINBOW_STEP_MS  1000U
#define RAINBOW_PAUSE_MS 500U
#define FADE_STEP_MS     10U // ~2.56 s per trip around the color wheel

#define MS_PER_S          1000U
#define TIM2_IRQ_PRIORITY 14U

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

// round(127.5 + 127.5 * sin(2*pi*i/256))
static const uint8_t sin_lut[256] = {
    128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173, 176, 179, 182,
    185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215, 218, 220, 222, 224, 226, 228,
    230, 232, 234, 235, 237, 238, 240, 241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253,
    253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251, 250,
    250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232, 230, 228, 226, 224, 222,
    220, 218, 215, 213, 211, 208, 206, 203, 201, 198, 196, 193, 190, 188, 185, 182, 179, 176, 173,
    170, 167, 165, 162, 158, 155, 152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115,
    112, 109, 106, 103, 100, 97,  93,  90,  88,  85,  82,  79,  76,  73,  70,  67,  65,  62,  59,
    57,  54,  52,  49,  47,  44,  42,  40,  37,  35,  33,  31,  29,  27,  25,  23,  21,  20,  18,
    17,  15,  14,  12,  11,  10,  9,   7,   6,   5,   5,   4,   3,   2,   2,   1,   1,   1,   0,
    0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,   10,
    11,  12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,  37,  40,  42,  44,
    47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,  79,  82,  85,  88,  90,  93,  97,
    100, 103, 106, 109, 112, 115, 118, 121, 124,
};

// round(4095 * (i/255)^2.2): 8-bit perceived brightness -> 12-bit compare value
static const uint16_t gamma_lut[256] = {
    0,    0,    0,    0,    0,    1,    1,    2,    2,    3,    3,    4,    5,    6,    7,    8,
    9,    11,   12,   14,   15,   17,   19,   21,   23,   25,   27,   29,   32,   34,   37,   40,
    43,   46,   49,   52,   55,   59,   62,   66,   70,   73,   77,   82,   86,   90,   95,   99,
    104,  109,  114,  119,  124,  129,  135,  140,  146,  152,  158,  164,  170,  176,  182,  189,
    196,  202,  209,  216,  224,  231,  238,  246,  254,  261,  269,  277,  286,  294,  302,  311,
    320,  328,  337,  347,  356,  365,  375,  384,  394,  404,  414,  424,  435,  445,  456,  467,
    477,  488,  500,  511,  522,  534,  545,  557,  569,  581,  594,  606,  619,  631,  644,  657,
    670,  683,  697,  710,  724,  738,  752,  766,  780,  794,  809,  823,  838,  853,  868,  884,
    899,  914,  930,  946,  962,  978,  994,  1011, 1027, 1044, 1061, 1078, 1095, 1112, 1130, 1147,
    1165, 1183, 1201, 1219, 1237, 1256, 1274, 1293, 1312, 1331, 1350, 1370, 1389, 1409, 1429, 1449,
    1469, 1489, 1509, 1530, 1551, 1572, 1593, 1614, 1635, 1657, 1678, 1700, 1722, 1744, 1766, 1789,
    1811, 1834, 1857, 1880, 1903, 1926, 1950, 1974, 1997, 2021, 2045, 2070, 2094, 2119, 2143, 2168,
    2193, 2219, 2244, 2270, 2295, 2321, 2347, 2373, 2400, 2426, 2453, 2479, 2506, 2534, 2561, 2588,
    2616, 2644, 2671, 2700, 2728, 2756, 2785, 2813, 2842, 2871, 2900, 2930, 2959, 2989, 3019, 3049,
    3079, 3109, 3140, 3170, 3201, 3232, 3263, 3295, 3326, 3358, 3390, 3421, 3454, 3486, 3518, 3551,
    3584, 3617, 3650, 3683, 3716, 3750, 3784, 3818, 3852, 3886, 3920, 3955, 3990, 4025, 4060, 4095,
};

TIM_HandleTypeDef htim2;

// TIM2 update events per fade step, set by rgb_fade_start() before the IRQ is enabled
static uint32_t fade_div = 1U;

void MX_TIM2_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
void TIM2_IRQHandler(void);

// Set the LED to any color, each channel 0 (off) to 255 (full brightness)
void rgb_set(uint8_t red, uint8_t green, uint8_t blue) {
    __HAL_TIM_SET_COMPARE(&htim2, RED_CH, gamma_lut[red]);
    __HAL_TIM_SET_COMPARE(&htim2, GREEN_CH, gamma_lut[green]);
    __HAL_TIM_SET_COMPARE(&htim2, BLUE_CH, gamma_lut[blue]);
}

// Set the LED to a color struct, e.g. rgb_set_color(RGB_ORANGE)
void rgb_set_color(rgb_color_t color) {
    rgb_set(color.r, color.g, color.b);
}

// Color at a point on the wheel (0-255 wraps): three sines 120 degrees apart
rgb_color_t wheel_to_rgb(uint8_t phase) {
    return (rgb_color_t){sin_lut[phase], sin_lut[(uint8_t)(phase + WHEEL_THIRD)],
                         sin_lut[(uint8_t)(phase + (2U * WHEEL_THIRD))]};
}

// Timer kernel clock for APB1 timers: PCLK1, doubled when the APB1 prescaler is not 1
static uint32_t tim2_clock_hz(void) {
    uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_HCLK_DIV1) {
        pclk1 *= 2U;
    }
    return pclk1;
}

// Start stepping the color wheel from the TIM2 update interrupt
void rgb_fade_start(void) {
    // update events per FADE_STEP_MS, from the PWM frequency the prescaler actually produced
    uint32_t period_ticks = (htim2.Init.Prescaler + 1U) * PWM_RESOLUTION;
    uint32_t ticks_per_step = (tim2_clock_hz() / MS_PER_S) * FADE_STEP_MS;
    fade_div = (ticks_per_step + (period_ticks / 2U)) / period_ticks;
    if (fade_div == 0U) {
        fade_div = 1U;
    }

    HAL_NVIC_SetPriority(TIM2_IRQn, TIM2_IRQ_PRIORITY, 0U);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
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

    // TIM2_IRQHandler does the rest
    rgb_fade_start();
    while (1) {
    }

    return 0;
}

// Handles the TIM2 update flag directly -- see the header comment for why this
// must not go through HAL_TIM_IRQHandler()
void TIM2_IRQHandler(void) {
    static uint32_t updates = 0U;
    static uint8_t phase = 0U;

    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET) {
        // clear first so the write lands before the ISR returns and can't re-trigger it
        __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);

        if (++updates >= fade_div) {
            updates = 0U;
            rgb_set_color(wheel_to_rgb(phase++));
        }
    }
}

void MX_TIM2_Init(void) {
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    // computed at runtime so the PWM frequency doesn't depend on the HSE value.
    // A slow timer clock can make the division 0; clamp so PSC can't wrap to 0xFFFFFFFF
    // (the PWM then just runs below PWM_FREQ_HZ).
    uint32_t psc_div = tim2_clock_hz() / (PWM_RESOLUTION * PWM_FREQ_HZ);
    if (psc_div == 0U) {
        psc_div = 1U;
    }

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = psc_div - 1U;
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
        HAL_NVIC_DisableIRQ(TIM2_IRQn);
        __HAL_RCC_TIM2_CLK_DISABLE();
        HAL_GPIO_DeInit(RGB_PORT, RED_PIN | GREEN_PIN | BLUE_PIN);
    }
}
