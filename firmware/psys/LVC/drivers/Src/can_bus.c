#include "can_bus.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"

/**
 * @brief Initialize CAN (FDCAN2, classic frames)
 */
can_status_t CANbus_init(void)
{
    // Create Filter (accept all standard IDs into FIFO0)
    FDCAN_FilterTypeDef sFilterConfig = {0};
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x000;

    // Setup FDCAN2 Initialization
    hfdcan2->Instance = FDCAN2;
    hfdcan2->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan2->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan2->Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan2->Init.AutoRetransmission = DISABLE;
    hfdcan2->Init.TransmitPause = DISABLE;
    hfdcan2->Init.ProtocolException = DISABLE;
    hfdcan2->Init.NominalPrescaler = 20;
    hfdcan2->Init.NominalSyncJumpWidth = 1;
    hfdcan2->Init.NominalTimeSeg1 = 13;
    hfdcan2->Init.NominalTimeSeg2 = 2;
    // data phase unused in classic mode
    hfdcan2->Init.DataPrescaler = 1;
    hfdcan2->Init.DataSyncJumpWidth = 1;
    hfdcan2->Init.DataTimeSeg1 = 1;
    hfdcan2->Init.DataTimeSeg2 = 1;
    hfdcan2->Init.StdFiltersNbr = 1;
    hfdcan2->Init.ExtFiltersNbr = 0;
    // FIFO mode sends in request order; QUEUE mode would send by ID priority
    hfdcan2->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    // Initialize FDCAN2
    if (can_fd_init(hfdcan2, &sFilterConfig) != CAN_OK) {
        return CAN_ERR;
    }
    // Start FDCAN2
    if (can_fd_start(hfdcan2) != CAN_OK) {
        return CAN_ERR;
    }
    return CAN_OK;
}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  if(hfdcan->Instance==FDCAN2) {
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB5     ------> FDCAN2_RX (AF9)
    PB6     ------> FDCAN2_TX (AF9)
    */
    GPIO_InitStruct.Pin = CAN_RX_PIN|CAN_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(CAN_RX_PORT, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, CAN_INTERRUPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, CAN_INTERRUPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan) {
  if(hfdcan->Instance==FDCAN2) {
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    HAL_GPIO_DeInit(CAN_RX_PORT, CAN_RX_PIN|CAN_TX_PIN);

    /* FDCAN2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
  }
}

can_status_t CANbus_send(uint16_t id, uint8_t data[], uint8_t length, TickType_t timeout) {
    if (length > 8) {
        return CAN_ERR;
    }
    FDCAN_TxHeaderTypeDef tx_header = {0};
    tx_header.Identifier = id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES(length);
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;

    return can_fd_send(hfdcan2, &tx_header, data, timeout);
}

can_status_t CANbus_recv(uint16_t id, FDCAN_RxHeaderTypeDef *header, uint8_t data[], TickType_t timeout) {
  return can_fd_recv(hfdcan2, id, header, data, timeout);
}
