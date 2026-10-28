/** @file lighting_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Named raw values for LeftSignalOn; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_BOTTOM_SHELL_LIGHTING_STATUS_LEFT_SIGNAL_ON_OFF = 0,
    LIGHTING_CAN_BOTTOM_SHELL_LIGHTING_STATUS_LEFT_SIGNAL_ON_ON = 1,
} lighting_can_bottom_shell_lighting_status_left_signal_on_choice_t;
/** Named raw values for RightSignalOn; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_BOTTOM_SHELL_LIGHTING_STATUS_RIGHT_SIGNAL_ON_OFF = 0,
    LIGHTING_CAN_BOTTOM_SHELL_LIGHTING_STATUS_RIGHT_SIGNAL_ON_ON = 1,
} lighting_can_bottom_shell_lighting_status_right_signal_on_choice_t;
/** Named raw values for HeadlightsOn; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_BOTTOM_SHELL_LIGHTING_STATUS_HEADLIGHTS_ON_OFF = 0,
    LIGHTING_CAN_BOTTOM_SHELL_LIGHTING_STATUS_HEADLIGHTS_ON_ON = 1,
} lighting_can_bottom_shell_lighting_status_headlights_on_choice_t;
/** Bottom-shell lighting circuitry is integrated on DEI; DEI transmits this status. No separate BottomShellLighting CAN node. */
typedef struct {
    /** LeftSignalOn; unit=none; physical=raw*1+0; raw=[0,1]. LeftSignalOn */
    uint8_t left_signal_on;
    /** RightSignalOn; unit=none; physical=raw*1+0; raw=[0,1]. RightSignalOn */
    uint8_t right_signal_on;
    /** HeadlightsOn; unit=none; physical=raw*1+0; raw=[0,1]. HeadlightsOn */
    uint8_t headlights_on;
} lighting_can_bottom_shell_lighting_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t lighting_can_bottom_shell_lighting_status_pack(can_frame_t* dst, const lighting_can_bottom_shell_lighting_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t lighting_can_bottom_shell_lighting_status_unpack(lighting_can_bottom_shell_lighting_status_t* dst, const can_frame_t* src);

/** Named raw values for BrakeOn; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_TOP_SHELL_LIGHTING_STATUS_BRAKE_ON_OFF = 0,
    LIGHTING_CAN_TOP_SHELL_LIGHTING_STATUS_BRAKE_ON_ON = 1,
} lighting_can_top_shell_lighting_status_brake_on_choice_t;
/** Named raw values for LeftSignalOn; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_TOP_SHELL_LIGHTING_STATUS_LEFT_SIGNAL_ON_OFF = 0,
    LIGHTING_CAN_TOP_SHELL_LIGHTING_STATUS_LEFT_SIGNAL_ON_ON = 1,
} lighting_can_top_shell_lighting_status_left_signal_on_choice_t;
/** Named raw values for RightSignalOn; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_TOP_SHELL_LIGHTING_STATUS_RIGHT_SIGNAL_ON_OFF = 0,
    LIGHTING_CAN_TOP_SHELL_LIGHTING_STATUS_RIGHT_SIGNAL_ON_ON = 1,
} lighting_can_top_shell_lighting_status_right_signal_on_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** BrakeOn; unit=none; physical=raw*1+0; raw=[0,1]. BrakeOn */
    uint8_t brake_on;
    /** LeftSignalOn; unit=none; physical=raw*1+0; raw=[0,1]. LeftSignalOn */
    uint8_t left_signal_on;
    /** RightSignalOn; unit=none; physical=raw*1+0; raw=[0,1]. RightSignalOn */
    uint8_t right_signal_on;
} lighting_can_top_shell_lighting_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t lighting_can_top_shell_lighting_status_pack(can_frame_t* dst, const lighting_can_top_shell_lighting_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t lighting_can_top_shell_lighting_status_unpack(lighting_can_top_shell_lighting_status_t* dst, const can_frame_t* src);

/** Named raw values for Sync; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_LIGHTING_COMMAND_SYNC_OFF = 0,
    LIGHTING_CAN_LIGHTING_COMMAND_SYNC_ON = 1,
} lighting_can_lighting_command_sync_choice_t;
/** Named raw values for Headlight; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_LIGHTING_COMMAND_HEADLIGHT_OFF = 0,
    LIGHTING_CAN_LIGHTING_COMMAND_HEADLIGHT_ON = 1,
} lighting_can_lighting_command_headlight_choice_t;
/** Named raw values for Left; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_LIGHTING_COMMAND_LEFT_OFF = 0,
    LIGHTING_CAN_LIGHTING_COMMAND_LEFT_ON = 1,
} lighting_can_lighting_command_left_choice_t;
/** Named raw values for Right; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_LIGHTING_COMMAND_RIGHT_OFF = 0,
    LIGHTING_CAN_LIGHTING_COMMAND_RIGHT_ON = 1,
} lighting_can_lighting_command_right_choice_t;
/** Named raw values for Brake; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_LIGHTING_COMMAND_BRAKE_OFF = 0,
    LIGHTING_CAN_LIGHTING_COMMAND_BRAKE_ON = 1,
} lighting_can_lighting_command_brake_choice_t;
/** Named raw values for Strobe; unknown values remain integers. */
typedef enum {
    LIGHTING_CAN_LIGHTING_COMMAND_STROBE_OFF = 0,
    LIGHTING_CAN_LIGHTING_COMMAND_STROBE_ON = 1,
} lighting_can_lighting_command_strobe_choice_t;
/** Cyclic lighting commands from DEI; rear PWM raw counts are 0-255. */
typedef struct {
    /** Sync; unit=none; physical=raw*1+0; raw=[0,1]. Synchronization signal previously sent in BottomShellLightingStatus. */
    uint8_t sync;
    /** Headlight; unit=none; physical=raw*1+0; raw=[0,1]. Headlight lighting command. */
    uint8_t headlight;
    /** Left; unit=none; physical=raw*1+0; raw=[0,1]. Left lighting command. */
    uint8_t left;
    /** Right; unit=none; physical=raw*1+0; raw=[0,1]. Right lighting command. */
    uint8_t right;
    /** Brake; unit=none; physical=raw*1+0; raw=[0,1]. Brake lighting command. */
    uint8_t brake;
    /** Strobe; unit=none; physical=raw*1+0; raw=[0,1]. Strobe lighting command. */
    uint8_t strobe;
    /** RearRightPWM; unit=none; physical=raw*1+0; raw=[0,255]. PWM command: 0 = off, 255 = full duty. */
    uint8_t rear_right_pwm;
    /** RearCenterPWM; unit=none; physical=raw*1+0; raw=[0,255]. PWM command: 0 = off, 255 = full duty. */
    uint8_t rear_center_pwm;
    /** RearLeftPWM; unit=none; physical=raw*1+0; raw=[0,255]. PWM command: 0 = off, 255 = full duty. */
    uint8_t rear_left_pwm;
} lighting_can_lighting_command_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t lighting_can_lighting_command_pack(can_frame_t* dst, const lighting_can_lighting_command_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t lighting_can_lighting_command_unpack(lighting_can_lighting_command_t* dst, const can_frame_t* src);

typedef struct {
    lighting_can_bottom_shell_lighting_status_t bottom_shell_lighting_status;
    lighting_can_lighting_command_t lighting_command;
} lighting_can_dei_t;

typedef struct {
    lighting_can_top_shell_lighting_status_t top_shell_lighting_status;
} lighting_can_top_shell_lighting_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t bottom_shell_lighting_status;
        can_message_info_t lighting_command;
    } dei;
    struct {
        can_message_info_t top_shell_lighting_status;
    } top_shell_lighting;
} lighting_can_metadata_t;
extern const lighting_can_metadata_t lighting_can;
