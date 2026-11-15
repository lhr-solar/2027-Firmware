/** @file cotscan.h
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
} cotscan_mppta_power_measurements_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_power_measurements_pack(can_frame_t* dst, const cotscan_mppta_power_measurements_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_power_measurements_unpack(cotscan_mppta_power_measurements_t* dst, const can_frame_t* src);

/** Named raw values for OperatingMode; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_INPUT_VOLTAGE = 0,
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_INPUT_CURRENT = 1,
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_MINIMUM_INPUT_CURRENT = 2,
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_VOLTAGE = 3,
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_CURRENT = 4,
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_TEMPERATURE_DERATING = 5,
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_MINIMUM_INPUT_VOLTAGE = 6,
    COTSCAN_MPPTA_STATUS_OPERATING_MODE_FAULT = 7,
} cotscan_mppta_status_operating_mode_choice_t;
/** Named raw values for Fault; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTA_STATUS_FAULT_OK = 0,
    COTSCAN_MPPTA_STATUS_FAULT_CONFIGURATION_ERROR = 1,
    COTSCAN_MPPTA_STATUS_FAULT_INPUT_OVER_VOLTAGE = 2,
    COTSCAN_MPPTA_STATUS_FAULT_OUTPUT_OVER_VOLTAGE = 3,
    COTSCAN_MPPTA_STATUS_FAULT_OUTPUT_OVER_CURRENT = 4,
    COTSCAN_MPPTA_STATUS_FAULT_INPUT_OVER_CURRENT = 5,
    COTSCAN_MPPTA_STATUS_FAULT_INPUT_UNDER_CURRENT = 6,
    COTSCAN_MPPTA_STATUS_FAULT_PHASE_OVER_CURRENT = 7,
    COTSCAN_MPPTA_STATUS_FAULT_FAULT = 8,
} cotscan_mppta_status_fault_choice_t;
/** Named raw values for Enabled; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTA_STATUS_ENABLED_DISABLED = 0,
    COTSCAN_MPPTA_STATUS_ENABLED_ENABLED = 1,
} cotscan_mppta_status_enabled_choice_t;
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
} cotscan_mppta_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_status_pack(can_frame_t* dst, const cotscan_mppta_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_status_unpack(cotscan_mppta_status_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** SampleIndex; unit=none; physical=raw*1+0; raw=[0,255]. SampleIndex */
    uint8_t sample_index;
    /** Current; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. Current */
    double current;
    /** Voltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. Voltage */
    double voltage;
} cotscan_mppta_sweep_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_sweep_data_pack(can_frame_t* dst, const cotscan_mppta_sweep_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_sweep_data_unpack(cotscan_mppta_sweep_data_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mppta_fill_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_fill_rx_buffer_pack(can_frame_t* dst, const cotscan_mppta_fill_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_fill_rx_buffer_unpack(cotscan_mppta_fill_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mppta_fill_rx_buffer_long_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_fill_rx_buffer_long_pack(can_frame_t* dst, const cotscan_mppta_fill_rx_buffer_long_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_fill_rx_buffer_long_unpack(cotscan_mppta_fill_rx_buffer_long_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mppta_process_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_process_rx_buffer_pack(can_frame_t* dst, const cotscan_mppta_process_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_process_rx_buffer_unpack(cotscan_mppta_process_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mppta_process_short_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_process_short_buffer_pack(can_frame_t* dst, const cotscan_mppta_process_short_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_process_short_buffer_unpack(cotscan_mppta_process_short_buffer_t* dst, const can_frame_t* src);

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTA_SET_MODE_MODE_DISABLED = 0,
    COTSCAN_MPPTA_SET_MODE_MODE_PERTURB_AND_OBSERVE = 1,
    COTSCAN_MPPTA_SET_MODE_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} cotscan_mppta_set_mode_mode_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,255]. Mode */
    uint8_t mode;
} cotscan_mppta_set_mode_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_set_mode_pack(can_frame_t* dst, const cotscan_mppta_set_mode_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_set_mode_unpack(cotscan_mppta_set_mode_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** StartVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. StartVoltage */
    double start_voltage;
    /** EndVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. EndVoltage */
    double end_voltage;
    /** SampleCount; unit=none; physical=raw*1+0; raw=[4,128]. SampleCount */
    uint8_t sample_count;
} cotscan_mppta_perform_sweep_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_perform_sweep_pack(can_frame_t* dst, const cotscan_mppta_perform_sweep_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_perform_sweep_unpack(cotscan_mppta_perform_sweep_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** VoltageLimit; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. VoltageLimit */
    double voltage_limit;
} cotscan_mppta_set_output_voltage_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_set_output_voltage_limit_pack(can_frame_t* dst, const cotscan_mppta_set_output_voltage_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_set_output_voltage_limit_unpack(cotscan_mppta_set_output_voltage_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** CurrentLimit; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. CurrentLimit */
    double current_limit;
} cotscan_mppta_set_output_current_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mppta_set_output_current_limit_pack(can_frame_t* dst, const cotscan_mppta_set_output_current_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mppta_set_output_current_limit_unpack(cotscan_mppta_set_output_current_limit_t* dst, const can_frame_t* src);

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
} cotscan_mpptb_power_measurements_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_power_measurements_pack(can_frame_t* dst, const cotscan_mpptb_power_measurements_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_power_measurements_unpack(cotscan_mpptb_power_measurements_t* dst, const can_frame_t* src);

/** Named raw values for OperatingMode; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_INPUT_VOLTAGE = 0,
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_INPUT_CURRENT = 1,
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_MINIMUM_INPUT_CURRENT = 2,
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_VOLTAGE = 3,
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_CURRENT = 4,
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_TEMPERATURE_DERATING = 5,
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_MINIMUM_INPUT_VOLTAGE = 6,
    COTSCAN_MPPTB_STATUS_OPERATING_MODE_FAULT = 7,
} cotscan_mpptb_status_operating_mode_choice_t;
/** Named raw values for Fault; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTB_STATUS_FAULT_OK = 0,
    COTSCAN_MPPTB_STATUS_FAULT_CONFIGURATION_ERROR = 1,
    COTSCAN_MPPTB_STATUS_FAULT_INPUT_OVER_VOLTAGE = 2,
    COTSCAN_MPPTB_STATUS_FAULT_OUTPUT_OVER_VOLTAGE = 3,
    COTSCAN_MPPTB_STATUS_FAULT_OUTPUT_OVER_CURRENT = 4,
    COTSCAN_MPPTB_STATUS_FAULT_INPUT_OVER_CURRENT = 5,
    COTSCAN_MPPTB_STATUS_FAULT_INPUT_UNDER_CURRENT = 6,
    COTSCAN_MPPTB_STATUS_FAULT_PHASE_OVER_CURRENT = 7,
    COTSCAN_MPPTB_STATUS_FAULT_FAULT = 8,
} cotscan_mpptb_status_fault_choice_t;
/** Named raw values for Enabled; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTB_STATUS_ENABLED_DISABLED = 0,
    COTSCAN_MPPTB_STATUS_ENABLED_ENABLED = 1,
} cotscan_mpptb_status_enabled_choice_t;
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
} cotscan_mpptb_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_status_pack(can_frame_t* dst, const cotscan_mpptb_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_status_unpack(cotscan_mpptb_status_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** SampleIndex; unit=none; physical=raw*1+0; raw=[0,255]. SampleIndex */
    uint8_t sample_index;
    /** Current; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. Current */
    double current;
    /** Voltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. Voltage */
    double voltage;
} cotscan_mpptb_sweep_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_sweep_data_pack(can_frame_t* dst, const cotscan_mpptb_sweep_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_sweep_data_unpack(cotscan_mpptb_sweep_data_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptb_fill_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_fill_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptb_fill_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_fill_rx_buffer_unpack(cotscan_mpptb_fill_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptb_fill_rx_buffer_long_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_fill_rx_buffer_long_pack(can_frame_t* dst, const cotscan_mpptb_fill_rx_buffer_long_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_fill_rx_buffer_long_unpack(cotscan_mpptb_fill_rx_buffer_long_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptb_process_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_process_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptb_process_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_process_rx_buffer_unpack(cotscan_mpptb_process_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptb_process_short_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_process_short_buffer_pack(can_frame_t* dst, const cotscan_mpptb_process_short_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_process_short_buffer_unpack(cotscan_mpptb_process_short_buffer_t* dst, const can_frame_t* src);

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTB_SET_MODE_MODE_DISABLED = 0,
    COTSCAN_MPPTB_SET_MODE_MODE_PERTURB_AND_OBSERVE = 1,
    COTSCAN_MPPTB_SET_MODE_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} cotscan_mpptb_set_mode_mode_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,255]. Mode */
    uint8_t mode;
} cotscan_mpptb_set_mode_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_set_mode_pack(can_frame_t* dst, const cotscan_mpptb_set_mode_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_set_mode_unpack(cotscan_mpptb_set_mode_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** StartVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. StartVoltage */
    double start_voltage;
    /** EndVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. EndVoltage */
    double end_voltage;
    /** SampleCount; unit=none; physical=raw*1+0; raw=[4,128]. SampleCount */
    uint8_t sample_count;
} cotscan_mpptb_perform_sweep_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_perform_sweep_pack(can_frame_t* dst, const cotscan_mpptb_perform_sweep_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_perform_sweep_unpack(cotscan_mpptb_perform_sweep_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** VoltageLimit; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. VoltageLimit */
    double voltage_limit;
} cotscan_mpptb_set_output_voltage_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_set_output_voltage_limit_pack(can_frame_t* dst, const cotscan_mpptb_set_output_voltage_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_set_output_voltage_limit_unpack(cotscan_mpptb_set_output_voltage_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** CurrentLimit; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. CurrentLimit */
    double current_limit;
} cotscan_mpptb_set_output_current_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptb_set_output_current_limit_pack(can_frame_t* dst, const cotscan_mpptb_set_output_current_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptb_set_output_current_limit_unpack(cotscan_mpptb_set_output_current_limit_t* dst, const can_frame_t* src);

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
} cotscan_mpptc_power_measurements_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_power_measurements_pack(can_frame_t* dst, const cotscan_mpptc_power_measurements_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_power_measurements_unpack(cotscan_mpptc_power_measurements_t* dst, const can_frame_t* src);

/** Named raw values for OperatingMode; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_INPUT_VOLTAGE = 0,
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_INPUT_CURRENT = 1,
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_MINIMUM_INPUT_CURRENT = 2,
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_VOLTAGE = 3,
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_CONSTANT_OUTPUT_CURRENT = 4,
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_TEMPERATURE_DERATING = 5,
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_MINIMUM_INPUT_VOLTAGE = 6,
    COTSCAN_MPPTC_STATUS_OPERATING_MODE_FAULT = 7,
} cotscan_mpptc_status_operating_mode_choice_t;
/** Named raw values for Fault; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTC_STATUS_FAULT_OK = 0,
    COTSCAN_MPPTC_STATUS_FAULT_CONFIGURATION_ERROR = 1,
    COTSCAN_MPPTC_STATUS_FAULT_INPUT_OVER_VOLTAGE = 2,
    COTSCAN_MPPTC_STATUS_FAULT_OUTPUT_OVER_VOLTAGE = 3,
    COTSCAN_MPPTC_STATUS_FAULT_OUTPUT_OVER_CURRENT = 4,
    COTSCAN_MPPTC_STATUS_FAULT_INPUT_OVER_CURRENT = 5,
    COTSCAN_MPPTC_STATUS_FAULT_INPUT_UNDER_CURRENT = 6,
    COTSCAN_MPPTC_STATUS_FAULT_PHASE_OVER_CURRENT = 7,
    COTSCAN_MPPTC_STATUS_FAULT_FAULT = 8,
} cotscan_mpptc_status_fault_choice_t;
/** Named raw values for Enabled; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTC_STATUS_ENABLED_DISABLED = 0,
    COTSCAN_MPPTC_STATUS_ENABLED_ENABLED = 1,
} cotscan_mpptc_status_enabled_choice_t;
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
} cotscan_mpptc_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_status_pack(can_frame_t* dst, const cotscan_mpptc_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_status_unpack(cotscan_mpptc_status_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** SampleIndex; unit=none; physical=raw*1+0; raw=[0,255]. SampleIndex */
    uint8_t sample_index;
    /** Current; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. Current */
    double current;
    /** Voltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. Voltage */
    double voltage;
} cotscan_mpptc_sweep_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_sweep_data_pack(can_frame_t* dst, const cotscan_mpptc_sweep_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_sweep_data_unpack(cotscan_mpptc_sweep_data_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptc_fill_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_fill_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptc_fill_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_fill_rx_buffer_unpack(cotscan_mpptc_fill_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptc_fill_rx_buffer_long_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_fill_rx_buffer_long_pack(can_frame_t* dst, const cotscan_mpptc_fill_rx_buffer_long_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_fill_rx_buffer_long_unpack(cotscan_mpptc_fill_rx_buffer_long_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptc_process_rx_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_process_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptc_process_rx_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_process_rx_buffer_unpack(cotscan_mpptc_process_rx_buffer_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mpptc_process_short_buffer_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_process_short_buffer_pack(can_frame_t* dst, const cotscan_mpptc_process_short_buffer_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_process_short_buffer_unpack(cotscan_mpptc_process_short_buffer_t* dst, const can_frame_t* src);

/** Named raw values for Mode; unknown values remain integers. */
typedef enum {
    COTSCAN_MPPTC_SET_MODE_MODE_DISABLED = 0,
    COTSCAN_MPPTC_SET_MODE_MODE_PERTURB_AND_OBSERVE = 1,
    COTSCAN_MPPTC_SET_MODE_MODE_CONSTANT_INPUT_VOLTAGE = 2,
} cotscan_mpptc_set_mode_mode_choice_t;
/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** Mode; unit=none; physical=raw*1+0; raw=[0,255]. Mode */
    uint8_t mode;
} cotscan_mpptc_set_mode_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_set_mode_pack(can_frame_t* dst, const cotscan_mpptc_set_mode_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_set_mode_unpack(cotscan_mpptc_set_mode_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** StartVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. StartVoltage */
    double start_voltage;
    /** EndVoltage; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. EndVoltage */
    double end_voltage;
    /** SampleCount; unit=none; physical=raw*1+0; raw=[4,128]. SampleCount */
    uint8_t sample_count;
} cotscan_mpptc_perform_sweep_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_perform_sweep_pack(can_frame_t* dst, const cotscan_mpptc_perform_sweep_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_perform_sweep_unpack(cotscan_mpptc_perform_sweep_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** VoltageLimit; unit=V; physical=raw*0.01+0; raw=[-32768,32767]. VoltageLimit */
    double voltage_limit;
} cotscan_mpptc_set_output_voltage_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_set_output_voltage_limit_pack(can_frame_t* dst, const cotscan_mpptc_set_output_voltage_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_set_output_voltage_limit_unpack(cotscan_mpptc_set_output_voltage_limit_t* dst, const can_frame_t* src);

/** TPEE SEC-B175-7A / Open-SEC. Source: can.h/can.c at 11bb2abe40cfa05160a197a04c03e1566eb88c51. Fixed classic CAN vendor format; retain signed big-endian fixed-point values. */
typedef struct {
    /** CurrentLimit; unit=A; physical=raw*0.0005+0; raw=[-32768,32767]. CurrentLimit */
    double current_limit;
} cotscan_mpptc_set_output_current_limit_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mpptc_set_output_current_limit_pack(can_frame_t* dst, const cotscan_mpptc_set_output_current_limit_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mpptc_set_output_current_limit_unpack(cotscan_mpptc_set_output_current_limit_t* dst, const can_frame_t* src);

/** Named raw values for OK; unknown values remain integers. */
typedef enum {
    COTSCAN_LWS_STANDARD_OK_INVALID = 0,
    COTSCAN_LWS_STANDARD_OK_VALID = 1,
} cotscan_lws_standard_ok_choice_t;
/** Named raw values for Calibrated; unknown values remain integers. */
typedef enum {
    COTSCAN_LWS_STANDARD_CALIBRATED_NOT_CALIBRATED = 0,
    COTSCAN_LWS_STANDARD_CALIBRATED_CALIBRATED = 1,
} cotscan_lws_standard_calibrated_choice_t;
/** Named raw values for Trimmed; unknown values remain integers. */
typedef enum {
    COTSCAN_LWS_STANDARD_TRIMMED_NOT_TRIMMED = 0,
    COTSCAN_LWS_STANDARD_TRIMMED_TRIMMED = 1,
} cotscan_lws_standard_trimmed_choice_t;
/** Bosch Motorsport LWS fixed classic CAN format; Intel little-endian. DEI is the only configuration transmitter. */
typedef struct {
    /** Angle; unit=deg; physical=raw*0.1+0; raw=[-32768,32767]. 0x7FFF is invalid/not-calibrated sentinel; validate OK/CAL/TRIM before using angle. */
    double angle;
    /** AngularSpeed; unit=deg/s; physical=raw*4+0; raw=[0,255]. 0xFF is invalid sentinel; valid measurement range is 0..1016 deg/s. */
    uint16_t angular_speed;
    /** OK; unit=none; physical=raw*1+0; raw=[0,1]. OK */
    uint8_t ok;
    /** Calibrated; unit=none; physical=raw*1+0; raw=[0,1]. Calibrated */
    uint8_t calibrated;
    /** Trimmed; unit=none; physical=raw*1+0; raw=[0,1]. Trimmed */
    uint8_t trimmed;
} cotscan_lws_standard_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_lws_standard_pack(can_frame_t* dst, const cotscan_lws_standard_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_lws_standard_unpack(cotscan_lws_standard_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorVelocitySetpoint; unit=rpm; IEEE-754 binary32. Signed velocity setpoint; reverse requires negative rpm. Operational limits are controller settings, not wire limits. */
    float mc_motor_velocity_setpoint;
    /** MC_MotorCurrentSetpoint; unit=ratio; IEEE-754 binary32. Vendor current fraction: 0.0 to 1.0, never 0 to 100. */
    float mc_motor_current_setpoint;
} cotscan_mc_drive_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_drive_command_pack(can_frame_t* dst, const cotscan_mc_drive_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_drive_command_unpack(cotscan_mc_drive_command_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorPowerSetpoint; unit=ratio; IEEE-754 binary32. Vendor current fraction: 0.0 to 1.0, never 0 to 100. */
    float mc_motor_power_setpoint;
} cotscan_mc_power_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_power_command_pack(can_frame_t* dst, const cotscan_mc_power_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_power_command_unpack(cotscan_mc_power_command_t* dst, const can_frame_t* src);

/** Defined WS22 reset trigger: eight unused data bytes, transmitted as zeros. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_mc_reset_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_reset_command_pack(can_frame_t* dst, const cotscan_mc_reset_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_reset_command_unpack(cotscan_mc_reset_command_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_TritiumID; unit=none; physical=raw*1+0; raw=[0,4294967295]. Device identifier. 0x00004003 */
    uint32_t mc_tritium_id;
    /** MC_SerialNumber; unit=none; physical=raw*1+0; raw=[0,4294967295]. Device serial number, allocated at manufacture. */
    uint32_t mc_serial_number;
} cotscan_mc_info_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_info_pack(can_frame_t* dst, const cotscan_mc_info_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_info_unpack(cotscan_mc_info_t* dst, const can_frame_t* src);

/** Named raw values for MC_LIMIT_OutputVoltagePWM; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_LIMIT_OUTPUT_VOLTAGE_PWM_OFF = 0,
    COTSCAN_MC_STATUS_MC_LIMIT_OUTPUT_VOLTAGE_PWM_ON = 1,
} cotscan_mc_status_mc_limit_output_voltage_pwm_choice_t;
/** Named raw values for MC_LIMIT_MotorCurrent; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_LIMIT_MOTOR_CURRENT_OFF = 0,
    COTSCAN_MC_STATUS_MC_LIMIT_MOTOR_CURRENT_ON = 1,
} cotscan_mc_status_mc_limit_motor_current_choice_t;
/** Named raw values for MC_LIMIT_Velocity; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_LIMIT_VELOCITY_OFF = 0,
    COTSCAN_MC_STATUS_MC_LIMIT_VELOCITY_ON = 1,
} cotscan_mc_status_mc_limit_velocity_choice_t;
/** Named raw values for MC_LIMIT_BusCurrent; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_LIMIT_BUS_CURRENT_OFF = 0,
    COTSCAN_MC_STATUS_MC_LIMIT_BUS_CURRENT_ON = 1,
} cotscan_mc_status_mc_limit_bus_current_choice_t;
/** Named raw values for MC_LIMIT_BusVoltageUpper; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_UPPER_OFF = 0,
    COTSCAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_UPPER_ON = 1,
} cotscan_mc_status_mc_limit_bus_voltage_upper_choice_t;
/** Named raw values for MC_LIMIT_BusVoltageLower; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_LOWER_OFF = 0,
    COTSCAN_MC_STATUS_MC_LIMIT_BUS_VOLTAGE_LOWER_ON = 1,
} cotscan_mc_status_mc_limit_bus_voltage_lower_choice_t;
/** Named raw values for MC_LIMIT_MotorTemp; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_LIMIT_MOTOR_TEMP_OFF = 0,
    COTSCAN_MC_STATUS_MC_LIMIT_MOTOR_TEMP_ON = 1,
} cotscan_mc_status_mc_limit_motor_temp_choice_t;
/** Named raw values for MC_FAULT_HardwareOverCurrent; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_HARDWARE_OVER_CURRENT_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_HARDWARE_OVER_CURRENT_FAULT = 1,
} cotscan_mc_status_mc_fault_hardware_over_current_choice_t;
/** Named raw values for MC_FAULT_SoftwareOverCurrent; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_SOFTWARE_OVER_CURRENT_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_SOFTWARE_OVER_CURRENT_FAULT = 1,
} cotscan_mc_status_mc_fault_software_over_current_choice_t;
/** Named raw values for MC_FAULT_DcBusOverVoltage; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_DC_BUS_OVER_VOLTAGE_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_DC_BUS_OVER_VOLTAGE_FAULT = 1,
} cotscan_mc_status_mc_fault_dc_bus_over_voltage_choice_t;
/** Named raw values for MC_FAULT_BadMotorPositionHallSeq; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_BAD_MOTOR_POSITION_HALL_SEQ_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_BAD_MOTOR_POSITION_HALL_SEQ_FAULT = 1,
} cotscan_mc_status_mc_fault_bad_motor_position_hall_seq_choice_t;
/** Named raw values for MC_FAULT_WatchdogCausedLastReset; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_WATCHDOG_CAUSED_LAST_RESET_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_WATCHDOG_CAUSED_LAST_RESET_FAULT = 1,
} cotscan_mc_status_mc_fault_watchdog_caused_last_reset_choice_t;
/** Named raw values for MC_FAULT_ConfigRead; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_CONFIG_READ_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_CONFIG_READ_FAULT = 1,
} cotscan_mc_status_mc_fault_config_read_choice_t;
/** Named raw values for MC_FAULT_15vRailUnderVoltage; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_15V_RAIL_UNDER_VOLTAGE_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_15V_RAIL_UNDER_VOLTAGE_FAULT = 1,
} cotscan_mc_status_mc_fault_15v_rail_under_voltage_choice_t;
/** Named raw values for MC_FAULT_DesaturationFault; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_DESATURATION_FAULT_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_DESATURATION_FAULT_FAULT = 1,
} cotscan_mc_status_mc_fault_desaturation_fault_choice_t;
/** Named raw values for MC_FAULT_MotorOverSpeed; unknown values remain integers. */
typedef enum {
    COTSCAN_MC_STATUS_MC_FAULT_MOTOR_OVER_SPEED_OK = 0,
    COTSCAN_MC_STATUS_MC_FAULT_MOTOR_OVER_SPEED_FAULT = 1,
} cotscan_mc_status_mc_fault_motor_over_speed_choice_t;
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
} cotscan_mc_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_status_pack(can_frame_t* dst, const cotscan_mc_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_status_unpack(cotscan_mc_status_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_BusVoltage; unit=V; IEEE-754 binary32. DC bus voltage at the controller. */
    float mc_bus_voltage;
    /** MC_BusCurrent; unit=A; IEEE-754 binary32. Current drawn from the DC bus by the controller. */
    float mc_bus_current;
} cotscan_mc_bus_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_bus_measurement_pack(can_frame_t* dst, const cotscan_mc_bus_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_bus_measurement_unpack(cotscan_mc_bus_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorVelocity; unit=rpm; IEEE-754 binary32. Motor angular frequency in revolutions per minute. */
    float mc_motor_velocity;
    /** MC_VehicleVelocity; unit=m/s; IEEE-754 binary32. Vehicle velocity in metres / second. */
    float mc_vehicle_velocity;
} cotscan_mc_velocity_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_velocity_measurement_pack(can_frame_t* dst, const cotscan_mc_velocity_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_velocity_measurement_unpack(cotscan_mc_velocity_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_PhaseCurrentB; unit=A; IEEE-754 binary32. RMS current in motor Phase B. */
    float mc_phase_current_b;
    /** MC_PhaseCurrentC; unit=A; IEEE-754 binary32. RMS current in motor Phase C. */
    float mc_phase_current_c;
} cotscan_mc_phase_current_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_phase_current_measurement_pack(can_frame_t* dst, const cotscan_mc_phase_current_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_phase_current_measurement_unpack(cotscan_mc_phase_current_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Vq; unit=V; IEEE-754 binary32. Imaginary component of the applied non-rotating voltage vector to the motor. */
    float mc_vq;
    /** MC_Vd; unit=V; IEEE-754 binary32. Real component of the applied non-rotating voltage vector to the motor. */
    float mc_vd;
} cotscan_mc_motor_voltage_vector_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_motor_voltage_vector_measurement_pack(can_frame_t* dst, const cotscan_mc_motor_voltage_vector_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_motor_voltage_vector_measurement_unpack(cotscan_mc_motor_voltage_vector_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Iq; unit=A; IEEE-754 binary32. Imaginary component of the applied non-rotating current vector to the motor. This current produces torque in the motor and should be in phase with the back-EMF of the motor */
    float mc_iq;
    /** MC_Id; unit=A; IEEE-754 binary32. Real component of the applied non-rotating current vector to the motor. This vector represents the field current of the motor. */
    float mc_id;
} cotscan_mc_motor_current_vector_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_motor_current_vector_measurement_pack(can_frame_t* dst, const cotscan_mc_motor_current_vector_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_motor_current_vector_measurement_unpack(cotscan_mc_motor_current_vector_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_BEMFq; unit=V; IEEE-754 binary32. The peak of the phase to neutral motor voltage. */
    float mc_bem_fq;
    /** MC_BEMFd; unit=V; IEEE-754 binary32. By definition this value is always 0V. */
    float mc_bem_fd;
} cotscan_mc_back_emf_measurement_prediction_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_back_emf_measurement_prediction_pack(can_frame_t* dst, const cotscan_mc_back_emf_measurement_prediction_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_back_emf_measurement_prediction_unpack(cotscan_mc_back_emf_measurement_prediction_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Supply15V; unit=V; IEEE-754 binary32. Actual voltage level of the 15V power rail. */
    float mc_supply15_v;
} cotscan_mc_15_v_rail_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_15_v_rail_measurement_pack(can_frame_t* dst, const cotscan_mc_15_v_rail_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_15_v_rail_measurement_unpack(cotscan_mc_15_v_rail_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_Supply1V9; unit=V; IEEE-754 binary32. Actual voltage level of the 1.9V DSP power rail. */
    float mc_supply1_v9;
    /** MC_Supply3V3; unit=V; IEEE-754 binary32. Actual voltage level of the 3.3V power rail. */
    float mc_supply3_v3;
} cotscan_mc_3_v3_19_v_rail_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_3_v3_19_v_rail_measurement_pack(can_frame_t* dst, const cotscan_mc_3_v3_19_v_rail_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_3_v3_19_v_rail_measurement_unpack(cotscan_mc_3_v3_19_v_rail_measurement_t* dst, const can_frame_t* src);

/** Vendor-defined reserved frame; fixed eight-byte payload, no signals assigned. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_ws22_reserved0_a_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_ws22_reserved0_a_pack(can_frame_t* dst, const cotscan_ws22_reserved0_a_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_ws22_reserved0_a_unpack(cotscan_ws22_reserved0_a_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_MotorTemp; unit=degC; IEEE-754 binary32. Internal temperature of the motor */
    float mc_motor_temp;
    /** MC_HeatsinkTemp; unit=degC; IEEE-754 binary32. Internal temperature of Heat-sink (case). */
    float mc_heatsink_temp;
} cotscan_mc_motor_temp_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_motor_temp_measurement_pack(can_frame_t* dst, const cotscan_mc_motor_temp_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_motor_temp_measurement_unpack(cotscan_mc_motor_temp_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_DspBoardTemp; unit=degC; IEEE-754 binary32. Temperature of the DSP board. */
    float mc_dsp_board_temp;
} cotscan_mc_dsp_board_temp_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_dsp_board_temp_measurement_pack(can_frame_t* dst, const cotscan_mc_dsp_board_temp_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_dsp_board_temp_measurement_unpack(cotscan_mc_dsp_board_temp_measurement_t* dst, const can_frame_t* src);

/** Vendor-defined reserved frame; fixed eight-byte payload, no signals assigned. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} cotscan_ws22_reserved0_d_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_ws22_reserved0_d_pack(can_frame_t* dst, const cotscan_ws22_reserved0_d_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_ws22_reserved0_d_unpack(cotscan_ws22_reserved0_d_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_TripOdometer; unit=m; IEEE-754 binary32. Distance the vehicle has travelled since reset. */
    float mc_trip_odometer;
    /** MC_DCBusAh; unit=Ah; IEEE-754 binary32. Charge flow into the controller DC bus from the time of reset. */
    float mc_dc_bus_ah;
} cotscan_mc_odometer_bus_ah_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_odometer_bus_ah_measurement_pack(can_frame_t* dst, const cotscan_mc_odometer_bus_ah_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_odometer_bus_ah_measurement_unpack(cotscan_mc_odometer_bus_ah_measurement_t* dst, const can_frame_t* src);

/** Fixed WS22 classic CAN: little-endian IEEE-754 floats; vendor units and wire layout are exempt from team scaling. All WS22 frames have eight data bytes. */
typedef struct {
    /** MC_SlipSpeed; unit=Hz; IEEE-754 binary32. Slip speed when driving an induction motor. */
    float mc_slip_speed;
} cotscan_mc_slip_speed_measurement_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_mc_slip_speed_measurement_pack(can_frame_t* dst, const cotscan_mc_slip_speed_measurement_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_mc_slip_speed_measurement_unpack(cotscan_mc_slip_speed_measurement_t* dst, const can_frame_t* src);

/** Named raw values for Command; unknown values remain integers. */
typedef enum {
    COTSCAN_LWS_CONFIG_COMMAND_SET_ANGLE_ZERO = 3,
    COTSCAN_LWS_CONFIG_COMMAND_RESET_CALIBRATION = 5,
} cotscan_lws_config_command_choice_t;
/** Bosch Motorsport LWS fixed classic CAN format; Intel little-endian. DEI is the only configuration transmitter. */
typedef struct {
    /** Command; unit=none; physical=raw*1+0; raw=[0,15]. Command */
    uint8_t command;
} cotscan_lws_config_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t cotscan_lws_config_pack(can_frame_t* dst, const cotscan_lws_config_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t cotscan_lws_config_unpack(cotscan_lws_config_t* dst, const can_frame_t* src);

typedef struct {
    cotscan_lws_config_t lws_config;
} cotscan_dei_t;

typedef struct {
    cotscan_lws_standard_t lws_standard;
} cotscan_lws_t;

typedef struct {
    cotscan_mppta_power_measurements_t mppta_power_measurements;
    cotscan_mppta_status_t mppta_status;
    cotscan_mppta_sweep_data_t mppta_sweep_data;
} cotscan_mppta_t;

typedef struct {
    cotscan_mpptb_power_measurements_t mpptb_power_measurements;
    cotscan_mpptb_status_t mpptb_status;
    cotscan_mpptb_sweep_data_t mpptb_sweep_data;
} cotscan_mpptb_t;

typedef struct {
    cotscan_mpptc_power_measurements_t mpptc_power_measurements;
    cotscan_mpptc_status_t mpptc_status;
    cotscan_mpptc_sweep_data_t mpptc_sweep_data;
} cotscan_mpptc_t;

typedef struct {
    cotscan_mppta_fill_rx_buffer_t mppta_fill_rx_buffer;
    cotscan_mppta_fill_rx_buffer_long_t mppta_fill_rx_buffer_long;
    cotscan_mppta_process_rx_buffer_t mppta_process_rx_buffer;
    cotscan_mppta_process_short_buffer_t mppta_process_short_buffer;
    cotscan_mppta_set_mode_t mppta_set_mode;
    cotscan_mppta_perform_sweep_t mppta_perform_sweep;
    cotscan_mppta_set_output_voltage_limit_t mppta_set_output_voltage_limit;
    cotscan_mppta_set_output_current_limit_t mppta_set_output_current_limit;
    cotscan_mpptb_fill_rx_buffer_t mpptb_fill_rx_buffer;
    cotscan_mpptb_fill_rx_buffer_long_t mpptb_fill_rx_buffer_long;
    cotscan_mpptb_process_rx_buffer_t mpptb_process_rx_buffer;
    cotscan_mpptb_process_short_buffer_t mpptb_process_short_buffer;
    cotscan_mpptb_set_mode_t mpptb_set_mode;
    cotscan_mpptb_perform_sweep_t mpptb_perform_sweep;
    cotscan_mpptb_set_output_voltage_limit_t mpptb_set_output_voltage_limit;
    cotscan_mpptb_set_output_current_limit_t mpptb_set_output_current_limit;
    cotscan_mpptc_fill_rx_buffer_t mpptc_fill_rx_buffer;
    cotscan_mpptc_fill_rx_buffer_long_t mpptc_fill_rx_buffer_long;
    cotscan_mpptc_process_rx_buffer_t mpptc_process_rx_buffer;
    cotscan_mpptc_process_short_buffer_t mpptc_process_short_buffer;
    cotscan_mpptc_set_mode_t mpptc_set_mode;
    cotscan_mpptc_perform_sweep_t mpptc_perform_sweep;
    cotscan_mpptc_set_output_voltage_limit_t mpptc_set_output_voltage_limit;
    cotscan_mpptc_set_output_current_limit_t mpptc_set_output_current_limit;
    cotscan_mc_drive_command_t mc_drive_command;
    cotscan_mc_power_command_t mc_power_command;
    cotscan_mc_reset_command_t mc_reset_command;
} cotscan_vcu_t;

typedef struct {
    cotscan_mc_info_t mc_info;
    cotscan_mc_status_t mc_status;
    cotscan_mc_bus_measurement_t mc_bus_measurement;
    cotscan_mc_velocity_measurement_t mc_velocity_measurement;
    cotscan_mc_phase_current_measurement_t mc_phase_current_measurement;
    cotscan_mc_motor_voltage_vector_measurement_t mc_motor_voltage_vector_measurement;
    cotscan_mc_motor_current_vector_measurement_t mc_motor_current_vector_measurement;
    cotscan_mc_back_emf_measurement_prediction_t mc_back_emf_measurement_prediction;
    cotscan_mc_15_v_rail_measurement_t mc_15_v_rail_measurement;
    cotscan_mc_3_v3_19_v_rail_measurement_t mc_3_v3_19_v_rail_measurement;
    cotscan_ws22_reserved0_a_t ws22_reserved0_a;
    cotscan_mc_motor_temp_measurement_t mc_motor_temp_measurement;
    cotscan_mc_dsp_board_temp_measurement_t mc_dsp_board_temp_measurement;
    cotscan_ws22_reserved0_d_t ws22_reserved0_d;
    cotscan_mc_odometer_bus_ah_measurement_t mc_odometer_bus_ah_measurement;
    cotscan_mc_slip_speed_measurement_t mc_slip_speed_measurement;
} cotscan_ws22_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t lws_config;
    } dei;
    struct {
        can_message_info_t lws_standard;
    } lws;
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
} cotscan_metadata_t;
extern const cotscan_metadata_t cotscan;
