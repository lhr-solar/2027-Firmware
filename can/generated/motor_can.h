/** @file motor_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorVelocitySetpoint; unit=rpm; IEEE-754 binary32. Signed velocity setpoint; reverse requires negative rpm. Operational limits are controller settings, not wire limits. */
    float mc_motor_velocity_setpoint;
    /** MC_MotorCurrentSetpoint; unit=ratio; IEEE-754 binary32. Vendor current fraction: 0.0 to 1.0, never 0 to 100. */
    float mc_motor_current_setpoint;
} motor_can_mc_drive_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_drive_command_pack(can_frame_t* dst, const motor_can_mc_drive_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_drive_command_unpack(motor_can_mc_drive_command_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorPowerSetpoint; unit=ratio; IEEE-754 binary32. Vendor current fraction: 0.0 to 1.0, never 0 to 100. */
    float mc_motor_power_setpoint;
} motor_can_mc_power_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_power_command_pack(can_frame_t* dst, const motor_can_mc_power_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_power_command_unpack(motor_can_mc_power_command_t* dst, const can_frame_t* src);

/** Defined WS22 reset trigger: eight unused data bytes, transmitted as zeros. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} motor_can_mc_reset_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_reset_command_pack(can_frame_t* dst, const motor_can_mc_reset_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_reset_command_unpack(motor_can_mc_reset_command_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_TritiumID; unit=none; physical=raw*1+0; raw=[0,4294967295]. Device identifier. 0x00004003 */
    uint32_t mc_tritium_id;
    /** MC_SerialNumber; unit=none; physical=raw*1+0; raw=[0,4294967295]. Device serial number, allocated at manufacture. */
    uint32_t mc_serial_number;
} motor_can_mc_info_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_info_pack(can_frame_t* dst, const motor_can_mc_info_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_info_unpack(motor_can_mc_info_t* dst, const can_frame_t* src);

/** Named raw values for MC_LIMIT_OutputVoltagePWM; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_LIMIT_OUTPUT_VOLTAGE_PWM_OFF = 0,
    MOTOR_CAN_MC_STATUS_MC_LIMIT_OUTPUT_VOLTAGE_PWM_ON = 1,
} motor_can_mc_status_mc_limit_output_voltage_pwm_choice_t;
/** Named raw values for MC_LIMIT_MotorCurrent; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_LIMIT_MOTOR_CURRENT_OFF = 0,
    MOTOR_CAN_MC_STATUS_MC_LIMIT_MOTOR_CURRENT_ON = 1,
} motor_can_mc_status_mc_limit_motor_current_choice_t;
/** Named raw values for MC_LIMIT_Velocity; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_LIMIT_VELOCITY_OFF = 0,
    MOTOR_CAN_MC_STATUS_MC_LIMIT_VELOCITY_ON = 1,
} motor_can_mc_status_mc_limit_velocity_choice_t;
/** Named raw values for MC_LIMIT_BusCurrent; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_LIMIT_BUS_CURRENT_OFF = 0,
    MOTOR_CAN_MC_STATUS_MC_LIMIT_BUS_CURRENT_ON = 1,
} motor_can_mc_status_mc_limit_bus_current_choice_t;
/** Named raw values for MC_LIMIT_BusVoltageUpper; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_UPPER_OFF = 0,
    MOTOR_CAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_UPPER_ON = 1,
} motor_can_mc_status_mc_limit_bus_voltage_upper_choice_t;
/** Named raw values for MC_LIMIT_BusVoltageLower; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_LOWER_OFF = 0,
    MOTOR_CAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_LOWER_ON = 1,
} motor_can_mc_status_mc_limit_bus_voltage_lower_choice_t;
/** Named raw values for MC_LIMIT_MotorTemp; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_LIMIT_MOTOR_TEMP_OFF = 0,
    MOTOR_CAN_MC_STATUS_MC_LIMIT_MOTOR_TEMP_ON = 1,
} motor_can_mc_status_mc_limit_motor_temp_choice_t;
/** Named raw values for MC_FAULT_HardwareOverCurrent; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_HARDWARE_OVER_CURRENT_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_HARDWARE_OVER_CURRENT_FAULT = 1,
} motor_can_mc_status_mc_fault_hardware_over_current_choice_t;
/** Named raw values for MC_FAULT_SoftwareOverCurrent; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_SOFTWARE_OVER_CURRENT_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_SOFTWARE_OVER_CURRENT_FAULT = 1,
} motor_can_mc_status_mc_fault_software_over_current_choice_t;
/** Named raw values for MC_FAULT_DcBusOverVoltage; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_DC_BUS_OVER_VOLTAGE_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_DC_BUS_OVER_VOLTAGE_FAULT = 1,
} motor_can_mc_status_mc_fault_dc_bus_over_voltage_choice_t;
/** Named raw values for MC_FAULT_BadMotorPositionHallSeq; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_BAD_MOTOR_POSITION_HALL_SEQ_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_BAD_MOTOR_POSITION_HALL_SEQ_FAULT = 1,
} motor_can_mc_status_mc_fault_bad_motor_position_hall_seq_choice_t;
/** Named raw values for MC_FAULT_WatchdogCausedLastReset; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_WATCHDOG_CAUSED_LAST_RESET_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_WATCHDOG_CAUSED_LAST_RESET_FAULT = 1,
} motor_can_mc_status_mc_fault_watchdog_caused_last_reset_choice_t;
/** Named raw values for MC_FAULT_ConfigRead; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_CONFIG_READ_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_CONFIG_READ_FAULT = 1,
} motor_can_mc_status_mc_fault_config_read_choice_t;
/** Named raw values for MC_FAULT_15vRailUnderVoltage; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_15V_RAIL_UNDER_VOLTAGE_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_15V_RAIL_UNDER_VOLTAGE_FAULT = 1,
} motor_can_mc_status_mc_fault_15v_rail_under_voltage_choice_t;
/** Named raw values for MC_FAULT_DesaturationFault; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_DESATURATION_FAULT_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_DESATURATION_FAULT_FAULT = 1,
} motor_can_mc_status_mc_fault_desaturation_fault_choice_t;
/** Named raw values for MC_FAULT_MotorOverSpeed; unknown values remain integers. */
typedef enum {
    MOTOR_CAN_MC_STATUS_MC_FAULT_MOTOR_OVER_SPEED_OK = 0,
    MOTOR_CAN_MC_STATUS_MC_FAULT_MOTOR_OVER_SPEED_FAULT = 1,
} motor_can_mc_status_mc_fault_motor_over_speed_choice_t;
/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_LIMIT_OutputVoltagePWM; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_limit_output_voltage_pwm;
    /** MC_LIMIT_MotorCurrent; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_limit_motor_current;
    /** MC_LIMIT_Velocity; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_limit_velocity;
    /** MC_LIMIT_BusCurrent; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_limit_bus_current;
    /** MC_LIMIT_BusVoltageUpper; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_limit_bus_voltage_upper;
    /** MC_LIMIT_BusVoltageLower; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_limit_bus_voltage_lower;
    /** MC_LIMIT_MotorTemp; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_limit_motor_temp;
    /** MC_LIMIT_Reserved; unit=none; physical=raw*1+0; raw=[0,511]. Vendor-reserved bits; no enum semantics assigned. */
    uint16_t mc_limit_reserved;
    /** MC_FAULT_HardwareOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_hardware_over_current;
    /** MC_FAULT_SoftwareOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_software_over_current;
    /** MC_FAULT_DcBusOverVoltage; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_dc_bus_over_voltage;
    /** MC_FAULT_BadMotorPositionHallSeq; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_bad_motor_position_hall_seq;
    /** MC_FAULT_WatchdogCausedLastReset; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_watchdog_caused_last_reset;
    /** MC_FAULT_ConfigRead; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_config_read;
    /** MC_FAULT_15vRailUnderVoltage; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_15v_rail_under_voltage;
    /** MC_FAULT_DesaturationFault; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_desaturation_fault;
    /** MC_FAULT_MotorOverSpeed; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t mc_fault_motor_over_speed;
    /** MC_FAULT_Reserved; unit=none; physical=raw*1+0; raw=[0,127]. Vendor-reserved bits; no enum semantics assigned. */
    uint8_t mc_fault_reserved;
    /** MC_ActiveMotor; unit=none; physical=raw*1+0; raw=[0,65535]. The index of the active motor currently being used. */
    uint16_t mc_active_motor;
    /** MC_TxErrorCount; unit=none; physical=raw*1+0; raw=[0,255]. The DSP CAN transmission error counter (CAN 2.0) */
    uint8_t mc_tx_error_count;
    /** MC_RxErrorCount; unit=none; physical=raw*1+0; raw=[0,255]. The DSP CAN receive error counter (CAN 2.0) */
    uint8_t mc_rx_error_count;
} motor_can_mc_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_status_pack(can_frame_t* dst, const motor_can_mc_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_status_unpack(motor_can_mc_status_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_BusVoltage; unit=V; IEEE-754 binary32. DC bus voltage at the controller. */
    float mc_bus_voltage;
    /** MC_BusCurrent; unit=A; IEEE-754 binary32. Current drawn from the DC bus by the controller. */
    float mc_bus_current;
} motor_can_mc_bus_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_bus_measurement_pack(can_frame_t* dst, const motor_can_mc_bus_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_bus_measurement_unpack(motor_can_mc_bus_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorVelocity; unit=rpm; IEEE-754 binary32. Motor angular frequency in revolutions per minute. */
    float mc_motor_velocity;
    /** MC_VehicleVelocity; unit=m/s; IEEE-754 binary32. Vehicle velocity in metres / second. */
    float mc_vehicle_velocity;
} motor_can_mc_velocity_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_velocity_measurement_pack(can_frame_t* dst, const motor_can_mc_velocity_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_velocity_measurement_unpack(motor_can_mc_velocity_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_PhaseCurrentB; unit=A; IEEE-754 binary32. RMS current in motor Phase B. */
    float mc_phase_current_b;
    /** MC_PhaseCurrentC; unit=A; IEEE-754 binary32. RMS current in motor Phase C. */
    float mc_phase_current_c;
} motor_can_mc_phase_current_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_phase_current_measurement_pack(can_frame_t* dst, const motor_can_mc_phase_current_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_phase_current_measurement_unpack(motor_can_mc_phase_current_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Vq; unit=V; IEEE-754 binary32. Imaginary component of the applied non-rotating voltage vector to the motor. */
    float mc_vq;
    /** MC_Vd; unit=V; IEEE-754 binary32. Real component of the applied non-rotating voltage vector to the motor. */
    float mc_vd;
} motor_can_mc_motor_voltage_vector_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_motor_voltage_vector_measurement_pack(can_frame_t* dst, const motor_can_mc_motor_voltage_vector_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_motor_voltage_vector_measurement_unpack(motor_can_mc_motor_voltage_vector_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Iq; unit=A; IEEE-754 binary32. Imaginary component of the applied non-rotating current vector to the motor. This current produces torque in the motor and should be in phase with the back-EMF of the motor */
    float mc_iq;
    /** MC_Id; unit=A; IEEE-754 binary32. Real component of the applied non-rotating current vector to the motor. This vector represents the field current of the motor. */
    float mc_id;
} motor_can_mc_motor_current_vector_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_motor_current_vector_measurement_pack(can_frame_t* dst, const motor_can_mc_motor_current_vector_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_motor_current_vector_measurement_unpack(motor_can_mc_motor_current_vector_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_BEMFq; unit=V; IEEE-754 binary32. The peak of the phase to neutral motor voltage. */
    float mc_bem_fq;
    /** MC_BEMFd; unit=V; IEEE-754 binary32. By definition this value is always 0V. */
    float mc_bem_fd;
} motor_can_mc_back_emf_measurement_prediction_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_back_emf_measurement_prediction_pack(can_frame_t* dst, const motor_can_mc_back_emf_measurement_prediction_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_back_emf_measurement_prediction_unpack(motor_can_mc_back_emf_measurement_prediction_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Supply15V; unit=V; IEEE-754 binary32. Actual voltage level of the 15V power rail. */
    float mc_supply15_v;
} motor_can_mc_15_v_rail_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_15_v_rail_measurement_pack(can_frame_t* dst, const motor_can_mc_15_v_rail_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_15_v_rail_measurement_unpack(motor_can_mc_15_v_rail_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Supply1V9; unit=V; IEEE-754 binary32. Actual voltage level of the 1.9V DSP power rail. */
    float mc_supply1_v9;
    /** MC_Supply3V3; unit=V; IEEE-754 binary32. Actual voltage level of the 3.3V power rail. */
    float mc_supply3_v3;
} motor_can_mc_3_v3_19_v_rail_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_3_v3_19_v_rail_measurement_pack(can_frame_t* dst, const motor_can_mc_3_v3_19_v_rail_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_3_v3_19_v_rail_measurement_unpack(motor_can_mc_3_v3_19_v_rail_measurement_t* dst, const can_frame_t* src);

/** Vendor-defined reserved frame; fixed eight-byte payload, no signals assigned. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} motor_can_ws22_reserved0_a_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_ws22_reserved0_a_pack(can_frame_t* dst, const motor_can_ws22_reserved0_a_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_ws22_reserved0_a_unpack(motor_can_ws22_reserved0_a_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorTemp; unit=degC; IEEE-754 binary32. Internal temperature of the motor */
    float mc_motor_temp;
    /** MC_HeatsinkTemp; unit=degC; IEEE-754 binary32. Internal temperature of Heat-sink (case). */
    float mc_heatsink_temp;
} motor_can_mc_motor_temp_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_motor_temp_measurement_pack(can_frame_t* dst, const motor_can_mc_motor_temp_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_motor_temp_measurement_unpack(motor_can_mc_motor_temp_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_DspBoardTemp; unit=degC; IEEE-754 binary32. Temperature of the DSP board. */
    float mc_dsp_board_temp;
} motor_can_mc_dsp_board_temp_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_dsp_board_temp_measurement_pack(can_frame_t* dst, const motor_can_mc_dsp_board_temp_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_dsp_board_temp_measurement_unpack(motor_can_mc_dsp_board_temp_measurement_t* dst, const can_frame_t* src);

/** Vendor-defined reserved frame; fixed eight-byte payload, no signals assigned. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} motor_can_ws22_reserved0_d_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_ws22_reserved0_d_pack(can_frame_t* dst, const motor_can_ws22_reserved0_d_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_ws22_reserved0_d_unpack(motor_can_ws22_reserved0_d_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_TripOdometer; unit=m; IEEE-754 binary32. Distance the vehicle has travelled since reset. */
    float mc_trip_odometer;
    /** MC_DCBusAh; unit=Ah; IEEE-754 binary32. Charge flow into the controller DC bus from the time of reset. */
    float mc_dc_bus_ah;
} motor_can_mc_odometer_bus_ah_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_odometer_bus_ah_measurement_pack(can_frame_t* dst, const motor_can_mc_odometer_bus_ah_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_odometer_bus_ah_measurement_unpack(motor_can_mc_odometer_bus_ah_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_SlipSpeed; unit=Hz; IEEE-754 binary32. Slip speed when driving an induction motor. */
    float mc_slip_speed;
} motor_can_mc_slip_speed_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t motor_can_mc_slip_speed_measurement_pack(can_frame_t* dst, const motor_can_mc_slip_speed_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t motor_can_mc_slip_speed_measurement_unpack(motor_can_mc_slip_speed_measurement_t* dst, const can_frame_t* src);

typedef struct {
    motor_can_mc_drive_command_t mc_drive_command;
    motor_can_mc_power_command_t mc_power_command;
    motor_can_mc_reset_command_t mc_reset_command;
} motor_can_vcu_t;

typedef struct {
    motor_can_mc_info_t mc_info;
    motor_can_mc_status_t mc_status;
    motor_can_mc_bus_measurement_t mc_bus_measurement;
    motor_can_mc_velocity_measurement_t mc_velocity_measurement;
    motor_can_mc_phase_current_measurement_t mc_phase_current_measurement;
    motor_can_mc_motor_voltage_vector_measurement_t mc_motor_voltage_vector_measurement;
    motor_can_mc_motor_current_vector_measurement_t mc_motor_current_vector_measurement;
    motor_can_mc_back_emf_measurement_prediction_t mc_back_emf_measurement_prediction;
    motor_can_mc_15_v_rail_measurement_t mc_15_v_rail_measurement;
    motor_can_mc_3_v3_19_v_rail_measurement_t mc_3_v3_19_v_rail_measurement;
    motor_can_ws22_reserved0_a_t ws22_reserved0_a;
    motor_can_mc_motor_temp_measurement_t mc_motor_temp_measurement;
    motor_can_mc_dsp_board_temp_measurement_t mc_dsp_board_temp_measurement;
    motor_can_ws22_reserved0_d_t ws22_reserved0_d;
    motor_can_mc_odometer_bus_ah_measurement_t mc_odometer_bus_ah_measurement;
    motor_can_mc_slip_speed_measurement_t mc_slip_speed_measurement;
} motor_can_ws22_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t mc_drive_command;
        can_message_info_t mc_power_command;
        can_message_info_t mc_reset_command;
    } vcu;
    struct {
        can_message_info_t mc_info;
        can_message_info_t mc_status;
        can_message_info_t mc_bus_measurement;
        can_message_info_t mc_velocity_measurement;
        can_message_info_t mc_phase_current_measurement;
        can_message_info_t mc_motor_voltage_vector_measurement;
        can_message_info_t mc_motor_current_vector_measurement;
        can_message_info_t mc_back_emf_measurement_prediction;
        can_message_info_t mc_15_v_rail_measurement;
        can_message_info_t mc_3_v3_19_v_rail_measurement;
        can_message_info_t ws22_reserved0_a;
        can_message_info_t mc_motor_temp_measurement;
        can_message_info_t mc_dsp_board_temp_measurement;
        can_message_info_t ws22_reserved0_d;
        can_message_info_t mc_odometer_bus_ah_measurement;
        can_message_info_t mc_slip_speed_measurement;
    } ws22;
} motor_can_metadata_t;
extern const motor_can_metadata_t motor_can;
