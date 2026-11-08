#include "stm32xx_hal.h"
#include "sdcard.h"
#include <string.h>

// thread safe:
#include "FreeRTOS.h"
#include "task.h"

/* handles */
sd_handle_t sd;
SPI_HandleTypeDef hspi_user;

/* RTOS Task Buffers */
StaticTask_t xTask1Buffer;
StackType_t xTask1Stack[1024];

StaticTask_t xTask2Buffer;
StackType_t xTask2Stack[1024];

StaticTask_t xTask3Buffer;
StackType_t xTask3Stack[512];

/* pin def*/
#if defined(STM32L476xx)
    #define USER_SPI_INSTANCE SPI2
    #define USER_CS_PORT      GPIOB
    #define USER_CS_PIN       GPIO_PIN_12

    #define USER_SCK_PORT GPIOB
    #define USER_SCK_PIN  GPIO_PIN_10
    #define USER_SCK_AF   GPIO_AF5_SPI2

    #define USER_MISO_PORT GPIOC
    #define USER_MISO_PIN  GPIO_PIN_2
    #define USER_MISO_AF   GPIO_AF5_SPI2

    #define USER_MOSI_PORT GPIOC
    #define USER_MOSI_PIN  GPIO_PIN_3
    #define USER_MOSI_AF   GPIO_AF5_SPI2

    #define LED_PORT GPIOA
    #define LED_PIN  GPIO_PIN_5

#elif defined(STM32G473xx) || defined(STM32G4xx)

    #define USER_SPI_INSTANCE SPI1
    #define USER_CS_PORT      GPIOA
    #define USER_CS_PIN       GPIO_PIN_4

    #define USER_SCK_PORT GPIOA
    #define USER_SCK_PIN  GPIO_PIN_5
    #define USER_SCK_AF   GPIO_AF5_SPI1

    #define USER_MISO_PORT GPIOA
    #define USER_MISO_PIN  GPIO_PIN_6
    #define USER_MISO_AF   GPIO_AF5_SPI1

    #define USER_MOSI_PORT GPIOA
    #define USER_MOSI_PIN  GPIO_PIN_7
    #define USER_MOSI_AF   GPIO_AF5_SPI1

    #define LED_PORT GPIOC
    #define LED_PIN  GPIO_PIN_3

#else
    #define USER_SPI_INSTANCE SPI1
    #define USER_CS_PORT      GPIOB
    #define USER_CS_PIN       GPIO_PIN_9

    #define USER_SCK_PORT GPIOB
    #define USER_SCK_PIN  GPIO_PIN_13
    #define USER_SCK_AF   GPIO_AF5_SPI2

    #define USER_MISO_PORT GPIOB
    #define USER_MISO_PIN  GPIO_PIN_14
    #define USER_MISO_AF   GPIO_AF5_SPI2

    #define USER_MOSI_PORT GPIOB
    #define USER_MOSI_PIN  GPIO_PIN_15
    #define USER_MOSI_AF   GPIO_AF5_SPI2

    #define LED_PORT GPIOC
    #define LED_PIN  GPIO_PIN_3

#endif

void LED_Init(void);
void User_Hardware_Init(void);

void Task1_Entry(void* params);
void Task2_Entry(void* params);
void Task3_Entry(void* params);

// IRQ HANDLER
void SPI1_IRQHandler(void) {
    HAL_SPI_IRQHandler(&hspi_user);
}

void SPI2_IRQHandler(void) {
    HAL_SPI_IRQHandler(&hspi_user);
}

void SPI3_IRQHandler(void) {
    HAL_SPI_IRQHandler(&hspi_user);
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    LED_Init();
    User_Hardware_Init(); // Sets up SPI, Clocks, and NVIC Priority 10

    /* Link SD Handle */
    sd.hspi = &hspi_user;
    sd.cs_port = USER_CS_PORT;
    sd.cs_pin = USER_CS_PIN;

    // Initialize Queues and Mutexes
    // Spawns worker task
    if (USER_SD_Card_Init(&sd, tskIDLE_PRIORITY + 3) != SD_OK) {
        while (1) {
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            HAL_Delay(50);
        }
    }

    // Create Tasks
    // clang-format off
    xTaskCreateStatic(
        Task1_Entry,
        "WriteTask1",
        1024,
        NULL,
        tskIDLE_PRIORITY + 1,
        xTask1Stack,
        &xTask1Buffer);
    // clang-format on
    // clang-format off
    xTaskCreateStatic(
        Task2_Entry,
        "WriteTask2",
        1024,
        NULL,
        tskIDLE_PRIORITY + 1,
        xTask2Stack,
        &xTask2Buffer);
    // clang-format on
    // clang-format off
    xTaskCreateStatic(
        Task3_Entry,
        "HighFreq",
        512,
        NULL,
        tskIDLE_PRIORITY + 2,
        xTask3Stack,
        &xTask3Buffer);
    // clang-format on

    vTaskStartScheduler();
    while (1)
        ;
}

void Task1_Entry(void* params) {
    char* msg = "lsom37Task 1: Async\r\n";
    vTaskDelay(pdMS_TO_TICKS(2000));

    for (;;) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

        USER_SD_Card_Write_Async(&sd, "LOG1.TXT", msg, pdMS_TO_TICKS(10));

        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void Task2_Entry(void* params) {
    char* msg = "lsom37Task 2: Async\r\n";
    vTaskDelay(pdMS_TO_TICKS(2500));

    for (;;) {
        USER_SD_Card_Write_Async(&sd, "LOG1.TXT", msg, pdMS_TO_TICKS(10));
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void Task3_Entry(void* params) {
    for (;;) {
        // steady heartbeat on the logic analyzer -> the SD card isn't blocking the CPU.
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// recieve and transmit
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    sdcard_SPI_TxRxCpltCallback(hspi, &xHigherPriorityTaskWoken);

    // Context switch if a higher priority task was woken up
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi) {
    HAL_SPI_TxRxCpltCallback(hspi);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
    HAL_SPI_TxRxCpltCallback(hspi);
}

// GPIO for LED init
void LED_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void User_Hardware_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Force Clocks On
    #if defined(STM32L476xx)
    __HAL_RCC_SPI2_CLK_ENABLE();
    #elif defined(STM32G473xx) || defined(STM32G4xx)
    __HAL_RCC_SPI1_CLK_ENABLE();
    #else
    __HAL_RCC_SPI1_CLK_ENABLE();
    #endif

    /* Configure GPIO (SCK, MISO, MOSI, CS) */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = USER_SCK_PIN;
    GPIO_InitStruct.Alternate = USER_SCK_AF;
    HAL_GPIO_Init(USER_SCK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USER_MISO_PIN;
    GPIO_InitStruct.Alternate = USER_MISO_AF;
    HAL_GPIO_Init(USER_MISO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USER_MOSI_PIN;
    GPIO_InitStruct.Alternate = USER_MOSI_AF;
    HAL_GPIO_Init(USER_MOSI_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(USER_CS_PORT, USER_CS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = USER_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(USER_CS_PORT, &GPIO_InitStruct);

    /* Configure SPI Peripheral */
    hspi_user.Instance = USER_SPI_INSTANCE;
    hspi_user.Init.Mode = SPI_MODE_MASTER;
    hspi_user.Init.Direction = SPI_DIRECTION_2LINES;
    hspi_user.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi_user.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi_user.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi_user.Init.NSS = SPI_NSS_SOFT;

    #if defined(STM32G473xx) || defined(STM32G4xx)
    hspi_user.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    #else
    hspi_user.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    #endif

    hspi_user.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi_user.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi_user.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    #if defined(STM32L4xx)
    hspi_user.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi_user.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    #elif defined(STM32G473xx) || defined(STM32G4xx)
    hspi_user.Init.CRCPolynomial = 7;
    hspi_user.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi_user.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    #endif

    if (HAL_SPI_Init(&hspi_user) != HAL_OK) {
        while (1)
            ;
    }
    __HAL_SPI_ENABLE(&hspi_user);

    // ENABLE INTERRUPTS (Priority 10)
    #if defined(STM32L476xx)
    HAL_NVIC_SetPriority(SPI2_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
    #elif defined(STM32G473xx) || defined(STM32G4xx)
    HAL_NVIC_SetPriority(SPI1_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    #else
    HAL_NVIC_SetPriority(SPI1_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    #endif
}