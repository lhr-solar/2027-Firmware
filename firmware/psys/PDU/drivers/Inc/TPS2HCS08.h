#ifndef TPS2HCS08_H
#define TPS2HCS08_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32xx_hal.h"
#include "TPS2HCS08_Regs.h"


#define TPS_FRAME_LEN 3 // 24-bit frame = 3 bytes
#define TPS_FRAME_WRITE_BIT 0x80 // tx first bit: 1 = write, 0 = read
#define TPS_FRAME_ADDR_MASK 0x7F // tx next 7 bits: register address

#define TPS_REG_DEV_ID 0x00
#define TPS_REG_GLOBAL_FAULT 0x04
#define TPS_REG_SW_STATE 0x07


// DEV_ID values (8.6.1)
#define TPS_DEV_ID_A 0xFFF0
#define TPS_DEV_ID_B 0xFFF1

// SW_STATE bits (8.6.7)
#define TPS_SW_CH1_ON (1u << 0)
#define TPS_SW_CH2_ON (1u << 1)

// GLOBAL_FAULT_TYPE bits (8.6.5)
#define TPS_GF_CH2_FLT (1u << 13)
#define TPS_GF_CH1_FLT (1u << 12)
#define TPS_GF_LPM_STATUS (1u << 11)
#define TPS_GF_OCP_I2T_TSD (1u << 10)

// ADC_RESULT_CHx_I bits (8.6.15)
#define TPS_ADC_I_READY (1u << 10)
#define TPS_ADC_I_MASK 0x03FF  // 10-bit result

// One of these per chip. On the PDU you make 6, one per pair.
typedef struct {
    // Function that sends len bytes and receives len bytes on the SPI bus.
    // On the PDU this is PDU_SPI_Xfer. In tests it's a fake.
    HAL_StatusTypeDef (*xfer)(const uint8_t *tx, uint8_t *rx, uint16_t len);

    GPIO_TypeDef *cs_port;      // this chip's chip-select pin
    uint16_t      cs_pin;

    uint8_t       last_fault;   // byte 0 of the last reply = GLOBAL_FAULT_TYPE[15:8]
    uint16_t      sw_state;     // what was written to SW_STATE (avoid overwrites ch1 and ch2)
} TPS2HCS08_t;

typedef enum {
    TPS_OK = 0,
    TPS_ERR_SPI,        // HAL_SPI returned an error
    TPS_ERR_BAD_ID,     // DEV_ID wasn't 0xFFF0 (wrong chip, or SPI wiring/mode wrong)
    TPS_ERR_ARG,        // bad channel number etc.
} TPS_Status_t;

// single message to the chip and get its reply.  (datasheet 8.3.3)
TPS_Status_t TPS_Frame(TPS2HCS08_t *dev, bool write, uint8_t addr,
                       uint16_t data_in, uint16_t *data_out);

// registers 
TPS_Status_t TPS_WriteReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t value);
TPS_Status_t TPS_ReadReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t *value);

// bring-up
TPS_Status_t TPS_Init(TPS2HCS08_t *dev);
TPS_Status_t TPS_Wake(TPS2HCS08_t *dev);           

// PDU funcs
TPS_Status_t TPS_SetChannel(TPS2HCS08_t *dev, uint8_t ch, bool on); // ch: 0 or 1
TPS_Status_t TPS_ReadCurrentRaw(TPS2HCS08_t *dev, uint8_t ch, uint16_t *raw); // 0..1023
TPS_Status_t TPS_ReadFaults(TPS2HCS08_t *dev, uint16_t *global,
                            uint16_t *ch1_flt, uint16_t *ch2_flt);

#endif
