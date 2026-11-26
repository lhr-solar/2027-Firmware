#pragma once

#include "CAN_FD.h"
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "stm32g484xx.h"

#define FDCAN_NVIC_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3 // 8

void CAN_Init();
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan);
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan);