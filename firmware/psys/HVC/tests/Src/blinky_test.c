/**
 * @file blinky_test.c
 * @brief HVC bring-up test: FreeRTOS heartbeat cycling the RGB LED red -> green -> blue.
 *
 * Package: STM32G473CET6 (LQFP48). PA0/PA1/PA2 are plain push-pull outputs, no AF.
 *
 * Task layout:
 *   main()        HAL + clocks, creates InitTask, starts the scheduler
 *   InitTask      one-shot: GPIO init, 1 s lamp test, creates HeartbeatTask, deletes itself
 *   HeartbeatTask forever: one colour per STEP_MS, paced with vTaskDelayUntil
 *
 * What you should see:
 *   - all three channels on (white) for ~1 s
 *   - then red, green, blue, one at a time, 500 ms each, forever
 *
 * Diagnosing:
 *   - nothing lights: SystemClock_Config() hung in Error_Handler() (HSE crystal on
 *     PF0/PF1 did not start, expected 8 MHz) or the scheduler never started
 *   - stuck white: InitTask ran but HeartbeatTask did not
 */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"

// PA0 is labeled GPIO_Output in CubeMX; it drives the blue channel of the RGB LED
#define LED_BLUE_Pin       GPIO_PIN_0
#define LED_BLUE_GPIO_Port GPIOA

#define LED_ALL_Pins (GPIO_LED_RED_Pin | GPIO_LED_GREEN_Pin | LED_BLUE_Pin)

#define STEP_MS      500
#define LAMP_TEST_MS 1000

// init must finish before anything it sets up is used, so it outranks the heartbeat
#define INIT_TASK_PRIORITY      (tskIDLE_PRIORITY + 2)
#define HEARTBEAT_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t heartbeatTaskBuffer;
StackType_t heartbeatTaskStack[configMINIMAL_STACK_SIZE];

static void HeartbeatTask(void* argument) {
    (void)argument;

    const uint16_t sequence[] = {GPIO_LED_RED_Pin, GPIO_LED_GREEN_Pin, LED_BLUE_Pin};
    uint32_t i = 0;
    TickType_t lastWake = xTaskGetTickCount();

    while (1) {
        HAL_GPIO_WritePin(GPIOA, LED_ALL_Pins, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, sequence[i], GPIO_PIN_SET);
        i = (i + 1) % (sizeof(sequence) / sizeof(sequence[0]));

        // fixed period regardless of how long the body takes
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(STEP_MS));
    }
}

static void InitTask(void* argument) {
    (void)argument;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef led_config = {
        .Pin = LED_ALL_Pins,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(GPIOA, &led_config);

    // lamp test: every channel on at once
    HAL_GPIO_WritePin(GPIOA, LED_ALL_Pins, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(LAMP_TEST_MS));

    // clang-format off
    xTaskCreateStatic(
        HeartbeatTask,
        "Heartbeat",
        configMINIMAL_STACK_SIZE,
        NULL,
        HEARTBEAT_TASK_PRIORITY,
        heartbeatTaskStack,
        &heartbeatTaskBuffer);
    // clang-format on

    // one-shot: free the CPU time; static TCB/stack stay reserved but unused
    vTaskDelete(NULL);
}

int main() {
    HAL_Init();
    SystemClock_Config(); // must precede the scheduler: FreeRTOS sizes SysTick from SystemCoreClock

    // clang-format off
    xTaskCreateStatic(
        InitTask,
        "Init",
        configMINIMAL_STACK_SIZE,
        NULL,
        INIT_TASK_PRIORITY,
        initTaskStack,
        &initTaskBuffer);
    // clang-format on

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    while (1) {
    }

    return 0;
}
