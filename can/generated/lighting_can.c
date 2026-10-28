#include "lighting_can.h"
#include "detail/lighting_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const lighting_can_metadata_t lighting_can = {
    .dei = {
        .bottom_shell_lighting_status = { .id = UINT32_C(128), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = false },
        .lighting_command = { .id = UINT32_C(131), .payload_bytes = 4, .wire_bytes = 4, .dlc = 4, .is_extended = false, .is_fd = false },
    },
    .top_shell_lighting = {
        .top_shell_lighting_status = { .id = UINT32_C(129), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = false },
    },
};

can_status_t lighting_can_bottom_shell_lighting_status_pack(can_frame_t* dst, const lighting_can_bottom_shell_lighting_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct lighting_can_raw_bottom_shell_lighting_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->left_signal_on;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.left_signal_on = (uint8_t)value;
    }
    {
        uint8_t value = src->right_signal_on;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.right_signal_on = (uint8_t)value;
    }
    {
        uint8_t value = src->headlights_on;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.headlights_on = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(128),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (lighting_can_raw_bottom_shell_lighting_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t lighting_can_bottom_shell_lighting_status_unpack(lighting_can_bottom_shell_lighting_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(128) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct lighting_can_raw_bottom_shell_lighting_status_t raw = {0};
    if (lighting_can_raw_bottom_shell_lighting_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    lighting_can_bottom_shell_lighting_status_t result = {0};
    if (raw.left_signal_on > INT64_C(1)) return CAN_RANGE;
    result.left_signal_on = (uint8_t)raw.left_signal_on;
    if (raw.right_signal_on > INT64_C(1)) return CAN_RANGE;
    result.right_signal_on = (uint8_t)raw.right_signal_on;
    if (raw.headlights_on > INT64_C(1)) return CAN_RANGE;
    result.headlights_on = (uint8_t)raw.headlights_on;
    *dst = result;
    return CAN_OK;
}

can_status_t lighting_can_top_shell_lighting_status_pack(can_frame_t* dst, const lighting_can_top_shell_lighting_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct lighting_can_raw_top_shell_lighting_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->brake_on;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.brake_on = (uint8_t)value;
    }
    {
        uint8_t value = src->left_signal_on;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.left_signal_on = (uint8_t)value;
    }
    {
        uint8_t value = src->right_signal_on;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.right_signal_on = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(129),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (lighting_can_raw_top_shell_lighting_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t lighting_can_top_shell_lighting_status_unpack(lighting_can_top_shell_lighting_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(129) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct lighting_can_raw_top_shell_lighting_status_t raw = {0};
    if (lighting_can_raw_top_shell_lighting_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    lighting_can_top_shell_lighting_status_t result = {0};
    if (raw.brake_on > INT64_C(1)) return CAN_RANGE;
    result.brake_on = (uint8_t)raw.brake_on;
    if (raw.left_signal_on > INT64_C(1)) return CAN_RANGE;
    result.left_signal_on = (uint8_t)raw.left_signal_on;
    if (raw.right_signal_on > INT64_C(1)) return CAN_RANGE;
    result.right_signal_on = (uint8_t)raw.right_signal_on;
    *dst = result;
    return CAN_OK;
}

can_status_t lighting_can_lighting_command_pack(can_frame_t* dst, const lighting_can_lighting_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct lighting_can_raw_lighting_command_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->sync;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sync = (uint8_t)value;
    }
    {
        uint8_t value = src->headlight;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.headlight = (uint8_t)value;
    }
    {
        uint8_t value = src->left;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.left = (uint8_t)value;
    }
    {
        uint8_t value = src->right;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.right = (uint8_t)value;
    }
    {
        uint8_t value = src->brake;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.brake = (uint8_t)value;
    }
    {
        uint8_t value = src->strobe;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.strobe = (uint8_t)value;
    }
    {
        uint8_t value = src->rear_right_pwm;
        raw.rear_right_pwm = (uint8_t)value;
    }
    {
        uint8_t value = src->rear_center_pwm;
        raw.rear_center_pwm = (uint8_t)value;
    }
    {
        uint8_t value = src->rear_left_pwm;
        raw.rear_left_pwm = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(131),
        .len = 4, .is_extended = false,
        .is_fd = false };
    if (lighting_can_raw_lighting_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t lighting_can_lighting_command_unpack(lighting_can_lighting_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(131) || src->len != 4 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct lighting_can_raw_lighting_command_t raw = {0};
    if (lighting_can_raw_lighting_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    lighting_can_lighting_command_t result = {0};
    if (raw.sync > INT64_C(1)) return CAN_RANGE;
    result.sync = (uint8_t)raw.sync;
    if (raw.headlight > INT64_C(1)) return CAN_RANGE;
    result.headlight = (uint8_t)raw.headlight;
    if (raw.left > INT64_C(1)) return CAN_RANGE;
    result.left = (uint8_t)raw.left;
    if (raw.right > INT64_C(1)) return CAN_RANGE;
    result.right = (uint8_t)raw.right;
    if (raw.brake > INT64_C(1)) return CAN_RANGE;
    result.brake = (uint8_t)raw.brake;
    if (raw.strobe > INT64_C(1)) return CAN_RANGE;
    result.strobe = (uint8_t)raw.strobe;
    result.rear_right_pwm = (uint8_t)raw.rear_right_pwm;
    result.rear_center_pwm = (uint8_t)raw.rear_center_pwm;
    result.rear_left_pwm = (uint8_t)raw.rear_left_pwm;
    *dst = result;
    return CAN_OK;
}
