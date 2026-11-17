/** @file elcon_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Named raw values for Elcon_CONTROL_FLAG; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CONTROL_ELCON_CONTROL_FLAG_CHARGE = 0,
    ELCON_CAN_ELCON_CONTROL_ELCON_CONTROL_FLAG_DISABLE = 1,
} elcon_can_elcon_control_elcon_control_flag_choice_t;
/** Preserved HighNoon Elcon protocol variant: extended classic CAN, existing DLC and multiplexed layout. Confirm charger model/firmware before using; do not assume CAN-3865 eight-byte extensions apply. */
typedef struct {
    /** Elcon_VOL_SET; unit=V; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_vol_set;
    /** Elcon_CUR_SET; unit=A; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_cur_set;
    /** Elcon_CONTROL_FLAG; unit=none; physical=raw*1+0; raw=[0,255]. */
    uint8_t elcon_control_flag;
} elcon_can_elcon_control_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t elcon_can_elcon_control_pack(can_frame_t* dst, const elcon_can_elcon_control_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t elcon_can_elcon_control_unpack(elcon_can_elcon_control_t* dst, const can_frame_t* src);

/** Named raw values for Elcon_CONTROL_FLAG; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_CONTROL_FLAG_CHARGE = 0,
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_CONTROL_FLAG_DISABLE = 1,
} elcon_can_elcon_control_mux_elcon_control_flag_choice_t;
/** Named raw values for Elcon_BATTERY_STATE_UV; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_BATTERY_STATE_UV_OK = 0,
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_BATTERY_STATE_UV_FAULT = 1,
} elcon_can_elcon_control_mux_elcon_battery_state_uv_choice_t;
/** Named raw values for Elcon_BATTERY_STATE_OV; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_BATTERY_STATE_OV_OK = 0,
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_BATTERY_STATE_OV_FAULT = 1,
} elcon_can_elcon_control_mux_elcon_battery_state_ov_choice_t;
/** Named raw values for Elcon_MuxPage; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_MUX_PAGE_PAGE1 = 1,
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_MUX_PAGE_PAGE2 = 2,
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_MUX_PAGE_PAGE3 = 3,
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_MUX_PAGE_PAGE4 = 4,
    ELCON_CAN_ELCON_CONTROL_MUX_ELCON_MUX_PAGE_PAGE5 = 5,
} elcon_can_elcon_control_mux_elcon_mux_page_choice_t;
/** Preserved HighNoon Elcon protocol variant: extended classic CAN, existing DLC and multiplexed layout. Confirm charger model/firmware before using; do not assume CAN-3865 eight-byte extensions apply. */
typedef struct {
    /** Elcon_AH_NOMINAL; unit=Ah; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_ah_nominal;
    /** Elcon_BATTERY_VOL; unit=V; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_battery_vol;
    /** Elcon_MAX_CELL_VOL; unit=V; physical=raw*0.001+0; raw=[0,65535]. */
    double elcon_max_cell_vol;
    /** Elcon_NUM_CELLS_EXT; unit=none; physical=raw*1+0; raw=[0,255]. */
    uint8_t elcon_num_cells_ext;
    /** Elcon_VOL_SET; unit=V; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_vol_set;
    /** Elcon_AH_ACTUAL; unit=Ah; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_ah_actual;
    /** Elcon_BATTERY_CHARGE_CUR; unit=A; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_battery_charge_cur;
    /** Elcon_CUR_SET; unit=A; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_cur_set;
    /** Elcon_MIN_CELL_VOL; unit=V; physical=raw*0.001+0; raw=[0,65535]. */
    double elcon_min_cell_vol;
    /** Elcon_BATTERY_SOC; unit=%; physical=raw*1+0; raw=[0,100]. */
    uint8_t elcon_battery_soc;
    /** Elcon_CELL_OV_THRESH; unit=V; physical=raw*0.001+0; raw=[0,65535]. */
    double elcon_cell_ov_thresh;
    /** Elcon_CELL_UV_THRESH; unit=V; physical=raw*0.001+0; raw=[0,65535]. */
    double elcon_cell_uv_thresh;
    /** Elcon_CONTROL_FLAG; unit=none; physical=raw*1+0; raw=[0,255]. */
    uint8_t elcon_control_flag;
    /** Elcon_DISCUR_MAX; unit=A; physical=raw*10+0; raw=[0,255]. */
    uint16_t elcon_discur_max;
    /** Elcon_TEMP_MAX; unit=degC; physical=raw*1+-100; raw=[0,255]. */
    int16_t elcon_temp_max;
    /** Elcon_NUM_CELLS; unit=none; physical=raw*1+0; raw=[0,255]. */
    uint8_t elcon_num_cells;
    /** Elcon_TEMP_MIN; unit=degC; physical=raw*1+-100; raw=[0,255]. */
    int16_t elcon_temp_min;
    /** Elcon_BATTERY_STATE_UV; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t elcon_battery_state_uv;
    /** Elcon_BATTERY_STATE_OV; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t elcon_battery_state_ov;
    /** Elcon_MuxPage; unit=none; physical=raw*1+0; raw=[0,255]. */
    uint8_t elcon_mux_page;
} elcon_can_elcon_control_mux_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t elcon_can_elcon_control_mux_pack(can_frame_t* dst, const elcon_can_elcon_control_mux_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t elcon_can_elcon_control_mux_unpack(elcon_can_elcon_control_mux_t* dst, const can_frame_t* src);

/** Named raw values for Elcon_CMD_TIMEOUT; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_CMD_TIMEOUT_OK = 0,
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_CMD_TIMEOUT_FAULT = 1,
} elcon_can_elcon_ccs_status_elcon_cmd_timeout_choice_t;
/** Named raw values for Elcon_START_STATE; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_START_STATE_OFF = 0,
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_START_STATE_ON = 1,
} elcon_can_elcon_ccs_status_elcon_start_state_choice_t;
/** Named raw values for Elcon_INPUT_VOL_ERROR; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_INPUT_VOL_ERROR_OK = 0,
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_INPUT_VOL_ERROR_FAULT = 1,
} elcon_can_elcon_ccs_status_elcon_input_vol_error_choice_t;
/** Named raw values for Elcon_OVER_TEMPERATURE; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_OVER_TEMPERATURE_OK = 0,
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_OVER_TEMPERATURE_FAULT = 1,
} elcon_can_elcon_ccs_status_elcon_over_temperature_choice_t;
/** Named raw values for Elcon_HW_FAULT; unknown values remain integers. */
typedef enum {
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_HW_FAULT_OK = 0,
    ELCON_CAN_ELCON_CCS_STATUS_ELCON_HW_FAULT_FAULT = 1,
} elcon_can_elcon_ccs_status_elcon_hw_fault_choice_t;
/** Preserved HighNoon Elcon protocol variant: extended classic CAN, existing DLC and multiplexed layout. Confirm charger model/firmware before using; do not assume CAN-3865 eight-byte extensions apply. */
typedef struct {
    /** Elcon_VOL_OUT; unit=V; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_vol_out;
    /** Elcon_CUR_OUT; unit=A; physical=raw*0.1+0; raw=[0,65535]. */
    double elcon_cur_out;
    /** Elcon_CMD_TIMEOUT; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t elcon_cmd_timeout;
    /** Elcon_START_STATE; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t elcon_start_state;
    /** Elcon_INPUT_VOL_ERROR; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t elcon_input_vol_error;
    /** Elcon_OVER_TEMPERATURE; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t elcon_over_temperature;
    /** Elcon_HW_FAULT; unit=none; physical=raw*1+0; raw=[0,1]. */
    uint8_t elcon_hw_fault;
} elcon_can_elcon_ccs_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t elcon_can_elcon_ccs_status_pack(can_frame_t* dst, const elcon_can_elcon_ccs_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t elcon_can_elcon_ccs_status_unpack(elcon_can_elcon_ccs_status_t* dst, const can_frame_t* src);

typedef struct {
    elcon_can_elcon_ccs_status_t elcon_ccs_status;
} elcon_can_elcon_t;

typedef struct {
    elcon_can_elcon_control_t elcon_control;
    elcon_can_elcon_control_mux_t elcon_control_mux;
} elcon_can_elcon_interface_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t elcon_ccs_status;
    } elcon;
    struct {
        can_message_info_t elcon_control;
        can_message_info_t elcon_control_mux;
    } elcon_interface;
} elcon_can_metadata_t;
extern const elcon_can_metadata_t elcon_can;
