#pragma once

#include "tusb.h"

// USB peripheral clock, NVIC and PCD init; call before tusb_init()
void MX_USB_PCD_Init(void);

// USB string descriptor overrides, call before tusb_init(). NULL keeps the default.
// Strings must stay valid forever (use literals), max 32 chars.
void usb_set_product_name(const char* name);
void usb_set_cdc_name(uint8_t port, const char* name);

// Creates the task that runs tud_task(). Call after tusb_init(), before the scheduler.
// log_init() already calls it.
void usb_poll_task_create(void);
