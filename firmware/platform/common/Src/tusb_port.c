#include "stm32xx_hal.h"
#include "bsp/board_api.h"
#include "tusb_port.h"

#define USB_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)

PCD_HandleTypeDef hpcd_USB_FS;

// 96-bit MCU unique ID, used for the USB serial string
size_t board_get_unique_id(uint8_t id[], size_t max_len) {
  size_t len = (max_len < 12) ? max_len : 12;
  memcpy(id, (const void *) UID_BASE, len);
  return len;
}

// Forward USB interrupts to TinyUSB
void USB_HP_IRQHandler(void) {
  tud_int_handler(0);
}

void USB_LP_IRQHandler(void) {
  tud_int_handler(0);
}

void USBWakeUp_IRQHandler(void) {
    tud_int_handler(0);
}

// USB inits should be the same
void MX_USB_PCD_Init(void) {
    hpcd_USB_FS.Instance = USB;
    hpcd_USB_FS.Init.dev_endpoints = 8;
    hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_USB_FS.Init.Sof_enable = DISABLE;
    hpcd_USB_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_FS.Init.lpm_enable = DISABLE;
    hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
    if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK) {
        Error_Handler();
    }
}

void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd) {
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (hpcd->Instance == USB) {
        /** Initializes the peripherals clocks */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_USB_CLK_ENABLE();
        /* USB interrupt Init */
        HAL_NVIC_SetPriority(USB_HP_IRQn, USB_PRIO, 0);
        HAL_NVIC_EnableIRQ(USB_HP_IRQn);
        HAL_NVIC_SetPriority(USB_LP_IRQn, USB_PRIO, 0);
        HAL_NVIC_EnableIRQ(USB_LP_IRQn);
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd) {
    if (hpcd->Instance == USB) {
        /* Peripheral clock disable */
        __HAL_RCC_USB_CLK_DISABLE();
        /* USB interrupt DeInit */
        HAL_NVIC_DisableIRQ(USB_HP_IRQn);
        HAL_NVIC_DisableIRQ(USB_LP_IRQn);
    }
}