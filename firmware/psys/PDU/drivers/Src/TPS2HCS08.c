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

/*
 * Send ONE 3-byte message to the chip and get its 3-byte reply.
 *
 *   tx[0] = [R/W bit 0][7-bit register address]
 *   tx[1] = data high byte   (0 for a read)
 *   tx[2] = data low byte    (0 for a read)
 *
 *   rx[0] = chip's fault summary (GLOBAL_FAULT_TYPE[15:8])
 *   rx[1..2] = answer to the PREVIOUS message (datasheet 8.3.3)
 */
TPS_Status_t TPS_Frame(TPS2HCS08_t *dev, bool write, uint8_t addr, uint16_t data_in, uint16_t *data_out)
{
    uint8_t tx[TPS_FRAME_LEN]; // transmit
    uint8_t rx[TPS_FRAME_LEN] = {0}; // recieve

    tx[0] = (write ? TPS_FRAME_WRITE_BIT : 0x00) | (addr & TPS_FRAME_ADDR_MASK);
    tx[1] = (uint8_t)(data_in >> 8);
    tx[2] = (uint8_t)(data_in & 0xFF);

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET); // select chip, cs low
    HAL_StatusTypeDef ok = dev->xfer(tx, rx, TPS_FRAME_LEN); // send and receive 3 bytes
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET); // deselect, cs high

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
TPS_Status_t TPS_WriteReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t value)
{
    TPS_Status_t status = TPS_Frame(dev, true, addr, value, NULL);
    if (status != TPS_OK) {
        return status;
    }
    return TPS_OK;
}

// Read register addr into *value.
TPS_Status_t TPS_ReadReg(TPS2HCS08_t *dev, uint8_t addr, uint16_t *value)
{
    // TODO: the reply arrives one message late, so:
    // 1. TPS_Frame(read, addr) and ignore the reply (pass NULL). Return early if it fails.
    TPS_Status_t status = TPS_Frame(dev, false, addr, 0, NULL);
    if (status != TPS_OK) {
        return status;
    }
    // 2. TPS_Frame(read, addr) again, this time passing value to catch the answer.
    status = TPS_Frame(dev, false, addr, 0, value);
    if (status != TPS_OK) {
        return status;
    }
    return TPS_OK;
}

// Wake the chip and check it's really there.
TPS_Status_t TPS_Init(TPS2HCS08_t *dev)
{
    // TODO:
    // 1. dev->sw_state = 0;
    // 2. Wake it: one TPS_Frame (any read), then HAL_Delay(1).  (chip needs ~65 us to wake)
    // 3. TPS_ReadReg TPS_REG_DEV_ID. If it's not TPS_DEV_ID_A, return TPS_ERR_BAD_ID.
    // 4. TPS_ReadReg TPS_REG_GLOBAL_FAULT once (this clears the power-on fault flag).
    // 5. TPS_WriteReg TPS_REG_SW_STATE = 0 so both outputs start OFF.
    TPS_Status_t wake = TPS_Wake(dev);
    if (wake != TPS_OK) {
        return wake;
    }
    uint16_t dev_id;
    TPS_Status_t read_id = TPS_ReadReg(dev, TPS_REG_DEV_ID, &dev_id);
    if (read_id != TPS_OK) {
        return read_id;
    }
    if (dev_id != TPS_DEV_ID_A) {
        return TPS_ERR_BAD_ID;
    }
    // Clear the power-on fault flag
    TPS_ReadReg(dev, TPS_REG_GLOBAL_FAULT, NULL);
    // Set both outputs to OFF
    TPS_WriteReg(dev, TPS_REG_SW_STATE, 0);
    return TPS_OK;
}

TPS_Status_t TPS_Wake(TPS2HCS08_t *dev)
{
    dev->sw_state = 0;
    TPS_Status_t status = TPS_Frame(dev, false, TPS_REG_DEV_ID, 0, NULL);
    if (status != TPS_OK) {
        return status;
    }
    HAL_Delay(1);
    return TPS_OK;
}


TPS_Status_t TPS_SetChannel(TPS2HCS08_t *dev, uint8_t ch, bool on)
{
    // TODO:
    // 1. If ch is not 0 or 1, return TPS_ERR_ARG.
    // 2. Pick the bit: ch 0 -> TPS_SW_CH1_ON, ch 1 -> TPS_SW_CH2_ON.
    // 3. on  -> set that bit in dev->sw_state    (dev->sw_state |= bit;)
    //    off -> clear that bit in dev->sw_state  (dev->sw_state &= ~bit;)
    // 4. Write dev->sw_state to TPS_REG_SW_STATE with TPS_WriteReg and return its result.
    (void)dev; (void)ch; (void)on;
    return TPS_ERR_ARG;
}

TPS_Status_t TPS_ReadCurrentRaw(TPS2HCS08_t *dev, uint8_t ch, uint16_t *raw)
{
    // TODO:
    // 1. Pick the register: ch 0 -> TPS_REG_ADC_CH1_I, ch 1 -> TPS_REG_ADC_CH2_I.
    // 2. TPS_ReadReg it into a uint16_t.
    // 3. Keep only the bottom 10 bits: *raw = value & TPS_ADC_I_MASK;
    (void)dev; (void)ch; (void)raw;
    return TPS_ERR_ARG;
}

TPS_Status_t TPS_ReadFaults(TPS2HCS08_t *dev, uint16_t *global,
                            uint16_t *ch1_flt, uint16_t *ch2_flt)
{
    // TODO:
    // Read TPS_REG_GLOBAL_FAULT, TPS_REG_FLT_STAT_CH1 and TPS_REG_FLT_STAT_CH2
    // into the three pointers. Return early if any read fails.
    (void)dev; (void)global; (void)ch1_flt; (void)ch2_flt;
    return TPS_ERR_ARG;
}
