#ifndef TPS2HCS08_REGS_H
#define TPS2HCS08_REGS_H


// Register addresses (Table 8-13)
#define TPS_REG_DEV_ID              0x00
#define TPS_REG_CRC_CONFIG          0x01
#define TPS_REG_SLEEP               0x02
#define TPS_REG_LPM                 0x03
#define TPS_REG_GLOBAL_FAULT        0x04  // GLOBAL_FAULT_TYPE
#define TPS_REG_FAULT_MASK          0x05
#define TPS_REG_SW_STATE            0x07
#define TPS_REG_DEV_CONFIG          0x09
#define TPS_REG_ADC_CONFIG          0x0A
#define TPS_REG_ADC_VBB             0x0B  // ADC_RESULT_VBB
#define TPS_REG_FLT_STAT_CH1        0x0D
#define TPS_REG_PWM_CH1             0x0E
#define TPS_REG_ILIM_CONFIG_CH1     0x0F
#define TPS_REG_CH1_CONFIG          0x10
#define TPS_REG_ADC_CH1_I           0x11  // ADC_RESULT_CH1_I
#define TPS_REG_ADC_CH1_T           0x12  // ADC_RESULT_CH1_T
#define TPS_REG_ADC_CH1_V           0x13  // ADC_RESULT_CH1_V
#define TPS_REG_ADC_CH1_VDS         0x14  // ADC_RESULT_CH1_VDS
#define TPS_REG_I2T_CONFIG_CH1      0x15
#define TPS_REG_FLT_STAT_CH2        0x16
#define TPS_REG_PWM_CH2             0x17
#define TPS_REG_ILIM_CONFIG_CH2     0x18
#define TPS_REG_CH2_CONFIG          0x19
#define TPS_REG_ADC_CH2_I           0x1A  // ADC_RESULT_CH2_I
#define TPS_REG_ADC_CH2_T           0x1B  // ADC_RESULT_CH2_T
#define TPS_REG_ADC_CH2_V           0x1C  // ADC_RESULT_CH2_V
#define TPS_REG_ADC_CH2_VDS         0x1D  // ADC_RESULT_CH2_VDS
#define TPS_REG_I2T_CONFIG_CH2      0x1E

// DEV_ID values 
#define TPS_DEV_ID_A                0xFFF0  // SW_STATE controls the outputs
#define TPS_DEV_ID_B                0xFFF1  // DI pins only, SW_STATE ignored

// SW_STATE bits Bits 
// 15:2 are reserved and read back as 1.
#define TPS_SW_CH1_ON               (1u << 0)
#define TPS_SW_CH2_ON               (1u << 1)
#define TPS_SW_MASK                 (TPS_SW_CH1_ON | TPS_SW_CH2_ON)

// GLOBAL_FAULT_TYPE bits
// Bits 15:8 also arrive as the status byte of every reply.
#define TPS_GF_CH2_FLT              (1u << 13)
#define TPS_GF_CH1_FLT              (1u << 12)
#define TPS_GF_LPM_STATUS           (1u << 11)
#define TPS_GF_OCP_I2T_TSD          (1u << 10)
#define TPS_GF_OL_SHRT_VBB_OFF      (1u << 9)
#define TPS_GF_GLOBAL_ERR_WRN       (1u << 8)   // one of bits 7:0 below is set
#define TPS_GF_LIMPHOME_STAT        (1u << 7)
#define TPS_GF_POR                  (1u << 6)
#define TPS_GF_LPM_STATUS_1         (1u << 5)
#define TPS_GF_SPI_ERR              (1u << 4)
#define TPS_GF_WD_ERR               (1u << 3)
#define TPS_GF_VDD_UVLO             (1u << 2)
#define TPS_GF_VBB_UV_WRN           (1u << 1)
#define TPS_GF_VBB_UVLO             (1u << 0)

// FLT_STAT_CHx bits  
// Bits marked RC clear when FLT_STAT_CHx is read and the fault is gone.
#define TPS_FLT_I2T_MOD             (1u << 12)
#define TPS_FLT_LATCH_STAT          (1u << 11)  // latched off after OCP/TSD
#define TPS_FLT_FLT                 (1u << 10)  // in retry wait, latch off, or I2T cooldown
#define TPS_FLT_SW_STATE_STAT       (1u << 9)   // real output state, whatever the mode
#define TPS_FLT_VOUT_ERR            (1u << 8)
#define TPS_FLT_I2T_FLT             (1u << 7)   // RC
#define TPS_FLT_LPM_WAKE            (1u << 6)   // RC
#define TPS_FLT_THERMAL_SD          (1u << 5)   // RC
#define TPS_FLT_ILIMIT              (1u << 4)   // RC, overcurrent trip
#define TPS_FLT_SHRT_VBB            (1u << 3)   // RC
#define TPS_FLT_OL_OFF              (1u << 2)   // RC
#define TPS_FLT_THERMAL_WRN         (1u << 0)   // RC

// ADC_RESULT_CHx_I bits
#define TPS_ADC_I_SCALE_EFF         (1u << 11)  // 1 = 8x ISNS scaling was used
#define TPS_ADC_I_READY             (1u << 10)  // new result since last read
#define TPS_ADC_I_MASK              0x03FF      // 10-bit result

#endif
