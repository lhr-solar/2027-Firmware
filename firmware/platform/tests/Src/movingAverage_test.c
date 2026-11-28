#include "movingAverage.h"
#include "stm32xx_hal.h"
#include "UART.h"
#include "log.h"

StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];

#define SERIAL_PORT 0

DEFINE_MOVING_AVG(MA_u16, uint16_t, uint32_t, 10)
DEFINE_MOVING_AVG(MA_u32, uint32_t, uint64_t, 10)
DEFINE_MOVING_AVG(MA_s32, int32_t, int64_t, 10)

void test_u16() {
    LOG(SERIAL_PORT, L_INFO, "\n--- uint16_t test ---\n\r");

    MA_u16 ma;
    MA_u16_init(&ma);

    uint16_t inputs[] = {2, 4, 6, 8, 10, 12};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        uint16_t avg = MA_u16_update(&ma, inputs[i]);
        LOG(SERIAL_PORT, L_INFO, "input=%u -> avg=%u\n\r", inputs[i], avg);
    }
}

void test_u32() {
    LOG(SERIAL_PORT, L_INFO, "\n--- uint32_t test ---\n\r");

    MA_u32 ma;
    MA_u32_init(&ma);

    uint32_t inputs[] = {100, 200, 300, 400, 500, 600};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        uint32_t avg = MA_u32_update(&ma, inputs[i]);
        LOG(SERIAL_PORT, L_INFO, "input=%lu -> avg=%lu\n\r", inputs[i], avg);
    }
}

void test_s32() {
    LOG(SERIAL_PORT, L_INFO, "\n--- int32_t test ---\n\r");

    MA_s32 ma;
    MA_s32_init(&ma);

    int32_t inputs[] = {-2, -4, 6, 8, 10, -10};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        int32_t avg = MA_s32_update(&ma, inputs[i]);
        LOG(SERIAL_PORT, L_INFO, "input=%ld -> avg=%ld\n\r", inputs[i], avg);
    }
}

void TxTask(void* argument) {
    LOG(SERIAL_PORT, L_INFO, "Running moving average tests...\n\r");

    test_u16();
    test_u32();
    test_s32();

    while (1) {
    }
}


int main() {

    HAL_Init();
    SystemClock_Config();
    MX_USB_PCD_Init();
    // also starts the USB poll task
    log_init(&(log_init_t){.levels = LOG_BIT(L_INFO)});

    // clang-format off
    xTaskCreateStatic(
        TxTask,
        "TX",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        txTaskStack,
        &txTaskBuffer);
    // clang-format on

    vTaskStartScheduler();


    while (1) {
    }


    return 0;
}

void HAL_UART_MspGPIOInit(UART_HandleTypeDef* huart) {

    #ifdef USART3
    GPIO_InitTypeDef init = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    init.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &init);
    #endif
}
