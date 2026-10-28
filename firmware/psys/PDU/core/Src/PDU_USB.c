#include "PDU_USB.h"

/*
will change after i pull
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
