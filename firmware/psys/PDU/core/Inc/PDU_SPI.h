#ifndef PDU_SPI_H
#define PDU_SPI_H

#include "PDU_Pins.h"

// SPI1, shared by all 6 TPS2HCS08 chips. CS is driven by software (PDU_Pins.h).
extern SPI_HandleTypeDef hspi1;


void PDU_SPI_Init(void);

// Bounded so a stuck bus returns an error instead of hanging. A 3-byte frame at 1 MHz is ~24 us.
#define PDU_SPI_TIMEOUT_MS 2

// Full-duplex transfer of len bytes on SPI1. Caller drives CS.
HAL_StatusTypeDef PDU_SPI_Xfer(const uint8_t *tx, uint8_t *rx, uint16_t len);

#endif
