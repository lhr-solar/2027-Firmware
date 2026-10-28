/** @file car_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_COMMAND_MODE_NORMAL = 0,
    CAR_CAN_BPS_COMMAND_MODE_ADVANCED = 1,
} car_can_bps_command_mode_choice_t;
/** Named raw values for MPPTChargeScheme; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_COMMAND_MPPT_CHARGE_SCHEME_NORMAL = 0,
    CAR_CAN_BPS_COMMAND_MPPT_CHARGE_SCHEME_ADVANCED = 1,
} car_can_bps_command_mppt_charge_scheme_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,1]. Mode */
    uint8_t mode;
    /** MPPTChargeScheme; unit=none; physical=raw*1+0; raw=[0,1]. MPPTChargeScheme */
    uint8_t mppt_charge_scheme;
    /** VoltageOverrideMask; unit=none; physical=raw*1+0; raw=[0,4294967295]. Bit 0 selects tap 1; bit 31 selects tap 32. Set bit requests the corresponding override. */
    uint32_t voltage_override_mask;
    /** TemperatureOverrideMask; unit=none; physical=raw*1+0; raw=[0,4294967295]. Bit 0 selects temperature 1; bit 31 selects temperature 32. Set bit requests the corresponding override. */
    uint32_t temperature_override_mask;
} car_can_bps_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_command_pack(can_frame_t* dst, const car_can_bps_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_command_unpack(car_can_bps_command_t* dst, const can_frame_t* src);

/** Named raw values for Enable; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_COMMAND_ENABLE_DISABLED = 0,
    CAR_CAN_PDU_COMMAND_ENABLE_ENABLED = 1,
} car_can_pdu_command_enable_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** ChannelIndex; unit=none; physical=raw*1+0; raw=[0,11]. Zero-based output index 0..11 selects Channel1..Channel12. Chip n controls Channel(2n-1) and Channel(2n). Indices 12..15 are invalid. */
    uint8_t channel_index;
    /** Enable; unit=none; physical=raw*1+0; raw=[0,1]. Enable */
    uint8_t enable;
} car_can_pdu_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pdu_command_pack(can_frame_t* dst, const car_can_pdu_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pdu_command_unpack(car_can_pdu_command_t* dst, const can_frame_t* src);

/** Named raw values for CommandSource; unknown values remain integers. */
typedef enum {
    CAR_CAN_MOTOR_CONTROLLER_COMMAND_COMMAND_SOURCE_VCU = 0,
    CAR_CAN_MOTOR_CONTROLLER_COMMAND_COMMAND_SOURCE_EXTERNAL = 1,
} car_can_motor_controller_command_command_source_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** CommandSource; unit=none; physical=raw*1+0; raw=[0,1]. CommandSource */
    uint8_t command_source;
} car_can_motor_controller_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_motor_controller_command_pack(can_frame_t* dst, const car_can_motor_controller_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_motor_controller_command_unpack(car_can_motor_controller_command_t* dst, const can_frame_t* src);

/** Defined empty trigger: request a sweep from all MPPTs through MPPTInterface. Zero-byte CAN-FD payload. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} car_can_sweep_mpp_ts_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_sweep_mpp_ts_pack(can_frame_t* dst, const car_can_sweep_mpp_ts_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_sweep_mpp_ts_unpack(car_can_sweep_mpp_ts_t* dst, const can_frame_t* src);

/** Named raw values for Status; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_STATUS_OK = 0,
    CAR_CAN_BPS_STATUS_STATUS_FAULT = 1,
    CAR_CAN_BPS_STATUS_STATUS_WAITING = 2,
} car_can_bps_status_status_choice_t;
/** Named raw values for FaultSummary; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_SUMMARY_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_SUMMARY_SETPOINT_FAULT = 1,
    CAR_CAN_BPS_STATUS_FAULT_SUMMARY_EMERGENCY_STOP = 2,
    CAR_CAN_BPS_STATUS_FAULT_SUMMARY_CONTACTOR_RELAY_FAULT = 3,
    CAR_CAN_BPS_STATUS_FAULT_SUMMARY_PRECHARGE_FAULT = 4,
    CAR_CAN_BPS_STATUS_FAULT_SUMMARY_SMB_FAULT = 5,
} car_can_bps_status_fault_summary_choice_t;
/** Named raw values for FaultOverTemperature; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_OVER_TEMPERATURE_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_OVER_TEMPERATURE_FAULT = 1,
} car_can_bps_status_fault_over_temperature_choice_t;
/** Named raw values for FaultOverVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_OVER_VOLTAGE_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_OVER_VOLTAGE_FAULT = 1,
} car_can_bps_status_fault_over_voltage_choice_t;
/** Named raw values for FaultUnderVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_UNDER_VOLTAGE_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_UNDER_VOLTAGE_FAULT = 1,
} car_can_bps_status_fault_under_voltage_choice_t;
/** Named raw values for FaultChargeOverCurrent; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_CHARGE_OVER_CURRENT_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_CHARGE_OVER_CURRENT_FAULT = 1,
} car_can_bps_status_fault_charge_over_current_choice_t;
/** Named raw values for FaultDischargeOverCurrent; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_DISCHARGE_OVER_CURRENT_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_DISCHARGE_OVER_CURRENT_FAULT = 1,
} car_can_bps_status_fault_discharge_over_current_choice_t;
/** Named raw values for FaultHVPositiveContactorMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_HV_POSITIVE_CONTACTOR_MISMATCH_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_HV_POSITIVE_CONTACTOR_MISMATCH_FAULT = 1,
} car_can_bps_status_fault_hv_positive_contactor_mismatch_choice_t;
/** Named raw values for FaultMotorContactorMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_CONTACTOR_MISMATCH_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_CONTACTOR_MISMATCH_FAULT = 1,
} car_can_bps_status_fault_motor_contactor_mismatch_choice_t;
/** Named raw values for FaultHVNegativeContactorMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_HV_NEGATIVE_CONTACTOR_MISMATCH_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_HV_NEGATIVE_CONTACTOR_MISMATCH_FAULT = 1,
} car_can_bps_status_fault_hv_negative_contactor_mismatch_choice_t;
/** Named raw values for FaultMainPrechargeRelayMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_RELAY_MISMATCH_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_RELAY_MISMATCH_FAULT = 1,
} car_can_bps_status_fault_main_precharge_relay_mismatch_choice_t;
/** Named raw values for FaultMotorPrechargeRelayMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_RELAY_MISMATCH_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_RELAY_MISMATCH_FAULT = 1,
} car_can_bps_status_fault_motor_precharge_relay_mismatch_choice_t;
/** Named raw values for FaultDriverEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_DRIVER_ESTOP_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_DRIVER_ESTOP_FAULT = 1,
} car_can_bps_status_fault_driver_estop_choice_t;
/** Named raw values for FaultExternalEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_EXTERNAL_ESTOP_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_EXTERNAL_ESTOP_FAULT = 1,
} car_can_bps_status_fault_external_estop_choice_t;
/** Named raw values for FaultBatteryEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_BATTERY_ESTOP_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_BATTERY_ESTOP_FAULT = 1,
} car_can_bps_status_fault_battery_estop_choice_t;
/** Named raw values for FaultChargerEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_CHARGER_ESTOP_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_CHARGER_ESTOP_FAULT = 1,
} car_can_bps_status_fault_charger_estop_choice_t;
/** Named raw values for FaultMainPrechargeTimeout; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_TIMEOUT_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_TIMEOUT_FAULT = 1,
} car_can_bps_status_fault_main_precharge_timeout_choice_t;
/** Named raw values for FaultMotorPrechargeTimeout; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_TIMEOUT_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_TIMEOUT_FAULT = 1,
} car_can_bps_status_fault_motor_precharge_timeout_choice_t;
/** Named raw values for FaultMainVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MAIN_VOLTAGE_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MAIN_VOLTAGE_FAULT = 1,
} car_can_bps_status_fault_main_voltage_choice_t;
/** Named raw values for FaultMotorVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_VOLTAGE_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_VOLTAGE_FAULT = 1,
} car_can_bps_status_fault_motor_voltage_choice_t;
/** Named raw values for FaultMainPrechargeVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_VOLTAGE_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_VOLTAGE_FAULT = 1,
} car_can_bps_status_fault_main_precharge_voltage_choice_t;
/** Named raw values for FaultMotorPrechargeVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_VOLTAGE_OK = 0,
    CAR_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_VOLTAGE_FAULT = 1,
} car_can_bps_status_fault_motor_precharge_voltage_choice_t;
/** Named raw values for WarningSMBCommunication; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_SMB_COMMUNICATION_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_SMB_COMMUNICATION_WARNING = 1,
} car_can_bps_status_warning_smb_communication_choice_t;
/** Named raw values for WarningUnderVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_UNDER_VOLTAGE_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_UNDER_VOLTAGE_WARNING = 1,
} car_can_bps_status_warning_under_voltage_choice_t;
/** Named raw values for WarningOverVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_OVER_VOLTAGE_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_OVER_VOLTAGE_WARNING = 1,
} car_can_bps_status_warning_over_voltage_choice_t;
/** Named raw values for WarningChargeOverTemperature; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_CHARGE_OVER_TEMPERATURE_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_CHARGE_OVER_TEMPERATURE_WARNING = 1,
} car_can_bps_status_warning_charge_over_temperature_choice_t;
/** Named raw values for WarningDischargeOverTemperature; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_DISCHARGE_OVER_TEMPERATURE_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_DISCHARGE_OVER_TEMPERATURE_WARNING = 1,
} car_can_bps_status_warning_discharge_over_temperature_choice_t;
/** Named raw values for WarningBatteryFan1; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_BATTERY_FAN1_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_BATTERY_FAN1_WARNING = 1,
} car_can_bps_status_warning_battery_fan1_choice_t;
/** Named raw values for WarningBatteryFan2; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_BATTERY_FAN2_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_BATTERY_FAN2_WARNING = 1,
} car_can_bps_status_warning_battery_fan2_choice_t;
/** Named raw values for WarningBPSFan; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_WARNING_BPS_FAN_NO_WARNING = 0,
    CAR_CAN_BPS_STATUS_WARNING_BPS_FAN_WARNING = 1,
} car_can_bps_status_warning_bps_fan_choice_t;
/** Named raw values for DriveOK; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_DRIVE_OK_NOT_ALLOWED = 0,
    CAR_CAN_BPS_STATUS_DRIVE_OK_ALLOWED = 1,
} car_can_bps_status_drive_ok_choice_t;
/** Named raw values for ChargeOK; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_CHARGE_OK_NOT_ALLOWED = 0,
    CAR_CAN_BPS_STATUS_CHARGE_OK_ALLOWED = 1,
} car_can_bps_status_charge_ok_choice_t;
/** Named raw values for MPPTSetMode; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_MPPT_SET_MODE_DISABLED = 0,
    CAR_CAN_BPS_STATUS_MPPT_SET_MODE_PERTURB_AND_OBSERVE = 1,
    CAR_CAN_BPS_STATUS_MPPT_SET_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} car_can_bps_status_mppt_set_mode_choice_t;
/** Named raw values for ElconChargeOK; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_ELCON_CHARGE_OK_NOT_ALLOWED = 0,
    CAR_CAN_BPS_STATUS_ELCON_CHARGE_OK_ALLOWED = 1,
} car_can_bps_status_elcon_charge_ok_choice_t;
/** Named raw values for RegenChargeOK; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_REGEN_CHARGE_OK_NOT_ALLOWED = 0,
    CAR_CAN_BPS_STATUS_REGEN_CHARGE_OK_ALLOWED = 1,
} car_can_bps_status_regen_charge_ok_choice_t;
/** Named raw values for HVPositiveContactor; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_HV_POSITIVE_CONTACTOR_OFF = 0,
    CAR_CAN_BPS_STATUS_HV_POSITIVE_CONTACTOR_ON = 1,
} car_can_bps_status_hv_positive_contactor_choice_t;
/** Named raw values for MotorContactor; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_MOTOR_CONTACTOR_OFF = 0,
    CAR_CAN_BPS_STATUS_MOTOR_CONTACTOR_ON = 1,
} car_can_bps_status_motor_contactor_choice_t;
/** Named raw values for HVNegativeContactor; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_HV_NEGATIVE_CONTACTOR_OFF = 0,
    CAR_CAN_BPS_STATUS_HV_NEGATIVE_CONTACTOR_ON = 1,
} car_can_bps_status_hv_negative_contactor_choice_t;
/** Named raw values for MainPrechargeRelay; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_MAIN_PRECHARGE_RELAY_OFF = 0,
    CAR_CAN_BPS_STATUS_MAIN_PRECHARGE_RELAY_ON = 1,
} car_can_bps_status_main_precharge_relay_choice_t;
/** Named raw values for MotorPrechargeRelay; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_STATUS_MOTOR_PRECHARGE_RELAY_OFF = 0,
    CAR_CAN_BPS_STATUS_MOTOR_PRECHARGE_RELAY_ON = 1,
} car_can_bps_status_motor_precharge_relay_choice_t;
/** BPS faults, warnings, state, contactors, control permissions, MPPT limits, and fan tachometers. Pack measurements are in BPSPackData (0x070). CAN-FD; no transmit period assigned yet. */
typedef struct {
    /** Status; unit=none; physical=raw*1+0; raw=[0,3]. Status */
    uint8_t status;
    /** FaultSummary; unit=none; physical=raw*1+0; raw=[0,7]. FaultSummary */
    uint8_t fault_summary;
    /** FaultOverTemperature; unit=none; physical=raw*1+0; raw=[0,1]. FaultOverTemperature */
    uint8_t fault_over_temperature;
    /** FaultOverVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultOverVoltage */
    uint8_t fault_over_voltage;
    /** FaultUnderVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultUnderVoltage */
    uint8_t fault_under_voltage;
    /** FaultChargeOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. FaultChargeOverCurrent */
    uint8_t fault_charge_over_current;
    /** FaultDischargeOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. FaultDischargeOverCurrent */
    uint8_t fault_discharge_over_current;
    /** FaultHVPositiveContactorMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultHVPositiveContactorMismatch */
    uint8_t fault_hv_positive_contactor_mismatch;
    /** FaultMotorContactorMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorContactorMismatch */
    uint8_t fault_motor_contactor_mismatch;
    /** FaultHVNegativeContactorMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultHVNegativeContactorMismatch */
    uint8_t fault_hv_negative_contactor_mismatch;
    /** FaultMainPrechargeRelayMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainPrechargeRelayMismatch */
    uint8_t fault_main_precharge_relay_mismatch;
    /** FaultMotorPrechargeRelayMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorPrechargeRelayMismatch */
    uint8_t fault_motor_precharge_relay_mismatch;
    /** FaultDriverEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultDriverEstop */
    uint8_t fault_driver_estop;
    /** FaultExternalEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultExternalEstop */
    uint8_t fault_external_estop;
    /** FaultBatteryEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultBatteryEstop */
    uint8_t fault_battery_estop;
    /** FaultChargerEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultChargerEstop */
    uint8_t fault_charger_estop;
    /** FaultMainPrechargeTimeout; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainPrechargeTimeout */
    uint8_t fault_main_precharge_timeout;
    /** FaultMotorPrechargeTimeout; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorPrechargeTimeout */
    uint8_t fault_motor_precharge_timeout;
    /** FaultMainVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainVoltage */
    uint8_t fault_main_voltage;
    /** FaultMotorVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorVoltage */
    uint8_t fault_motor_voltage;
    /** FaultMainPrechargeVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainPrechargeVoltage */
    uint8_t fault_main_precharge_voltage;
    /** FaultMotorPrechargeVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorPrechargeVoltage */
    uint8_t fault_motor_precharge_voltage;
    /** WarningSMBCommunication; unit=none; physical=raw*1+0; raw=[0,1]. WarningSMBCommunication */
    uint8_t warning_smb_communication;
    /** WarningUnderVoltage; unit=none; physical=raw*1+0; raw=[0,1]. WarningUnderVoltage */
    uint8_t warning_under_voltage;
    /** WarningOverVoltage; unit=none; physical=raw*1+0; raw=[0,1]. WarningOverVoltage */
    uint8_t warning_over_voltage;
    /** WarningChargeOverTemperature; unit=none; physical=raw*1+0; raw=[0,1]. WarningChargeOverTemperature */
    uint8_t warning_charge_over_temperature;
    /** WarningDischargeOverTemperature; unit=none; physical=raw*1+0; raw=[0,1]. WarningDischargeOverTemperature */
    uint8_t warning_discharge_over_temperature;
    /** WarningBatteryFan1; unit=none; physical=raw*1+0; raw=[0,1]. WarningBatteryFan1 */
    uint8_t warning_battery_fan1;
    /** WarningBatteryFan2; unit=none; physical=raw*1+0; raw=[0,1]. WarningBatteryFan2 */
    uint8_t warning_battery_fan2;
    /** WarningBPSFan; unit=none; physical=raw*1+0; raw=[0,1]. WarningBPSFan */
    uint8_t warning_bps_fan;
    /** DriveOK; unit=none; physical=raw*1+0; raw=[0,1]. DriveOK */
    uint8_t drive_ok;
    /** ChargeOK; unit=none; physical=raw*1+0; raw=[0,1]. ChargeOK */
    uint8_t charge_ok;
    /** MPPTSetMode; unit=none; physical=raw*1+0; raw=[0,3]. MPPTSetMode */
    uint8_t mppt_set_mode;
    /** ElconChargeOK; unit=none; physical=raw*1+0; raw=[0,1]. ElconChargeOK */
    uint8_t elcon_charge_ok;
    /** RegenChargeOK; unit=none; physical=raw*1+0; raw=[0,1]. RegenChargeOK */
    uint8_t regen_charge_ok;
    /** HVPositiveContactor; unit=none; physical=raw*1+0; raw=[0,1]. HVPositiveContactor */
    uint8_t hv_positive_contactor;
    /** MotorContactor; unit=none; physical=raw*1+0; raw=[0,1]. MotorContactor */
    uint8_t motor_contactor;
    /** HVNegativeContactor; unit=none; physical=raw*1+0; raw=[0,1]. HVNegativeContactor */
    uint8_t hv_negative_contactor;
    /** MainPrechargeRelay; unit=none; physical=raw*1+0; raw=[0,1]. MainPrechargeRelay */
    uint8_t main_precharge_relay;
    /** MotorPrechargeRelay; unit=none; physical=raw*1+0; raw=[0,1]. MotorPrechargeRelay */
    uint8_t motor_precharge_relay;
    /** MPPTVoltageLimit; unit=V; physical=raw*0.05+0; raw=[0,3500]. MPPTVoltageLimit */
    double mppt_voltage_limit;
    /** MPPTCurrentLimit; unit=A; physical=raw*0.01+0; raw=[0,1638]. MPPTCurrentLimit */
    double mppt_current_limit;
    /** BatteryFanTach1; unit=rpm; physical=raw*5+0; raw=[0,4095]. BatteryFan1Tach */
    uint16_t battery_fan_tach1;
    /** BatteryFanTach2; unit=rpm; physical=raw*5+0; raw=[0,4095]. BatteryFan2Tach */
    uint16_t battery_fan_tach2;
    /** BPSFanTach; unit=rpm; physical=raw*5+0; raw=[0,4095]. BPSFanTach */
    uint16_t bps_fan_tach;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_bps_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_status_pack(can_frame_t* dst, const car_can_bps_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_status_unpack(car_can_bps_status_t* dst, const can_frame_t* src);

/** Named raw values for Status; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_STATUS_OK = 0,
    CAR_CAN_PEDALS_STATUS_STATUS_FAULT = 1,
} car_can_pedals_status_status_choice_t;
/** Named raw values for FaultNoAccelerator; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_FAULT_NO_ACCELERATOR_OK = 0,
    CAR_CAN_PEDALS_STATUS_FAULT_NO_ACCELERATOR_FAULT = 1,
} car_can_pedals_status_fault_no_accelerator_choice_t;
/** Named raw values for WarningMainAcceleratorDisconnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_WARNING_MAIN_ACCELERATOR_DISCONNECTED_NO_WARNING = 0,
    CAR_CAN_PEDALS_STATUS_WARNING_MAIN_ACCELERATOR_DISCONNECTED_WARNING = 1,
} car_can_pedals_status_warning_main_accelerator_disconnected_choice_t;
/** Named raw values for WarningRedundantAcceleratorDisconnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_WARNING_REDUNDANT_ACCELERATOR_DISCONNECTED_NO_WARNING = 0,
    CAR_CAN_PEDALS_STATUS_WARNING_REDUNDANT_ACCELERATOR_DISCONNECTED_WARNING = 1,
} car_can_pedals_status_warning_redundant_accelerator_disconnected_choice_t;
/** Named raw values for WarningPressureSwitchMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_WARNING_PRESSURE_SWITCH_MISMATCH_NO_WARNING = 0,
    CAR_CAN_PEDALS_STATUS_WARNING_PRESSURE_SWITCH_MISMATCH_WARNING = 1,
} car_can_pedals_status_warning_pressure_switch_mismatch_choice_t;
/** Named raw values for BrakeActive; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_BRAKE_ACTIVE_OFF = 0,
    CAR_CAN_PEDALS_STATUS_BRAKE_ACTIVE_ON = 1,
} car_can_pedals_status_brake_active_choice_t;
/** Named raw values for FrontPressureSwitch; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_FRONT_PRESSURE_SWITCH_OFF = 0,
    CAR_CAN_PEDALS_STATUS_FRONT_PRESSURE_SWITCH_ON = 1,
} car_can_pedals_status_front_pressure_switch_choice_t;
/** Named raw values for HandbrakePressureSwitch; unknown values remain integers. */
typedef enum {
    CAR_CAN_PEDALS_STATUS_HANDBRAKE_PRESSURE_SWITCH_OFF = 0,
    CAR_CAN_PEDALS_STATUS_HANDBRAKE_PRESSURE_SWITCH_ON = 1,
} car_can_pedals_status_handbrake_pressure_switch_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** Status; unit=none; physical=raw*1+0; raw=[0,1]. Status */
    uint8_t status;
    /** FaultNoAccelerator; unit=none; physical=raw*1+0; raw=[0,1]. FaultNoAccelerator */
    uint8_t fault_no_accelerator;
    /** WarningMainAcceleratorDisconnected; unit=none; physical=raw*1+0; raw=[0,1]. WarningMainAcceleratorDisconnected */
    uint8_t warning_main_accelerator_disconnected;
    /** WarningRedundantAcceleratorDisconnected; unit=none; physical=raw*1+0; raw=[0,1]. WarningRedundantAcceleratorDisconnected */
    uint8_t warning_redundant_accelerator_disconnected;
    /** WarningPressureSwitchMismatch; unit=none; physical=raw*1+0; raw=[0,1]. WarningPressureSwitchMismatch */
    uint8_t warning_pressure_switch_mismatch;
    /** BrakeActive; unit=none; physical=raw*1+0; raw=[0,1]. BrakeActive */
    uint8_t brake_active;
    /** FrontPressureSwitch; unit=none; physical=raw*1+0; raw=[0,1]. FrontPressureSwitch */
    uint8_t front_pressure_switch;
    /** HandbrakePressureSwitch; unit=none; physical=raw*1+0; raw=[0,1]. HandbrakePressureSwitch */
    uint8_t handbrake_pressure_switch;
    /** AcceleratorMainPosition; unit=%; physical=raw*0.39215686274509803+0; raw=[0,255]. AcceleratorMainPosition */
    double accelerator_main_position;
    /** AcceleratorRedundantPosition; unit=%; physical=raw*0.39215686274509803+0; raw=[0,255]. AcceleratorRedundantPosition */
    double accelerator_redundant_position;
    /** FrontPressure; unit=PSI; physical=raw*1+0; raw=[0,3000]. FrontPressure */
    uint16_t front_pressure;
    /** RearPressure; unit=PSI; physical=raw*1+0; raw=[0,3000]. RearPressure */
    uint16_t rear_pressure;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_pedals_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pedals_status_pack(can_frame_t* dst, const car_can_pedals_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pedals_status_unpack(car_can_pedals_status_t* dst, const can_frame_t* src);

/** Named raw values for IgnitionLV; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_IGNITION_LV_OFF = 0,
    CAR_CAN_DEI_STATUS_IGNITION_LV_ON = 1,
} car_can_dei_status_ignition_lv_choice_t;
/** Named raw values for IgnitionCharge; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_IGNITION_CHARGE_OFF = 0,
    CAR_CAN_DEI_STATUS_IGNITION_CHARGE_ON = 1,
} car_can_dei_status_ignition_charge_choice_t;
/** Named raw values for IgnitionDrive; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_IGNITION_DRIVE_OFF = 0,
    CAR_CAN_DEI_STATUS_IGNITION_DRIVE_ON = 1,
} car_can_dei_status_ignition_drive_choice_t;
/** Named raw values for Gear; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_GEAR_NEUTRAL = 0,
    CAR_CAN_DEI_STATUS_GEAR_FORWARD = 1,
    CAR_CAN_DEI_STATUS_GEAR_REVERSE = 2,
    CAR_CAN_DEI_STATUS_GEAR_INVALID = 3,
} car_can_dei_status_gear_choice_t;
/** Named raw values for Hazards; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_HAZARDS_OFF = 0,
    CAR_CAN_DEI_STATUS_HAZARDS_ON = 1,
} car_can_dei_status_hazards_choice_t;
/** Named raw values for LeftTurnSignal; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_LEFT_TURN_SIGNAL_OFF = 0,
    CAR_CAN_DEI_STATUS_LEFT_TURN_SIGNAL_ON = 1,
} car_can_dei_status_left_turn_signal_choice_t;
/** Named raw values for RightTurnSignal; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_RIGHT_TURN_SIGNAL_OFF = 0,
    CAR_CAN_DEI_STATUS_RIGHT_TURN_SIGNAL_ON = 1,
} car_can_dei_status_right_turn_signal_choice_t;
/** Named raw values for RegenEnable; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_REGEN_ENABLE_DISABLED = 0,
    CAR_CAN_DEI_STATUS_REGEN_ENABLE_ENABLED = 1,
} car_can_dei_status_regen_enable_choice_t;
/** Named raw values for RegenActivate; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_REGEN_ACTIVATE_OFF = 0,
    CAR_CAN_DEI_STATUS_REGEN_ACTIVATE_ON = 1,
} car_can_dei_status_regen_activate_choice_t;
/** Named raw values for Horn; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_HORN_OFF = 0,
    CAR_CAN_DEI_STATUS_HORN_ON = 1,
} car_can_dei_status_horn_choice_t;
/** Named raw values for PushToTalk; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_PUSH_TO_TALK_NOT_PRESSED = 0,
    CAR_CAN_DEI_STATUS_PUSH_TO_TALK_PRESSED = 1,
} car_can_dei_status_push_to_talk_choice_t;
/** Named raw values for Headlights; unknown values remain integers. */
typedef enum {
    CAR_CAN_DEI_STATUS_HEADLIGHTS_OFF = 0,
    CAR_CAN_DEI_STATUS_HEADLIGHTS_ON = 1,
} car_can_dei_status_headlights_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** IgnitionLV; unit=none; physical=raw*1+0; raw=[0,1]. IgnitionLV */
    uint8_t ignition_lv;
    /** IgnitionCharge; unit=none; physical=raw*1+0; raw=[0,1]. IgnitionCharge */
    uint8_t ignition_charge;
    /** IgnitionDrive; unit=none; physical=raw*1+0; raw=[0,1]. IgnitionDrive */
    uint8_t ignition_drive;
    /** Gear; unit=none; physical=raw*1+0; raw=[0,3]. Gear */
    uint8_t gear;
    /** Hazards; unit=none; physical=raw*1+0; raw=[0,1]. Hazards */
    uint8_t hazards;
    /** LeftTurnSignal; unit=none; physical=raw*1+0; raw=[0,1]. LeftTurnSignal */
    uint8_t left_turn_signal;
    /** RightTurnSignal; unit=none; physical=raw*1+0; raw=[0,1]. RightTurnSignal */
    uint8_t right_turn_signal;
    /** RegenEnable; unit=none; physical=raw*1+0; raw=[0,1]. RegenEnable */
    uint8_t regen_enable;
    /** RegenActivate; unit=none; physical=raw*1+0; raw=[0,1]. RegenActivate */
    uint8_t regen_activate;
    /** Horn; unit=none; physical=raw*1+0; raw=[0,1]. Horn */
    uint8_t horn;
    /** PushToTalk; unit=none; physical=raw*1+0; raw=[0,1]. PushToTalk */
    uint8_t push_to_talk;
    /** Headlights; unit=none; physical=raw*1+0; raw=[0,1]. Headlights */
    uint8_t headlights;
} car_can_dei_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_dei_status_pack(can_frame_t* dst, const car_can_dei_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_dei_status_unpack(car_can_dei_status_t* dst, const can_frame_t* src);

/** Named raw values for Status; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_STATUS_OK = 0,
    CAR_CAN_VCU_STATUS_STATUS_FAULT = 1,
} car_can_vcu_status_status_choice_t;
/** Named raw values for FaultMotor; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FAULT_MOTOR_OK = 0,
    CAR_CAN_VCU_STATUS_FAULT_MOTOR_FAULT = 1,
} car_can_vcu_status_fault_motor_choice_t;
/** Named raw values for FaultBPS; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FAULT_BPS_OK = 0,
    CAR_CAN_VCU_STATUS_FAULT_BPS_FAULT = 1,
} car_can_vcu_status_fault_bps_choice_t;
/** Named raw values for FaultPedals; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FAULT_PEDALS_OK = 0,
    CAR_CAN_VCU_STATUS_FAULT_PEDALS_FAULT = 1,
} car_can_vcu_status_fault_pedals_choice_t;
/** Named raw values for FaultMotorControllerWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FAULT_MOTOR_CONTROLLER_WATCHDOG_OK = 0,
    CAR_CAN_VCU_STATUS_FAULT_MOTOR_CONTROLLER_WATCHDOG_FAULT = 1,
} car_can_vcu_status_fault_motor_controller_watchdog_choice_t;
/** Named raw values for FaultBPSWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FAULT_BPS_WATCHDOG_OK = 0,
    CAR_CAN_VCU_STATUS_FAULT_BPS_WATCHDOG_FAULT = 1,
} car_can_vcu_status_fault_bps_watchdog_choice_t;
/** Named raw values for FaultDriverInputWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FAULT_DRIVER_INPUT_WATCHDOG_OK = 0,
    CAR_CAN_VCU_STATUS_FAULT_DRIVER_INPUT_WATCHDOG_FAULT = 1,
} car_can_vcu_status_fault_driver_input_watchdog_choice_t;
/** Named raw values for FaultPedalsWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FAULT_PEDALS_WATCHDOG_OK = 0,
    CAR_CAN_VCU_STATUS_FAULT_PEDALS_WATCHDOG_FAULT = 1,
} car_can_vcu_status_fault_pedals_watchdog_choice_t;
/** Named raw values for WarningTipping; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_WARNING_TIPPING_NO_WARNING = 0,
    CAR_CAN_VCU_STATUS_WARNING_TIPPING_WARNING = 1,
} car_can_vcu_status_warning_tipping_choice_t;
/** Named raw values for WarningDirectionChangeLockout; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_WARNING_DIRECTION_CHANGE_LOCKOUT_NO_WARNING = 0,
    CAR_CAN_VCU_STATUS_WARNING_DIRECTION_CHANGE_LOCKOUT_WARNING = 1,
} car_can_vcu_status_warning_direction_change_lockout_choice_t;
/** Named raw values for WarningMotorControllerFan; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_WARNING_MOTOR_CONTROLLER_FAN_NO_WARNING = 0,
    CAR_CAN_VCU_STATUS_WARNING_MOTOR_CONTROLLER_FAN_WARNING = 1,
} car_can_vcu_status_warning_motor_controller_fan_choice_t;
/** Named raw values for WarningMotorFan; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_WARNING_MOTOR_FAN_NO_WARNING = 0,
    CAR_CAN_VCU_STATUS_WARNING_MOTOR_FAN_WARNING = 1,
} car_can_vcu_status_warning_motor_fan_choice_t;
/** Named raw values for RegenEnabled; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_REGEN_ENABLED_DISABLED = 0,
    CAR_CAN_VCU_STATUS_REGEN_ENABLED_ENABLED = 1,
} car_can_vcu_status_regen_enabled_choice_t;
/** Named raw values for State; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_STATE_INIT = 0,
    CAR_CAN_VCU_STATUS_STATE_FORWARD_DRIVE = 1,
    CAR_CAN_VCU_STATUS_STATE_NEUTRAL_DRIVE = 2,
    CAR_CAN_VCU_STATUS_STATE_REVERSE_DRIVE = 3,
    CAR_CAN_VCU_STATUS_STATE_REGEN = 4,
    CAR_CAN_VCU_STATUS_STATE_CRUISE_CONTROL = 5,
    CAR_CAN_VCU_STATUS_STATE_DISABLED = 6,
    CAR_CAN_VCU_STATUS_STATE_VEHICLE_NOT_READY = 7,
} car_can_vcu_status_state_choice_t;
/** Named raw values for FSMBrake; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FSM_BRAKE_OFF = 0,
    CAR_CAN_VCU_STATUS_FSM_BRAKE_ON = 1,
} car_can_vcu_status_fsm_brake_choice_t;
/** Named raw values for FSMBPSDriveOK; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FSMBPS_DRIVE_OK_NOT_ALLOWED = 0,
    CAR_CAN_VCU_STATUS_FSMBPS_DRIVE_OK_ALLOWED = 1,
} car_can_vcu_status_fsmbps_drive_ok_choice_t;
/** Named raw values for FSMBPSRegenOK; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FSMBPS_REGEN_OK_NOT_ALLOWED = 0,
    CAR_CAN_VCU_STATUS_FSMBPS_REGEN_OK_ALLOWED = 1,
} car_can_vcu_status_fsmbps_regen_ok_choice_t;
/** Named raw values for FSMDriverRegenEnable; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FSM_DRIVER_REGEN_ENABLE_DISABLED = 0,
    CAR_CAN_VCU_STATUS_FSM_DRIVER_REGEN_ENABLE_ENABLED = 1,
} car_can_vcu_status_fsm_driver_regen_enable_choice_t;
/** Named raw values for FSMDriverRegenRequest; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FSM_DRIVER_REGEN_REQUEST_OFF = 0,
    CAR_CAN_VCU_STATUS_FSM_DRIVER_REGEN_REQUEST_ON = 1,
} car_can_vcu_status_fsm_driver_regen_request_choice_t;
/** Named raw values for FSMGear; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_FSM_GEAR_NEUTRAL = 0,
    CAR_CAN_VCU_STATUS_FSM_GEAR_FORWARD = 1,
    CAR_CAN_VCU_STATUS_FSM_GEAR_REVERSE = 2,
    CAR_CAN_VCU_STATUS_FSM_GEAR_INVALID = 3,
} car_can_vcu_status_fsm_gear_choice_t;
/** Named raw values for MotorCommandSource; unknown values remain integers. */
typedef enum {
    CAR_CAN_VCU_STATUS_MOTOR_COMMAND_SOURCE_VCU = 0,
    CAR_CAN_VCU_STATUS_MOTOR_COMMAND_SOURCE_EXTERNAL = 1,
} car_can_vcu_status_motor_command_source_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** Status; unit=none; physical=raw*1+0; raw=[0,1]. Status */
    uint8_t status;
    /** FaultMotor; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotor */
    uint8_t fault_motor;
    /** FaultBPS; unit=none; physical=raw*1+0; raw=[0,1]. FaultBPS */
    uint8_t fault_bps;
    /** FaultPedals; unit=none; physical=raw*1+0; raw=[0,1]. FaultPedals */
    uint8_t fault_pedals;
    /** FaultMotorControllerWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorControllerWatchdog */
    uint8_t fault_motor_controller_watchdog;
    /** FaultBPSWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. FaultBPSWatchdog */
    uint8_t fault_bps_watchdog;
    /** FaultDriverInputWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. FaultDriverInputWatchdog */
    uint8_t fault_driver_input_watchdog;
    /** FaultPedalsWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. FaultPedalsWatchdog */
    uint8_t fault_pedals_watchdog;
    /** WarningTipping; unit=none; physical=raw*1+0; raw=[0,1]. WarningTipping */
    uint8_t warning_tipping;
    /** WarningDirectionChangeLockout; unit=none; physical=raw*1+0; raw=[0,1]. WarningDirectionChangeLockout */
    uint8_t warning_direction_change_lockout;
    /** WarningMotorControllerFan; unit=none; physical=raw*1+0; raw=[0,1]. WarningMotorControllerFan */
    uint8_t warning_motor_controller_fan;
    /** WarningMotorFan; unit=none; physical=raw*1+0; raw=[0,1]. WarningMotorFan */
    uint8_t warning_motor_fan;
    /** RegenEnabled; unit=none; physical=raw*1+0; raw=[0,1]. RegenEnabled */
    uint8_t regen_enabled;
    /** State; unit=none; physical=raw*1+0; raw=[0,7]. State */
    uint8_t state;
    /** FSMBrake; unit=none; physical=raw*1+0; raw=[0,1]. FSMBrake */
    uint8_t fsm_brake;
    /** FSMBPSDriveOK; unit=none; physical=raw*1+0; raw=[0,1]. FSMBPSDriveOK */
    uint8_t fsmbps_drive_ok;
    /** FSMBPSRegenOK; unit=none; physical=raw*1+0; raw=[0,1]. FSMBPSRegenOK */
    uint8_t fsmbps_regen_ok;
    /** FSMDriverRegenEnable; unit=none; physical=raw*1+0; raw=[0,1]. FSMDriverRegenEnable */
    uint8_t fsm_driver_regen_enable;
    /** FSMDriverRegenRequest; unit=none; physical=raw*1+0; raw=[0,1]. FSMDriverRegenRequest */
    uint8_t fsm_driver_regen_request;
    /** FSMGear; unit=none; physical=raw*1+0; raw=[0,3]. FSMGear */
    uint8_t fsm_gear;
    /** MotorCommandSource; unit=none; physical=raw*1+0; raw=[0,1]. MotorCommandSource */
    uint8_t motor_command_source;
    /** MotorControllerFanTach; unit=rpm; physical=raw*5+0; raw=[0,4095]. MotorControllerFanTach */
    uint16_t motor_controller_fan_tach;
    /** MotorFanTach; unit=rpm; physical=raw*5+0; raw=[0,4095]. MotorFanTach */
    uint16_t motor_fan_tach;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_vcu_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_vcu_status_pack(can_frame_t* dst, const car_can_vcu_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_vcu_status_unpack(car_can_vcu_status_t* dst, const can_frame_t* src);

/** BPS pack electrical measurements: cell-sum and main/motor battery and precharge voltages, then battery current. Cell-voltage extrema are in BPSVoltageData; temperature summaries are in BPSTemperatureData. Signal encodings unchanged; no transmit period assigned yet. */
typedef struct {
    /** CellSumVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. Sum of the 32 cell-tap voltages, independent of precharge measurements. Bulk voltage encoding: unsigned 12 bits, 0.05 V/bit, offset 0 V. */
    double cell_sum_voltage;
    /** MainBatteryVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. MainBatteryVoltage */
    double main_battery_voltage;
    /** MotorBatteryVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. MotorBatteryVoltage */
    double motor_battery_voltage;
    /** MainVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. MainVoltage */
    double main_voltage;
    /** MotorVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. MotorVoltage */
    double motor_voltage;
    /** BatteryCurrent; unit=A; physical=raw*0.01+-50; raw=[0,16383]. BatteryCurrent */
    double battery_current;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_bps_pack_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_pack_data_pack(can_frame_t* dst, const car_can_bps_pack_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_pack_data_unpack(car_can_bps_pack_data_t* dst, const can_frame_t* src);

/** BPS individual cell voltages followed by minimum/maximum cell voltage; unchanged physical encodings. */
typedef struct {
    /** CellVoltage01; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage01 */
    uint16_t cell_voltage01;
    /** CellVoltage02; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage02 */
    uint16_t cell_voltage02;
    /** CellVoltage03; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage03 */
    uint16_t cell_voltage03;
    /** CellVoltage04; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage04 */
    uint16_t cell_voltage04;
    /** CellVoltage05; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage05 */
    uint16_t cell_voltage05;
    /** CellVoltage06; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage06 */
    uint16_t cell_voltage06;
    /** CellVoltage07; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage07 */
    uint16_t cell_voltage07;
    /** CellVoltage08; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage08 */
    uint16_t cell_voltage08;
    /** CellVoltage09; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage09 */
    uint16_t cell_voltage09;
    /** CellVoltage10; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage10 */
    uint16_t cell_voltage10;
    /** CellVoltage11; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage11 */
    uint16_t cell_voltage11;
    /** CellVoltage12; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage12 */
    uint16_t cell_voltage12;
    /** CellVoltage13; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage13 */
    uint16_t cell_voltage13;
    /** CellVoltage14; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage14 */
    uint16_t cell_voltage14;
    /** CellVoltage15; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage15 */
    uint16_t cell_voltage15;
    /** CellVoltage16; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage16 */
    uint16_t cell_voltage16;
    /** CellVoltage17; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage17 */
    uint16_t cell_voltage17;
    /** CellVoltage18; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage18 */
    uint16_t cell_voltage18;
    /** CellVoltage19; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage19 */
    uint16_t cell_voltage19;
    /** CellVoltage20; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage20 */
    uint16_t cell_voltage20;
    /** CellVoltage21; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage21 */
    uint16_t cell_voltage21;
    /** CellVoltage22; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage22 */
    uint16_t cell_voltage22;
    /** CellVoltage23; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage23 */
    uint16_t cell_voltage23;
    /** CellVoltage24; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage24 */
    uint16_t cell_voltage24;
    /** CellVoltage25; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage25 */
    uint16_t cell_voltage25;
    /** CellVoltage26; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage26 */
    uint16_t cell_voltage26;
    /** CellVoltage27; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage27 */
    uint16_t cell_voltage27;
    /** CellVoltage28; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage28 */
    uint16_t cell_voltage28;
    /** CellVoltage29; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage29 */
    uint16_t cell_voltage29;
    /** CellVoltage30; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage30 */
    uint16_t cell_voltage30;
    /** CellVoltage31; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage31 */
    uint16_t cell_voltage31;
    /** CellVoltage32; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage32 */
    uint16_t cell_voltage32;
    /** MinCellVoltage; unit=mV; physical=raw*1+2000; raw=[0,4095]. MinCellVoltage */
    uint16_t min_cell_voltage;
    /** MaxCellVoltage; unit=mV; physical=raw*1+2000; raw=[0,4095]. MaxCellVoltage */
    uint16_t max_cell_voltage;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_bps_voltage_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_voltage_data_pack(can_frame_t* dst, const car_can_bps_voltage_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_voltage_data_unpack(car_can_bps_voltage_data_t* dst, const can_frame_t* src);

/** BPS individual cell temperatures followed by maximum/average battery temperature; unchanged physical encodings. */
typedef struct {
    /** CellTemperature01; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature01 */
    double cell_temperature01;
    /** CellTemperature02; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature02 */
    double cell_temperature02;
    /** CellTemperature03; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature03 */
    double cell_temperature03;
    /** CellTemperature04; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature04 */
    double cell_temperature04;
    /** CellTemperature05; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature05 */
    double cell_temperature05;
    /** CellTemperature06; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature06 */
    double cell_temperature06;
    /** CellTemperature07; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature07 */
    double cell_temperature07;
    /** CellTemperature08; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature08 */
    double cell_temperature08;
    /** CellTemperature09; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature09 */
    double cell_temperature09;
    /** CellTemperature10; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature10 */
    double cell_temperature10;
    /** CellTemperature11; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature11 */
    double cell_temperature11;
    /** CellTemperature12; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature12 */
    double cell_temperature12;
    /** CellTemperature13; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature13 */
    double cell_temperature13;
    /** CellTemperature14; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature14 */
    double cell_temperature14;
    /** CellTemperature15; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature15 */
    double cell_temperature15;
    /** CellTemperature16; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature16 */
    double cell_temperature16;
    /** CellTemperature17; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature17 */
    double cell_temperature17;
    /** CellTemperature18; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature18 */
    double cell_temperature18;
    /** CellTemperature19; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature19 */
    double cell_temperature19;
    /** CellTemperature20; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature20 */
    double cell_temperature20;
    /** CellTemperature21; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature21 */
    double cell_temperature21;
    /** CellTemperature22; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature22 */
    double cell_temperature22;
    /** CellTemperature23; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature23 */
    double cell_temperature23;
    /** CellTemperature24; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature24 */
    double cell_temperature24;
    /** CellTemperature25; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature25 */
    double cell_temperature25;
    /** CellTemperature26; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature26 */
    double cell_temperature26;
    /** CellTemperature27; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature27 */
    double cell_temperature27;
    /** CellTemperature28; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature28 */
    double cell_temperature28;
    /** CellTemperature29; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature29 */
    double cell_temperature29;
    /** CellTemperature30; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature30 */
    double cell_temperature30;
    /** CellTemperature31; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature31 */
    double cell_temperature31;
    /** CellTemperature32; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature32 */
    double cell_temperature32;
    /** MaxBatteryTemperature; unit=degC; physical=raw*0.05+0; raw=[0,2047]. MaxBatteryTemperature */
    double max_battery_temperature;
    /** AverageBatteryTemperature; unit=degC; physical=raw*0.05+0; raw=[0,2047]. AverageBatteryTemperature */
    double average_battery_temperature;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_bps_temperature_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_temperature_data_pack(can_frame_t* dst, const car_can_bps_temperature_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_temperature_data_unpack(car_can_bps_temperature_data_t* dst, const can_frame_t* src);

/** PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. Four SMB status bitmaps: widths and bit definitions pending. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} car_can_smd_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_smd_status_pack(can_frame_t* dst, const car_can_smd_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_smd_status_unpack(car_can_smd_status_t* dst, const can_frame_t* src);

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_COMMAND_ECHO_MODE_NORMAL = 0,
    CAR_CAN_BPS_COMMAND_ECHO_MODE_ADVANCED = 1,
} car_can_bps_command_echo_mode_choice_t;
/** Named raw values for MPPTChargeScheme; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_COMMAND_ECHO_MPPT_CHARGE_SCHEME_NORMAL = 0,
    CAR_CAN_BPS_COMMAND_ECHO_MPPT_CHARGE_SCHEME_ADVANCED = 1,
} car_can_bps_command_echo_mppt_charge_scheme_choice_t;
/** Cyclic BPS report mirroring the BPSCommand payload: Mode, MPPTChargeScheme, VoltageOverrideMask, and TemperatureOverrideMask. Reports command values held by BPS; does not imply successful application or act as a per-request acknowledgement. Cyclic report every 1000 ms. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,1]. Mode */
    uint8_t mode;
    /** MPPTChargeScheme; unit=none; physical=raw*1+0; raw=[0,1]. MPPTChargeScheme */
    uint8_t mppt_charge_scheme;
    /** VoltageOverrideMask; unit=none; physical=raw*1+0; raw=[0,4294967295]. Bit 0 selects tap 1; bit 31 selects tap 32. Set bit requests the corresponding override. */
    uint32_t voltage_override_mask;
    /** TemperatureOverrideMask; unit=none; physical=raw*1+0; raw=[0,4294967295]. Bit 0 selects temperature 1; bit 31 selects temperature 32. Set bit requests the corresponding override. */
    uint32_t temperature_override_mask;
} car_can_bps_command_echo_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_command_echo_pack(can_frame_t* dst, const car_can_bps_command_echo_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_command_echo_unpack(car_can_bps_command_echo_t* dst, const can_frame_t* src);

/** Named raw values for FaultSummary; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_SUMMARY_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_SUMMARY_SETPOINT_FAULT = 1,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_SUMMARY_EMERGENCY_STOP = 2,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_SUMMARY_CONTACTOR_RELAY_FAULT = 3,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_SUMMARY_PRECHARGE_FAULT = 4,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_SUMMARY_SMB_FAULT = 5,
} car_can_bps_fault_snapshot_fault_summary_choice_t;
/** Named raw values for FaultOverTemperature; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_OVER_TEMPERATURE_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_OVER_TEMPERATURE_FAULT = 1,
} car_can_bps_fault_snapshot_fault_over_temperature_choice_t;
/** Named raw values for FaultOverVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_OVER_VOLTAGE_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_OVER_VOLTAGE_FAULT = 1,
} car_can_bps_fault_snapshot_fault_over_voltage_choice_t;
/** Named raw values for FaultUnderVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_UNDER_VOLTAGE_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_UNDER_VOLTAGE_FAULT = 1,
} car_can_bps_fault_snapshot_fault_under_voltage_choice_t;
/** Named raw values for FaultChargeOverCurrent; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_CHARGE_OVER_CURRENT_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_CHARGE_OVER_CURRENT_FAULT = 1,
} car_can_bps_fault_snapshot_fault_charge_over_current_choice_t;
/** Named raw values for FaultDischargeOverCurrent; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_DISCHARGE_OVER_CURRENT_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_DISCHARGE_OVER_CURRENT_FAULT = 1,
} car_can_bps_fault_snapshot_fault_discharge_over_current_choice_t;
/** Named raw values for FaultHVPositiveContactorMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_HV_POSITIVE_CONTACTOR_MISMATCH_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_HV_POSITIVE_CONTACTOR_MISMATCH_FAULT = 1,
} car_can_bps_fault_snapshot_fault_hv_positive_contactor_mismatch_choice_t;
/** Named raw values for FaultMotorContactorMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_CONTACTOR_MISMATCH_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_CONTACTOR_MISMATCH_FAULT = 1,
} car_can_bps_fault_snapshot_fault_motor_contactor_mismatch_choice_t;
/** Named raw values for FaultHVNegativeContactorMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_HV_NEGATIVE_CONTACTOR_MISMATCH_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_HV_NEGATIVE_CONTACTOR_MISMATCH_FAULT = 1,
} car_can_bps_fault_snapshot_fault_hv_negative_contactor_mismatch_choice_t;
/** Named raw values for FaultMainPrechargeRelayMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_PRECHARGE_RELAY_MISMATCH_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_PRECHARGE_RELAY_MISMATCH_FAULT = 1,
} car_can_bps_fault_snapshot_fault_main_precharge_relay_mismatch_choice_t;
/** Named raw values for FaultMotorPrechargeRelayMismatch; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_PRECHARGE_RELAY_MISMATCH_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_PRECHARGE_RELAY_MISMATCH_FAULT = 1,
} car_can_bps_fault_snapshot_fault_motor_precharge_relay_mismatch_choice_t;
/** Named raw values for FaultDriverEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_DRIVER_ESTOP_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_DRIVER_ESTOP_FAULT = 1,
} car_can_bps_fault_snapshot_fault_driver_estop_choice_t;
/** Named raw values for FaultExternalEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_EXTERNAL_ESTOP_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_EXTERNAL_ESTOP_FAULT = 1,
} car_can_bps_fault_snapshot_fault_external_estop_choice_t;
/** Named raw values for FaultBatteryEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_BATTERY_ESTOP_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_BATTERY_ESTOP_FAULT = 1,
} car_can_bps_fault_snapshot_fault_battery_estop_choice_t;
/** Named raw values for FaultChargerEstop; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_CHARGER_ESTOP_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_CHARGER_ESTOP_FAULT = 1,
} car_can_bps_fault_snapshot_fault_charger_estop_choice_t;
/** Named raw values for FaultMainPrechargeTimeout; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_PRECHARGE_TIMEOUT_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_PRECHARGE_TIMEOUT_FAULT = 1,
} car_can_bps_fault_snapshot_fault_main_precharge_timeout_choice_t;
/** Named raw values for FaultMotorPrechargeTimeout; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_PRECHARGE_TIMEOUT_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_PRECHARGE_TIMEOUT_FAULT = 1,
} car_can_bps_fault_snapshot_fault_motor_precharge_timeout_choice_t;
/** Named raw values for FaultMainVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_VOLTAGE_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_VOLTAGE_FAULT = 1,
} car_can_bps_fault_snapshot_fault_main_voltage_choice_t;
/** Named raw values for FaultMotorVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_VOLTAGE_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_VOLTAGE_FAULT = 1,
} car_can_bps_fault_snapshot_fault_motor_voltage_choice_t;
/** Named raw values for FaultMainPrechargeVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_PRECHARGE_VOLTAGE_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MAIN_PRECHARGE_VOLTAGE_FAULT = 1,
} car_can_bps_fault_snapshot_fault_main_precharge_voltage_choice_t;
/** Named raw values for FaultMotorPrechargeVoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_PRECHARGE_VOLTAGE_OK = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_FAULT_MOTOR_PRECHARGE_VOLTAGE_FAULT = 1,
} car_can_bps_fault_snapshot_fault_motor_precharge_voltage_choice_t;
/** Named raw values for CurrentValid; unknown values remain integers. */
typedef enum {
    CAR_CAN_BPS_FAULT_SNAPSHOT_CURRENT_VALID_INVALID = 0,
    CAR_CAN_BPS_FAULT_SNAPSHOT_CURRENT_VALID_VALID = 1,
} car_can_bps_fault_snapshot_current_valid_choice_t;
/** Latch once when BPS transitions from no active faults to any active fault, before opening contactors where possible. Send immediately, then every 1000 ms only while any BPS fault remains active. Keep captured samples frozen; never replace them with post-trip zeros. Rearm after all faults clear. Triggered transmission with a 1000 ms active-fault repeat interval; no transmission outside the send condition. Fault bits record the first fault state. BatteryCurrent preserves the live message scaling. CurrentSampleAgeMs describes sampling latency; this is not a guaranteed instantaneous peak measurement. */
typedef struct {
    /** FaultSummary; unit=none; physical=raw*1+0; raw=[0,7]. FaultSummary */
    uint8_t fault_summary;
    /** FaultOverTemperature; unit=none; physical=raw*1+0; raw=[0,1]. FaultOverTemperature */
    uint8_t fault_over_temperature;
    /** FaultOverVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultOverVoltage */
    uint8_t fault_over_voltage;
    /** FaultUnderVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultUnderVoltage */
    uint8_t fault_under_voltage;
    /** FaultChargeOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. FaultChargeOverCurrent */
    uint8_t fault_charge_over_current;
    /** FaultDischargeOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. FaultDischargeOverCurrent */
    uint8_t fault_discharge_over_current;
    /** FaultHVPositiveContactorMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultHVPositiveContactorMismatch */
    uint8_t fault_hv_positive_contactor_mismatch;
    /** FaultMotorContactorMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorContactorMismatch */
    uint8_t fault_motor_contactor_mismatch;
    /** FaultHVNegativeContactorMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultHVNegativeContactorMismatch */
    uint8_t fault_hv_negative_contactor_mismatch;
    /** FaultMainPrechargeRelayMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainPrechargeRelayMismatch */
    uint8_t fault_main_precharge_relay_mismatch;
    /** FaultMotorPrechargeRelayMismatch; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorPrechargeRelayMismatch */
    uint8_t fault_motor_precharge_relay_mismatch;
    /** FaultDriverEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultDriverEstop */
    uint8_t fault_driver_estop;
    /** FaultExternalEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultExternalEstop */
    uint8_t fault_external_estop;
    /** FaultBatteryEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultBatteryEstop */
    uint8_t fault_battery_estop;
    /** FaultChargerEstop; unit=none; physical=raw*1+0; raw=[0,1]. FaultChargerEstop */
    uint8_t fault_charger_estop;
    /** FaultMainPrechargeTimeout; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainPrechargeTimeout */
    uint8_t fault_main_precharge_timeout;
    /** FaultMotorPrechargeTimeout; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorPrechargeTimeout */
    uint8_t fault_motor_precharge_timeout;
    /** FaultMainVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainVoltage */
    uint8_t fault_main_voltage;
    /** FaultMotorVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorVoltage */
    uint8_t fault_motor_voltage;
    /** FaultMainPrechargeVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMainPrechargeVoltage */
    uint8_t fault_main_precharge_voltage;
    /** FaultMotorPrechargeVoltage; unit=none; physical=raw*1+0; raw=[0,1]. FaultMotorPrechargeVoltage */
    uint8_t fault_motor_precharge_voltage;
    /** CurrentValid; unit=none; physical=raw*1+0; raw=[0,1]. 1 only if BatteryCurrent was sampled successfully before capture; otherwise ignore BatteryCurrent and CurrentSampleAgeMs. */
    uint8_t current_valid;
    /** CaptureTimeMs; unit=ms; physical=raw*1+0; raw=[0,4294967295]. BPS monotonic uptime at fault detection, wraps at 2^32 ms; independent of legacy TimestampSync ticks. */
    uint32_t capture_time_ms;
    /** BatteryCurrent; unit=A; physical=raw*0.01+-50; raw=[0,16383]. BatteryCurrent */
    double battery_current;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_bps_fault_snapshot_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_fault_snapshot_pack(can_frame_t* dst, const car_can_bps_fault_snapshot_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_fault_snapshot_unpack(car_can_bps_fault_snapshot_t* dst, const can_frame_t* src);

/** Latch once when BPS transitions from no active faults to any active fault, before opening contactors where possible. Send immediately, then every 1000 ms only while any BPS fault remains active. Keep captured samples frozen; never replace them with post-trip zeros. Rearm after all faults clear. Triggered transmission with a 1000 ms active-fault repeat interval; no transmission outside the send condition. The last completed scan available at fault detection is frozen, not acquired after shutdown. Scaling and offsets match BPSVoltageData. */
typedef struct {
    /** SampleValidMask; unit=none; physical=raw*1+0; raw=[0,4294967295]. Bit 0 corresponds to cell 01; bit 31 to cell 32. Clear bits mean unavailable readings and their encoded values must be ignored. */
    uint32_t sample_valid_mask;
    /** CellVoltage01; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage01 */
    uint16_t cell_voltage01;
    /** CellVoltage02; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage02 */
    uint16_t cell_voltage02;
    /** CellVoltage03; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage03 */
    uint16_t cell_voltage03;
    /** CellVoltage04; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage04 */
    uint16_t cell_voltage04;
    /** CellVoltage05; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage05 */
    uint16_t cell_voltage05;
    /** CellVoltage06; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage06 */
    uint16_t cell_voltage06;
    /** CellVoltage07; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage07 */
    uint16_t cell_voltage07;
    /** CellVoltage08; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage08 */
    uint16_t cell_voltage08;
    /** CellVoltage09; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage09 */
    uint16_t cell_voltage09;
    /** CellVoltage10; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage10 */
    uint16_t cell_voltage10;
    /** CellVoltage11; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage11 */
    uint16_t cell_voltage11;
    /** CellVoltage12; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage12 */
    uint16_t cell_voltage12;
    /** CellVoltage13; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage13 */
    uint16_t cell_voltage13;
    /** CellVoltage14; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage14 */
    uint16_t cell_voltage14;
    /** CellVoltage15; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage15 */
    uint16_t cell_voltage15;
    /** CellVoltage16; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage16 */
    uint16_t cell_voltage16;
    /** CellVoltage17; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage17 */
    uint16_t cell_voltage17;
    /** CellVoltage18; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage18 */
    uint16_t cell_voltage18;
    /** CellVoltage19; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage19 */
    uint16_t cell_voltage19;
    /** CellVoltage20; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage20 */
    uint16_t cell_voltage20;
    /** CellVoltage21; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage21 */
    uint16_t cell_voltage21;
    /** CellVoltage22; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage22 */
    uint16_t cell_voltage22;
    /** CellVoltage23; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage23 */
    uint16_t cell_voltage23;
    /** CellVoltage24; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage24 */
    uint16_t cell_voltage24;
    /** CellVoltage25; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage25 */
    uint16_t cell_voltage25;
    /** CellVoltage26; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage26 */
    uint16_t cell_voltage26;
    /** CellVoltage27; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage27 */
    uint16_t cell_voltage27;
    /** CellVoltage28; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage28 */
    uint16_t cell_voltage28;
    /** CellVoltage29; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage29 */
    uint16_t cell_voltage29;
    /** CellVoltage30; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage30 */
    uint16_t cell_voltage30;
    /** CellVoltage31; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage31 */
    uint16_t cell_voltage31;
    /** CellVoltage32; unit=mV; physical=raw*1+2000; raw=[0,4095]. CellVoltage32 */
    uint16_t cell_voltage32;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_bps_fault_voltage_snapshot_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_fault_voltage_snapshot_pack(can_frame_t* dst, const car_can_bps_fault_voltage_snapshot_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_fault_voltage_snapshot_unpack(car_can_bps_fault_voltage_snapshot_t* dst, const can_frame_t* src);

/** Latch once when BPS transitions from no active faults to any active fault, before opening contactors where possible. Send immediately, then every 1000 ms only while any BPS fault remains active. Keep captured samples frozen; never replace them with post-trip zeros. Rearm after all faults clear. Triggered transmission with a 1000 ms active-fault repeat interval; no transmission outside the send condition. The last completed scan available at fault detection is frozen, not acquired after shutdown. Scaling and offsets match BPSTemperatureData. */
typedef struct {
    /** SampleValidMask; unit=none; physical=raw*1+0; raw=[0,4294967295]. Bit 0 corresponds to cell 01; bit 31 to cell 32. Clear bits mean unavailable readings and their encoded values must be ignored. */
    uint32_t sample_valid_mask;
    /** CellTemperature01; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature01 */
    double cell_temperature01;
    /** CellTemperature02; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature02 */
    double cell_temperature02;
    /** CellTemperature03; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature03 */
    double cell_temperature03;
    /** CellTemperature04; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature04 */
    double cell_temperature04;
    /** CellTemperature05; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature05 */
    double cell_temperature05;
    /** CellTemperature06; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature06 */
    double cell_temperature06;
    /** CellTemperature07; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature07 */
    double cell_temperature07;
    /** CellTemperature08; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature08 */
    double cell_temperature08;
    /** CellTemperature09; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature09 */
    double cell_temperature09;
    /** CellTemperature10; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature10 */
    double cell_temperature10;
    /** CellTemperature11; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature11 */
    double cell_temperature11;
    /** CellTemperature12; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature12 */
    double cell_temperature12;
    /** CellTemperature13; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature13 */
    double cell_temperature13;
    /** CellTemperature14; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature14 */
    double cell_temperature14;
    /** CellTemperature15; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature15 */
    double cell_temperature15;
    /** CellTemperature16; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature16 */
    double cell_temperature16;
    /** CellTemperature17; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature17 */
    double cell_temperature17;
    /** CellTemperature18; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature18 */
    double cell_temperature18;
    /** CellTemperature19; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature19 */
    double cell_temperature19;
    /** CellTemperature20; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature20 */
    double cell_temperature20;
    /** CellTemperature21; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature21 */
    double cell_temperature21;
    /** CellTemperature22; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature22 */
    double cell_temperature22;
    /** CellTemperature23; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature23 */
    double cell_temperature23;
    /** CellTemperature24; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature24 */
    double cell_temperature24;
    /** CellTemperature25; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature25 */
    double cell_temperature25;
    /** CellTemperature26; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature26 */
    double cell_temperature26;
    /** CellTemperature27; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature27 */
    double cell_temperature27;
    /** CellTemperature28; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature28 */
    double cell_temperature28;
    /** CellTemperature29; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature29 */
    double cell_temperature29;
    /** CellTemperature30; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature30 */
    double cell_temperature30;
    /** CellTemperature31; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature31 */
    double cell_temperature31;
    /** CellTemperature32; unit=degC; physical=raw*0.05+0; raw=[0,2047]. CellTemperature32 */
    double cell_temperature32;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_bps_fault_temperature_snapshot_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_bps_fault_temperature_snapshot_pack(can_frame_t* dst, const car_can_bps_fault_temperature_snapshot_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_bps_fault_temperature_snapshot_unpack(car_can_bps_fault_temperature_snapshot_t* dst, const can_frame_t* src);

/** Named raw values for WarningBPSWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_BPS_WATCHDOG_NO_WARNING = 0,
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_BPS_WATCHDOG_WARNING = 1,
} car_can_mppt_interface_status_warning_bps_watchdog_choice_t;
/** Named raw values for WarningMPPTAWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_MPPTA_WATCHDOG_NO_WARNING = 0,
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_MPPTA_WATCHDOG_WARNING = 1,
} car_can_mppt_interface_status_warning_mppta_watchdog_choice_t;
/** Named raw values for WarningMPPTBWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_MPPTB_WATCHDOG_NO_WARNING = 0,
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_MPPTB_WATCHDOG_WARNING = 1,
} car_can_mppt_interface_status_warning_mpptb_watchdog_choice_t;
/** Named raw values for WarningMPPTCWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_MPPTC_WATCHDOG_NO_WARNING = 0,
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_MPPTC_WATCHDOG_WARNING = 1,
} car_can_mppt_interface_status_warning_mpptc_watchdog_choice_t;
/** Named raw values for WarningFan; unknown values remain integers. */
typedef enum {
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_FAN_NO_WARNING = 0,
    CAR_CAN_MPPT_INTERFACE_STATUS_WARNING_FAN_WARNING = 1,
} car_can_mppt_interface_status_warning_fan_choice_t;
/** Named raw values for MPPTSetMode; unknown values remain integers. */
typedef enum {
    CAR_CAN_MPPT_INTERFACE_STATUS_MPPT_SET_MODE_DISABLED = 0,
    CAR_CAN_MPPT_INTERFACE_STATUS_MPPT_SET_MODE_PERTURB_AND_OBSERVE = 1,
    CAR_CAN_MPPT_INTERFACE_STATUS_MPPT_SET_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} car_can_mppt_interface_status_mppt_set_mode_choice_t;
/** Named raw values for SweepingActive; unknown values remain integers. */
typedef enum {
    CAR_CAN_MPPT_INTERFACE_STATUS_SWEEPING_ACTIVE_OFF = 0,
    CAR_CAN_MPPT_INTERFACE_STATUS_SWEEPING_ACTIVE_ON = 1,
} car_can_mppt_interface_status_sweeping_active_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** WarningBPSWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. WarningBPSWatchdog */
    uint8_t warning_bps_watchdog;
    /** WarningMPPTAWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. WarningMPPTAWatchdog */
    uint8_t warning_mppta_watchdog;
    /** WarningMPPTBWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. WarningMPPTBWatchdog */
    uint8_t warning_mpptb_watchdog;
    /** WarningMPPTCWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. WarningMPPTCWatchdog */
    uint8_t warning_mpptc_watchdog;
    /** WarningFan; unit=none; physical=raw*1+0; raw=[0,1]. WarningFan */
    uint8_t warning_fan;
    /** MPPTSetMode; unit=none; physical=raw*1+0; raw=[0,3]. MPPTSetMode */
    uint8_t mppt_set_mode;
    /** SweepingActive; unit=none; physical=raw*1+0; raw=[0,1]. SweepingActive */
    uint8_t sweeping_active;
    /** FanTach; unit=rpm; physical=raw*5+0; raw=[0,4095]. FanTach */
    uint16_t fan_tach;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_mppt_interface_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_mppt_interface_status_pack(can_frame_t* dst, const car_can_mppt_interface_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_mppt_interface_status_unpack(car_can_mppt_interface_status_t* dst, const can_frame_t* src);

/** Named raw values for WarningTemperature; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_WARNING_TEMPERATURE_NO_WARNING = 0,
    CAR_CAN_MIKA_STATUS_WARNING_TEMPERATURE_WARNING = 1,
} car_can_mika_status_warning_temperature_choice_t;
/** Named raw values for WarningOverTemperature; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_WARNING_OVER_TEMPERATURE_NO_WARNING = 0,
    CAR_CAN_MIKA_STATUS_WARNING_OVER_TEMPERATURE_WARNING = 1,
} car_can_mika_status_warning_over_temperature_choice_t;
/** Named raw values for WarningDisplayDisconnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_WARNING_DISPLAY_DISCONNECTED_NO_WARNING = 0,
    CAR_CAN_MIKA_STATUS_WARNING_DISPLAY_DISCONNECTED_WARNING = 1,
} car_can_mika_status_warning_display_disconnected_choice_t;
/** Named raw values for WarningRearCameraDisconnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_WARNING_REAR_CAMERA_DISCONNECTED_NO_WARNING = 0,
    CAR_CAN_MIKA_STATUS_WARNING_REAR_CAMERA_DISCONNECTED_WARNING = 1,
} car_can_mika_status_warning_rear_camera_disconnected_choice_t;
/** Named raw values for WarningRouterDisconnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_WARNING_ROUTER_DISCONNECTED_NO_WARNING = 0,
    CAR_CAN_MIKA_STATUS_WARNING_ROUTER_DISCONNECTED_WARNING = 1,
} car_can_mika_status_warning_router_disconnected_choice_t;
/** Named raw values for CellularConnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_CELLULAR_CONNECTED_DISCONNECTED = 0,
    CAR_CAN_MIKA_STATUS_CELLULAR_CONNECTED_CONNECTED = 1,
} car_can_mika_status_cellular_connected_choice_t;
/** Named raw values for InternetConnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_INTERNET_CONNECTED_DISCONNECTED = 0,
    CAR_CAN_MIKA_STATUS_INTERNET_CONNECTED_CONNECTED = 1,
} car_can_mika_status_internet_connected_choice_t;
/** Named raw values for GPSConnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_GPS_CONNECTED_DISCONNECTED = 0,
    CAR_CAN_MIKA_STATUS_GPS_CONNECTED_CONNECTED = 1,
} car_can_mika_status_gps_connected_choice_t;
/** Named raw values for DriverCameraConnected; unknown values remain integers. */
typedef enum {
    CAR_CAN_MIKA_STATUS_DRIVER_CAMERA_CONNECTED_DISCONNECTED = 0,
    CAR_CAN_MIKA_STATUS_DRIVER_CAMERA_CONNECTED_CONNECTED = 1,
} car_can_mika_status_driver_camera_connected_choice_t;
/** HMI and CM5 status from MIKA (combined HMI and telemetry board). Application ID 0x082 and signal layout retained. */
typedef struct {
    /** WarningTemperature; unit=none; physical=raw*1+0; raw=[0,1]. WarningTemperature */
    uint8_t warning_temperature;
    /** WarningOverTemperature; unit=none; physical=raw*1+0; raw=[0,1]. WarningOverTemperature */
    uint8_t warning_over_temperature;
    /** WarningDisplayDisconnected; unit=none; physical=raw*1+0; raw=[0,1]. WarningDisplayDisconnected */
    uint8_t warning_display_disconnected;
    /** WarningRearCameraDisconnected; unit=none; physical=raw*1+0; raw=[0,1]. Rear camera disconnected warning; the driver camera has a separate connected bit. */
    uint8_t warning_rear_camera_disconnected;
    /** WarningRouterDisconnected; unit=none; physical=raw*1+0; raw=[0,1]. WarningRouterDisconnected */
    uint8_t warning_router_disconnected;
    /** CellularConnected; unit=none; physical=raw*1+0; raw=[0,1]. CellularConnected */
    uint8_t cellular_connected;
    /** InternetConnected; unit=none; physical=raw*1+0; raw=[0,1]. InternetConnected */
    uint8_t internet_connected;
    /** GPSConnected; unit=none; physical=raw*1+0; raw=[0,1]. GPSConnected */
    uint8_t gps_connected;
    /** DriverCameraConnected; unit=none; physical=raw*1+0; raw=[0,1]. Driver camera connection state: 0 disconnected, 1 connected. */
    uint8_t driver_camera_connected;
    /** DisplayFrameRate; unit=FPS; physical=raw*1+0; raw=[0,60]. DisplayFrameRate */
    uint8_t display_frame_rate;
    /** SoCTemperature; unit=degC; physical=raw*0.5+-20; raw=[0,210]. User-approved explicit exception: uint8, 0.5 degC/bit, -20 degC offset. */
    double so_c_temperature;
    /** CM5FanTach; unit=rpm; physical=raw*5+0; raw=[0,4095]. CM5 cooling fan tachometer: unsigned 12 bits, 5 rpm/bit, offset 0 rpm. */
    uint16_t cm5_fan_tach;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_mika_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_mika_status_pack(can_frame_t* dst, const car_can_mika_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_mika_status_unpack(car_can_mika_status_t* dst, const can_frame_t* src);

/** Named raw values for WarningHSSOverCurrent; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSS_OVER_CURRENT_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSS_OVER_CURRENT_WARNING = 1,
} car_can_lvc_status_warning_hss_over_current_choice_t;
/** Named raw values for WarningHSSI2T; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSSI2_T_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSSI2_T_WARNING = 1,
} car_can_lvc_status_warning_hssi2_t_choice_t;
/** Named raw values for WarningHSSOverTemperature; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSS_OVER_TEMPERATURE_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSS_OVER_TEMPERATURE_WARNING = 1,
} car_can_lvc_status_warning_hss_over_temperature_choice_t;
/** Named raw values for WarningHSSOpenLoad; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSS_OPEN_LOAD_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSS_OPEN_LOAD_WARNING = 1,
} car_can_lvc_status_warning_hss_open_load_choice_t;
/** Named raw values for WarningHSSShortToBattery; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSS_SHORT_TO_BATTERY_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSS_SHORT_TO_BATTERY_WARNING = 1,
} car_can_lvc_status_warning_hss_short_to_battery_choice_t;
/** Named raw values for WarningHSSVBBUndervoltage; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSSVBB_UNDERVOLTAGE_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSSVBB_UNDERVOLTAGE_WARNING = 1,
} car_can_lvc_status_warning_hssvbb_undervoltage_choice_t;
/** Named raw values for WarningHSSSPIError; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSSSPI_ERROR_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSSSPI_ERROR_WARNING = 1,
} car_can_lvc_status_warning_hssspi_error_choice_t;
/** Named raw values for WarningHSSWatchdog; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_HSS_WATCHDOG_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_HSS_WATCHDOG_WARNING = 1,
} car_can_lvc_status_warning_hss_watchdog_choice_t;
/** Named raw values for WarningNoDCDCOutput; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_NO_DCDC_OUTPUT_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_NO_DCDC_OUTPUT_WARNING = 1,
} car_can_lvc_status_warning_no_dcdc_output_choice_t;
/** Named raw values for WarningBPSLVBlownFuse; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_WARNING_BPSLV_BLOWN_FUSE_NO_WARNING = 0,
    CAR_CAN_LVC_STATUS_WARNING_BPSLV_BLOWN_FUSE_WARNING = 1,
} car_can_lvc_status_warning_bpslv_blown_fuse_choice_t;
/** Named raw values for DCDCEnabled; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_DCDC_ENABLED_DISABLED = 0,
    CAR_CAN_LVC_STATUS_DCDC_ENABLED_ENABLED = 1,
} car_can_lvc_status_dcdc_enabled_choice_t;
/** Named raw values for BPSLVEnabled; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_STATUS_BPSLV_ENABLED_DISABLED = 0,
    CAR_CAN_LVC_STATUS_BPSLV_ENABLED_ENABLED = 1,
} car_can_lvc_status_bpslv_enabled_choice_t;
/** LVC warnings and discrete states only; no fault bits. HSS warnings describe the single BPS feed HSS. BPSLVBlownFuse uses separate hardware monitoring; the PDU feed is separate. */
typedef struct {
    /** WarningHSSOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. BPS HSS output OverCurrent warning; overtemperature combines absolute and relative thermal shutdown. */
    uint8_t warning_hss_over_current;
    /** WarningHSSI2T; unit=none; physical=raw*1+0; raw=[0,1]. BPS HSS output I2T warning; overtemperature combines absolute and relative thermal shutdown. */
    uint8_t warning_hssi2_t;
    /** WarningHSSOverTemperature; unit=none; physical=raw*1+0; raw=[0,1]. BPS HSS output OverTemperature warning; overtemperature combines absolute and relative thermal shutdown. */
    uint8_t warning_hss_over_temperature;
    /** WarningHSSOpenLoad; unit=none; physical=raw*1+0; raw=[0,1]. BPS HSS output OpenLoad warning; overtemperature combines absolute and relative thermal shutdown. */
    uint8_t warning_hss_open_load;
    /** WarningHSSShortToBattery; unit=none; physical=raw*1+0; raw=[0,1]. BPS HSS output ShortToBattery warning; overtemperature combines absolute and relative thermal shutdown. */
    uint8_t warning_hss_short_to_battery;
    /** WarningHSSVBBUndervoltage; unit=none; physical=raw*1+0; raw=[0,1]. LVC BPS HSS chip diagnostic: VBBUndervoltage. SPIError includes MCU-detected SPI/frame/CRC failures. */
    uint8_t warning_hssvbb_undervoltage;
    /** WarningHSSSPIError; unit=none; physical=raw*1+0; raw=[0,1]. LVC BPS HSS chip diagnostic: SPIError. SPIError includes MCU-detected SPI/frame/CRC failures. */
    uint8_t warning_hssspi_error;
    /** WarningHSSWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. LVC BPS HSS chip diagnostic: Watchdog. SPIError includes MCU-detected SPI/frame/CRC failures. */
    uint8_t warning_hss_watchdog;
    /** WarningNoDCDCOutput; unit=none; physical=raw*1+0; raw=[0,1]. DC-DC output unavailable. */
    uint8_t warning_no_dcdc_output;
    /** WarningBPSLVBlownFuse; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware fuse monitor on the BPS feed; independent of HSS open-load diagnostics. */
    uint8_t warning_bpslv_blown_fuse;
    /** DCDCEnabled; unit=none; physical=raw*1+0; raw=[0,1]. DC-DC enable state. */
    uint8_t dcdc_enabled;
    /** BPSLVEnabled; unit=none; physical=raw*1+0; raw=[0,1]. Requested enable state of BPS HSS output channel 1; not proof of output voltage. */
    uint8_t bpslv_enabled;
} car_can_lvc_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_lvc_status_pack(can_frame_t* dst, const car_can_lvc_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_lvc_status_unpack(car_can_lvc_status_t* dst, const can_frame_t* src);

/** 10 Hz measured LV currents. BPSLVCurrent belongs to the single populated HSS output; no fictitious PDU HSS channel is reported. */
typedef struct {
    /** DCDCCurrent; unit=A; physical=raw*0.01+0; raw=[0,16383]. Measured DCDCCurrent. */
    double dcdc_current;
    /** LVBCurrent; unit=A; physical=raw*0.01+-50; raw=[0,16383]. Measured LVBCurrent. Positive = battery discharge into LV supply; negative = battery charging. */
    double lvb_current;
    /** TotalOutputCurrent; unit=A; physical=raw*0.01+0; raw=[0,16383]. Measured TotalOutputCurrent. Independently measured total output, not a sum of HSS channels. The PDU feed is included only through this actual total measurement. */
    double total_output_current;
    /** BPSLVCurrent; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured BPSLVCurrent. */
    double bpslv_current;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_lvc_current_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_lvc_current_data_pack(can_frame_t* dst, const car_can_lvc_current_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_lvc_current_data_unpack(car_can_lvc_current_data_t* dst, const can_frame_t* src);

/** 2 Hz LV voltage telemetry. HSS-derived BPSLVVoltage can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
typedef struct {
    /** DCDCVoltage; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured DCDCVoltage; zero is representable to expose a powered-off output. */
    double dcdc_voltage;
    /** LVBVoltage; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured LVBVoltage; zero is representable to expose a powered-off output. */
    double lvb_voltage;
    /** PowerSupplyVoltage; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured PowerSupplyVoltage; zero is representable to expose a powered-off output. */
    double power_supply_voltage;
    /** BPSLVVoltage; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured BPSLVVoltage; zero is representable to expose a powered-off output. */
    double bpslv_voltage;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_lvc_voltage_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_lvc_voltage_data_pack(can_frame_t* dst, const car_can_lvc_voltage_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_lvc_voltage_data_unpack(car_can_lvc_voltage_data_t* dst, const can_frame_t* src);

/** 1 Hz BPS HSS FET temperature; temperature conversions can be suspended in applicable I2T modes; consult sample age and chip diagnostics. */
typedef struct {
    /** BPSFETTemperature; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Internal temperature of the BPS channel power FET, not load/ambient temperature. */
    double bpsfet_temperature;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_lvc_temperature_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_lvc_temperature_data_pack(can_frame_t* dst, const car_can_lvc_temperature_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_lvc_temperature_data_unpack(car_can_lvc_temperature_data_t* dst, const can_frame_t* src);

/** Named raw values for FaultOverCurrent; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_OVER_CURRENT_SNAPSHOT_FAULT_OVER_CURRENT_OK = 0,
    CAR_CAN_LVC_OVER_CURRENT_SNAPSHOT_FAULT_OVER_CURRENT_FAULT = 1,
} car_can_lvc_over_current_snapshot_fault_over_current_choice_t;
/** Named raw values for FaultI2T; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_OVER_CURRENT_SNAPSHOT_FAULT_I2_T_OK = 0,
    CAR_CAN_LVC_OVER_CURRENT_SNAPSHOT_FAULT_I2_T_FAULT = 1,
} car_can_lvc_over_current_snapshot_fault_i2_t_choice_t;
/** Named raw values for TripCurrentSaturated; unknown values remain integers. */
typedef enum {
    CAR_CAN_LVC_OVER_CURRENT_SNAPSHOT_TRIP_CURRENT_SATURATED_OFF = 0,
    CAR_CAN_LVC_OVER_CURRENT_SNAPSHOT_TRIP_CURRENT_SATURATED_ON = 1,
} car_can_lvc_over_current_snapshot_trip_current_saturated_choice_t;
/** Latch the first overcurrent or I2T trip per output until an explicit firmware fault-reset action. Freeze the last pre-trip current, its age at detection and the sending board monotonic uptime at detection. Reading diagnostic registers does not clear the telemetry latch. Send immediately on a new trip, then every 1000 ms while a trip remains latched. Never overwrite a captured current with post-shutdown zero. No snapshot/event identifier is sent. Capture time resets on board reboot. This records a sampled current, not a guaranteed instantaneous analog peak. */
typedef struct {
    /** FaultOverCurrent; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for BPS feed. */
    uint8_t fault_over_current;
    /** FaultI2T; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for BPS feed. */
    uint8_t fault_i2_t;
    /** TripCurrentSaturated; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated;
    /** CaptureTimeMs; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms;
    /** TripCurrent; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_lvc_over_current_snapshot_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_lvc_over_current_snapshot_pack(can_frame_t* dst, const car_can_lvc_over_current_snapshot_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_lvc_over_current_snapshot_unpack(car_can_lvc_over_current_snapshot_t* dst, const can_frame_t* src);

/** Named raw values for FaultChannelOverCurrent1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT1_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current1_choice_t;
/** Named raw values for FaultChannelI2T1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T1_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t1_choice_t;
/** Named raw values for FaultChannelOverTemperature1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE1_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature1_choice_t;
/** Named raw values for FaultChannelOpenLoad1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD1_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load1_choice_t;
/** Named raw values for FaultChannelShortToBattery1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY1_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery1_choice_t;
/** Named raw values for FaultChannelOverCurrent2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT2_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current2_choice_t;
/** Named raw values for FaultChannelI2T2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T2_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t2_choice_t;
/** Named raw values for FaultChannelOverTemperature2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE2_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature2_choice_t;
/** Named raw values for FaultChannelOpenLoad2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD2_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load2_choice_t;
/** Named raw values for FaultChannelShortToBattery2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY2_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery2_choice_t;
/** Named raw values for FaultChannelOverCurrent3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT3_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current3_choice_t;
/** Named raw values for FaultChannelI2T3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T3_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t3_choice_t;
/** Named raw values for FaultChannelOverTemperature3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE3_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature3_choice_t;
/** Named raw values for FaultChannelOpenLoad3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD3_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load3_choice_t;
/** Named raw values for FaultChannelShortToBattery3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY3_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery3_choice_t;
/** Named raw values for FaultChannelOverCurrent4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT4_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current4_choice_t;
/** Named raw values for FaultChannelI2T4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T4_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t4_choice_t;
/** Named raw values for FaultChannelOverTemperature4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE4_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature4_choice_t;
/** Named raw values for FaultChannelOpenLoad4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD4_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load4_choice_t;
/** Named raw values for FaultChannelShortToBattery4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY4_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery4_choice_t;
/** Named raw values for FaultChannelOverCurrent5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT5_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current5_choice_t;
/** Named raw values for FaultChannelI2T5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T5_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t5_choice_t;
/** Named raw values for FaultChannelOverTemperature5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE5_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature5_choice_t;
/** Named raw values for FaultChannelOpenLoad5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD5_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load5_choice_t;
/** Named raw values for FaultChannelShortToBattery5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY5_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery5_choice_t;
/** Named raw values for FaultChannelOverCurrent6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT6_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current6_choice_t;
/** Named raw values for FaultChannelI2T6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T6_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t6_choice_t;
/** Named raw values for FaultChannelOverTemperature6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE6_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature6_choice_t;
/** Named raw values for FaultChannelOpenLoad6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD6_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load6_choice_t;
/** Named raw values for FaultChannelShortToBattery6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY6_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery6_choice_t;
/** Named raw values for FaultChannelOverCurrent7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT7_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT7_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current7_choice_t;
/** Named raw values for FaultChannelI2T7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T7_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T7_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t7_choice_t;
/** Named raw values for FaultChannelOverTemperature7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE7_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE7_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature7_choice_t;
/** Named raw values for FaultChannelOpenLoad7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD7_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD7_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load7_choice_t;
/** Named raw values for FaultChannelShortToBattery7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY7_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY7_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery7_choice_t;
/** Named raw values for FaultChannelOverCurrent8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT8_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT8_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current8_choice_t;
/** Named raw values for FaultChannelI2T8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T8_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T8_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t8_choice_t;
/** Named raw values for FaultChannelOverTemperature8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE8_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE8_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature8_choice_t;
/** Named raw values for FaultChannelOpenLoad8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD8_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD8_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load8_choice_t;
/** Named raw values for FaultChannelShortToBattery8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY8_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY8_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery8_choice_t;
/** Named raw values for FaultChannelOverCurrent9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT9_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT9_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current9_choice_t;
/** Named raw values for FaultChannelI2T9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T9_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T9_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t9_choice_t;
/** Named raw values for FaultChannelOverTemperature9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE9_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE9_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature9_choice_t;
/** Named raw values for FaultChannelOpenLoad9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD9_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD9_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load9_choice_t;
/** Named raw values for FaultChannelShortToBattery9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY9_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY9_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery9_choice_t;
/** Named raw values for FaultChannelOverCurrent10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT10_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT10_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current10_choice_t;
/** Named raw values for FaultChannelI2T10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T10_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T10_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t10_choice_t;
/** Named raw values for FaultChannelOverTemperature10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE10_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE10_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature10_choice_t;
/** Named raw values for FaultChannelOpenLoad10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD10_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD10_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load10_choice_t;
/** Named raw values for FaultChannelShortToBattery10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY10_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY10_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery10_choice_t;
/** Named raw values for FaultChannelOverCurrent11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT11_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT11_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current11_choice_t;
/** Named raw values for FaultChannelI2T11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T11_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T11_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t11_choice_t;
/** Named raw values for FaultChannelOverTemperature11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE11_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE11_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature11_choice_t;
/** Named raw values for FaultChannelOpenLoad11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD11_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD11_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load11_choice_t;
/** Named raw values for FaultChannelShortToBattery11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY11_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY11_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery11_choice_t;
/** Named raw values for FaultChannelOverCurrent12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT12_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_CURRENT12_FAULT = 1,
} car_can_pdu_status_fault_channel_over_current12_choice_t;
/** Named raw values for FaultChannelI2T12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T12_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_I2_T12_FAULT = 1,
} car_can_pdu_status_fault_channel_i2_t12_choice_t;
/** Named raw values for FaultChannelOverTemperature12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE12_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OVER_TEMPERATURE12_FAULT = 1,
} car_can_pdu_status_fault_channel_over_temperature12_choice_t;
/** Named raw values for FaultChannelOpenLoad12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD12_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_OPEN_LOAD12_FAULT = 1,
} car_can_pdu_status_fault_channel_open_load12_choice_t;
/** Named raw values for FaultChannelShortToBattery12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY12_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHANNEL_SHORT_TO_BATTERY12_FAULT = 1,
} car_can_pdu_status_fault_channel_short_to_battery12_choice_t;
/** Named raw values for FaultChipVBBUndervoltage1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE1_FAULT = 1,
} car_can_pdu_status_fault_chip_vbb_undervoltage1_choice_t;
/** Named raw values for FaultChipSPIError1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR1_FAULT = 1,
} car_can_pdu_status_fault_chip_spi_error1_choice_t;
/** Named raw values for FaultChipWatchdog1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG1_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG1_FAULT = 1,
} car_can_pdu_status_fault_chip_watchdog1_choice_t;
/** Named raw values for FaultChipVBBUndervoltage2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE2_FAULT = 1,
} car_can_pdu_status_fault_chip_vbb_undervoltage2_choice_t;
/** Named raw values for FaultChipSPIError2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR2_FAULT = 1,
} car_can_pdu_status_fault_chip_spi_error2_choice_t;
/** Named raw values for FaultChipWatchdog2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG2_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG2_FAULT = 1,
} car_can_pdu_status_fault_chip_watchdog2_choice_t;
/** Named raw values for FaultChipVBBUndervoltage3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE3_FAULT = 1,
} car_can_pdu_status_fault_chip_vbb_undervoltage3_choice_t;
/** Named raw values for FaultChipSPIError3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR3_FAULT = 1,
} car_can_pdu_status_fault_chip_spi_error3_choice_t;
/** Named raw values for FaultChipWatchdog3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG3_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG3_FAULT = 1,
} car_can_pdu_status_fault_chip_watchdog3_choice_t;
/** Named raw values for FaultChipVBBUndervoltage4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE4_FAULT = 1,
} car_can_pdu_status_fault_chip_vbb_undervoltage4_choice_t;
/** Named raw values for FaultChipSPIError4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR4_FAULT = 1,
} car_can_pdu_status_fault_chip_spi_error4_choice_t;
/** Named raw values for FaultChipWatchdog4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG4_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG4_FAULT = 1,
} car_can_pdu_status_fault_chip_watchdog4_choice_t;
/** Named raw values for FaultChipVBBUndervoltage5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE5_FAULT = 1,
} car_can_pdu_status_fault_chip_vbb_undervoltage5_choice_t;
/** Named raw values for FaultChipSPIError5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR5_FAULT = 1,
} car_can_pdu_status_fault_chip_spi_error5_choice_t;
/** Named raw values for FaultChipWatchdog5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG5_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG5_FAULT = 1,
} car_can_pdu_status_fault_chip_watchdog5_choice_t;
/** Named raw values for FaultChipVBBUndervoltage6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_VBB_UNDERVOLTAGE6_FAULT = 1,
} car_can_pdu_status_fault_chip_vbb_undervoltage6_choice_t;
/** Named raw values for FaultChipSPIError6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_SPI_ERROR6_FAULT = 1,
} car_can_pdu_status_fault_chip_spi_error6_choice_t;
/** Named raw values for FaultChipWatchdog6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG6_OK = 0,
    CAR_CAN_PDU_STATUS_FAULT_CHIP_WATCHDOG6_FAULT = 1,
} car_can_pdu_status_fault_chip_watchdog6_choice_t;
/** Named raw values for WarningBlownFuse; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_BLOWN_FUSE_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_BLOWN_FUSE_WARNING = 1,
} car_can_pdu_status_warning_blown_fuse_choice_t;
/** Named raw values for WarningChannelBlownFuse1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE1_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE1_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse1_choice_t;
/** Named raw values for WarningChannelBlownFuse2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE2_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE2_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse2_choice_t;
/** Named raw values for WarningChannelBlownFuse3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE3_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE3_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse3_choice_t;
/** Named raw values for WarningChannelBlownFuse4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE4_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE4_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse4_choice_t;
/** Named raw values for WarningChannelBlownFuse5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE5_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE5_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse5_choice_t;
/** Named raw values for WarningChannelBlownFuse6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE6_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE6_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse6_choice_t;
/** Named raw values for WarningChannelBlownFuse7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE7_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE7_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse7_choice_t;
/** Named raw values for WarningChannelBlownFuse8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE8_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE8_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse8_choice_t;
/** Named raw values for WarningChannelBlownFuse9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE9_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE9_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse9_choice_t;
/** Named raw values for WarningChannelBlownFuse10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE10_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE10_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse10_choice_t;
/** Named raw values for WarningChannelBlownFuse11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE11_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE11_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse11_choice_t;
/** Named raw values for WarningChannelBlownFuse12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE12_NO_WARNING = 0,
    CAR_CAN_PDU_STATUS_WARNING_CHANNEL_BLOWN_FUSE12_WARNING = 1,
} car_can_pdu_status_warning_channel_blown_fuse12_choice_t;
/** Named raw values for ChannelEnabled1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED1_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED1_ENABLED = 1,
} car_can_pdu_status_channel_enabled1_choice_t;
/** Named raw values for ChannelEnabled2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED2_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED2_ENABLED = 1,
} car_can_pdu_status_channel_enabled2_choice_t;
/** Named raw values for ChannelEnabled3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED3_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED3_ENABLED = 1,
} car_can_pdu_status_channel_enabled3_choice_t;
/** Named raw values for ChannelEnabled4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED4_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED4_ENABLED = 1,
} car_can_pdu_status_channel_enabled4_choice_t;
/** Named raw values for ChannelEnabled5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED5_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED5_ENABLED = 1,
} car_can_pdu_status_channel_enabled5_choice_t;
/** Named raw values for ChannelEnabled6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED6_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED6_ENABLED = 1,
} car_can_pdu_status_channel_enabled6_choice_t;
/** Named raw values for ChannelEnabled7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED7_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED7_ENABLED = 1,
} car_can_pdu_status_channel_enabled7_choice_t;
/** Named raw values for ChannelEnabled8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED8_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED8_ENABLED = 1,
} car_can_pdu_status_channel_enabled8_choice_t;
/** Named raw values for ChannelEnabled9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED9_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED9_ENABLED = 1,
} car_can_pdu_status_channel_enabled9_choice_t;
/** Named raw values for ChannelEnabled10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED10_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED10_ENABLED = 1,
} car_can_pdu_status_channel_enabled10_choice_t;
/** Named raw values for ChannelEnabled11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED11_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED11_ENABLED = 1,
} car_can_pdu_status_channel_enabled11_choice_t;
/** Named raw values for ChannelEnabled12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED12_DISABLED = 0,
    CAR_CAN_PDU_STATUS_CHANNEL_ENABLED12_ENABLED = 1,
} car_can_pdu_status_channel_enabled12_choice_t;
/** 12 outputs, 6 TPS2HCS08A dual-channel chips. Faults -> warnings -> discrete states. Chip n owns channels 2n-1/2n. Per-channel fuse warnings come from separate hardware. No continuous measurements. */
typedef struct {
    /** FaultChannelOverCurrent1; unit=none; physical=raw*1+0; raw=[0,1]. Channel 1, HSS chip 1, output 1: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current1;
    /** FaultChannelI2T1; unit=none; physical=raw*1+0; raw=[0,1]. Channel 1, HSS chip 1, output 1: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t1;
    /** FaultChannelOverTemperature1; unit=none; physical=raw*1+0; raw=[0,1]. Channel 1, HSS chip 1, output 1: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature1;
    /** FaultChannelOpenLoad1; unit=none; physical=raw*1+0; raw=[0,1]. Channel 1, HSS chip 1, output 1: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load1;
    /** FaultChannelShortToBattery1; unit=none; physical=raw*1+0; raw=[0,1]. Channel 1, HSS chip 1, output 1: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery1;
    /** FaultChannelOverCurrent2; unit=none; physical=raw*1+0; raw=[0,1]. Channel 2, HSS chip 1, output 2: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current2;
    /** FaultChannelI2T2; unit=none; physical=raw*1+0; raw=[0,1]. Channel 2, HSS chip 1, output 2: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t2;
    /** FaultChannelOverTemperature2; unit=none; physical=raw*1+0; raw=[0,1]. Channel 2, HSS chip 1, output 2: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature2;
    /** FaultChannelOpenLoad2; unit=none; physical=raw*1+0; raw=[0,1]. Channel 2, HSS chip 1, output 2: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load2;
    /** FaultChannelShortToBattery2; unit=none; physical=raw*1+0; raw=[0,1]. Channel 2, HSS chip 1, output 2: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery2;
    /** FaultChannelOverCurrent3; unit=none; physical=raw*1+0; raw=[0,1]. Channel 3, HSS chip 2, output 1: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current3;
    /** FaultChannelI2T3; unit=none; physical=raw*1+0; raw=[0,1]. Channel 3, HSS chip 2, output 1: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t3;
    /** FaultChannelOverTemperature3; unit=none; physical=raw*1+0; raw=[0,1]. Channel 3, HSS chip 2, output 1: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature3;
    /** FaultChannelOpenLoad3; unit=none; physical=raw*1+0; raw=[0,1]. Channel 3, HSS chip 2, output 1: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load3;
    /** FaultChannelShortToBattery3; unit=none; physical=raw*1+0; raw=[0,1]. Channel 3, HSS chip 2, output 1: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery3;
    /** FaultChannelOverCurrent4; unit=none; physical=raw*1+0; raw=[0,1]. Channel 4, HSS chip 2, output 2: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current4;
    /** FaultChannelI2T4; unit=none; physical=raw*1+0; raw=[0,1]. Channel 4, HSS chip 2, output 2: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t4;
    /** FaultChannelOverTemperature4; unit=none; physical=raw*1+0; raw=[0,1]. Channel 4, HSS chip 2, output 2: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature4;
    /** FaultChannelOpenLoad4; unit=none; physical=raw*1+0; raw=[0,1]. Channel 4, HSS chip 2, output 2: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load4;
    /** FaultChannelShortToBattery4; unit=none; physical=raw*1+0; raw=[0,1]. Channel 4, HSS chip 2, output 2: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery4;
    /** FaultChannelOverCurrent5; unit=none; physical=raw*1+0; raw=[0,1]. Channel 5, HSS chip 3, output 1: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current5;
    /** FaultChannelI2T5; unit=none; physical=raw*1+0; raw=[0,1]. Channel 5, HSS chip 3, output 1: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t5;
    /** FaultChannelOverTemperature5; unit=none; physical=raw*1+0; raw=[0,1]. Channel 5, HSS chip 3, output 1: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature5;
    /** FaultChannelOpenLoad5; unit=none; physical=raw*1+0; raw=[0,1]. Channel 5, HSS chip 3, output 1: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load5;
    /** FaultChannelShortToBattery5; unit=none; physical=raw*1+0; raw=[0,1]. Channel 5, HSS chip 3, output 1: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery5;
    /** FaultChannelOverCurrent6; unit=none; physical=raw*1+0; raw=[0,1]. Channel 6, HSS chip 3, output 2: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current6;
    /** FaultChannelI2T6; unit=none; physical=raw*1+0; raw=[0,1]. Channel 6, HSS chip 3, output 2: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t6;
    /** FaultChannelOverTemperature6; unit=none; physical=raw*1+0; raw=[0,1]. Channel 6, HSS chip 3, output 2: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature6;
    /** FaultChannelOpenLoad6; unit=none; physical=raw*1+0; raw=[0,1]. Channel 6, HSS chip 3, output 2: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load6;
    /** FaultChannelShortToBattery6; unit=none; physical=raw*1+0; raw=[0,1]. Channel 6, HSS chip 3, output 2: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery6;
    /** FaultChannelOverCurrent7; unit=none; physical=raw*1+0; raw=[0,1]. Channel 7, HSS chip 4, output 1: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current7;
    /** FaultChannelI2T7; unit=none; physical=raw*1+0; raw=[0,1]. Channel 7, HSS chip 4, output 1: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t7;
    /** FaultChannelOverTemperature7; unit=none; physical=raw*1+0; raw=[0,1]. Channel 7, HSS chip 4, output 1: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature7;
    /** FaultChannelOpenLoad7; unit=none; physical=raw*1+0; raw=[0,1]. Channel 7, HSS chip 4, output 1: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load7;
    /** FaultChannelShortToBattery7; unit=none; physical=raw*1+0; raw=[0,1]. Channel 7, HSS chip 4, output 1: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery7;
    /** FaultChannelOverCurrent8; unit=none; physical=raw*1+0; raw=[0,1]. Channel 8, HSS chip 4, output 2: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current8;
    /** FaultChannelI2T8; unit=none; physical=raw*1+0; raw=[0,1]. Channel 8, HSS chip 4, output 2: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t8;
    /** FaultChannelOverTemperature8; unit=none; physical=raw*1+0; raw=[0,1]. Channel 8, HSS chip 4, output 2: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature8;
    /** FaultChannelOpenLoad8; unit=none; physical=raw*1+0; raw=[0,1]. Channel 8, HSS chip 4, output 2: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load8;
    /** FaultChannelShortToBattery8; unit=none; physical=raw*1+0; raw=[0,1]. Channel 8, HSS chip 4, output 2: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery8;
    /** FaultChannelOverCurrent9; unit=none; physical=raw*1+0; raw=[0,1]. Channel 9, HSS chip 5, output 1: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current9;
    /** FaultChannelI2T9; unit=none; physical=raw*1+0; raw=[0,1]. Channel 9, HSS chip 5, output 1: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t9;
    /** FaultChannelOverTemperature9; unit=none; physical=raw*1+0; raw=[0,1]. Channel 9, HSS chip 5, output 1: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature9;
    /** FaultChannelOpenLoad9; unit=none; physical=raw*1+0; raw=[0,1]. Channel 9, HSS chip 5, output 1: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load9;
    /** FaultChannelShortToBattery9; unit=none; physical=raw*1+0; raw=[0,1]. Channel 9, HSS chip 5, output 1: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery9;
    /** FaultChannelOverCurrent10; unit=none; physical=raw*1+0; raw=[0,1]. Channel 10, HSS chip 5, output 2: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current10;
    /** FaultChannelI2T10; unit=none; physical=raw*1+0; raw=[0,1]. Channel 10, HSS chip 5, output 2: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t10;
    /** FaultChannelOverTemperature10; unit=none; physical=raw*1+0; raw=[0,1]. Channel 10, HSS chip 5, output 2: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature10;
    /** FaultChannelOpenLoad10; unit=none; physical=raw*1+0; raw=[0,1]. Channel 10, HSS chip 5, output 2: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load10;
    /** FaultChannelShortToBattery10; unit=none; physical=raw*1+0; raw=[0,1]. Channel 10, HSS chip 5, output 2: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery10;
    /** FaultChannelOverCurrent11; unit=none; physical=raw*1+0; raw=[0,1]. Channel 11, HSS chip 6, output 1: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current11;
    /** FaultChannelI2T11; unit=none; physical=raw*1+0; raw=[0,1]. Channel 11, HSS chip 6, output 1: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t11;
    /** FaultChannelOverTemperature11; unit=none; physical=raw*1+0; raw=[0,1]. Channel 11, HSS chip 6, output 1: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature11;
    /** FaultChannelOpenLoad11; unit=none; physical=raw*1+0; raw=[0,1]. Channel 11, HSS chip 6, output 1: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load11;
    /** FaultChannelShortToBattery11; unit=none; physical=raw*1+0; raw=[0,1]. Channel 11, HSS chip 6, output 1: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery11;
    /** FaultChannelOverCurrent12; unit=none; physical=raw*1+0; raw=[0,1]. Channel 12, HSS chip 6, output 2: OverCurrent. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_current12;
    /** FaultChannelI2T12; unit=none; physical=raw*1+0; raw=[0,1]. Channel 12, HSS chip 6, output 2: I2T. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_i2_t12;
    /** FaultChannelOverTemperature12; unit=none; physical=raw*1+0; raw=[0,1]. Channel 12, HSS chip 6, output 2: OverTemperature. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_over_temperature12;
    /** FaultChannelOpenLoad12; unit=none; physical=raw*1+0; raw=[0,1]. Channel 12, HSS chip 6, output 2: OpenLoad. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_open_load12;
    /** FaultChannelShortToBattery12; unit=none; physical=raw*1+0; raw=[0,1]. Channel 12, HSS chip 6, output 2: ShortToBattery. OverTemperature combines absolute and relative thermal shutdown. */
    uint8_t fault_channel_short_to_battery12;
    /** FaultChipVBBUndervoltage1; unit=none; physical=raw*1+0; raw=[0,1]. Chip 1, shared by channels 1/2. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_vbb_undervoltage1;
    /** FaultChipSPIError1; unit=none; physical=raw*1+0; raw=[0,1]. Chip 1, shared by channels 1/2. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_spi_error1;
    /** FaultChipWatchdog1; unit=none; physical=raw*1+0; raw=[0,1]. Chip 1, shared by channels 1/2. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_watchdog1;
    /** FaultChipVBBUndervoltage2; unit=none; physical=raw*1+0; raw=[0,1]. Chip 2, shared by channels 3/4. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_vbb_undervoltage2;
    /** FaultChipSPIError2; unit=none; physical=raw*1+0; raw=[0,1]. Chip 2, shared by channels 3/4. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_spi_error2;
    /** FaultChipWatchdog2; unit=none; physical=raw*1+0; raw=[0,1]. Chip 2, shared by channels 3/4. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_watchdog2;
    /** FaultChipVBBUndervoltage3; unit=none; physical=raw*1+0; raw=[0,1]. Chip 3, shared by channels 5/6. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_vbb_undervoltage3;
    /** FaultChipSPIError3; unit=none; physical=raw*1+0; raw=[0,1]. Chip 3, shared by channels 5/6. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_spi_error3;
    /** FaultChipWatchdog3; unit=none; physical=raw*1+0; raw=[0,1]. Chip 3, shared by channels 5/6. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_watchdog3;
    /** FaultChipVBBUndervoltage4; unit=none; physical=raw*1+0; raw=[0,1]. Chip 4, shared by channels 7/8. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_vbb_undervoltage4;
    /** FaultChipSPIError4; unit=none; physical=raw*1+0; raw=[0,1]. Chip 4, shared by channels 7/8. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_spi_error4;
    /** FaultChipWatchdog4; unit=none; physical=raw*1+0; raw=[0,1]. Chip 4, shared by channels 7/8. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_watchdog4;
    /** FaultChipVBBUndervoltage5; unit=none; physical=raw*1+0; raw=[0,1]. Chip 5, shared by channels 9/10. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_vbb_undervoltage5;
    /** FaultChipSPIError5; unit=none; physical=raw*1+0; raw=[0,1]. Chip 5, shared by channels 9/10. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_spi_error5;
    /** FaultChipWatchdog5; unit=none; physical=raw*1+0; raw=[0,1]. Chip 5, shared by channels 9/10. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_watchdog5;
    /** FaultChipVBBUndervoltage6; unit=none; physical=raw*1+0; raw=[0,1]. Chip 6, shared by channels 11/12. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_vbb_undervoltage6;
    /** FaultChipSPIError6; unit=none; physical=raw*1+0; raw=[0,1]. Chip 6, shared by channels 11/12. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_spi_error6;
    /** FaultChipWatchdog6; unit=none; physical=raw*1+0; raw=[0,1]. Chip 6, shared by channels 11/12. SPIError includes MCU-detected SPI/frame/CRC errors. */
    uint8_t fault_chip_watchdog6;
    /** WarningBlownFuse; unit=none; physical=raw*1+0; raw=[0,1]. Aggregate OR of WarningChannelBlownFuse1..12 from the separate hardware channel monitors. */
    uint8_t warning_blown_fuse;
    /** WarningChannelBlownFuse1; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 1; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse1;
    /** WarningChannelBlownFuse2; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 2; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse2;
    /** WarningChannelBlownFuse3; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 3; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse3;
    /** WarningChannelBlownFuse4; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 4; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse4;
    /** WarningChannelBlownFuse5; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 5; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse5;
    /** WarningChannelBlownFuse6; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 6; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse6;
    /** WarningChannelBlownFuse7; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 7; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse7;
    /** WarningChannelBlownFuse8; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 8; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse8;
    /** WarningChannelBlownFuse9; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 9; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse9;
    /** WarningChannelBlownFuse10; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 10; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse10;
    /** WarningChannelBlownFuse11; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 11; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse11;
    /** WarningChannelBlownFuse12; unit=none; physical=raw*1+0; raw=[0,1]. Separate hardware blown-fuse monitor for channel 12; not inferred from HSS open-load fault. */
    uint8_t warning_channel_blown_fuse12;
    /** ChannelEnabled1; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 1 enable state; not proof of output voltage. */
    uint8_t channel_enabled1;
    /** ChannelEnabled2; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 2 enable state; not proof of output voltage. */
    uint8_t channel_enabled2;
    /** ChannelEnabled3; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 3 enable state; not proof of output voltage. */
    uint8_t channel_enabled3;
    /** ChannelEnabled4; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 4 enable state; not proof of output voltage. */
    uint8_t channel_enabled4;
    /** ChannelEnabled5; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 5 enable state; not proof of output voltage. */
    uint8_t channel_enabled5;
    /** ChannelEnabled6; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 6 enable state; not proof of output voltage. */
    uint8_t channel_enabled6;
    /** ChannelEnabled7; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 7 enable state; not proof of output voltage. */
    uint8_t channel_enabled7;
    /** ChannelEnabled8; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 8 enable state; not proof of output voltage. */
    uint8_t channel_enabled8;
    /** ChannelEnabled9; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 9 enable state; not proof of output voltage. */
    uint8_t channel_enabled9;
    /** ChannelEnabled10; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 10 enable state; not proof of output voltage. */
    uint8_t channel_enabled10;
    /** ChannelEnabled11; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 11 enable state; not proof of output voltage. */
    uint8_t channel_enabled11;
    /** ChannelEnabled12; unit=none; physical=raw*1+0; raw=[0,1]. Requested channel 12 enable state; not proof of output voltage. */
    uint8_t channel_enabled12;
} car_can_pdu_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pdu_status_pack(can_frame_t* dst, const car_can_pdu_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pdu_status_unpack(car_can_pdu_status_t* dst, const can_frame_t* src);

/** 10 Hz channel and total current measurements. Independent input measurement can include board consumption; do not label a computed channel sum as TotalCurrent. */
typedef struct {
    /** ChannelCurrent1; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 1; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current1;
    /** ChannelCurrent2; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 2; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current2;
    /** ChannelCurrent3; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 3; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current3;
    /** ChannelCurrent4; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 4; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current4;
    /** ChannelCurrent5; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 5; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current5;
    /** ChannelCurrent6; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 6; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current6;
    /** ChannelCurrent7; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 7; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current7;
    /** ChannelCurrent8; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 8; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current8;
    /** ChannelCurrent9; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 9; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current9;
    /** ChannelCurrent10; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 10; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current10;
    /** ChannelCurrent11; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 11; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current11;
    /** ChannelCurrent12; unit=A; physical=raw*0.01+0; raw=[0,4095]. Measured HSS output current for channel 12; See chip SPI/watchdog faults and sample age when interpreting this reading. */
    double channel_current12;
    /** TotalCurrent; unit=A; physical=raw*0.01+0; raw=[0,16383]. Independently measured PDU input/total current, not a calculated channel sum. */
    double total_current;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_pdu_current_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pdu_current_data_pack(can_frame_t* dst, const car_can_pdu_current_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pdu_current_data_unpack(car_can_pdu_current_data_t* dst, const can_frame_t* src);

/** 2 Hz input and per-channel output voltages, including 0 V when disabled. No per-reading validity bits are transmitted. */
typedef struct {
    /** InputVoltage; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured InputVoltage. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double input_voltage;
    /** ChannelVoltage1; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage1. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage1;
    /** ChannelVoltage2; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage2. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage2;
    /** ChannelVoltage3; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage3. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage3;
    /** ChannelVoltage4; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage4. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage4;
    /** ChannelVoltage5; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage5. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage5;
    /** ChannelVoltage6; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage6. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage6;
    /** ChannelVoltage7; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage7. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage7;
    /** ChannelVoltage8; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage8. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage8;
    /** ChannelVoltage9; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage9. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage9;
    /** ChannelVoltage10; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage10. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage10;
    /** ChannelVoltage11; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage11. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage11;
    /** ChannelVoltage12; unit=V; physical=raw*0.05+0; raw=[0,1023]. Measured ChannelVoltage12. HSS-derived readings can stop updating when ADC conversion is suspended; consult sample age and chip diagnostics. */
    double channel_voltage12;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_pdu_voltage_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pdu_voltage_data_pack(can_frame_t* dst, const car_can_pdu_voltage_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pdu_voltage_data_unpack(car_can_pdu_voltage_data_t* dst, const can_frame_t* src);

/** 1 Hz per-channel FET temperatures; conversions may pause in some I2T modes; consult sample age and chip diagnostics. */
typedef struct {
    /** ChannelTemperature1; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 1 internal power-FET temperature, not load temperature. */
    double channel_temperature1;
    /** ChannelTemperature2; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 2 internal power-FET temperature, not load temperature. */
    double channel_temperature2;
    /** ChannelTemperature3; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 3 internal power-FET temperature, not load temperature. */
    double channel_temperature3;
    /** ChannelTemperature4; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 4 internal power-FET temperature, not load temperature. */
    double channel_temperature4;
    /** ChannelTemperature5; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 5 internal power-FET temperature, not load temperature. */
    double channel_temperature5;
    /** ChannelTemperature6; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 6 internal power-FET temperature, not load temperature. */
    double channel_temperature6;
    /** ChannelTemperature7; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 7 internal power-FET temperature, not load temperature. */
    double channel_temperature7;
    /** ChannelTemperature8; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 8 internal power-FET temperature, not load temperature. */
    double channel_temperature8;
    /** ChannelTemperature9; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 9 internal power-FET temperature, not load temperature. */
    double channel_temperature9;
    /** ChannelTemperature10; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 10 internal power-FET temperature, not load temperature. */
    double channel_temperature10;
    /** ChannelTemperature11; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 11 internal power-FET temperature, not load temperature. */
    double channel_temperature11;
    /** ChannelTemperature12; unit=degC; physical=raw*0.05+-40; raw=[0,4800]. Channel 12 internal power-FET temperature, not load temperature. */
    double channel_temperature12;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_pdu_temperature_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pdu_temperature_data_pack(can_frame_t* dst, const car_can_pdu_temperature_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pdu_temperature_data_unpack(car_can_pdu_temperature_data_t* dst, const can_frame_t* src);

/** Named raw values for FaultOverCurrent1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT1_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT1_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_over_current1_choice_t;
/** Named raw values for FaultOverCurrent2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT2_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT2_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_over_current2_choice_t;
/** Named raw values for FaultOverCurrent3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT3_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT3_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_over_current3_choice_t;
/** Named raw values for FaultOverCurrent4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT4_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT4_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_over_current4_choice_t;
/** Named raw values for FaultOverCurrent5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT5_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT5_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_over_current5_choice_t;
/** Named raw values for FaultOverCurrent6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT6_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_OVER_CURRENT6_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_over_current6_choice_t;
/** Named raw values for FaultI2T1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T1_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T1_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_i2_t1_choice_t;
/** Named raw values for FaultI2T2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T2_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T2_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_i2_t2_choice_t;
/** Named raw values for FaultI2T3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T3_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T3_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_i2_t3_choice_t;
/** Named raw values for FaultI2T4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T4_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T4_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_i2_t4_choice_t;
/** Named raw values for FaultI2T5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T5_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T5_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_i2_t5_choice_t;
/** Named raw values for FaultI2T6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T6_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_FAULT_I2_T6_FAULT = 1,
} car_can_pdu_over_current_snapshot_a_fault_i2_t6_choice_t;
/** Named raw values for TripCurrentSaturated1; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED1_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED1_ON = 1,
} car_can_pdu_over_current_snapshot_a_trip_current_saturated1_choice_t;
/** Named raw values for TripCurrentSaturated2; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED2_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED2_ON = 1,
} car_can_pdu_over_current_snapshot_a_trip_current_saturated2_choice_t;
/** Named raw values for TripCurrentSaturated3; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED3_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED3_ON = 1,
} car_can_pdu_over_current_snapshot_a_trip_current_saturated3_choice_t;
/** Named raw values for TripCurrentSaturated4; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED4_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED4_ON = 1,
} car_can_pdu_over_current_snapshot_a_trip_current_saturated4_choice_t;
/** Named raw values for TripCurrentSaturated5; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED5_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED5_ON = 1,
} car_can_pdu_over_current_snapshot_a_trip_current_saturated5_choice_t;
/** Named raw values for TripCurrentSaturated6; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED6_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_A_TRIP_CURRENT_SATURATED6_ON = 1,
} car_can_pdu_over_current_snapshot_a_trip_current_saturated6_choice_t;
/** Latch the first overcurrent or I2T trip per output until an explicit firmware fault-reset action. Freeze the last pre-trip current, its age at detection and the sending board monotonic uptime at detection. Reading diagnostic registers does not clear the telemetry latch. Send immediately on a new trip, then every 1000 ms while a trip remains latched. Never overwrite a captured current with post-shutdown zero. No snapshot/event identifier is sent. Capture time resets on board reboot. This records a sampled current, not a guaranteed instantaneous analog peak. Each output retains its own first trip; a later trip on another channel never replaces existing channel records. */
typedef struct {
    /** FaultOverCurrent1; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 1. */
    uint8_t fault_over_current1;
    /** FaultOverCurrent2; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 2. */
    uint8_t fault_over_current2;
    /** FaultOverCurrent3; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 3. */
    uint8_t fault_over_current3;
    /** FaultOverCurrent4; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 4. */
    uint8_t fault_over_current4;
    /** FaultOverCurrent5; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 5. */
    uint8_t fault_over_current5;
    /** FaultOverCurrent6; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 6. */
    uint8_t fault_over_current6;
    /** FaultI2T1; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 1. */
    uint8_t fault_i2_t1;
    /** FaultI2T2; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 2. */
    uint8_t fault_i2_t2;
    /** FaultI2T3; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 3. */
    uint8_t fault_i2_t3;
    /** FaultI2T4; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 4. */
    uint8_t fault_i2_t4;
    /** FaultI2T5; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 5. */
    uint8_t fault_i2_t5;
    /** FaultI2T6; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 6. */
    uint8_t fault_i2_t6;
    /** TripCurrentSaturated1; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated1;
    /** TripCurrentSaturated2; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated2;
    /** TripCurrentSaturated3; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated3;
    /** TripCurrentSaturated4; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated4;
    /** TripCurrentSaturated5; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated5;
    /** TripCurrentSaturated6; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated6;
    /** CaptureTimeMs1; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms1;
    /** CurrentSampleAgeMs1; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms1;
    /** TripCurrent1; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current1;
    /** CaptureTimeMs2; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms2;
    /** CurrentSampleAgeMs2; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms2;
    /** TripCurrent2; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current2;
    /** CaptureTimeMs3; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms3;
    /** CurrentSampleAgeMs3; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms3;
    /** TripCurrent3; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current3;
    /** CaptureTimeMs4; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms4;
    /** CurrentSampleAgeMs4; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms4;
    /** TripCurrent4; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current4;
    /** CaptureTimeMs5; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms5;
    /** CurrentSampleAgeMs5; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms5;
    /** TripCurrent5; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current5;
    /** CaptureTimeMs6; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms6;
    /** CurrentSampleAgeMs6; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms6;
    /** TripCurrent6; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current6;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_pdu_over_current_snapshot_a_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pdu_over_current_snapshot_a_pack(can_frame_t* dst, const car_can_pdu_over_current_snapshot_a_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pdu_over_current_snapshot_a_unpack(car_can_pdu_over_current_snapshot_a_t* dst, const can_frame_t* src);

/** Named raw values for FaultOverCurrent7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT7_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT7_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_over_current7_choice_t;
/** Named raw values for FaultOverCurrent8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT8_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT8_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_over_current8_choice_t;
/** Named raw values for FaultOverCurrent9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT9_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT9_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_over_current9_choice_t;
/** Named raw values for FaultOverCurrent10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT10_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT10_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_over_current10_choice_t;
/** Named raw values for FaultOverCurrent11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT11_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT11_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_over_current11_choice_t;
/** Named raw values for FaultOverCurrent12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT12_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_OVER_CURRENT12_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_over_current12_choice_t;
/** Named raw values for FaultI2T7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T7_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T7_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_i2_t7_choice_t;
/** Named raw values for FaultI2T8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T8_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T8_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_i2_t8_choice_t;
/** Named raw values for FaultI2T9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T9_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T9_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_i2_t9_choice_t;
/** Named raw values for FaultI2T10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T10_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T10_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_i2_t10_choice_t;
/** Named raw values for FaultI2T11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T11_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T11_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_i2_t11_choice_t;
/** Named raw values for FaultI2T12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T12_OK = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_FAULT_I2_T12_FAULT = 1,
} car_can_pdu_over_current_snapshot_b_fault_i2_t12_choice_t;
/** Named raw values for TripCurrentSaturated7; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED7_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED7_ON = 1,
} car_can_pdu_over_current_snapshot_b_trip_current_saturated7_choice_t;
/** Named raw values for TripCurrentSaturated8; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED8_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED8_ON = 1,
} car_can_pdu_over_current_snapshot_b_trip_current_saturated8_choice_t;
/** Named raw values for TripCurrentSaturated9; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED9_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED9_ON = 1,
} car_can_pdu_over_current_snapshot_b_trip_current_saturated9_choice_t;
/** Named raw values for TripCurrentSaturated10; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED10_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED10_ON = 1,
} car_can_pdu_over_current_snapshot_b_trip_current_saturated10_choice_t;
/** Named raw values for TripCurrentSaturated11; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED11_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED11_ON = 1,
} car_can_pdu_over_current_snapshot_b_trip_current_saturated11_choice_t;
/** Named raw values for TripCurrentSaturated12; unknown values remain integers. */
typedef enum {
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED12_OFF = 0,
    CAR_CAN_PDU_OVER_CURRENT_SNAPSHOT_B_TRIP_CURRENT_SATURATED12_ON = 1,
} car_can_pdu_over_current_snapshot_b_trip_current_saturated12_choice_t;
/** Latch the first overcurrent or I2T trip per output until an explicit firmware fault-reset action. Freeze the last pre-trip current, its age at detection and the sending board monotonic uptime at detection. Reading diagnostic registers does not clear the telemetry latch. Send immediately on a new trip, then every 1000 ms while a trip remains latched. Never overwrite a captured current with post-shutdown zero. No snapshot/event identifier is sent. Capture time resets on board reboot. This records a sampled current, not a guaranteed instantaneous analog peak. Each output retains its own first trip; a later trip on another channel never replaces existing channel records. */
typedef struct {
    /** FaultOverCurrent7; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 7. */
    uint8_t fault_over_current7;
    /** FaultOverCurrent8; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 8. */
    uint8_t fault_over_current8;
    /** FaultOverCurrent9; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 9. */
    uint8_t fault_over_current9;
    /** FaultOverCurrent10; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 10. */
    uint8_t fault_over_current10;
    /** FaultOverCurrent11; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 11. */
    uint8_t fault_over_current11;
    /** FaultOverCurrent12; unit=none; physical=raw*1+0; raw=[0,1]. Latched OverCurrent trip for channel 12. */
    uint8_t fault_over_current12;
    /** FaultI2T7; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 7. */
    uint8_t fault_i2_t7;
    /** FaultI2T8; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 8. */
    uint8_t fault_i2_t8;
    /** FaultI2T9; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 9. */
    uint8_t fault_i2_t9;
    /** FaultI2T10; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 10. */
    uint8_t fault_i2_t10;
    /** FaultI2T11; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 11. */
    uint8_t fault_i2_t11;
    /** FaultI2T12; unit=none; physical=raw*1+0; raw=[0,1]. Latched I2T trip for channel 12. */
    uint8_t fault_i2_t12;
    /** TripCurrentSaturated7; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated7;
    /** TripCurrentSaturated8; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated8;
    /** TripCurrentSaturated9; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated9;
    /** TripCurrentSaturated10; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated10;
    /** TripCurrentSaturated11; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated11;
    /** TripCurrentSaturated12; unit=none; physical=raw*1+0; raw=[0,1]. 1 if the captured current is clipped or the sensing path was saturated; the current is then a lower bound, not an exact peak. */
    uint8_t trip_current_saturated12;
    /** CaptureTimeMs7; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms7;
    /** CurrentSampleAgeMs7; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms7;
    /** TripCurrent7; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current7;
    /** CaptureTimeMs8; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms8;
    /** CurrentSampleAgeMs8; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms8;
    /** TripCurrent8; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current8;
    /** CaptureTimeMs9; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms9;
    /** CurrentSampleAgeMs9; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms9;
    /** TripCurrent9; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current9;
    /** CaptureTimeMs10; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms10;
    /** CurrentSampleAgeMs10; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms10;
    /** TripCurrent10; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current10;
    /** CaptureTimeMs11; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms11;
    /** CurrentSampleAgeMs11; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms11;
    /** TripCurrent11; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current11;
    /** CaptureTimeMs12; unit=ms; physical=raw*1+0; raw=[0,4294967295]. Board uptime at the first latched trip; wraps at 2^32 ms. Interpret only while a trip flag is set. */
    uint32_t capture_time_ms12;
    /** CurrentSampleAgeMs12; unit=ms; physical=raw*1+0; raw=[0,65535]. Age of the saved current sample when this channel tripped; frozen, saturates at 65535 ms. */
    uint16_t current_sample_age_ms12;
    /** TripCurrent12; unit=A; physical=raw*0.01+0; raw=[0,65535]. Captured current; independent record per output; saturation marks a lower bound. */
    double trip_current12;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} car_can_pdu_over_current_snapshot_b_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t car_can_pdu_over_current_snapshot_b_pack(can_frame_t* dst, const car_can_pdu_over_current_snapshot_b_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t car_can_pdu_over_current_snapshot_b_unpack(car_can_pdu_over_current_snapshot_b_t* dst, const can_frame_t* src);

typedef struct {
    car_can_bps_status_t bps_status;
    car_can_bps_pack_data_t bps_pack_data;
    car_can_bps_voltage_data_t bps_voltage_data;
    car_can_bps_temperature_data_t bps_temperature_data;
    car_can_smd_status_t smd_status;
    car_can_bps_command_echo_t bps_command_echo;
    car_can_bps_fault_snapshot_t bps_fault_snapshot;
    car_can_bps_fault_voltage_snapshot_t bps_fault_voltage_snapshot;
    car_can_bps_fault_temperature_snapshot_t bps_fault_temperature_snapshot;
} car_can_bps_t;

typedef struct {
    car_can_dei_status_t dei_status;
} car_can_dei_t;

typedef struct {
    car_can_bps_command_t bps_command;
    car_can_pdu_command_t pdu_command;
    car_can_motor_controller_command_t motor_controller_command;
    car_can_sweep_mpp_ts_t sweep_mpp_ts;
} car_can_external_t;

typedef struct {
    car_can_lvc_status_t lvc_status;
    car_can_lvc_current_data_t lvc_current_data;
    car_can_lvc_voltage_data_t lvc_voltage_data;
    car_can_lvc_temperature_data_t lvc_temperature_data;
    car_can_lvc_over_current_snapshot_t lvc_over_current_snapshot;
} car_can_lvc_t;

typedef struct {
    car_can_mika_status_t mika_status;
} car_can_mika_t;

typedef struct {
    car_can_mppt_interface_status_t mppt_interface_status;
} car_can_mppt_interface_t;

typedef struct {
    car_can_pdu_status_t pdu_status;
    car_can_pdu_current_data_t pdu_current_data;
    car_can_pdu_voltage_data_t pdu_voltage_data;
    car_can_pdu_temperature_data_t pdu_temperature_data;
    car_can_pdu_over_current_snapshot_a_t pdu_over_current_snapshot_a;
    car_can_pdu_over_current_snapshot_b_t pdu_over_current_snapshot_b;
} car_can_pdu_t;

typedef struct {
    car_can_pedals_status_t pedals_status;
} car_can_pedals_t;

typedef struct {
    car_can_vcu_status_t vcu_status;
} car_can_vcu_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t bps_status;
        can_message_info_t bps_pack_data;
        can_message_info_t bps_voltage_data;
        can_message_info_t bps_temperature_data;
        can_message_info_t smd_status;
        can_message_info_t bps_command_echo;
        can_message_info_t bps_fault_snapshot;
        can_message_info_t bps_fault_voltage_snapshot;
        can_message_info_t bps_fault_temperature_snapshot;
    } bps;
    struct {
        can_message_info_t dei_status;
    } dei;
    struct {
        can_message_info_t bps_command;
        can_message_info_t pdu_command;
        can_message_info_t motor_controller_command;
        can_message_info_t sweep_mpp_ts;
    } external;
    struct {
        can_message_info_t lvc_status;
        can_message_info_t lvc_current_data;
        can_message_info_t lvc_voltage_data;
        can_message_info_t lvc_temperature_data;
        can_message_info_t lvc_over_current_snapshot;
    } lvc;
    struct {
        can_message_info_t mika_status;
    } mika;
    struct {
        can_message_info_t mppt_interface_status;
    } mppt_interface;
    struct {
        can_message_info_t pdu_status;
        can_message_info_t pdu_current_data;
        can_message_info_t pdu_voltage_data;
        can_message_info_t pdu_temperature_data;
        can_message_info_t pdu_over_current_snapshot_a;
        can_message_info_t pdu_over_current_snapshot_b;
    } pdu;
    struct {
        can_message_info_t pedals_status;
    } pedals;
    struct {
        can_message_info_t vcu_status;
    } vcu;
} car_can_metadata_t;
extern const car_can_metadata_t car_can;
