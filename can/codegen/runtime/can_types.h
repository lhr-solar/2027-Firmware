/**
 * @file can_types.h
 * @brief Transport-independent frames shared by generated CAN codecs.
 *
 * Set physical fields, pack into a caller-owned frame, then send with a driver.
 * Unpack validates frame metadata before updating caller-owned physical fields.
 * No allocation, global mutable state, HAL dependency, or transport side effects.
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    CAN_OK = 0,
    CAN_CLAMPED = 1,  /**< Pack succeeded; at least one input exceeded reachable limits. */
    CAN_INVALID = -1, /**< Null argument, nonfinite input, bad metadata or multiplexor. */
    CAN_RANGE = -2    /**< Received signal is outside the declared DBC limits. */
} can_status_t;

typedef struct {
    uint32_t id; /**< Actual 11/29-bit identifier, without DBC's extended-ID marker. */
    uint8_t len; /**< Legal wire length in bytes, not the hardware DLC encoding. */
    bool is_extended;
    bool is_fd;
    bool bitrate_switch; /**< Pack defaults to false; the sending driver/caller selects BRS. */
    uint8_t data[64];    /**< Pack zeroes unused bytes, including FD padding. */
} can_frame_t;

typedef struct {
    uint32_t id;
    uint8_t payload_bytes; /**< Signal layout length declared in the DBC. */
    uint8_t wire_bytes;    /**< Smallest legal CAN-FD length containing the payload. */
    uint8_t dlc;           /**< Numeric DLC 0..15; a HAL adapter may need to shift/translate it. */
    bool is_extended;
    bool is_fd;
} can_message_info_t;
