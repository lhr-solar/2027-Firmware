# STM32G473CET6 (LQFP48) — Alternate functions

Alternate functions for the 38 GPIO pins available on the LQFP48 package. Derived from *STM32G473xB/xC/xE datasheet DS12712 Rev 5*: Table 13 (pp. 71–77) filtered by the LQFP48 column of Table 12 (pp. 56–70).

Rows are in port order; the second column is the physical package pin number. Power, ground and reference pins are not listed (they have no alternate functions).

## AF column legend

| AF | Peripherals |
|---|---|
| AF0 | SYS_AF |
| AF1 | LPTIM1/TIM2/5/15/16/17 |
| AF2 | I2C3/TIM1/2/3/4/5/8/15/20/GPCOMP1 |
| AF3 | QUADSPI/I2C3/4/SAI/USB/TIM8/15/20/GPCOMP3/TSC |
| AF4 | I2C1/2/3/4/TIM1/8/16/17 |
| AF5 | QUADSPI/SPI1/2/3/4/I2S2/3/I2C4/UART4/5/TIM8/Infrared |
| AF6 | QUADSPI/SPI2/3/I2S2/3/TIM1/5/8/20/Infrared |
| AF7 | USART1/2/3/CAN/GPCOMP5/6/7 |
| AF8 | I2C3/4/UART4/5/LPUART1/GPCOMP1/2/3/4/5/6/7 |
| AF9 | CAN/TIM1/8/15/CAN1/2 |
| AF10 | QUADSPI/TIM2/3/4/8/17 |
| AF11 | LPTIM1/TIM1/8/CAN1/3 |
| AF12 | SDIO/FMC/LPUART1/SAI/TIM1 |
| AF13 | SAI/OPAMP2 |
| AF14 | UART4/5/SAI/TIM2/15/UCPD |
| AF15 | EVENT |

## Alternate functions

| Port | Pin | AF0 | AF1 | AF2 | AF3 | AF4 | AF5 | AF6 | AF7 | AF8 | AF9 | AF10 | AF11 | AF12 | AF13 | AF14 | AF15 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| PA0 | 8 | - | TIM2_CH1 | TIM5_CH1 | - | - | - | - | USART2_CTS | COMP1_OUT | TIM8_BKIN | TIM8_ETR | - | - | - | TIM2_ETR | EVENTOUT |
| PA1 | 9 | RTC_REFIN | TIM2_CH2 | TIM5_CH2 | - | - | - | - | USART2_RTS_DE | - | TIM15_CH1N | - | - | - | - | - | EVENTOUT |
| PA2 | 10 | - | TIM2_CH3 | TIM5_CH3 | - | - | - | - | USART2_TX | COMP2_OUT | TIM15_CH1 | QUADSPI_BK1_NCS | - | LPUART1_TX | - | UCPD_FRSTX | EVENTOUT |
| PA3 | 11 | - | TIM2_CH4 | TIM5_CH4 | SAI_CK1 | - | - | - | USART2_RX | - | TIM15_CH2 | QUADSPI_CLK | - | LPUART1_RX | SAI_MCLK_A | - | EVENTOUT |
| PA4 | 12 | - | - | TIM3_CH2 | - | - | SPI1_NSS | SPI3_NSS/I2S3_WS | USART2_CK | - | - | - | - | - | SAI_FS_B | - | EVENTOUT |
| PA5 | 13 | - | TIM2_CH1 | TIM2_ETR | - | - | SPI1_SCK | - | - | - | - | - | - | - | - | UCPD_FRSTX | EVENTOUT |
| PA6 | 14 | - | TIM16_CH1 | TIM3_CH1 | - | TIM8_BKIN | SPI1_MISO | TIM1_BKIN | - | COMP1_OUT | - | QUADSPI_BK1_IO3 | - | LPUART1_CTS | - | - | EVENTOUT |
| PA7 | 15 | - | TIM17_CH1 | TIM3_CH2 | - | TIM8_CH1N | SPI1_MOSI | TIM1_CH1N | - | COMP2_OUT | - | QUADSPI_BK1_IO2 | - | - | - | UCPD_FRSTX | EVENTOUT |
| PA8 | 30 | MCO | - | - | - | I2C2_SDA | I2S2_MCK | TIM1_CH1 | USART1_CK | COMP7_OUT | - | TIM4_ETR | FDCAN3_RX | SAI_CK2 | - | SAI_SCK_A | EVENTOUT |
| PA9 | 31 | - | - | I2C3_SMBA | - | I2C2_SCL | I2S3_MCK | TIM1_CH2 | USART1_TX | COMP5_OUT | TIM15_BKIN | TIM2_CH3 | FDCAN1_RX | - | - | SAI_FS_A | EVENTOUT |
| PA10 | 32 | - | TIM17_BKIN | I2C3_SCL | USB_CRS_SYNC | I2C2_SMBA | SPI2_MISO | TIM1_CH3 | USART1_RX | COMP6_OUT | FDCAN1_TX | TIM2_CH4 | TIM8_BKIN | SAI_D1 | - | SAI_SD_A | EVENTOUT |
| PA11 | 33 | - | - | - | - | - | SPI2_MOSI/I2S2_SD | TIM1_CH1N | USART1_CTS | COMP1_OUT | FDCAN1_RX | TIM4_CH1 | TIM1_CH4 | TIM1_BKIN2 | - | - | EVENTOUT |
| PA12 | 34 | - | TIM16_CH1 | - | - | - | I2SCKIN | TIM1_CH2N | USART1_RTS_DE | COMP2_OUT | FDCAN1_TX | TIM4_CH2 | TIM1_ETR | - | - | - | EVENTOUT |
| PA13 | 37 | SWDIO-JTMS | TIM16_CH1N | - | - | - | IR_OUT | - | USART3_CTS | - | - | TIM4_CH3 | - | - | SAI_SD_B | - | EVENTOUT |
| PA14 | 38 | SWCLK-JTCK | LPTIM1_OUT | - | I2C4_SMBA | I2C1_SDA | TIM8_CH2 | TIM1_BKIN | USART2_TX | - | - | - | FDCAN3_TX | - | SAI_FS_B | - | EVENTOUT |
| PA15 | 39 | JTDI | TIM2_CH1 | TIM8_CH1 | - | I2C1_SCL | SPI1_NSS | SPI3_NSS/I2S3_WS | USART2_RX | UART4_RTS_DE | TIM1_BKIN | - | FDCAN3_TX | - | - | TIM2_ETR | EVENTOUT |
| PB0 | 16 | - | - | TIM3_CH3 | - | TIM8_CH2N | - | TIM1_CH2N | - | - | - | QUADSPI_BK1_IO1 | - | - | - | UCPD_FRSTX | EVENTOUT |
| PB1 | 17 | - | - | TIM3_CH4 | - | TIM8_CH3N | - | TIM1_CH3N | - | COMP4_OUT | - | QUADSPI_BK1_IO0 | - | LPUART1_RTS_DE | - | - | EVENTOUT |
| PB2 | 18 | - | LPTIM1_OUT | TIM5_CH1 | TIM20_CH1 | I2C3_SMBA | - | - | - | - | - | QUADSPI_BK2_IO1 | - | - | - | - | EVENTOUT |
| PB3 | 40 | JTDO-TRACESWO | TIM2_CH2 | TIM4_ETR | USB_CRS_SYNC | TIM8_CH1N | SPI1_SCK | SPI3_SCK/I2S3_CK | USART2_TX | - | - | TIM3_ETR | FDCAN3_RX | - | - | SAI_SCK_B | EVENTOUT |
| PB4 | 41 | JTRST | TIM16_CH1 | TIM3_CH1 | - | TIM8_CH2N | SPI1_MISO | SPI3_MISO | USART2_RX | UART5_RTS_DE | - | TIM17_BKIN | FDCAN3_TX | - | - | SAI_MCLK_B | EVENTOUT |
| PB5 | 42 | - | TIM16_BKIN | TIM3_CH2 | TIM8_CH3N | I2C1_SMBA | SPI1_MOSI | SPI3_MOSI/I2S3_SD | USART2_CK | I2C3_SDA | FDCAN2_RX | TIM17_CH1 | LPTIM1_IN1 | SAI_SD_B | - | UART5_CTS | EVENTOUT |
| PB6 | 43 | - | TIM16_CH1N | TIM4_CH1 | - | - | TIM8_CH1 | TIM8_ETR | USART1_TX | COMP4_OUT | FDCAN2_TX | TIM8_BKIN2 | LPTIM1_ETR | - | - | SAI_FS_B | EVENTOUT |
| PB7 | 44 | - | TIM17_CH1N | TIM4_CH2 | I2C4_SDA | I2C1_SDA | TIM8_BKIN | - | USART1_RX | COMP3_OUT | FDCAN2_TX | TIM3_CH4 | LPTIM1_IN2 | FMC_NL | - | UART4_CTS | EVENTOUT |
| PB8 (BOOT0) | 45 | - | TIM16_CH1 | TIM4_CH3 | SAI_CK1 | I2C1_SCL | - | - | USART3_RX | COMP1_OUT | FDCAN1_RX | TIM8_CH2 | - | TIM1_BKIN | - | SAI_MCLK_A | EVENTOUT |
| PB9 | 46 | - | TIM17_CH1 | TIM4_CH4 | SAI_D2 | I2C1_SDA | - | IR_OUT | USART3_TX | COMP2_OUT | FDCAN1_TX | TIM8_CH3 | - | TIM1_CH3N | - | SAI_FS_A | EVENTOUT |
| PB10 | 22 | - | TIM2_CH3 | - | - | - | - | - | USART3_TX | LPUART1_RX | - | QUADSPI_CLK | FDCAN3_TX | TIM1_BKIN | - | SAI_SCK_A | EVENTOUT |
| PB11 | 25 | - | TIM2_CH4 | - | - | - | - | - | USART3_RX | LPUART1_TX | - | QUADSPI_BK1_NCS | FDCAN3_RX | - | - | - | EVENTOUT |
| PB12 | 26 | - | - | TIM5_ETR | - | I2C2_SMBA | SPI2_NSS/I2S2_WS | TIM1_BKIN | USART3_CK | LPUART1_RTS_DE | FDCAN2_RX | - | - | - | - | - | EVENTOUT |
| PB13 | 27 | - | - | - | - | - | SPI2_SCK/I2S2_CK | TIM1_CH1N | USART3_CTS | LPUART1_CTS | FDCAN2_TX | - | - | - | - | - | EVENTOUT |
| PB14 | 28 | - | TIM15_CH1 | - | - | - | SPI2_MISO | TIM1_CH2N | USART3_RTS_DE | COMP4_OUT | - | - | - | - | - | - | EVENTOUT |
| PB15 | 29 | RTC_REFIN | TIM15_CH2 | TIM15_CH1N | COMP3_OUT | TIM1_CH3N | SPI2_MOSI/I2S2_SD | - | - | - | - | - | - | - | - | - | EVENTOUT |
| PC13 | 2 | - | - | TIM1_BKIN | - | TIM1_CH1N | - | TIM8_CH4N | - | - | - | - | - | - | - | - | EVENTOUT |
| PC14 (OSC32_IN) | 3 | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | EVENTOUT |
| PC15 (OSC32_OUT) | 4 | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | EVENTOUT |
| PF0 (OSC_IN) | 5 | - | - | - | - | I2C2_SDA | SPI2_NSS/I2S2_WS | TIM1_CH3N | - | - | - | - | - | - | - | - | EVENTOUT |
| PF1 (OSC_OUT) | 6 | - | - | - | - | - | SPI2_SCK/I2S2_CK | - | - | - | - | - | - | - | - | - | EVENTOUT |
| PG10 (NRST) | 7 | MCO | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
