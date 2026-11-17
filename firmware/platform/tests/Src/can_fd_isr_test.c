#include "CAN_FD.h"
#include "gpio_helpers.h"
#include "can_msgs.h"
#include "stm32xx_hal.h"
#include "queue.h"

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

StaticTask_t task_buffer;
StackType_t task_stack[512];

StaticTask_t task2_buffer;
StackType_t task2_stack[512];

/*
configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY is the maximum FreeRTOS priority
for an interrupt
*/
#define FDCAN_NVIC_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)

// LSOM has heartbeat pin defined as PC3
// Most other nucleos have a heartbeat for A5
#define LED_PIN  GPIO_PIN_3
#define LED_PORT GPIOC

// Scope pins toggled from the CAN callbacks
#define CAN_TX_PROFILE_PORT GPIOB
#define CAN_TX_PROFILE_PIN  GPIO_PIN_11

#define CAN_RX_PROFILE_PORT GPIOA
#define CAN_RX_PROFILE_PIN  GPIO_PIN_15

void can_profile_pins_init() {
    GPIO_InitTypeDef tx_pin_config = {
        .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Pin = CAN_TX_PROFILE_PIN};

    switch ((uint32_t)CAN_TX_PROFILE_PORT) {
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

    HAL_GPIO_Init(CAN_TX_PROFILE_PORT, &tx_pin_config);

    GPIO_InitTypeDef rx_pin_config = {
        .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Pin = CAN_RX_PROFILE_PIN};

    switch ((uint32_t)CAN_RX_PROFILE_PORT) {
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
    HAL_GPIO_Init(CAN_RX_PROFILE_PORT, &rx_pin_config);
}

// Initialize clock for heartbeat LED port
void Heartbeat_Init() {
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Pin = LED_PIN};

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

void Error_Handler() {
    while (1) {
        // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(1000);
    }
}

void Success_Handler() {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}


#define CANFD1_TX_QUEUE_SIZE 10

static StaticQueue_t mycanfd1_tx_queue_buffer;
static QueueHandle_t mycanfd1_tx_queue;
static uint8_t mycanfd1_tx_queue_storage[CANFD1_TX_QUEUE_SIZE * sizeof(can_rx_payload_t)];


static void task(void* pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        // HAL_GPIO_TogglePin(CAN_TX_PROFILE_PORT, CAN_TX_PROFILE_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
                             can_rx_payload_t recv_payload) {
    // only forward motorCAN messages to CarCAN

    BaseType_t hpw = pdFALSE; // added this

    if (hfdcan1 != NULL && hfdcan->Instance == hfdcan1->Instance) {
        FDCAN_TxHeaderTypeDef tx_header = {
            .Identifier = recv_payload.header.Identifier,
            .IdType = recv_payload.header.IdType,
            .TxFrameType = FDCAN_DATA_FRAME,
            .DataLength = recv_payload.header.DataLength,
            .ErrorStateIndicator = FDCAN_ESI_ACTIVE,
            .BitRateSwitch = FDCAN_BRS_OFF,
            .FDFormat = FDCAN_CLASSIC_CAN,
            .TxEventFifoControl = FDCAN_STORE_TX_EVENTS,
            .MessageMarker = 0,
        };

        can_fd_send_isr(hfdcan1, &tx_header, recv_payload.data, &hpw); //changed this
        HAL_GPIO_TogglePin(CAN_RX_PROFILE_PORT, CAN_RX_PROFILE_PIN);
        // don't yield at the end of this since the rest of the ISR needs to run
    }
}


int main(void) {
    HAL_Init();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    SystemClock_Config();

    Heartbeat_Init(); // enable LED for LED_PORT
    // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    can_profile_pins_init(); // enable pins we toggle during interrupts

    mycanfd1_tx_queue = xQueueCreateStatic(CANFD1_TX_QUEUE_SIZE, sizeof(can_rx_payload_t),
                                           mycanfd1_tx_queue_storage, &mycanfd1_tx_queue_buffer);

    if (mycanfd1_tx_queue == NULL) {
        Error_Handler();
    }

    #ifdef FDCAN1
    hfdcan1->Instance = FDCAN1;
    hfdcan1->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

    // internal loopback shorts the CAN RX and TX internally.
    // In production, this should be set to FDCAN_MODE_NORMAL
    hfdcan1->Init.Mode = FDCAN_MODE_NORMAL;
    // hfdcan1->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
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

    // ExtFiltersNB bits sets # of ID allowed
    // 0 = no extended ID allowed (all extended ID rejected)
    // 1 =  filter enabled to match to ID
    // each CAN needs its own filter
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

    #ifdef FDCAN2

    hfdcan2->Instance = FDCAN2;
    hfdcan2->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan2->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

    // internal loopback shorts the CAN RX and TX internally.
    // In production, this should be set to FDCAN_MODE_NORMAL
    hfdcan2->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
    hfdcan2->Init.AutoRetransmission = DISABLE;
    hfdcan2->Init.TransmitPause = DISABLE;
    hfdcan2->Init.ProtocolException = DISABLE;
    hfdcan2->Init.NominalPrescaler = 24;
    hfdcan2->Init.NominalSyncJumpWidth = 1;
    hfdcan2->Init.NominalTimeSeg1 = 13;
    hfdcan2->Init.NominalTimeSeg2 = 2;
    hfdcan2->Init.DataPrescaler = 1;
    hfdcan2->Init.DataSyncJumpWidth = 1;
    hfdcan2->Init.DataTimeSeg1 = 1;
    hfdcan2->Init.DataTimeSeg2 = 1;
    hfdcan2->Init.StdFiltersNbr = 1;

    // ExtFiltersNB bits sets # of ID allowed
    // 0 = no extended ID allowed (all extended ID rejected)
    // 1 =  filter enabled to match to ID
    // each CAN needs its own filter
    hfdcan2->Init.ExtFiltersNbr = 0;
    hfdcan2->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    // FDCAN2 Filter Config

    FDCAN_FilterTypeDef sFilterConfig2;
    sFilterConfig2.IdType = FDCAN_STANDARD_ID;
    sFilterConfig2.FilterIndex = 0;
    sFilterConfig2.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig2.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0

    // this accepts all CAN IDs
    sFilterConfig2.FilterID1 = 0x000;
    sFilterConfig2.FilterID2 = 0x000;

    if (can_fd_init(hfdcan2, &sFilterConfig2) != CAN_OK) {
        Error_Handler();
    }

    if (can_fd_start(hfdcan2) != CAN_OK) {
        Error_Handler();
    }

    #endif

    #ifdef FDCAN3

    hfdcan3->Instance = FDCAN3;
    hfdcan3->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan3->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

    // internal loopback shorts the CAN RX and TX internally.
    // In production, this should be set to FDCAN_MODE_NORMAL
    hfdcan3->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
    hfdcan3->Init.AutoRetransmission = DISABLE;
    hfdcan3->Init.TransmitPause = DISABLE;
    hfdcan3->Init.ProtocolException = DISABLE;
    hfdcan3->Init.NominalPrescaler = 24;
    hfdcan3->Init.NominalSyncJumpWidth = 1;
    hfdcan3->Init.NominalTimeSeg1 = 13;
    hfdcan3->Init.NominalTimeSeg2 = 2;
    hfdcan3->Init.DataPrescaler = 1;
    hfdcan3->Init.DataSyncJumpWidth = 1;
    hfdcan3->Init.DataTimeSeg1 = 1;
    hfdcan3->Init.DataTimeSeg2 = 1;
    hfdcan3->Init.StdFiltersNbr = 1;

    // ExtFiltersNB bits sets # of ID allowed
    // 0 = no extended ID allowed (all extended ID rejected)
    // 1 =  filter enabled to match to ID
    // each CAN needs its own filter
    hfdcan3->Init.ExtFiltersNbr = 0;
    hfdcan3->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    // FDCAN3 Filter Config

    FDCAN_FilterTypeDef sFilterConfig3;
    sFilterConfig3.IdType = FDCAN_STANDARD_ID;
    sFilterConfig3.FilterIndex = 0;
    sFilterConfig3.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig3.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0

    // this accepts all CAN IDs
    sFilterConfig3.FilterID1 = 0x000;
    sFilterConfig3.FilterID2 = 0x000;

    if (can_fd_init(hfdcan3, &sFilterConfig3) != CAN_OK) {
        Error_Handler();
    }

    if (can_fd_start(hfdcan3) != CAN_OK) {
        Error_Handler();
    }

    #endif
    // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    // you can only send CAN messages within a FreeRTOS task
    // clang-format off
    xTaskCreateStatic(
        task,
        "task",
        512,
        NULL,
        tskIDLE_PRIORITY + 2,
        task_stack,
        &task_buffer);
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
