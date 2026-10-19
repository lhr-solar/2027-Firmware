//read through spi
//digital current
//output voltage
//FET temperature

//function
// light switch for 12v
// measures how much current is flowing through it,
// turns itself off if there’s a short or too much current (like a fuse that resets),
// remembers what went wrong (overcurrent, too hot, low voltage...).


//test: protection through thermal shutdown, overcurrent protection, transient withstand, and
//reverse battery operation

#include "TPS2HCS08.h"

// Harmless register to read when we only want the reply to the previous frame.
// DEV_ID has no clear-on-read bits, so reading it never throws away a fault.
#define TPS_NOP_REG TPS_REG_DEV_ID


// Keep CS high long enough between two frames.
// "SPI transfer inactive time (time between two transfers) during which CS must remain high" (min 500 ns)
static void prvCsGap(void)
{
    uint32_t loops = (SystemCoreClock / 2000000u) + 1u;
    for (volatile uint32_t i = 0; i < loops; i++) {
    }
}

// Send one 3-byte frame and get the 3-byte reply.
// "Please note that the 16-bit wide "Data Out" in the SDO output is always for the
//  previous SPI command frame (Read or Write)."
// 1. CS low, transfer 3 bytes, CS high, wait the CS gap.
// 2. Save the fault byte; return the 16-bit data (answer to the previous frame).
TPS_Status_t TPS_Frame(TPS2HCS08_t *dev, bool write, uint8_t addr, uint16_t data_in, uint16_t *data_out)
{
    if (dev == NULL || dev->xfer == NULL || dev->cs_port == NULL) {
        return TPS_ERR_ARG;
    }

    uint8_t tx[TPS_FRAME_LEN];
    uint8_t rx[TPS_FRAME_LEN] = {0};

    tx[0] = (write ? TPS_FRAME_WRITE_BIT : 0x00) | (addr & TPS_FRAME_ADDR_MASK);
    tx[1] = (uint8_t)(data_in >> 8);
    tx[2] = (uint8_t)(data_in & 0xFF);

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef ok = dev->xfer(tx, rx, TPS_FRAME_LEN);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
    prvCsGap();

    if (ok != HAL_OK) {
        return TPS_ERR_SPI;
    }

    dev->last_fault = rx[0];
    if (data_out != NULL) {
        *data_out = ((uint16_t)rx[1] << 8) | rx[2];
    }
    return TPS_OK;
}

// Write a 16-bit value into register addr.
// "Addressed registers are updated if there is no SPI communication error and if it is an SPI write command."
TPS_Status_t TPS_WriteReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t value)
{
    return TPS_Frame(dev, true, addr, value, NULL);
}

// Read register addr into *value (value may be NULL to just do the read).
// "The microcontroller may issue a READ command to retrieve information stored in the registers."
// 1. Read addr, ignore the reply (it answers the previous frame).
// 2. Read the NOP register to collect addr's value (not addr again, so a clear-on-read fault isn't lost).
TPS_Status_t TPS_ReadReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t *value)
{
    TPS_Status_t status = TPS_Frame(dev, false, addr, 0, NULL);
    if (status != TPS_OK) {
        return status;
    }
    return TPS_Frame(dev, false, TPS_NOP_REG, 0, value);
}

// Wake the chip, check its ID, clear power-on faults, start both outputs OFF.
// "Upon wakeup from SLEEP state, the values in the registers will be set to their reset values"
// "the POR, VDD_UVLO, and VBB_UVLO fault bits will be asserted and will be cleared when the
//  GLOBAL_FAULT_TYPE register is read if none of these faults currently exist when read."
// 1. Wake.
// 2. Check DEV_ID is the A version.
// 3. Read GLOBAL_FAULT_TYPE to clear POR/UVLO flags.
// 4. Turn both outputs OFF and verify.
TPS_Status_t TPS_Init(TPS2HCS08_t *dev)
{
    TPS_Status_t status = TPS_Wake(dev);
    if (status != TPS_OK) {
        return status;
    }

    uint16_t dev_id;
    status = TPS_ReadReg(dev, TPS_REG_DEV_ID, &dev_id);
    if (status != TPS_OK) {
        return status;
    }
    if (dev_id != TPS_DEV_ID_A) {
        return TPS_ERR_BAD_ID;
    }

    status = TPS_ReadReg(dev, TPS_REG_GLOBAL_FAULT, NULL);
    if (status != TPS_OK) {
        return status;
    }

    return TPS_SetChannel(dev, 0, false);
}

// Wake the chip from SLEEP with one short dummy read.
// "A dummy SPI transaction could be used to sastify method #1 if the dummy SPI transaction is
//  completed in t < tREADY." (method #1: "Pulse the CSN pin low for t < tREADY", tREADY = ~65 us)
// Wake resets every register, so sw_state is reset to 0 (both off) to match.
TPS_Status_t TPS_Wake(TPS2HCS08_t *dev)
{
    if (dev == NULL) {
        return TPS_ERR_ARG;
    }
    dev->sw_state = 0;

    TPS_Status_t status = TPS_Frame(dev, false, TPS_NOP_REG, 0, NULL);
    if (status != TPS_OK) {
        return status;
    }
    HAL_Delay(1);
    return TPS_OK;
}

// Turn one output on/off, then read SW_STATE back to confirm.
// "The state of the eFuse outputs for ACTIVE state for TPS2HCS08A-Q1 is controlled by the
//  CHx_ON bits in the SW_STATE register."
// 1. OFF is saved to sw_state before the write (so the other channel can't turn it back on); ON only after verify.
// 2. Write SW_STATE.
// 3. Read it back (a rejected write is otherwise silent).
TPS_Status_t TPS_SetChannel(TPS2HCS08_t *dev, uint8_t ch, bool on)
{
    if (dev == NULL || ch > 1) {
        return TPS_ERR_ARG;
    }

    uint16_t bit = (ch == 0) ? TPS_SW_CH1_ON : TPS_SW_CH2_ON;
    uint16_t want = on ? (dev->sw_state | bit) : (dev->sw_state & ~bit);

    if (!on) {
        dev->sw_state = want;
    }

    TPS_Status_t status = TPS_WriteReg(dev, TPS_REG_SW_STATE, want);
    if (status != TPS_OK) {
        return status;
    }

    uint16_t readback;
    status = TPS_ReadReg(dev, TPS_REG_SW_STATE, &readback);
    if (status != TPS_OK) {
        return status;
    }
    if ((readback & TPS_SW_MASK) != want) {
        return TPS_ERR_VERIFY;
    }

    dev->sw_state = want;
    return TPS_OK;
}

// Read one channel's raw 10-bit load current (0..1023).
// "The voltage created by the ISNS and RSNS is then sampled by the internal 10-bit ADC where the
//  result of the ADC conversion is stored in ADC_RESULT_CHx_I for each channel."
TPS_Status_t TPS_ReadCurrentRaw(TPS2HCS08_t *dev, uint8_t ch, uint16_t *raw)
{
    if (dev == NULL || raw == NULL || ch > 1) {
        return TPS_ERR_ARG;
    }

    uint8_t reg = (ch == 0) ? TPS_REG_ADC_CH1_I : TPS_REG_ADC_CH2_I;
    uint16_t value;
    TPS_Status_t status = TPS_ReadReg(dev, reg, &value);
    if (status != TPS_OK) {
        return status;
    }

    *raw = value & TPS_ADC_I_MASK;
    return TPS_OK;
}

// Read the global and both channel fault registers (clear-on-read).
// "The device also provides fault information through SPI through a global fault register
//  (GLOBAL_FAULT_TYPE) and channel specific fault registers (FLT_STAT_CHx)"
// Each register is read exactly once; each reply belongs to the frame before it.
TPS_Status_t TPS_ReadFaults(TPS2HCS08_t *dev, uint16_t *global,
                            uint16_t *ch1_flt, uint16_t *ch2_flt)
{
    if (dev == NULL || global == NULL || ch1_flt == NULL || ch2_flt == NULL) {
        return TPS_ERR_ARG;
    }

    TPS_Status_t status = TPS_Frame(dev, false, TPS_REG_GLOBAL_FAULT, 0, NULL);
    if (status != TPS_OK) {
        return status;
    }
    status = TPS_Frame(dev, false, TPS_REG_FLT_STAT_CH1, 0, global);
    if (status != TPS_OK) {
        return status;
    }
    status = TPS_Frame(dev, false, TPS_REG_FLT_STAT_CH2, 0, ch1_flt);
    if (status != TPS_OK) {
        return status;
    }
    return TPS_Frame(dev, false, TPS_NOP_REG, 0, ch2_flt);
}
