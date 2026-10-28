/**
 * @file BQ79616_regs.h
 * @brief Register addresses, bit fields and datasheet constants for the BQ79600 + BQ79616 driver.
 *
 * @details
 * Kept separate from BQ79616.h so the API header stays short. Included by BQ79616.h, so callers
 * that use bq_read()/bq_write() directly have these too.
 *
 * References are "<datasheet> <section> p<page>":
 *  - BQ79600: BQ79600-Q1 datasheet, SLUSDS1A
 *  - BQ79616: BQ79616 datasheet, SLUSF21A
 */

#pragma once

#include <stdint.h>

// ---- Registers shared by the BQ79600 and BQ79616 ----------------------------------------------

#define BQ_REG_DIR0_ADDR (0x0306)          /* BQ79600 7.5.2 p39, BQ79616 6.5.4.1.4 p121 */
#define BQ_REG_CONTROL1 (0x0309)           /* BQ79600 7.5.4 p39, BQ79616 6.5.4.3.11 p126 */
#define BQ_REG_CONTROL1_SEND_WAKE (1 << 5)
#define BQ_REG_CONTROL1_ADDR_WR (1 << 0)

// ---- BQ79600 bridge only ----------------------------------------------------------------------

#define BQ_REG_DEV_CONF1 (0x2001)          /* BQ79600 7.5.7 p41 */
#define BQ_REG_DEV_CONF1_RESET (0x14)      /* reset value, checked in BQ79600 Table 7-9 step 10 p28 */
#define BQ_REG_BRIDGE_PARTID (0x2120)      /* BQ79600 7.5.22 p47: silicon revision, 0x00 = A0 */
#define BQ_REG_FAULT_RST (0x2030)          /* BQ79600 7.5.14 p44 */
#define BQ_REG_FAULT_RST_RST_SYS (1 << 1)  /* clears FAULT_SYS (7.5.17 p45) and FAULT_SUMMARY[FAULT_SYS] */
#define BQ_REG_FAULT_RST_RST_FCOMM_DET (1 << 5)  /* clears FAULT_COMM1[FCOMM_DET]: a stack device reported a fault */

// ---- BQ79616 monitor only ---------------------------------------------------------------------

#define BQ_REG_ACTIVE_CELL (0x0003)        /* BQ79616 6.5.4.3.2 p123 */
#define BQ_REG_ACTIVE_CELL_NUM(cells) ((uint8_t)((cells) - 6U))    /* NUM_CELL: 0x0 = 6S ... 0xA = 16S */

#define BQ_REG_GPIO_CONF1 (0x000E)         /* BQ79616 6.5.4.9.1-4 p156-157: GPIO_CONF1..4 = 0x000E..0x0011 */
#define BQ_REG_GPIO_CONF4 (0x0011)
#define BQ_GPIO_MODE_ADC_ONLY (0x2)        /* GPIOn[2:0] = 010 */
#define BQ_REG_GPIO_CONF_PAIR(odd, even) ((uint8_t)(((even) << 3) | (odd)))  /* odd GPIO in [2:0], even in [5:3] */

#define BQ_REG_COMM_TIMEOUT_CONF (0x0019)  /* BQ79616 6.5.4.3.6 p124 */
#define BQ_REG_COMM_TIMEOUT_CONF_CTL_ACT_SHUTDOWN (1 << 3)          /* long timeout -> SHUTDOWN, not SLEEP */
#define BQ_REG_COMM_TIMEOUT_CONF_CTL_TIME_MASK (0x7)               /* CTL_TIME[2:0], see bq_comm_timeout_t */

#define BQ_REG_CUST_CRC_HI (0x0036)        /* BQ79616 6.5.4.3.13-14 p126: expected customer CRC, HI then LO (0x0037) */

#define BQ_REG_CUST_MISC1 (0x0021)         /* BQ79616 6.5.4.1.3 p121: CUST_MISC1..8 scratch pad, 0x0021-0x0028 */

#define BQ_REG_COMM_CTRL (0x0308)          /* BQ79616 6.5.4.3.10 p125 */
#define BQ_REG_COMM_CTRL_TOP_STACK (1 << 0)
#define BQ_REG_COMM_CTRL_STACK_DEV (1 << 1)

#define BQ_REG_CONTROL2 (0x030A)           /* BQ79616 6.5.4.3.12 p126 */
#define BQ_REG_CONTROL2_TSREF_EN (1 << 0)

#define BQ_REG_ADC_CTRL1 (0x030D)          /* BQ79616 6.5.4.5.7 p132 */
#define BQ_REG_ADC_CTRL1_MAIN_MODE_SINGLE (0x1)                     /* 8 round robins, then stop */
#define BQ_REG_ADC_CTRL1_MAIN_MODE_CONT (0x2)                       /* round robin until stopped */
#define BQ_REG_ADC_CTRL1_MAIN_GO (1 << 2)

#define BQ_REG_OTP_ECC_DATAIN1 (0x0343)    /* BQ79616 6.5.4.15.5 p184; DATAIN1..8 are the */
#define BQ_REG_OTP_ECC_DATAIN8 (0x034A)    /* auto-addressing DLL sync dummies, BQ79600 Table 7-9 p27 */

#define BQ_REG_PARTID (0x0500)             /* BQ79616 6.5.4.2.1 p122 */
#define BQ_REG_PARTID_BQ79616 (0x21)

#define BQ_REG_FAULT_RST1 (0x0331)         /* BQ79616 6.5.4.12.3 p164 */
#define BQ_REG_FAULT_RST1_RST_SYS (1 << 1) /* clears FAULT_SYS, e.g. DRST after a wake */
#define BQ_REG_FAULT_RST2 (0x0332)         /* BQ79616 6.5.4.12.4 p164 */
#define BQ_REG_FAULT_RST2_RST_OTP_CRC (1 << 6)     /* clears FAULT_OTP[CUST_CRC] and [FACT_CRC] */
#define BQ_REG_FAULT_RST2_RST_COMM3_FCOMM (1 << 4) /* clears FAULT_COMM3[FCOMM_DET] (fault passed down from above) */

#define BQ_REG_CUST_CRC_RSLT_HI (0x050C)   /* BQ79616 6.5.4.3.15-16 p127: device-computed customer CRC, HI then LO */
#define BQ_REG_DEV_STAT (0x052C)           /* BQ79616 6.5.4.4.6 p130 */
#define BQ_REG_DEV_STAT_CUST_CRC_DONE (1 << 5)     /* a customer CRC check has finished; cleared by reading DEV_STAT */

#define BQ_REG_ADC_STAT1 (0x0527)          /* BQ79616 6.5.4.4.2 p128 */
#define BQ_REG_ADC_STAT1_DRDY_MAIN (1 << 0)

/* BQ79616 6.5.4.6.1-16 p134-139: VCELL1_HI = 0x0586 ... VCELL16_HI = 0x0568 */
#define BQ_REG_VCELL_HI(cell) ((uint16_t)(0x0586U - 2U * ((cell) - 1U)))

#define BQ_REG_TSREF_HI (0x058C)           /* BQ79616 6.5.4.6.18 p140 */ 

// ---- BQ79616 datasheet values -----------------------------------------------------------------

#define BQ_VCELL_UV_PER_LSB_X100 19073      /* VLSB_ADC = 190.73 uV/LSB, BQ79616 5.5 p10; x100 to stay integer */
#define BQ_GPIO_UV_PER_LSB_X100  15259      /* VLSB_GPIO = 152.59 uV/LSB, BQ79616 5.5 p10; x100 to stay integer */
#define BQ_TSREF_UV_PER_LSB_X100 16954      /* VLSB_TSREF = 169.54 uV/LSB, BQ79616 5.5 p10; x100 to stay integer */
#define BQ_ADC_RESULT_INVALID   ((int16_t)0x8000)   /* result register reset value: never converted */
#define BQ_AFE_SETTLE_MS        4U          /* tAFE_SETTLE, BQ79616 5.6 p15; needed after a NUM_CELL change (6.3.2.1.3.1 p26) */
#define BQ_ADC_SINGLE_RUN_MS    2U          /* single run = 8 x tMAIN_ADC_CYCLE (192 us, BQ79616 5.6 p15) = 1.536 ms */
#define BQ_CRC_DONE_TIMEOUT_MS  50U         /* bound on waiting for a customer CRC check; the datasheet gives no period */
#define BQ_TSREF_SETTLE_MS      10U         /* tTSREF_ON = 6 ms with CTSREF = 1 uF, BQ79616 5.6 p13; more C needs longer */
