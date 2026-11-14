#pragma once

#include "stm32xx_hal.h"

/**
 * Brings up USB FS and starts the TinyUSB device stack: CRS (HSI48 trimmed to
 * USB SOF), USB clock, USB IRQs, tusb_init.
 * Requires PDU_Clock_Init() first (HSI48 on, USB kernel clock = HSI48).
 * TinyUSB only runs once a task calls tud_task() in a loop (see app.c).
 * Returns HAL_ERROR if tusb_init fails.
 */
HAL_StatusTypeDef PDU_USB_Init(void);
