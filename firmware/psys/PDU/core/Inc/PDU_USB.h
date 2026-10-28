#pragma once

#include "stm32xx_hal.h"

/**
 * Brings up the USB FS peripheral clock and CRS (HSI48 trimmed to USB SOF).
 * Requires PDU_Clock_Init() first (HSI48 on, USB kernel clock = HSI48).
 * Does not start a device stack -- that is TinyUSB's job (see TODO in .c).
 */
void PDU_USB_Init(void);

