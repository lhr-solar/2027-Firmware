#include "stm32xx_hal.h"
#include "tusb_port.h"
#include "printf.h"
#include "CAN_FD.h"
#include "gpio_helpers.h"


/*
FDCAN pinout

IF YOU ARE USING THIS TEST, MAKE SURE TO CHANGE TO YOUR BOARD'S PINOUT!!!!

note: PD0/PD1 only exist on LQFP100. PA11/PA12 are USB, PB8 is BOOT0, so FDCAN1 stays off both.
*/
#define FDCAN1_RX_PORT GPIOD
#define FDCAN1_RX_PIN  GPIO_PIN_0
#define FDCAN1_RX_AF   GPIO_AF9_FDCAN1
#define FDCAN1_TX_PORT GPIOD
#define FDCAN1_TX_PIN  GPIO_PIN_1
#define FDCAN1_TX_AF   GPIO_AF9_FDCAN1

#define FDCAN2_RX_PORT GPIOB
#define FDCAN2_RX_PIN  GPIO_PIN_5
#define FDCAN2_RX_AF   GPIO_AF9_FDCAN2
#define FDCAN2_TX_PORT GPIOB
#define FDCAN2_TX_PIN  GPIO_PIN_6
#define FDCAN2_TX_AF   GPIO_AF9_FDCAN2

#define FDCAN3_RX_PORT GPIOB
#define FDCAN3_RX_PIN  GPIO_PIN_3
#define FDCAN3_RX_AF   GPIO_AF11_FDCAN3
#define FDCAN3_TX_PORT GPIOB
#define FDCAN3_TX_PIN  GPIO_PIN_4
#define FDCAN3_TX_AF   GPIO_AF11_FDCAN3

StaticTask_t task_1_buffer;
StackType_t task_1_stack[512];

StaticTask_t task_2_buffer;
StackType_t task_2_stack[512];

StaticTask_t rx_task_buffer;
StackType_t rx_task_stack[512];

/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}

#define TEST_QUEUE_SET_ID_COUNT 2

static const uint32_t test_can_ids[TEST_QUEUE_SET_ID_COUNT] = {
    0x001,
    0x321,
};

#define QUEUESET_NUM_ELEMENTS 30

static StaticQueue_t test_queue_set_buffer;
static uint8_t test_queue_set_storage[QUEUESET_NUM_ELEMENTS * sizeof(QueueSetMemberHandle_t)];

static QueueSetHandle_t canQueueSet;

/*
configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY is the maximum FreeRTOS priority for an interrupt 
*/
#define FDCAN_NVIC_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3

// LSOM has heartbeat pin defined as PC3
// Most other nucleos have a heartbeat for A5
#define LED_PIN  GPIO_PIN_3
#define LED_PORT GPIOC

// Function definitions
void Heartbeat_Init();

void Error_Handler() {
    while (1) {
        // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(1000);
    }
}

void Success_Handler() {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

static void rx_task(void* pvParameters) {

    canQueueSet = xQueueCreateSetStatic(QUEUESET_NUM_ELEMENTS, test_queue_set_storage,
                                        &test_queue_set_buffer);

    can_id_set_t canQueueSetStruct = {
        .ids = test_can_ids,
        .id_count = TEST_QUEUE_SET_ID_COUNT,
        .queueSet = canQueueSet,
    };

    can_status_t setInitStatus = can_fd_register_id_set(hfdcan1, &canQueueSetStruct);

    if (setInitStatus != CAN_OK) {
        Error_Handler();
    }

    uint32_t id = 0;

    FDCAN_RxHeaderTypeDef fdcan1_rx_header = {0};
    uint8_t fdcan1_rx_data[8] = {0};

    while (1) {

        if (can_fd_recv_set(hfdcan1, &canQueueSetStruct, &id, portMAX_DELAY) == CAN_OK) {
            can_fd_recv(hfdcan1, id, &fdcan1_rx_header, fdcan1_rx_data, 0);

            printf("Recieved Can message from id: %lu\n\r", id);

            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        }
    }
}


static void task_1(void* pvParameters) {

    int test_id = 0x321;
    FDCAN_TxHeaderTypeDef tx_header = {0};
    tx_header.Identifier = test_id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
    tx_header.MessageMarker = 0;

    // send x1234 to 0x321
    uint8_t tx_data[8] = {0};
    tx_data[0] = 0x12;
    tx_data[1] = 0x34;
    tx_data[2] = 0x56;
    tx_data[3] = 0x78;
    tx_data[4] = 0x9A;
    tx_data[5] = 0xBC;
    tx_data[6] = 0xDE;
    tx_data[7] = 0xFF;

    while (1) {

        #ifdef FDCAN1
        if (can_fd_send(hfdcan1, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR) {
            Error_Handler();
        }
        #endif

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void task_2(void* pvParameters) {

    int test_id = 0x001;
    FDCAN_TxHeaderTypeDef tx_header = {0};
    tx_header.Identifier = test_id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
    tx_header.MessageMarker = 0;

    // send x1234 to 0x321
    uint8_t tx_data[8] = {0};
    tx_data[7] = 0x12;
    tx_data[6] = 0x34;
    tx_data[5] = 0x56;
    tx_data[4] = 0x78;
    tx_data[3] = 0x9A;
    tx_data[2] = 0xBC;
    tx_data[1] = 0xDE;
    tx_data[0] = 0xFF;

    while (1) {

        #ifdef FDCAN1
        if (can_fd_send(hfdcan1, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR) {
            Error_Handler();
        }
        #endif

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    HAL_Init();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    SystemClock_Config();

    Heartbeat_Init(); // enable LED for LED_PORT
    MX_USB_PCD_Init();
    // tusb must be up before the poll task runs, tud_task() returns immediately otherwise
    printf_init();

    #ifdef FDCAN1
    hfdcan1->Instance = FDCAN1;
    hfdcan1->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

    // internal loopback shorts the CAN RX and TX internally.
    // In production, this should be set to FDCAN_MODE_NORMAL
    hfdcan1->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
    hfdcan1->Init.AutoRetransmission = DISABLE;
    hfdcan1->Init.TransmitPause = DISABLE;
    hfdcan1->Init.ProtocolException = DISABLE;
    hfdcan1->Init.NominalPrescaler = 24;
    hfdcan1->Init.NominalSyncJumpWidth = 1;
    hfdcan1->Init.NominalTimeSeg1 = 13;
    hfdcan1->Init.NominalTimeSeg2 = 2;
    hfdcan1->Init.DataPrescaler = 1;
    hfdcan1->Init.DataSyncJumpWidth = 1;
    hfdcan1->Init.DataTimeSeg1 = 1;
    hfdcan1->Init.DataTimeSeg2 = 1;
    hfdcan1->Init.StdFiltersNbr = 1;
    hfdcan1->Init.ExtFiltersNbr = 0;
    hfdcan1->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;


    // FDCAN1 Filter Config
    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0

    // this accepts all CAN IDs
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x000;

    if (can_fd_init(hfdcan1, &sFilterConfig) != CAN_OK) {
        Error_Handler();
    }

    if (can_fd_start(hfdcan1) != CAN_OK) {
        Error_Handler();
    }
    #endif

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

    // you can only send CAN messages within a FreeRTOS task
    // clang-format off
    xTaskCreateStatic(
        task_1,
        "Transmit task 1",
        512,
        NULL,
        tskIDLE_PRIORITY + 2,
        task_1_stack,
        &task_1_buffer);
    // clang-format on

    // clang-format off
    xTaskCreateStatic(
        task_2,
        "Transmit task 2",
        512,
        NULL,
        tskIDLE_PRIORITY + 2,
        task_2_stack,
        &task_2_buffer);
    // clang-format on

    // clang-format off
    xTaskCreateStatic(
        rx_task,
        "rx task",
        512,
        NULL,
        tskIDLE_PRIORITY + 2,
        rx_task_stack,
        &rx_task_buffer);
    // clang-format on

    vTaskStartScheduler();
    while (1) {
    }

    Error_Handler();
    return 0;
}

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if (0) { // placeholder for if else if there are no FDCANs defined

    }
    #ifdef FDCAN1
    else if (hfdcan->Instance == FDCAN1) {

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* FDCAN1 clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        enable_gpio_clock(FDCAN1_RX_PORT);
        enable_gpio_clock(FDCAN1_TX_PORT);
        GPIO_InitStruct.Pin = FDCAN1_RX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = FDCAN1_RX_AF;
        HAL_GPIO_Init(FDCAN1_RX_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = FDCAN1_TX_PIN;
        GPIO_InitStruct.Alternate = FDCAN1_TX_AF;
        HAL_GPIO_Init(FDCAN1_TX_PORT, &GPIO_InitStruct);

        /* FDCAN1 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
    }
    #endif // FDCAN1
    #ifdef FDCAN2
    else if (hfdcan->Instance == FDCAN2) {

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* FDCAN2 clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        enable_gpio_clock(FDCAN2_RX_PORT);
        enable_gpio_clock(FDCAN2_TX_PORT);
        GPIO_InitStruct.Pin = FDCAN2_RX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = FDCAN2_RX_AF;
        HAL_GPIO_Init(FDCAN2_RX_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = FDCAN2_TX_PIN;
        GPIO_InitStruct.Alternate = FDCAN2_TX_AF;
        HAL_GPIO_Init(FDCAN2_TX_PORT, &GPIO_InitStruct);

        /* FDCAN2 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);

    }
    #endif // FDCAN2

    #ifdef FDCAN3
    else if (hfdcan->Instance == FDCAN3) {
        /** Initializes the peripherals clocks
  */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* FDCAN3 clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        enable_gpio_clock(FDCAN3_RX_PORT);
        enable_gpio_clock(FDCAN3_TX_PORT);
        GPIO_InitStruct.Pin = FDCAN3_RX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = FDCAN3_RX_AF;
        HAL_GPIO_Init(FDCAN3_RX_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = FDCAN3_TX_PIN;
        GPIO_InitStruct.Alternate = FDCAN3_TX_AF;
        HAL_GPIO_Init(FDCAN3_TX_PORT, &GPIO_InitStruct);

        /* FDCAN3 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN3_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT1_IRQn);
    }
    #endif
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan) {

    if (hfdcan->Instance == FDCAN1) {
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        HAL_GPIO_DeInit(FDCAN1_RX_PORT, FDCAN1_RX_PIN);
        HAL_GPIO_DeInit(FDCAN1_TX_PORT, FDCAN1_TX_PIN);

        /* FDCAN1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
        /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

        /* USER CODE END FDCAN1_MspDeInit 1 */
    } else if (hfdcan->Instance == FDCAN2) {
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        HAL_GPIO_DeInit(FDCAN2_RX_PORT, FDCAN2_RX_PIN);
        HAL_GPIO_DeInit(FDCAN2_TX_PORT, FDCAN2_TX_PIN);

        /* FDCAN2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
        /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

        /* USER CODE END FDCAN2_MspDeInit 1 */
    } else if (hfdcan->Instance == FDCAN3) {
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        HAL_GPIO_DeInit(FDCAN3_RX_PORT, FDCAN3_RX_PIN);
        HAL_GPIO_DeInit(FDCAN3_TX_PORT, FDCAN3_TX_PIN);

        /* FDCAN3 interrupt Deinit */
        HAL_NVIC_DisableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN3_IT1_IRQn);
    }
}


// Initialize clock for heartbeat LED port
void Heartbeat_Init() {
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN,
    };

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

    HAL_GPIO_Init(LED_PORT, &led_config);
}
