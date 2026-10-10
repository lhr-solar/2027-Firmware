#ifndef PDU_SPI_H
#define PDU_SPI_H

#include "PDU_Pins.h"

// SPI1, shared by all 6 TPS2HCS08 chips. CS is driven by software (PDU_Pins.h).
extern SPI_HandleTypeDef hspi1;

/**
 * Master, 8-bit, 1 MHz (APB2 16 MHz / 16), software NSS.
 * Calls Error_Handler() on failure.
 */
void PDU_SPI_Init(void);

#endif
