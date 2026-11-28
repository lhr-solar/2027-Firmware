#pragma once

#include "pinDefs.h"
#include "stm32xx_hal.h"

typedef enum {
    CHARGER_OFF,
    CHARGER_ON
} charger_status_t;

void Charger_Init(void);
void charger_write(charger_status_t newState);
void read_status(void);