#include "PDU_SPI.h"

SPI_HandleTypeDef hspi1;

void PDU_SPI_Init(void)
{
    hspi1.Instance               = SPI1;
    hspi1.Init.Mode              = SPI_MODE_MASTER;
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;  // multi-byte frames sent as bytes
    // SPI mode 1: TPS2HCS08 latches SDI on the falling SCLK edge and shifts SDO
    // on the rising edge, SCLK idle low (datasheet SLVSHR0 8.3.3)
    hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase          = SPI_PHASE_2EDGE;
    // Six chips, six GPIO chip selects: the hardware NSS pin (PA4) is HSS6_CS
    hspi1.Init.NSS               = SPI_NSS_SOFT;
    hspi1.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
    // 16 MHz / 16 = 1 MHz for bring-up; raise once the bus is proven
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial     = 7;
    hspi1.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
}

HAL_StatusTypeDef PDU_SPI_Xfer(const uint8_t *tx, uint8_t *rx, uint16_t len)
{
    return HAL_SPI_TransmitReceive(&hspi1, tx, rx, len, PDU_SPI_TIMEOUT_MS);
}

// Called by HAL_SPI_Init
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef init = {0};

    if (hspi->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        // PA5 SCK, PA6 MISO, PA7 MOSI -- all AF5 on these pins
        init.Pin       = SPI1_SCK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN;
        init.Mode      = GPIO_MODE_AF_PP;
        init.Pull      = GPIO_NOPULL;
        init.Speed     = GPIO_SPEED_FREQ_MEDIUM;
        init.Alternate = SPI1_PINS_AF;
        HAL_GPIO_Init(GPIOA, &init);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, SPI1_SCK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN);
    }
}
