#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "PDU_Clock.h"
#include "PDU_GPIO.h"


static StaticTask_t ledTaskBuffer;
static StackType_t  ledStack[configMINIMAL_STACK_SIZE * 2];

static void led_task(void *pvParameters)
{
    (void)pvParameters;

    while (1) {
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(1000));
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);

        HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(1000));
        HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
    }
}

int main(void)
{
    HAL_Init();
    PDU_Clock_Init();
    PDU_GPIO_Init();

    xTaskCreateStatic(led_task,
        "LED Test",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        tskIDLE_PRIORITY + 1,
        ledStack,
        &ledTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
