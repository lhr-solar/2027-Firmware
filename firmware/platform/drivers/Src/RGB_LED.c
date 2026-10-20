#include "RGB_LED.h"

#include <stddef.h>

#define RGB_LED_PWM_FREQ_HZ    20000U
#define RGB_LED_PWM_RESOLUTION 4096U // ARR = 4095, 12-bit duty

#define RGB_LED_WHEEL_THIRD 85U // 120 degrees on a 256-step wheel

// Colour wheel shape: one period of a sine scaled to 0-255, read by
// rgb_led_wheel() at three offsets to get red, green and blue.
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

// Brightness correction: the eye sees PWM duty non-linearly, so rgb_led_set()
// maps each 0-255 channel through this to a 12-bit compare value.
// round(4095 * (i/255)^2.2)
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

// Kernel clock of an APB1 timer: PCLK1, doubled when the APB1 prescaler is not 1.
// TODO: replace with the shared clock helper once Aarav's clock PR lands.
static uint32_t apb1_timer_clock_hz(void) {
    uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_HCLK_DIV1) {
        pclk1 *= 2U;
    }
    return pclk1;
}

// Turn on the bus clock for a supported timer. Only APB1 timers are allowed
// because apb1_timer_clock_hz() is the only clock calculation the driver has.
static rgb_led_status_t enable_timer_clock(const TIM_TypeDef* timer) {
    if (timer == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    } else if (timer == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    } else if (timer == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE();
    } else if (timer == TIM5) {
        __HAL_RCC_TIM5_CLK_ENABLE();
    } else {
        return RGB_LED_ERROR;
    }
    return RGB_LED_OK;
}

// Turn on the bus clock for the GPIO port the LED pins are on.
static rgb_led_status_t enable_gpio_clock(const GPIO_TypeDef* port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    } else {
        return RGB_LED_ERROR;
    }
    return RGB_LED_OK;
}

// Write all three compare registers and remember the colour. Caller must hold
// led->mutex. The registers are preloaded, so the new duty cycles take effect
// together at the next timer update rather than one channel at a time.
static void write_compare(rgb_led_handle_t* led, rgb_led_color_t color) {
    __HAL_TIM_SET_COMPARE(led->htim, led->red_channel, gamma_lut[color.r]);
    __HAL_TIM_SET_COMPARE(led->htim, led->green_channel, gamma_lut[color.g]);
    __HAL_TIM_SET_COMPARE(led->htim, led->blue_channel, gamma_lut[color.b]);
    led->color = color;
}

// See RGB_LED.h. Order: clocks, pins, timer base, channels, start PWM, then
// the mutex last so a handle whose init failed is rejected by set/get.
rgb_led_status_t rgb_led_init(rgb_led_handle_t* led, TIM_HandleTypeDef* htim,
                              const rgb_led_config_t* cfg) {
    if (led == NULL || htim == NULL || cfg == NULL) {
        return RGB_LED_NULL_ERROR;
    }
    if (enable_timer_clock(cfg->timer) != RGB_LED_OK ||
        enable_gpio_clock(cfg->port) != RGB_LED_OK) {
        return RGB_LED_ERROR;
    }

    led->htim = htim;
    led->red_channel = cfg->red_channel;
    led->green_channel = cfg->green_channel;
    led->blue_channel = cfg->blue_channel;
    led->color = RGB_LED_OFF;
    led->mutex = NULL; // set last, so set/get refuse a handle whose init failed

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = cfg->red_pin | cfg->green_pin | cfg->blue_pin;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = cfg->alternate;
    HAL_GPIO_Init(cfg->port, &gpio);

    // A slow timer clock can make this 0; clamp so PSC can't wrap to 0xFFFFFFFF
    // (PWM then just runs below RGB_LED_PWM_FREQ_HZ).
    uint32_t psc_div = apb1_timer_clock_hz() / (RGB_LED_PWM_RESOLUTION * RGB_LED_PWM_FREQ_HZ);
    if (psc_div == 0U) {
        psc_div = 1U;
    }

    htim->Instance = cfg->timer;
    htim->Init.Prescaler = psc_div - 1U;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = RGB_LED_PWM_RESOLUTION - 1U;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(htim) != HAL_OK) {
        return RGB_LED_ERROR;
    }

    TIM_OC_InitTypeDef oc_cfg = {0};
    oc_cfg.OCMode = TIM_OCMODE_PWM1;
    oc_cfg.Pulse = 0;
    oc_cfg.OCPolarity = TIM_OCPOLARITY_HIGH; // common cathode: pin high = on
    oc_cfg.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(htim, &oc_cfg, led->red_channel) != HAL_OK ||
        HAL_TIM_PWM_ConfigChannel(htim, &oc_cfg, led->green_channel) != HAL_OK ||
        HAL_TIM_PWM_ConfigChannel(htim, &oc_cfg, led->blue_channel) != HAL_OK) {
        return RGB_LED_ERROR;
    }

    if (HAL_TIM_PWM_Start(htim, led->red_channel) != HAL_OK ||
        HAL_TIM_PWM_Start(htim, led->green_channel) != HAL_OK ||
        HAL_TIM_PWM_Start(htim, led->blue_channel) != HAL_OK) {
        return RGB_LED_ERROR;
    }

    led->mutex = xSemaphoreCreateMutexStatic(&led->mutexBuf);
    return RGB_LED_OK;
}

// See RGB_LED.h. Holds the mutex only for the three register writes.
rgb_led_status_t rgb_led_set(rgb_led_handle_t* led, rgb_led_color_t color, TickType_t timeout) {
    if (led == NULL || led->mutex == NULL) {
        return RGB_LED_NULL_ERROR;
    }
    if (xSemaphoreTake(led->mutex, timeout) != pdTRUE) {
        return RGB_LED_BUSY;
    }
    write_compare(led, color);
    xSemaphoreGive(led->mutex);
    return RGB_LED_OK;
}

// See RGB_LED.h. Returns the saved colour, not a read-back of the registers.
rgb_led_status_t rgb_led_get(rgb_led_handle_t* led, rgb_led_color_t* color, TickType_t timeout) {
    if (led == NULL || color == NULL || led->mutex == NULL) {
        return RGB_LED_NULL_ERROR;
    }
    if (xSemaphoreTake(led->mutex, timeout) != pdTRUE) {
        return RGB_LED_BUSY;
    }
    *color = led->color;
    xSemaphoreGive(led->mutex);
    return RGB_LED_OK;
}

// See RGB_LED.h. Reads the sine table at three points 120 degrees apart, so as
// one channel fades out the next fades in.
rgb_led_color_t rgb_led_wheel(uint8_t phase) {
    return (rgb_led_color_t){sin_lut[phase], sin_lut[(uint8_t)(phase + RGB_LED_WHEEL_THIRD)],
                             sin_lut[(uint8_t)(phase + (2U * RGB_LED_WHEEL_THIRD))]};
}
