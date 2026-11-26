#pragma once

#include "CAN_FD.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"

void CAN_Init();
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan);
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan);