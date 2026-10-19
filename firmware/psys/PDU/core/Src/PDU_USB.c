#include "PDU_USB.h"

/*
 * Why no HAL PCD (MX_USB_PCD_Init from CubeMX):
 * - The repo's USB stack is TinyUSB, which drives the USB peripheral itself
 *   and only needs the clocks + IRQ hookup below.
 * - HAL_PCD_MODULE_ENABLED is off in the shared platform hal_conf.
 * - A PCD with no stack does nothing; enabling its IRQs without a handler
 *   would land in Default_Handler and hang.
 *
 * TODO: once TinyUSB is in the platform build, enable USB_HP_IRQn/USB_LP_IRQn
 * here, route USB_HP_IRQHandler/USB_LP_IRQHandler/USBWakeUp_IRQHandler to
 * tud_int_handler(0), and call tusb_init() (see platform tusb_cdc_test.c).
 */
void PDU_USB_Init(void)
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
}
