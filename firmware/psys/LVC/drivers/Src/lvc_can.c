#include "lvc_can.h"

void CAN_Init(uint32_t mode) {
    hfdcan2->Instance = FDCAN2;
    hfdcan2->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan2->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

    // internal loopback shorts the CAN RX and TX internally.
    hfdcan2->Init.Mode = mode;
    hfdcan2->Init.AutoRetransmission = ENABLE;
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

    FDCAN_FilterTypeDef sFilterConfig = {0};
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0

    // this accepts all CAN IDs
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x000;

    if (can_fd_init(hfdcan2, &sFilterConfig) != CAN_OK) {
        Error_Handler();
    }

    if (can_fd_start(hfdcan2) != CAN_OK) {
        Error_Handler();
    }
}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
    __HAL_RCC_FDCAN_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = CAN_TX_PIN | CAN_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(CAN_TX_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan) {
    __HAL_RCC_FDCAN_CLK_DISABLE();

    HAL_GPIO_DeInit(CAN_TX_PORT, CAN_TX_PIN | CAN_RX_PIN);

    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
}