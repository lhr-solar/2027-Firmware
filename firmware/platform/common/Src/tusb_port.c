// i think imma delete/move this when we make USB PSP

#include "stm32g4xx.h"
#include "tusb.h"
#include "bsp/board_api.h"

// Forward USB interrupts to TinyUSB
void USB_HP_IRQHandler(void) {
  tud_int_handler(0);
}

void USB_LP_IRQHandler(void) {
  tud_int_handler(0);
}

// 96-bit MCU unique ID, used for the USB serial string
size_t board_get_unique_id(uint8_t id[], size_t max_len) {
  size_t len = (max_len < 12) ? max_len : 12;
  memcpy(id, (const void *) UID_BASE, len);
  return len;
}
