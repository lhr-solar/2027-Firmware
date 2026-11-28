#pragma once


#include "pinDefs.h"

// SPI1, shared by all 6 TPS2HCS08 chips. CS is driven by software (pinDefs.h).
extern SPI_HandleTypeDef hspi1;


// Returns HAL_OK, or the HAL_SPI_Init error. Caller decides how to handle failure.
HAL_StatusTypeDef PDU_SPI_Init(void);
