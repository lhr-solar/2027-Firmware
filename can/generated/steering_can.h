/** @file steering_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Named raw values for OK; unknown values remain integers. */
typedef enum {
    STEERING_CAN_LWS_STANDARD_OK_INVALID = 0,
    STEERING_CAN_LWS_STANDARD_OK_VALID = 1,
} steering_can_lws_standard_ok_choice_t;
/** Named raw values for Calibrated; unknown values remain integers. */
typedef enum {
    STEERING_CAN_LWS_STANDARD_CALIBRATED_NOT_CALIBRATED = 0,
    STEERING_CAN_LWS_STANDARD_CALIBRATED_CALIBRATED = 1,
} steering_can_lws_standard_calibrated_choice_t;
/** Named raw values for Trimmed; unknown values remain integers. */
typedef enum {
    STEERING_CAN_LWS_STANDARD_TRIMMED_NOT_TRIMMED = 0,
    STEERING_CAN_LWS_STANDARD_TRIMMED_TRIMMED = 1,
} steering_can_lws_standard_trimmed_choice_t;
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
} steering_can_lws_standard_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t steering_can_lws_standard_pack(can_frame_t* dst, const steering_can_lws_standard_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t steering_can_lws_standard_unpack(steering_can_lws_standard_t* dst, const can_frame_t* src);

/** Named raw values for Command; unknown values remain integers. */
typedef enum {
    STEERING_CAN_LWS_CONFIG_COMMAND_SET_ANGLE_ZERO = 3,
    STEERING_CAN_LWS_CONFIG_COMMAND_RESET_CALIBRATION = 5,
} steering_can_lws_config_command_choice_t;
/** Bosch Motorsport LWS fixed classic CAN format; Intel little-endian. DEI is the only configuration transmitter. */
typedef struct {
    /** Command; unit=none; physical=raw*1+0; raw=[0,15]. Command */
    uint8_t command;
} steering_can_lws_config_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t steering_can_lws_config_pack(can_frame_t* dst, const steering_can_lws_config_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t steering_can_lws_config_unpack(steering_can_lws_config_t* dst, const can_frame_t* src);

typedef struct {
    steering_can_lws_config_t lws_config;
} steering_can_dei_t;

typedef struct {
    steering_can_lws_standard_t lws_standard;
} steering_can_lws_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t lws_config;
    } dei;
    struct {
        can_message_info_t lws_standard;
    } lws;
} steering_can_metadata_t;
extern const steering_can_metadata_t steering_can;
