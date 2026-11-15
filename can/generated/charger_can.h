/** @file charger_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Charging measurements from BPS at 10 Hz. MainBatteryVoltage and MainVoltage match BPSPackData; cell and temperature summaries match the CarCAN BPS data encodings. */
typedef struct {
    /** CellSumVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. */
    double cell_sum_voltage;
    /** MainBatteryVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. */
    double main_battery_voltage;
    /** MainVoltage; unit=V; physical=raw*0.05+0; raw=[0,4095]. */
    double main_voltage;
    /** MinCellVoltage; unit=mV; physical=raw*1+2000; raw=[0,4095]. */
    uint16_t min_cell_voltage;
    /** MaxCellVoltage; unit=mV; physical=raw*1+2000; raw=[0,4095]. */
    uint16_t max_cell_voltage;
    /** MaxBatteryTemperature; unit=degC; physical=raw*0.05+0; raw=[0,2047]. */
    double max_battery_temperature;
    /** AverageBatteryTemperature; unit=degC; physical=raw*0.05+0; raw=[0,2047]. */
    double average_battery_temperature;
    /** BatteryCurrent; unit=A; physical=raw*0.01+-50; raw=[0,16383]. */
    double battery_current;
} charger_can_bps_charge_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t charger_can_bps_charge_data_pack(can_frame_t* dst, const charger_can_bps_charge_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t charger_can_bps_charge_data_unpack(charger_can_bps_charge_data_t* dst, const can_frame_t* src);

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
} charger_can_elcon_interface_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t charger_can_elcon_interface_status_pack(can_frame_t* dst, const charger_can_elcon_interface_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t charger_can_elcon_interface_status_unpack(charger_can_elcon_interface_status_t* dst, const can_frame_t* src);

typedef struct {
    charger_can_bps_charge_data_t bps_charge_data;
} charger_can_bps_t;

typedef struct {
    charger_can_elcon_interface_status_t elcon_interface_status;
} charger_can_elcon_interface_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t bps_charge_data;
    } bps;
    struct {
        can_message_info_t elcon_interface_status;
    } elcon_interface;
} charger_can_metadata_t;
extern const charger_can_metadata_t charger_can;
