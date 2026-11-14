#include "PDU_USB.h"
#include "tusb.h"

// USB IRQs call into FreeRTOS (via tud_int_handler), so their priority must be
// numerically >= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY (same as platform tusb_cdc_test).
#define USB_IRQ_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)

// The platform provides the TinyUSB stack, the USB descriptors, and the
// USB_HP/USB_LP IRQ handlers (platform/common/Src/tusb_port.c) -- do not define
// those handlers here. This file only does the PDU-specific part: clock, IRQs, start.
HAL_StatusTypeDef PDU_USB_Init(void)
{
    RCC_CRSInitTypeDef crs = {0};

    // CRS keeps HSI48 within USB tolerance by syncing to the host's 1 kHz SOF
    __HAL_RCC_CRS_CLK_ENABLE();
    crs.Prescaler             = RCC_CRS_SYNC_DIV1;
    crs.Source                = RCC_CRS_SYNC_SOURCE_USB;
    crs.Polarity              = RCC_CRS_SYNC_POLARITY_RISING;
    crs.ReloadValue           = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
    crs.ErrorLimitValue       = RCC_CRS_ERRORLIMIT_DEFAULT;
    crs.HSI48CalibrationValue = RCC_CRS_HSI48CALIBRATION_DEFAULT;
    HAL_RCCEx_CRSConfig(&crs);

    // PA11 (D-) / PA12 (D+) are fixed-function USB pins: no GPIO setup
    __HAL_RCC_USB_CLK_ENABLE();

    HAL_NVIC_SetPriority(USB_HP_IRQn, USB_IRQ_PRIO, 0);
    HAL_NVIC_EnableIRQ(USB_HP_IRQn);
    HAL_NVIC_SetPriority(USB_LP_IRQn, USB_IRQ_PRIO, 0);
    HAL_NVIC_EnableIRQ(USB_LP_IRQn);

    tusb_rhport_init_t dev_init = {
        .role  = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO,
    };
    if (!tusb_init(BOARD_TUD_RHPORT, &dev_init)) {
        return HAL_ERROR;
    }
    return HAL_OK;
}
