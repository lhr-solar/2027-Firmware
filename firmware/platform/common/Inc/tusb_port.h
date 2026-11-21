#pragma once

#include "tusb.h"

#define NUM_CDC_INSTANCES CFG_TUD_CDC

// USB peripheral clock, NVIC and PCD init; call before tusb_init()
void MX_USB_PCD_Init(void);
