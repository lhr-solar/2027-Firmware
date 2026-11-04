#include "steering_can.h"
#include "detail/steering_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const steering_can_metadata_t steering_can = {
    .dei = {
        .lws_config = { .id = UINT32_C(1984), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
    },
    .lws = {
        .lws_standard = { .id = UINT32_C(688), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
    },
};

can_status_t steering_can_lws_standard_pack(can_frame_t* dst, const steering_can_lws_standard_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct steering_can_raw_lws_standard_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->angle;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -3276.8) { value = -3276.8; status = CAN_CLAMPED; }
        if (value > 3276.7) { value = 3276.7; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.1);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.angle = (int16_t)encoded;
    }
    {
        double value = (double)src->angular_speed;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 1020.0) { value = 1020.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 4.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 255.0) encoded = 255.0;
        raw.angular_speed = (uint8_t)encoded;
    }
    {
        uint8_t value = src->ok;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.ok = (uint8_t)value;
    }
    {
        uint8_t value = src->calibrated;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.calibrated = (uint8_t)value;
    }
    {
        uint8_t value = src->trimmed;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trimmed = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(688),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (steering_can_raw_lws_standard_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t steering_can_lws_standard_unpack(steering_can_lws_standard_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(688) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct steering_can_raw_lws_standard_t raw = {0};
    if (steering_can_raw_lws_standard_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    steering_can_lws_standard_t result = {0};
    result.angle = (double)((double)raw.angle * 0.1 + 0.0);
    result.angular_speed = (uint16_t)((double)raw.angular_speed * 4.0 + 0.0);
    if (raw.ok > INT64_C(1)) return CAN_RANGE;
    result.ok = (uint8_t)raw.ok;
    if (raw.calibrated > INT64_C(1)) return CAN_RANGE;
    result.calibrated = (uint8_t)raw.calibrated;
    if (raw.trimmed > INT64_C(1)) return CAN_RANGE;
    result.trimmed = (uint8_t)raw.trimmed;
    *dst = result;
    return CAN_OK;
}

can_status_t steering_can_lws_config_pack(can_frame_t* dst, const steering_can_lws_config_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct steering_can_raw_lws_config_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->command;
        if (value > INT64_C(15)) {
            value = (uint8_t)INT64_C(15); status = CAN_CLAMPED;
        }
        raw.command = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1984),
        .len = 2, .is_extended = false,
        .is_fd = false };
    if (steering_can_raw_lws_config_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t steering_can_lws_config_unpack(steering_can_lws_config_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1984) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct steering_can_raw_lws_config_t raw = {0};
    if (steering_can_raw_lws_config_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    steering_can_lws_config_t result = {0};
    if (raw.command > INT64_C(15)) return CAN_RANGE;
    result.command = (uint8_t)raw.command;
    *dst = result;
    return CAN_OK;
}
