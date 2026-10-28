/** @file mpptcan.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** InputVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. InputVoltage */
    double input_voltage;
    /** InputCurrent; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. InputCurrent */
    double input_current;
    /** OutputVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. OutputVoltage */
    double output_voltage;
    /** OutputCurrent; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. OutputCurrent */
    double output_current;
} mpptcan_mppta_power_measurements_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_power_measurements_pack(can_frame_t* dst, const mpptcan_mppta_power_measurements_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_power_measurements_unpack(mpptcan_mppta_power_measurements_t* dst, const can_frame_t* src);

/** Named raw values for OperatingMode; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_INPUT_VOLTAGE = 0,
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_INPUT_CURRENT = 1,
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_MINIMUM_INPUT_CURRENT = 2,
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_VOLTAGE = 3,
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_CURRENT = 4,
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_TEMPERATURE_DERATING = 5,
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_MINIMUM_INPUT_VOLTAGE = 6,
    MPPTCAN_MPPTA_STATUS_OPERATING_MODE_FAULT = 7,
} mpptcan_mppta_status_operating_mode_choice_t;
/** Named raw values for Fault; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTA_STATUS_FAULT_OK = 0,
    MPPTCAN_MPPTA_STATUS_FAULT_CONFIGURATION_ERROR = 1,
    MPPTCAN_MPPTA_STATUS_FAULT_INPUT_OVER_VOLTAGE = 2,
    MPPTCAN_MPPTA_STATUS_FAULT_OUTPUT_OVER_VOLTAGE = 3,
    MPPTCAN_MPPTA_STATUS_FAULT_OUTPUT_OVER_CURRENT = 4,
    MPPTCAN_MPPTA_STATUS_FAULT_INPUT_OVER_CURRENT = 5,
    MPPTCAN_MPPTA_STATUS_FAULT_INPUT_UNDER_CURRENT = 6,
    MPPTCAN_MPPTA_STATUS_FAULT_PHASE_OVER_CURRENT = 7,
    MPPTCAN_MPPTA_STATUS_FAULT_FAULT = 8,
} mpptcan_mppta_status_fault_choice_t;
/** Named raw values for Enabled; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTA_STATUS_ENABLED_DISABLED = 0,
    MPPTCAN_MPPTA_STATUS_ENABLED_ENABLED = 1,
} mpptcan_mppta_status_enabled_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** OperatingMode; unit=none; physical=raw*1+0; raw=[0,255]. OperatingMode */
    uint8_t operating_mode;
    /** Fault; unit=none; physical=raw*1+0; raw=[0,255]. Fault */
    uint8_t fault;
    /** Enabled; unit=none; physical=raw*1+0; raw=[0,255]. Enabled */
    uint8_t enabled;
    /** AmbientTemperature; unit=degC; physical=raw*1+0; raw=[-128,127]. AmbientTemperature */
    int8_t ambient_temperature;
    /** HeatsinkTemperature; unit=degC; physical=raw*1+0; raw=[-128,127]. HeatsinkTemperature */
    int8_t heatsink_temperature;
} mpptcan_mppta_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_status_pack(can_frame_t* dst, const mpptcan_mppta_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_status_unpack(mpptcan_mppta_status_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** SampleIndex; unit=none; physical=raw*1+0; raw=[0,255]. SampleIndex */
    uint8_t sample_index;
    /** Current; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. Current */
    double current;
    /** Voltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. Voltage */
    double voltage;
} mpptcan_mppta_sweep_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_sweep_data_pack(can_frame_t* dst, const mpptcan_mppta_sweep_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_sweep_data_unpack(mpptcan_mppta_sweep_data_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mppta_fill_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_fill_rx_buffer_pack(can_frame_t* dst, const mpptcan_mppta_fill_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_fill_rx_buffer_unpack(mpptcan_mppta_fill_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mppta_fill_rx_buffer_long_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_fill_rx_buffer_long_pack(can_frame_t* dst, const mpptcan_mppta_fill_rx_buffer_long_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_fill_rx_buffer_long_unpack(mpptcan_mppta_fill_rx_buffer_long_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mppta_process_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_process_rx_buffer_pack(can_frame_t* dst, const mpptcan_mppta_process_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_process_rx_buffer_unpack(mpptcan_mppta_process_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mppta_process_short_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_process_short_buffer_pack(can_frame_t* dst, const mpptcan_mppta_process_short_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_process_short_buffer_unpack(mpptcan_mppta_process_short_buffer_t* dst, const can_frame_t* src);

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTA_SET_MODE_MODE_DISABLED = 0,
    MPPTCAN_MPPTA_SET_MODE_MODE_PERTURB_AND_OBSERVE = 1,
    MPPTCAN_MPPTA_SET_MODE_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} mpptcan_mppta_set_mode_mode_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,255]. Mode */
    uint8_t mode;
} mpptcan_mppta_set_mode_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_set_mode_pack(can_frame_t* dst, const mpptcan_mppta_set_mode_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_set_mode_unpack(mpptcan_mppta_set_mode_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** StartVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. StartVoltage */
    double start_voltage;
    /** EndVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. EndVoltage */
    double end_voltage;
    /** SampleCount; unit=none; physical=raw*1+0; raw=[4,128]. SampleCount */
    uint8_t sample_count;
} mpptcan_mppta_perform_sweep_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_perform_sweep_pack(can_frame_t* dst, const mpptcan_mppta_perform_sweep_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_perform_sweep_unpack(mpptcan_mppta_perform_sweep_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** VoltageLimit; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. VoltageLimit */
    double voltage_limit;
} mpptcan_mppta_set_output_voltage_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_set_output_voltage_limit_pack(can_frame_t* dst, const mpptcan_mppta_set_output_voltage_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_set_output_voltage_limit_unpack(mpptcan_mppta_set_output_voltage_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** CurrentLimit; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. CurrentLimit */
    double current_limit;
} mpptcan_mppta_set_output_current_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mppta_set_output_current_limit_pack(can_frame_t* dst, const mpptcan_mppta_set_output_current_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mppta_set_output_current_limit_unpack(mpptcan_mppta_set_output_current_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** InputVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. InputVoltage */
    double input_voltage;
    /** InputCurrent; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. InputCurrent */
    double input_current;
    /** OutputVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. OutputVoltage */
    double output_voltage;
    /** OutputCurrent; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. OutputCurrent */
    double output_current;
} mpptcan_mpptb_power_measurements_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_power_measurements_pack(can_frame_t* dst, const mpptcan_mpptb_power_measurements_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_power_measurements_unpack(mpptcan_mpptb_power_measurements_t* dst, const can_frame_t* src);

/** Named raw values for OperatingMode; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_INPUT_VOLTAGE = 0,
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_INPUT_CURRENT = 1,
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_MINIMUM_INPUT_CURRENT = 2,
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_VOLTAGE = 3,
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_CURRENT = 4,
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_TEMPERATURE_DERATING = 5,
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_MINIMUM_INPUT_VOLTAGE = 6,
    MPPTCAN_MPPTB_STATUS_OPERATING_MODE_FAULT = 7,
} mpptcan_mpptb_status_operating_mode_choice_t;
/** Named raw values for Fault; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTB_STATUS_FAULT_OK = 0,
    MPPTCAN_MPPTB_STATUS_FAULT_CONFIGURATION_ERROR = 1,
    MPPTCAN_MPPTB_STATUS_FAULT_INPUT_OVER_VOLTAGE = 2,
    MPPTCAN_MPPTB_STATUS_FAULT_OUTPUT_OVER_VOLTAGE = 3,
    MPPTCAN_MPPTB_STATUS_FAULT_OUTPUT_OVER_CURRENT = 4,
    MPPTCAN_MPPTB_STATUS_FAULT_INPUT_OVER_CURRENT = 5,
    MPPTCAN_MPPTB_STATUS_FAULT_INPUT_UNDER_CURRENT = 6,
    MPPTCAN_MPPTB_STATUS_FAULT_PHASE_OVER_CURRENT = 7,
    MPPTCAN_MPPTB_STATUS_FAULT_FAULT = 8,
} mpptcan_mpptb_status_fault_choice_t;
/** Named raw values for Enabled; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTB_STATUS_ENABLED_DISABLED = 0,
    MPPTCAN_MPPTB_STATUS_ENABLED_ENABLED = 1,
} mpptcan_mpptb_status_enabled_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** OperatingMode; unit=none; physical=raw*1+0; raw=[0,255]. OperatingMode */
    uint8_t operating_mode;
    /** Fault; unit=none; physical=raw*1+0; raw=[0,255]. Fault */
    uint8_t fault;
    /** Enabled; unit=none; physical=raw*1+0; raw=[0,255]. Enabled */
    uint8_t enabled;
    /** AmbientTemperature; unit=degC; physical=raw*1+0; raw=[-128,127]. AmbientTemperature */
    int8_t ambient_temperature;
    /** HeatsinkTemperature; unit=degC; physical=raw*1+0; raw=[-128,127]. HeatsinkTemperature */
    int8_t heatsink_temperature;
} mpptcan_mpptb_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_status_pack(can_frame_t* dst, const mpptcan_mpptb_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_status_unpack(mpptcan_mpptb_status_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** SampleIndex; unit=none; physical=raw*1+0; raw=[0,255]. SampleIndex */
    uint8_t sample_index;
    /** Current; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. Current */
    double current;
    /** Voltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. Voltage */
    double voltage;
} mpptcan_mpptb_sweep_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_sweep_data_pack(can_frame_t* dst, const mpptcan_mpptb_sweep_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_sweep_data_unpack(mpptcan_mpptb_sweep_data_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptb_fill_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_fill_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptb_fill_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_fill_rx_buffer_unpack(mpptcan_mpptb_fill_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptb_fill_rx_buffer_long_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_fill_rx_buffer_long_pack(can_frame_t* dst, const mpptcan_mpptb_fill_rx_buffer_long_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_fill_rx_buffer_long_unpack(mpptcan_mpptb_fill_rx_buffer_long_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptb_process_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_process_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptb_process_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_process_rx_buffer_unpack(mpptcan_mpptb_process_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptb_process_short_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_process_short_buffer_pack(can_frame_t* dst, const mpptcan_mpptb_process_short_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_process_short_buffer_unpack(mpptcan_mpptb_process_short_buffer_t* dst, const can_frame_t* src);

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTB_SET_MODE_MODE_DISABLED = 0,
    MPPTCAN_MPPTB_SET_MODE_MODE_PERTURB_AND_OBSERVE = 1,
    MPPTCAN_MPPTB_SET_MODE_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} mpptcan_mpptb_set_mode_mode_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,255]. Mode */
    uint8_t mode;
} mpptcan_mpptb_set_mode_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_set_mode_pack(can_frame_t* dst, const mpptcan_mpptb_set_mode_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_set_mode_unpack(mpptcan_mpptb_set_mode_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** StartVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. StartVoltage */
    double start_voltage;
    /** EndVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. EndVoltage */
    double end_voltage;
    /** SampleCount; unit=none; physical=raw*1+0; raw=[4,128]. SampleCount */
    uint8_t sample_count;
} mpptcan_mpptb_perform_sweep_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_perform_sweep_pack(can_frame_t* dst, const mpptcan_mpptb_perform_sweep_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_perform_sweep_unpack(mpptcan_mpptb_perform_sweep_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** VoltageLimit; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. VoltageLimit */
    double voltage_limit;
} mpptcan_mpptb_set_output_voltage_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_set_output_voltage_limit_pack(can_frame_t* dst, const mpptcan_mpptb_set_output_voltage_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_set_output_voltage_limit_unpack(mpptcan_mpptb_set_output_voltage_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** CurrentLimit; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. CurrentLimit */
    double current_limit;
} mpptcan_mpptb_set_output_current_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptb_set_output_current_limit_pack(can_frame_t* dst, const mpptcan_mpptb_set_output_current_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptb_set_output_current_limit_unpack(mpptcan_mpptb_set_output_current_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** InputVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. InputVoltage */
    double input_voltage;
    /** InputCurrent; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. InputCurrent */
    double input_current;
    /** OutputVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. OutputVoltage */
    double output_voltage;
    /** OutputCurrent; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. OutputCurrent */
    double output_current;
} mpptcan_mpptc_power_measurements_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_power_measurements_pack(can_frame_t* dst, const mpptcan_mpptc_power_measurements_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_power_measurements_unpack(mpptcan_mpptc_power_measurements_t* dst, const can_frame_t* src);

/** Named raw values for OperatingMode; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_INPUT_VOLTAGE = 0,
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_INPUT_CURRENT = 1,
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_MINIMUM_INPUT_CURRENT = 2,
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_VOLTAGE = 3,
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_CURRENT = 4,
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_TEMPERATURE_DERATING = 5,
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_MINIMUM_INPUT_VOLTAGE = 6,
    MPPTCAN_MPPTC_STATUS_OPERATING_MODE_FAULT = 7,
} mpptcan_mpptc_status_operating_mode_choice_t;
/** Named raw values for Fault; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTC_STATUS_FAULT_OK = 0,
    MPPTCAN_MPPTC_STATUS_FAULT_CONFIGURATION_ERROR = 1,
    MPPTCAN_MPPTC_STATUS_FAULT_INPUT_OVER_VOLTAGE = 2,
    MPPTCAN_MPPTC_STATUS_FAULT_OUTPUT_OVER_VOLTAGE = 3,
    MPPTCAN_MPPTC_STATUS_FAULT_OUTPUT_OVER_CURRENT = 4,
    MPPTCAN_MPPTC_STATUS_FAULT_INPUT_OVER_CURRENT = 5,
    MPPTCAN_MPPTC_STATUS_FAULT_INPUT_UNDER_CURRENT = 6,
    MPPTCAN_MPPTC_STATUS_FAULT_PHASE_OVER_CURRENT = 7,
    MPPTCAN_MPPTC_STATUS_FAULT_FAULT = 8,
} mpptcan_mpptc_status_fault_choice_t;
/** Named raw values for Enabled; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTC_STATUS_ENABLED_DISABLED = 0,
    MPPTCAN_MPPTC_STATUS_ENABLED_ENABLED = 1,
} mpptcan_mpptc_status_enabled_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** OperatingMode; unit=none; physical=raw*1+0; raw=[0,255]. OperatingMode */
    uint8_t operating_mode;
    /** Fault; unit=none; physical=raw*1+0; raw=[0,255]. Fault */
    uint8_t fault;
    /** Enabled; unit=none; physical=raw*1+0; raw=[0,255]. Enabled */
    uint8_t enabled;
    /** AmbientTemperature; unit=degC; physical=raw*1+0; raw=[-128,127]. AmbientTemperature */
    int8_t ambient_temperature;
    /** HeatsinkTemperature; unit=degC; physical=raw*1+0; raw=[-128,127]. HeatsinkTemperature */
    int8_t heatsink_temperature;
} mpptcan_mpptc_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_status_pack(can_frame_t* dst, const mpptcan_mpptc_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_status_unpack(mpptcan_mpptc_status_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** SampleIndex; unit=none; physical=raw*1+0; raw=[0,255]. SampleIndex */
    uint8_t sample_index;
    /** Current; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. Current */
    double current;
    /** Voltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. Voltage */
    double voltage;
} mpptcan_mpptc_sweep_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_sweep_data_pack(can_frame_t* dst, const mpptcan_mpptc_sweep_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_sweep_data_unpack(mpptcan_mpptc_sweep_data_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptc_fill_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_fill_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptc_fill_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_fill_rx_buffer_unpack(mpptcan_mpptc_fill_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptc_fill_rx_buffer_long_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_fill_rx_buffer_long_pack(can_frame_t* dst, const mpptcan_mpptc_fill_rx_buffer_long_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_fill_rx_buffer_long_unpack(mpptcan_mpptc_fill_rx_buffer_long_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptc_process_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_process_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptc_process_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_process_rx_buffer_unpack(mpptcan_mpptc_process_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} mpptcan_mpptc_process_short_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_process_short_buffer_pack(can_frame_t* dst, const mpptcan_mpptc_process_short_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_process_short_buffer_unpack(mpptcan_mpptc_process_short_buffer_t* dst, const can_frame_t* src);

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    MPPTCAN_MPPTC_SET_MODE_MODE_DISABLED = 0,
    MPPTCAN_MPPTC_SET_MODE_MODE_PERTURB_AND_OBSERVE = 1,
    MPPTCAN_MPPTC_SET_MODE_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} mpptcan_mpptc_set_mode_mode_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,255]. Mode */
    uint8_t mode;
} mpptcan_mpptc_set_mode_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_set_mode_pack(can_frame_t* dst, const mpptcan_mpptc_set_mode_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_set_mode_unpack(mpptcan_mpptc_set_mode_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** StartVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. StartVoltage */
    double start_voltage;
    /** EndVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. EndVoltage */
    double end_voltage;
    /** SampleCount; unit=none; physical=raw*1+0; raw=[4,128]. SampleCount */
    uint8_t sample_count;
} mpptcan_mpptc_perform_sweep_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_perform_sweep_pack(can_frame_t* dst, const mpptcan_mpptc_perform_sweep_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_perform_sweep_unpack(mpptcan_mpptc_perform_sweep_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** VoltageLimit; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. VoltageLimit */
    double voltage_limit;
} mpptcan_mpptc_set_output_voltage_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_set_output_voltage_limit_pack(can_frame_t* dst, const mpptcan_mpptc_set_output_voltage_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_set_output_voltage_limit_unpack(mpptcan_mpptc_set_output_voltage_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** CurrentLimit; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. CurrentLimit */
    double current_limit;
} mpptcan_mpptc_set_output_current_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t mpptcan_mpptc_set_output_current_limit_pack(can_frame_t* dst, const mpptcan_mpptc_set_output_current_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t mpptcan_mpptc_set_output_current_limit_unpack(mpptcan_mpptc_set_output_current_limit_t* dst, const can_frame_t* src);

typedef struct {
    mpptcan_mppta_power_measurements_t mppta_power_measurements;
    mpptcan_mppta_status_t mppta_status;
    mpptcan_mppta_sweep_data_t mppta_sweep_data;
} mpptcan_mppta_t;

typedef struct {
    mpptcan_mpptb_power_measurements_t mpptb_power_measurements;
    mpptcan_mpptb_status_t mpptb_status;
    mpptcan_mpptb_sweep_data_t mpptb_sweep_data;
} mpptcan_mpptb_t;

typedef struct {
    mpptcan_mpptc_power_measurements_t mpptc_power_measurements;
    mpptcan_mpptc_status_t mpptc_status;
    mpptcan_mpptc_sweep_data_t mpptc_sweep_data;
} mpptcan_mpptc_t;

typedef struct {
    mpptcan_mppta_fill_rx_buffer_t mppta_fill_rx_buffer;
    mpptcan_mppta_fill_rx_buffer_long_t mppta_fill_rx_buffer_long;
    mpptcan_mppta_process_rx_buffer_t mppta_process_rx_buffer;
    mpptcan_mppta_process_short_buffer_t mppta_process_short_buffer;
    mpptcan_mppta_set_mode_t mppta_set_mode;
    mpptcan_mppta_perform_sweep_t mppta_perform_sweep;
    mpptcan_mppta_set_output_voltage_limit_t mppta_set_output_voltage_limit;
    mpptcan_mppta_set_output_current_limit_t mppta_set_output_current_limit;
    mpptcan_mpptb_fill_rx_buffer_t mpptb_fill_rx_buffer;
    mpptcan_mpptb_fill_rx_buffer_long_t mpptb_fill_rx_buffer_long;
    mpptcan_mpptb_process_rx_buffer_t mpptb_process_rx_buffer;
    mpptcan_mpptb_process_short_buffer_t mpptb_process_short_buffer;
    mpptcan_mpptb_set_mode_t mpptb_set_mode;
    mpptcan_mpptb_perform_sweep_t mpptb_perform_sweep;
    mpptcan_mpptb_set_output_voltage_limit_t mpptb_set_output_voltage_limit;
    mpptcan_mpptb_set_output_current_limit_t mpptb_set_output_current_limit;
    mpptcan_mpptc_fill_rx_buffer_t mpptc_fill_rx_buffer;
    mpptcan_mpptc_fill_rx_buffer_long_t mpptc_fill_rx_buffer_long;
    mpptcan_mpptc_process_rx_buffer_t mpptc_process_rx_buffer;
    mpptcan_mpptc_process_short_buffer_t mpptc_process_short_buffer;
    mpptcan_mpptc_set_mode_t mpptc_set_mode;
    mpptcan_mpptc_perform_sweep_t mpptc_perform_sweep;
    mpptcan_mpptc_set_output_voltage_limit_t mpptc_set_output_voltage_limit;
    mpptcan_mpptc_set_output_current_limit_t mpptc_set_output_current_limit;
} mpptcan_mppt_interface_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t mppta_power_measurements;
        can_message_info_t mppta_status;
        can_message_info_t mppta_sweep_data;
    } mppta;
    struct {
        can_message_info_t mpptb_power_measurements;
        can_message_info_t mpptb_status;
        can_message_info_t mpptb_sweep_data;
    } mpptb;
    struct {
        can_message_info_t mpptc_power_measurements;
        can_message_info_t mpptc_status;
        can_message_info_t mpptc_sweep_data;
    } mpptc;
    struct {
        can_message_info_t mppta_fill_rx_buffer;
        can_message_info_t mppta_fill_rx_buffer_long;
        can_message_info_t mppta_process_rx_buffer;
        can_message_info_t mppta_process_short_buffer;
        can_message_info_t mppta_set_mode;
        can_message_info_t mppta_perform_sweep;
        can_message_info_t mppta_set_output_voltage_limit;
        can_message_info_t mppta_set_output_current_limit;
        can_message_info_t mpptb_fill_rx_buffer;
        can_message_info_t mpptb_fill_rx_buffer_long;
        can_message_info_t mpptb_process_rx_buffer;
        can_message_info_t mpptb_process_short_buffer;
        can_message_info_t mpptb_set_mode;
        can_message_info_t mpptb_perform_sweep;
        can_message_info_t mpptb_set_output_voltage_limit;
        can_message_info_t mpptb_set_output_current_limit;
        can_message_info_t mpptc_fill_rx_buffer;
        can_message_info_t mpptc_fill_rx_buffer_long;
        can_message_info_t mpptc_process_rx_buffer;
        can_message_info_t mpptc_process_short_buffer;
        can_message_info_t mpptc_set_mode;
        can_message_info_t mpptc_perform_sweep;
        can_message_info_t mpptc_set_output_voltage_limit;
        can_message_info_t mpptc_set_output_current_limit;
    } mppt_interface;
} mpptcan_metadata_t;
extern const mpptcan_metadata_t mpptcan;
