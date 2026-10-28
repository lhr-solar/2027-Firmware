/** @file charger_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Named raw values for Status; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_STATUS_OK = 0,
    CHARGER_CAN_BPS_STATUS_STATUS_FAULT = 1,
    CHARGER_CAN_BPS_STATUS_STATUS_WAITING = 2,
} charger_can_bps_status_status_choice_t;
/** Named raw values for FaultSummary; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_SUMMARY_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_SUMMARY_SETPOINT_FAULT = 1,
    CHARGER_CAN_BPS_STATUS_FAULT_SUMMARY_EMERGENCY_STOP = 2,
    CHARGER_CAN_BPS_STATUS_FAULT_SUMMARY_CONTACTOR_RELAY_FAULT = 3,
    CHARGER_CAN_BPS_STATUS_FAULT_SUMMARY_PRECHARGE_FAULT = 4,
    CHARGER_CAN_BPS_STATUS_FAULT_SUMMARY_SMB_FAULT = 5,
} charger_can_bps_status_fault_summary_choice_t;
/** Named raw values for FaultOverTemperature; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_OVER_TEMPERATURE_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_OVER_TEMPERATURE_FAULT = 1,
} charger_can_bps_status_fault_over_temperature_choice_t;
/** Named raw values for FaultOverVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_OVER_VOLTAGE_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_OVER_VOLTAGE_FAULT = 1,
} charger_can_bps_status_fault_over_voltage_choice_t;
/** Named raw values for FaultUnderVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_UNDER_VOLTAGE_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_UNDER_VOLTAGE_FAULT = 1,
} charger_can_bps_status_fault_under_voltage_choice_t;
/** Named raw values for FaultChargeOverCurrent; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_CHARGE_OVER_CURRENT_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_CHARGE_OVER_CURRENT_FAULT = 1,
} charger_can_bps_status_fault_charge_over_current_choice_t;
/** Named raw values for FaultDischargeOverCurrent; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_DISCHARGE_OVER_CURRENT_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_DISCHARGE_OVER_CURRENT_FAULT = 1,
} charger_can_bps_status_fault_discharge_over_current_choice_t;
/** Named raw values for FaultHVPositiveContactorMismatch; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_HV_POSITIVE_CONTACTOR_MISMATCH_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_HV_POSITIVE_CONTACTOR_MISMATCH_FAULT = 1,
} charger_can_bps_status_fault_hv_positive_contactor_mismatch_choice_t;
/** Named raw values for FaultMotorContactorMismatch; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_CONTACTOR_MISMATCH_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_CONTACTOR_MISMATCH_FAULT = 1,
} charger_can_bps_status_fault_motor_contactor_mismatch_choice_t;
/** Named raw values for FaultHVNegativeContactorMismatch; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_HV_NEGATIVE_CONTACTOR_MISMATCH_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_HV_NEGATIVE_CONTACTOR_MISMATCH_FAULT = 1,
} charger_can_bps_status_fault_hv_negative_contactor_mismatch_choice_t;
/** Named raw values for FaultMainPrechargeRelayMismatch; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_RELAY_MISMATCH_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_RELAY_MISMATCH_FAULT = 1,
} charger_can_bps_status_fault_main_precharge_relay_mismatch_choice_t;
/** Named raw values for FaultMotorPrechargeRelayMismatch; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_RELAY_MISMATCH_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_RELAY_MISMATCH_FAULT = 1,
} charger_can_bps_status_fault_motor_precharge_relay_mismatch_choice_t;
/** Named raw values for FaultDriverEstop; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_DRIVER_ESTOP_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_DRIVER_ESTOP_FAULT = 1,
} charger_can_bps_status_fault_driver_estop_choice_t;
/** Named raw values for FaultExternalEstop; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_EXTERNAL_ESTOP_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_EXTERNAL_ESTOP_FAULT = 1,
} charger_can_bps_status_fault_external_estop_choice_t;
/** Named raw values for FaultBatteryEstop; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_BATTERY_ESTOP_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_BATTERY_ESTOP_FAULT = 1,
} charger_can_bps_status_fault_battery_estop_choice_t;
/** Named raw values for FaultChargerEstop; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_CHARGER_ESTOP_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_CHARGER_ESTOP_FAULT = 1,
} charger_can_bps_status_fault_charger_estop_choice_t;
/** Named raw values for FaultMainPrechargeTimeout; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_TIMEOUT_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_TIMEOUT_FAULT = 1,
} charger_can_bps_status_fault_main_precharge_timeout_choice_t;
/** Named raw values for FaultMotorPrechargeTimeout; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_TIMEOUT_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_TIMEOUT_FAULT = 1,
} charger_can_bps_status_fault_motor_precharge_timeout_choice_t;
/** Named raw values for FaultMainVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_VOLTAGE_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_VOLTAGE_FAULT = 1,
} charger_can_bps_status_fault_main_voltage_choice_t;
/** Named raw values for FaultMotorVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_VOLTAGE_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_VOLTAGE_FAULT = 1,
} charger_can_bps_status_fault_motor_voltage_choice_t;
/** Named raw values for FaultMainPrechargeVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_VOLTAGE_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MAIN_PRECHARGE_VOLTAGE_FAULT = 1,
} charger_can_bps_status_fault_main_precharge_voltage_choice_t;
/** Named raw values for FaultMotorPrechargeVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_VOLTAGE_OK = 0,
    CHARGER_CAN_BPS_STATUS_FAULT_MOTOR_PRECHARGE_VOLTAGE_FAULT = 1,
} charger_can_bps_status_fault_motor_precharge_voltage_choice_t;
/** Named raw values for WarningSMBCommunication; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_SMB_COMMUNICATION_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_SMB_COMMUNICATION_WARNING = 1,
} charger_can_bps_status_warning_smb_communication_choice_t;
/** Named raw values for WarningUnderVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_UNDER_VOLTAGE_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_UNDER_VOLTAGE_WARNING = 1,
} charger_can_bps_status_warning_under_voltage_choice_t;
/** Named raw values for WarningOverVoltage; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_OVER_VOLTAGE_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_OVER_VOLTAGE_WARNING = 1,
} charger_can_bps_status_warning_over_voltage_choice_t;
/** Named raw values for WarningChargeOverTemperature; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_CHARGE_OVER_TEMPERATURE_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_CHARGE_OVER_TEMPERATURE_WARNING = 1,
} charger_can_bps_status_warning_charge_over_temperature_choice_t;
/** Named raw values for WarningDischargeOverTemperature; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_DISCHARGE_OVER_TEMPERATURE_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_DISCHARGE_OVER_TEMPERATURE_WARNING = 1,
} charger_can_bps_status_warning_discharge_over_temperature_choice_t;
/** Named raw values for WarningBatteryFan1; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_BATTERY_FAN1_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_BATTERY_FAN1_WARNING = 1,
} charger_can_bps_status_warning_battery_fan1_choice_t;
/** Named raw values for WarningBatteryFan2; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_BATTERY_FAN2_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_BATTERY_FAN2_WARNING = 1,
} charger_can_bps_status_warning_battery_fan2_choice_t;
/** Named raw values for WarningBPSFan; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_WARNING_BPS_FAN_NO_WARNING = 0,
    CHARGER_CAN_BPS_STATUS_WARNING_BPS_FAN_WARNING = 1,
} charger_can_bps_status_warning_bps_fan_choice_t;
/** Named raw values for DriveOK; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_DRIVE_OK_NOT_ALLOWED = 0,
    CHARGER_CAN_BPS_STATUS_DRIVE_OK_ALLOWED = 1,
} charger_can_bps_status_drive_ok_choice_t;
/** Named raw values for ChargeOK; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_CHARGE_OK_NOT_ALLOWED = 0,
    CHARGER_CAN_BPS_STATUS_CHARGE_OK_ALLOWED = 1,
} charger_can_bps_status_charge_ok_choice_t;
/** Named raw values for MPPTSetMode; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_MPPT_SET_MODE_DISABLED = 0,
    CHARGER_CAN_BPS_STATUS_MPPT_SET_MODE_PERTURB_AND_OBSERVE = 1,
    CHARGER_CAN_BPS_STATUS_MPPT_SET_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} charger_can_bps_status_mppt_set_mode_choice_t;
/** Named raw values for ElconChargeOK; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_ELCON_CHARGE_OK_NOT_ALLOWED = 0,
    CHARGER_CAN_BPS_STATUS_ELCON_CHARGE_OK_ALLOWED = 1,
} charger_can_bps_status_elcon_charge_ok_choice_t;
/** Named raw values for RegenChargeOK; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_REGEN_CHARGE_OK_NOT_ALLOWED = 0,
    CHARGER_CAN_BPS_STATUS_REGEN_CHARGE_OK_ALLOWED = 1,
} charger_can_bps_status_regen_charge_ok_choice_t;
/** Named raw values for HVPositiveContactor; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_HV_POSITIVE_CONTACTOR_OFF = 0,
    CHARGER_CAN_BPS_STATUS_HV_POSITIVE_CONTACTOR_ON = 1,
} charger_can_bps_status_hv_positive_contactor_choice_t;
/** Named raw values for MotorContactor; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_MOTOR_CONTACTOR_OFF = 0,
    CHARGER_CAN_BPS_STATUS_MOTOR_CONTACTOR_ON = 1,
} charger_can_bps_status_motor_contactor_choice_t;
/** Named raw values for HVNegativeContactor; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_HV_NEGATIVE_CONTACTOR_OFF = 0,
    CHARGER_CAN_BPS_STATUS_HV_NEGATIVE_CONTACTOR_ON = 1,
} charger_can_bps_status_hv_negative_contactor_choice_t;
/** Named raw values for MainPrechargeRelay; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_MAIN_PRECHARGE_RELAY_OFF = 0,
    CHARGER_CAN_BPS_STATUS_MAIN_PRECHARGE_RELAY_ON = 1,
} charger_can_bps_status_main_precharge_relay_choice_t;
/** Named raw values for MotorPrechargeRelay; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_BPS_STATUS_MOTOR_PRECHARGE_RELAY_OFF = 0,
    CHARGER_CAN_BPS_STATUS_MOTOR_PRECHARGE_RELAY_ON = 1,
} charger_can_bps_status_motor_precharge_relay_choice_t;
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
} charger_can_bps_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t charger_can_bps_status_pack(can_frame_t* dst, const charger_can_bps_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t charger_can_bps_status_unpack(charger_can_bps_status_t* dst, const can_frame_t* src);

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
} charger_can_bps_pack_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t charger_can_bps_pack_data_pack(can_frame_t* dst, const charger_can_bps_pack_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t charger_can_bps_pack_data_unpack(charger_can_bps_pack_data_t* dst, const can_frame_t* src);

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
} charger_can_bps_voltage_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t charger_can_bps_voltage_data_pack(can_frame_t* dst, const charger_can_bps_voltage_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t charger_can_bps_voltage_data_unpack(charger_can_bps_voltage_data_t* dst, const can_frame_t* src);

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
} charger_can_bps_temperature_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t charger_can_bps_temperature_data_pack(can_frame_t* dst, const charger_can_bps_temperature_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t charger_can_bps_temperature_data_unpack(charger_can_bps_temperature_data_t* dst, const can_frame_t* src);

/** Named raw values for Status; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_STATUS_OK = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_STATUS_FAULT = 1,
} charger_can_elcon_interface_status_status_choice_t;
/** Named raw values for FaultElconWatchdog; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_ELCON_WATCHDOG_OK = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_ELCON_WATCHDOG_FAULT = 1,
} charger_can_elcon_interface_status_fault_elcon_watchdog_choice_t;
/** Named raw values for FaultBPSWatchdog; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_BPS_WATCHDOG_OK = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_BPS_WATCHDOG_FAULT = 1,
} charger_can_elcon_interface_status_fault_bps_watchdog_choice_t;
/** Named raw values for FaultElcon; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_ELCON_OK = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_ELCON_FAULT = 1,
} charger_can_elcon_interface_status_fault_elcon_choice_t;
/** Named raw values for FaultBPS; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_BPS_OK = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_BPS_FAULT = 1,
} charger_can_elcon_interface_status_fault_bps_choice_t;
/** Named raw values for FaultEVSE; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_EVSE_OK = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_FAULT_EVSE_FAULT = 1,
} charger_can_elcon_interface_status_fault_evse_choice_t;
/** Named raw values for Estop; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_ESTOP_NOT_PRESSED = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_ESTOP_PRESSED = 1,
} charger_can_elcon_interface_status_estop_choice_t;
/** Named raw values for ChargeState; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_CHARGE_STATE_DISABLED = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_CHARGE_STATE_STANDBY = 1,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_CHARGE_STATE_CHARGING = 2,
} charger_can_elcon_interface_status_charge_state_choice_t;
/** Named raw values for EVSEConnected; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_CONNECTED_DISCONNECTED = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_CONNECTED_CONNECTED = 1,
} charger_can_elcon_interface_status_evse_connected_choice_t;
/** Named raw values for EVSELatchPressed; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_LATCH_PRESSED_NOT_PRESSED = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_LATCH_PRESSED_PRESSED = 1,
} charger_can_elcon_interface_status_evse_latch_pressed_choice_t;
/** Named raw values for EVSEState; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_STATE_A_DISCONNECTED = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_STATE_B_CONNECTED = 1,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_STATE_C_CHARGING_READY = 2,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_STATE_D_VENTILATION_REQUIRED = 3,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_STATE_E_FAULT = 4,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSE_STATE_F_EVSE_FAULT = 5,
} charger_can_elcon_interface_status_evse_state_choice_t;
/** Named raw values for EVSECPValid; unknown values remain integers. */
typedef enum {
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSECP_VALID_INVALID = 0,
    CHARGER_CAN_ELCON_INTERFACE_STATUS_EVSECP_VALID_VALID = 1,
} charger_can_elcon_interface_status_evsecp_valid_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** Status; unit=none; physical=raw*1+0; raw=[0,1]. Status */
    uint8_t status;
    /** FaultElconWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. FaultElconWatchdog */
    uint8_t fault_elcon_watchdog;
    /** FaultBPSWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. FaultBPSWatchdog */
    uint8_t fault_bps_watchdog;
    /** FaultElcon; unit=none; physical=raw*1+0; raw=[0,1]. FaultElcon */
    uint8_t fault_elcon;
    /** FaultBPS; unit=none; physical=raw*1+0; raw=[0,1]. FaultBPS */
    uint8_t fault_bps;
    /** FaultEVSE; unit=none; physical=raw*1+0; raw=[0,1]. FaultEVSE */
    uint8_t fault_evse;
    /** Estop; unit=none; physical=raw*1+0; raw=[0,1]. Estop */
    uint8_t estop;
    /** ChargeState; unit=none; physical=raw*1+0; raw=[0,3]. ChargeState */
    uint8_t charge_state;
    /** EVSEConnected; unit=none; physical=raw*1+0; raw=[0,1]. EVSEConnected */
    uint8_t evse_connected;
    /** EVSELatchPressed; unit=none; physical=raw*1+0; raw=[0,1]. EVSELatchPressed */
    uint8_t evse_latch_pressed;
    /** EVSEState; unit=none; physical=raw*1+0; raw=[0,7]. EVSEState */
    uint8_t evse_state;
    /** EVSECPValid; unit=none; physical=raw*1+0; raw=[0,1]. EVSECPValid */
    uint8_t evsecp_valid;
    /** EVSEAvailableCurrent; unit=A; physical=raw*0.01+0; raw=[0,2550]. EVSEAvailableCurrent */
    double evse_available_current;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} charger_can_elcon_interface_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t charger_can_elcon_interface_status_pack(can_frame_t* dst, const charger_can_elcon_interface_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t charger_can_elcon_interface_status_unpack(charger_can_elcon_interface_status_t* dst, const can_frame_t* src);

typedef struct {
    charger_can_bps_status_t bps_status;
    charger_can_bps_pack_data_t bps_pack_data;
    charger_can_bps_voltage_data_t bps_voltage_data;
    charger_can_bps_temperature_data_t bps_temperature_data;
} charger_can_bps_t;

typedef struct {
    charger_can_elcon_interface_status_t elcon_interface_status;
} charger_can_elcon_interface_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t bps_status;
        can_message_info_t bps_pack_data;
        can_message_info_t bps_voltage_data;
        can_message_info_t bps_temperature_data;
    } bps;
    struct {
        can_message_info_t elcon_interface_status;
    } elcon_interface;
} charger_can_metadata_t;
extern const charger_can_metadata_t charger_can;
