#pragma once

#include "tusb.h"

// USB peripheral clock, NVIC and PCD init; call before tusb_init()
void MX_USB_PCD_Init(void);
