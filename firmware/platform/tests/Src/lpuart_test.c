/* LPUART test

- Setups up LPUART loopback mode (the TX line is connected to the RX line)
- Send messages and verify correctness
*/
#include "stm32xx_hal.h"
#include "UART.h"
#include "tusb_port.h"
#include "printf.h" // only for debugging

void TxTask(void* argument);
void RxTask(void* argument);

// Static task creation resources
StaticTask_t txTaskBuffer;
StaticTask_t rxTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
StackType_t rxTaskStack[configMINIMAL_STACK_SIZE];
/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}

// Heartbeat pin on stm32l432kcu is PB3
#if defined(STM32L432xx)
    #define LED_PIN  GPIO_PIN_3
    #define LED_PORT GPIOB
#elif defined(STM32L431xx)
    #define LED_PIN  GPIO_PIN_11
    #define LED_PORT GPIOB
#elif defined(STM32G473xx)
    // LSOM
    #define LED_PIN  GPIO_PIN_3
    #define LED_PORT GPIOC
#else
// Most other nucleos have a heartbeat for A5
    #define LED_PIN  GPIO_PIN_5
    #define LED_PORT GPIOA
#endif

// Initialize clock for heartbeat LED port
void Heartbeat_Clock_Init() {
    switch ((uint32_t)LED_PORT) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
    }
}

void TxTask(void* argument) {

    const TickType_t xDelay = pdMS_TO_TICKS(1000); // 1 second delay
    #ifdef LPUART1
    uint8_t testData[] = "Test Message 123\r\n";
    const uint8_t msgLen = sizeof(testData) - 1;
    #endif
    uint32_t txCount = 0;

    while (1) {
        // Send test message
        uart_status_t status = UART_ERR;
        #ifdef LPUART1
        status = uart_send(hlpuart1, testData, msgLen, portMAX_DELAY);
        #endif
        if (status == UART_OK) {
            txCount++;
            // Toggle LED to indicate successful transmission
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            // printf("sucesfully transmitted!\n\r");
        }

        vTaskDelay(xDelay);
    }
}

void RxTask(void* argument) {

    uint8_t rxBuffer;
    uint32_t rxCount = 0;

    while (1) {
        uart_status_t status = UART_ERR;
        #ifdef LPUART1
        status = uart_recv(hlpuart1, &rxBuffer, 1, portMAX_DELAY);
        #endif

        if (status == UART_OK) {
            rxCount++;

            // Print received character, printf drops what doesn't fit in the USB TX buffer
            // so let it drain. The UART RX queue holds the rest of the message meanwhile
            printf("RX[%lu]: %c\n\r", rxCount, rxBuffer);
            vTaskDelay(pdMS_TO_TICKS(2));
        }
    }
}

int main(void) {

    HAL_Init();
    SystemClock_Config();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN,
    };

    Heartbeat_Clock_Init();               // enable clock for LED_PORT
    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config

    #ifdef LPUART1
    hlpuart1->Instance = LPUART1;
    hlpuart1->Init.BaudRate = 115200;
    hlpuart1->Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1->Init.StopBits = UART_STOPBITS_1;
    hlpuart1->Init.Parity = UART_PARITY_NONE;
    hlpuart1->Init.Mode = UART_MODE_TX_RX;
    hlpuart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hlpuart1->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    #ifndef STM32L4xx
    // ClockPrescaler is not a field on my L4 uart handles
    hlpuart1->Init.ClockPrescaler = UART_PRESCALER_DIV1;
    #endif
    hlpuart1->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    #endif


    #ifdef LPUART1
    // Initialize LPUART1
    uart_status_t status = uart_init(hlpuart1);
    if (status != UART_OK) {
        Error_Handler();
    }
    #endif


    MX_USB_PCD_Init();
    // tusb must be up before the poll task runs, tud_task() returns immediately otherwise
    printf_init();

    // Create the tasks statically
    // clang-format off
    xTaskCreateStatic(
        poll_events,
        "Poll Event Queue",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 3,
        pollEventsStack,
        &pollEventsTaskBuffer);
    // clang-format on

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

    // clang-format off
    xTaskCreateStatic(
        RxTask,
        "RX",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        rxTaskStack,
        &rxTaskBuffer);
    // clang-format on

    // Start the scheduler
    vTaskStartScheduler();

    while (1) {
        // Should never get here
    }
}


void HAL_UART_MspGPIOInit(UART_HandleTypeDef* huart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // UNUSED is for the case where none of the handles are enabled in the chip
    UNUSED(GPIO_InitStruct);
    #ifdef LPUART1
    if (huart->Instance == LPUART1) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**LPUART1 GPIO Configuration
        PB10     ------> LPUART1_RX
        PB11     ------> LPUART1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    #endif
}

void Error_Handler(void) {
    while (1) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    }
}
