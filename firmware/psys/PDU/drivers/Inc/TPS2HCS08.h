// https://www.ti.com/lit/ds/symlink/tps2hcs08-q1.pdf
#pragma once


#include <stdint.h>
#include <stdbool.h>
#include "stm32g4xx_hal_conf.h"
#include "TPS2HCS08_Regs.h"

// SPI frame format (SLVSHR0 §8.3.3, Figure 8-8)
#define TPS_FRAME_LEN       3    
#define TPS_FRAME_WRITE_BIT 0x80  
#define TPS_FRAME_ADDR_MASK 0x7F  

#define TPS_ISNS_ADC_FS_MV  1200u
#define TPS_KSNS            2000u
#define TPS_RSNS_OHM        1000u

// Bounded so a stuck bus returns an error instead of hanging. A 3-byte frame at 1 MHz is ~24 us.
#ifndef TPS_SPI_TIMEOUT_MS
#define TPS_SPI_TIMEOUT_MS  2u
#endif

// Not thread-safe: every TPS_* call for a given SPI bus must come from ONE task.
// A register read is two frames (the reply arrives in the next frame), so another
// task's frame in between would corrupt it. Other tasks request HSS actions from
// that owner task (e.g. through a queue) instead of calling the driver directly.

typedef struct {
    // SPI bus this chip is on, initialized by the board before TPS_Init.
    // Bus must be SPI mode 1 (CPOL 0, CPHA 1), MSB first, 8-bit (SLVSHR0 §8.3.3).
    SPI_HandleTypeDef *hspi;

    GPIO_TypeDef *cs_port;      // this chip's chip-select pin
    uint16_t      cs_pin;

    uint8_t       last_fault;   // byte 0 of the last reply = GLOBAL_FAULT_TYPE[15:8]
    uint16_t      sw_state;     // what was written to SW_STATE (avoid overwrites ch1 and ch2)
} TPS2HCS08_t;

typedef enum {
    TPS_OK = 0,
    TPS_ERR_SPI,        // HAL_SPI returned an error
    TPS_ERR_BAD_ID,     // DEV_ID wasn't 0xFFF0 (wrong chip, or SPI wiring/mode wrong)
    TPS_ERR_ARG,        // bad channel number, NULL pointer etc.
    TPS_ERR_VERIFY,     // register read back different from what was written
} TPS_Status_t;

// single message to the chip and get its reply. 
TPS_Status_t TPS_Frame(TPS2HCS08_t *dev, bool write, uint8_t addr, uint16_t data_in, uint16_t *data_out);

// registers
TPS_Status_t TPS_WriteReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t value);
TPS_Status_t TPS_ReadReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t *value);

// bring-up
TPS_Status_t TPS_Init(TPS2HCS08_t *dev);
TPS_Status_t TPS_Wake(TPS2HCS08_t *dev);

// PDU funcs
TPS_Status_t TPS_SetChannel(TPS2HCS08_t *dev, uint8_t ch, bool on); // ch: 0 or 1
TPS_Status_t TPS_ReadCurrentRaw(TPS2HCS08_t *dev, uint8_t ch, uint16_t *raw); // 0..1023
uint32_t     TPS_CurrentRawToMilliamps(uint16_t raw);
TPS_Status_t TPS_ReadFaults(TPS2HCS08_t *dev, uint16_t *global,
                            uint16_t *ch1_flt, uint16_t *ch2_flt);


