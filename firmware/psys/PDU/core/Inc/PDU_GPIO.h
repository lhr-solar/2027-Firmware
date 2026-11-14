#pragma once

#include "pinDefs.h"

/**
 * Configures every plain GPIO on the board (HSS CS/fault, shared HSS
 * control lines, status LEDs). SPI1 pins are muxed in HAL_SPI_MspInit.
 * Chip selects come up deasserted (high).
 */
void PDU_GPIO_Init(void);

