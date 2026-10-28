#include "mpptcan.h"
#include "detail/mpptcan_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const mpptcan_metadata_t mpptcan = {
    .mppta = {
        .mppta_power_measurements = { .id = UINT32_C(512), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mppta_status = { .id = UINT32_C(513), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
        .mppta_sweep_data = { .id = UINT32_C(514), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
    },
    .mpptb = {
        .mpptb_power_measurements = { .id = UINT32_C(544), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mpptb_status = { .id = UINT32_C(545), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
        .mpptb_sweep_data = { .id = UINT32_C(546), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
    },
    .mpptc = {
        .mpptc_power_measurements = { .id = UINT32_C(576), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mpptc_status = { .id = UINT32_C(577), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
        .mpptc_sweep_data = { .id = UINT32_C(578), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
    },
    .mppt_interface = {
        .mppta_fill_rx_buffer = { .id = UINT32_C(517), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mppta_fill_rx_buffer_long = { .id = UINT32_C(518), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mppta_process_rx_buffer = { .id = UINT32_C(519), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mppta_process_short_buffer = { .id = UINT32_C(520), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mppta_set_mode = { .id = UINT32_C(521), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = false },
        .mppta_perform_sweep = { .id = UINT32_C(522), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
        .mppta_set_output_voltage_limit = { .id = UINT32_C(523), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
        .mppta_set_output_current_limit = { .id = UINT32_C(524), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
        .mpptb_fill_rx_buffer = { .id = UINT32_C(549), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptb_fill_rx_buffer_long = { .id = UINT32_C(550), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptb_process_rx_buffer = { .id = UINT32_C(551), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptb_process_short_buffer = { .id = UINT32_C(552), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptb_set_mode = { .id = UINT32_C(553), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = false },
        .mpptb_perform_sweep = { .id = UINT32_C(554), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
        .mpptb_set_output_voltage_limit = { .id = UINT32_C(555), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
        .mpptb_set_output_current_limit = { .id = UINT32_C(556), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
        .mpptc_fill_rx_buffer = { .id = UINT32_C(581), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptc_fill_rx_buffer_long = { .id = UINT32_C(582), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptc_process_rx_buffer = { .id = UINT32_C(583), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptc_process_short_buffer = { .id = UINT32_C(584), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = false },
        .mpptc_set_mode = { .id = UINT32_C(585), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = false },
        .mpptc_perform_sweep = { .id = UINT32_C(586), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
        .mpptc_set_output_voltage_limit = { .id = UINT32_C(587), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
        .mpptc_set_output_current_limit = { .id = UINT32_C(588), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
    },
};

can_status_t mpptcan_mppta_power_measurements_pack(can_frame_t* dst, const mpptcan_mppta_power_measurements_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_power_measurements_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->input_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.input_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->input_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.input_current = (int16_t)encoded;
    }
    {
        double value = (double)src->output_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.output_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->output_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.output_current = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(512),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_power_measurements_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_power_measurements_unpack(mpptcan_mppta_power_measurements_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(512) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_power_measurements_t raw = {0};
    if (mpptcan_raw_mppta_power_measurements_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_power_measurements_t result = {0};
    result.input_voltage = (double)((double)raw.input_voltage * 0.01 + 0.0);
    result.input_current = (double)((double)raw.input_current * 0.0005 + 0.0);
    result.output_voltage = (double)((double)raw.output_voltage * 0.01 + 0.0);
    result.output_current = (double)((double)raw.output_current * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_status_pack(can_frame_t* dst, const mpptcan_mppta_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->operating_mode;
        raw.operating_mode = (uint8_t)value;
    }
    {
        uint8_t value = src->fault;
        raw.fault = (uint8_t)value;
    }
    {
        uint8_t value = src->enabled;
        raw.enabled = (uint8_t)value;
    }
    {
        int8_t value = src->ambient_temperature;
        raw.ambient_temperature = (int8_t)value;
    }
    {
        int8_t value = src->heatsink_temperature;
        raw.heatsink_temperature = (int8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(513),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_status_unpack(mpptcan_mppta_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(513) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_status_t raw = {0};
    if (mpptcan_raw_mppta_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_status_t result = {0};
    result.operating_mode = (uint8_t)raw.operating_mode;
    result.fault = (uint8_t)raw.fault;
    result.enabled = (uint8_t)raw.enabled;
    result.ambient_temperature = (int8_t)raw.ambient_temperature;
    result.heatsink_temperature = (int8_t)raw.heatsink_temperature;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_sweep_data_pack(can_frame_t* dst, const mpptcan_mppta_sweep_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_sweep_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->sample_index;
        raw.sample_index = (uint8_t)value;
    }
    {
        double value = (double)src->current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.current = (int16_t)encoded;
    }
    {
        double value = (double)src->voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.voltage = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(514),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_sweep_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_sweep_data_unpack(mpptcan_mppta_sweep_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(514) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_sweep_data_t raw = {0};
    if (mpptcan_raw_mppta_sweep_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_sweep_data_t result = {0};
    result.sample_index = (uint8_t)raw.sample_index;
    result.current = (double)((double)raw.current * 0.0005 + 0.0);
    result.voltage = (double)((double)raw.voltage * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_fill_rx_buffer_pack(can_frame_t* dst, const mpptcan_mppta_fill_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_fill_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(517),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_fill_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_fill_rx_buffer_unpack(mpptcan_mppta_fill_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(517) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_fill_rx_buffer_t raw = {0};
    if (mpptcan_raw_mppta_fill_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_fill_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_fill_rx_buffer_long_pack(can_frame_t* dst, const mpptcan_mppta_fill_rx_buffer_long_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_fill_rx_buffer_long_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(518),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_fill_rx_buffer_long_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_fill_rx_buffer_long_unpack(mpptcan_mppta_fill_rx_buffer_long_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(518) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_fill_rx_buffer_long_t raw = {0};
    if (mpptcan_raw_mppta_fill_rx_buffer_long_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_fill_rx_buffer_long_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_process_rx_buffer_pack(can_frame_t* dst, const mpptcan_mppta_process_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_process_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(519),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_process_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_process_rx_buffer_unpack(mpptcan_mppta_process_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(519) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_process_rx_buffer_t raw = {0};
    if (mpptcan_raw_mppta_process_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_process_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_process_short_buffer_pack(can_frame_t* dst, const mpptcan_mppta_process_short_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_process_short_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(520),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_process_short_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_process_short_buffer_unpack(mpptcan_mppta_process_short_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(520) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_process_short_buffer_t raw = {0};
    if (mpptcan_raw_mppta_process_short_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_process_short_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_set_mode_pack(can_frame_t* dst, const mpptcan_mppta_set_mode_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_set_mode_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        raw.mode = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(521),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_set_mode_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_set_mode_unpack(mpptcan_mppta_set_mode_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(521) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_set_mode_t raw = {0};
    if (mpptcan_raw_mppta_set_mode_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_set_mode_t result = {0};
    result.mode = (uint8_t)raw.mode;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_perform_sweep_pack(can_frame_t* dst, const mpptcan_mppta_perform_sweep_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_perform_sweep_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->start_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.start_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->end_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.end_voltage = (int16_t)encoded;
    }
    {
        uint8_t value = src->sample_count;
        if (value < INT64_C(4)) {
            value = (uint8_t)INT64_C(4); status = CAN_CLAMPED;
        }
        if (value > INT64_C(128)) {
            value = (uint8_t)INT64_C(128); status = CAN_CLAMPED;
        }
        raw.sample_count = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(522),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_perform_sweep_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_perform_sweep_unpack(mpptcan_mppta_perform_sweep_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(522) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_perform_sweep_t raw = {0};
    if (mpptcan_raw_mppta_perform_sweep_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_perform_sweep_t result = {0};
    result.start_voltage = (double)((double)raw.start_voltage * 0.01 + 0.0);
    result.end_voltage = (double)((double)raw.end_voltage * 0.01 + 0.0);
    if (raw.sample_count < INT64_C(4)) return CAN_RANGE;
    if (raw.sample_count > INT64_C(128)) return CAN_RANGE;
    result.sample_count = (uint8_t)raw.sample_count;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_set_output_voltage_limit_pack(can_frame_t* dst, const mpptcan_mppta_set_output_voltage_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_set_output_voltage_limit_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->voltage_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.voltage_limit = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(523),
        .len = 2, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_set_output_voltage_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_set_output_voltage_limit_unpack(mpptcan_mppta_set_output_voltage_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(523) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_set_output_voltage_limit_t raw = {0};
    if (mpptcan_raw_mppta_set_output_voltage_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_set_output_voltage_limit_t result = {0};
    result.voltage_limit = (double)((double)raw.voltage_limit * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mppta_set_output_current_limit_pack(can_frame_t* dst, const mpptcan_mppta_set_output_current_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mppta_set_output_current_limit_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->current_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.current_limit = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(524),
        .len = 2, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mppta_set_output_current_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mppta_set_output_current_limit_unpack(mpptcan_mppta_set_output_current_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(524) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mppta_set_output_current_limit_t raw = {0};
    if (mpptcan_raw_mppta_set_output_current_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mppta_set_output_current_limit_t result = {0};
    result.current_limit = (double)((double)raw.current_limit * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_power_measurements_pack(can_frame_t* dst, const mpptcan_mpptb_power_measurements_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_power_measurements_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->input_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.input_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->input_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.input_current = (int16_t)encoded;
    }
    {
        double value = (double)src->output_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.output_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->output_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.output_current = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(544),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_power_measurements_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_power_measurements_unpack(mpptcan_mpptb_power_measurements_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(544) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_power_measurements_t raw = {0};
    if (mpptcan_raw_mpptb_power_measurements_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_power_measurements_t result = {0};
    result.input_voltage = (double)((double)raw.input_voltage * 0.01 + 0.0);
    result.input_current = (double)((double)raw.input_current * 0.0005 + 0.0);
    result.output_voltage = (double)((double)raw.output_voltage * 0.01 + 0.0);
    result.output_current = (double)((double)raw.output_current * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_status_pack(can_frame_t* dst, const mpptcan_mpptb_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->operating_mode;
        raw.operating_mode = (uint8_t)value;
    }
    {
        uint8_t value = src->fault;
        raw.fault = (uint8_t)value;
    }
    {
        uint8_t value = src->enabled;
        raw.enabled = (uint8_t)value;
    }
    {
        int8_t value = src->ambient_temperature;
        raw.ambient_temperature = (int8_t)value;
    }
    {
        int8_t value = src->heatsink_temperature;
        raw.heatsink_temperature = (int8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(545),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_status_unpack(mpptcan_mpptb_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(545) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_status_t raw = {0};
    if (mpptcan_raw_mpptb_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_status_t result = {0};
    result.operating_mode = (uint8_t)raw.operating_mode;
    result.fault = (uint8_t)raw.fault;
    result.enabled = (uint8_t)raw.enabled;
    result.ambient_temperature = (int8_t)raw.ambient_temperature;
    result.heatsink_temperature = (int8_t)raw.heatsink_temperature;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_sweep_data_pack(can_frame_t* dst, const mpptcan_mpptb_sweep_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_sweep_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->sample_index;
        raw.sample_index = (uint8_t)value;
    }
    {
        double value = (double)src->current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.current = (int16_t)encoded;
    }
    {
        double value = (double)src->voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.voltage = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(546),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_sweep_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_sweep_data_unpack(mpptcan_mpptb_sweep_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(546) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_sweep_data_t raw = {0};
    if (mpptcan_raw_mpptb_sweep_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_sweep_data_t result = {0};
    result.sample_index = (uint8_t)raw.sample_index;
    result.current = (double)((double)raw.current * 0.0005 + 0.0);
    result.voltage = (double)((double)raw.voltage * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_fill_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptb_fill_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_fill_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(549),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_fill_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_fill_rx_buffer_unpack(mpptcan_mpptb_fill_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(549) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_fill_rx_buffer_t raw = {0};
    if (mpptcan_raw_mpptb_fill_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_fill_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_fill_rx_buffer_long_pack(can_frame_t* dst, const mpptcan_mpptb_fill_rx_buffer_long_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_fill_rx_buffer_long_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(550),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_fill_rx_buffer_long_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_fill_rx_buffer_long_unpack(mpptcan_mpptb_fill_rx_buffer_long_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(550) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_fill_rx_buffer_long_t raw = {0};
    if (mpptcan_raw_mpptb_fill_rx_buffer_long_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_fill_rx_buffer_long_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_process_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptb_process_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_process_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(551),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_process_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_process_rx_buffer_unpack(mpptcan_mpptb_process_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(551) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_process_rx_buffer_t raw = {0};
    if (mpptcan_raw_mpptb_process_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_process_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_process_short_buffer_pack(can_frame_t* dst, const mpptcan_mpptb_process_short_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_process_short_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(552),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_process_short_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_process_short_buffer_unpack(mpptcan_mpptb_process_short_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(552) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_process_short_buffer_t raw = {0};
    if (mpptcan_raw_mpptb_process_short_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_process_short_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_set_mode_pack(can_frame_t* dst, const mpptcan_mpptb_set_mode_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_set_mode_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        raw.mode = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(553),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_set_mode_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_set_mode_unpack(mpptcan_mpptb_set_mode_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(553) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_set_mode_t raw = {0};
    if (mpptcan_raw_mpptb_set_mode_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_set_mode_t result = {0};
    result.mode = (uint8_t)raw.mode;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_perform_sweep_pack(can_frame_t* dst, const mpptcan_mpptb_perform_sweep_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_perform_sweep_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->start_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.start_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->end_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.end_voltage = (int16_t)encoded;
    }
    {
        uint8_t value = src->sample_count;
        if (value < INT64_C(4)) {
            value = (uint8_t)INT64_C(4); status = CAN_CLAMPED;
        }
        if (value > INT64_C(128)) {
            value = (uint8_t)INT64_C(128); status = CAN_CLAMPED;
        }
        raw.sample_count = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(554),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_perform_sweep_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_perform_sweep_unpack(mpptcan_mpptb_perform_sweep_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(554) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_perform_sweep_t raw = {0};
    if (mpptcan_raw_mpptb_perform_sweep_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_perform_sweep_t result = {0};
    result.start_voltage = (double)((double)raw.start_voltage * 0.01 + 0.0);
    result.end_voltage = (double)((double)raw.end_voltage * 0.01 + 0.0);
    if (raw.sample_count < INT64_C(4)) return CAN_RANGE;
    if (raw.sample_count > INT64_C(128)) return CAN_RANGE;
    result.sample_count = (uint8_t)raw.sample_count;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_set_output_voltage_limit_pack(can_frame_t* dst, const mpptcan_mpptb_set_output_voltage_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_set_output_voltage_limit_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->voltage_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.voltage_limit = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(555),
        .len = 2, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_set_output_voltage_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_set_output_voltage_limit_unpack(mpptcan_mpptb_set_output_voltage_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(555) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_set_output_voltage_limit_t raw = {0};
    if (mpptcan_raw_mpptb_set_output_voltage_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_set_output_voltage_limit_t result = {0};
    result.voltage_limit = (double)((double)raw.voltage_limit * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptb_set_output_current_limit_pack(can_frame_t* dst, const mpptcan_mpptb_set_output_current_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptb_set_output_current_limit_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->current_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.current_limit = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(556),
        .len = 2, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptb_set_output_current_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptb_set_output_current_limit_unpack(mpptcan_mpptb_set_output_current_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(556) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptb_set_output_current_limit_t raw = {0};
    if (mpptcan_raw_mpptb_set_output_current_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptb_set_output_current_limit_t result = {0};
    result.current_limit = (double)((double)raw.current_limit * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_power_measurements_pack(can_frame_t* dst, const mpptcan_mpptc_power_measurements_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_power_measurements_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->input_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.input_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->input_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.input_current = (int16_t)encoded;
    }
    {
        double value = (double)src->output_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.output_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->output_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.output_current = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(576),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_power_measurements_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_power_measurements_unpack(mpptcan_mpptc_power_measurements_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(576) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_power_measurements_t raw = {0};
    if (mpptcan_raw_mpptc_power_measurements_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_power_measurements_t result = {0};
    result.input_voltage = (double)((double)raw.input_voltage * 0.01 + 0.0);
    result.input_current = (double)((double)raw.input_current * 0.0005 + 0.0);
    result.output_voltage = (double)((double)raw.output_voltage * 0.01 + 0.0);
    result.output_current = (double)((double)raw.output_current * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_status_pack(can_frame_t* dst, const mpptcan_mpptc_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->operating_mode;
        raw.operating_mode = (uint8_t)value;
    }
    {
        uint8_t value = src->fault;
        raw.fault = (uint8_t)value;
    }
    {
        uint8_t value = src->enabled;
        raw.enabled = (uint8_t)value;
    }
    {
        int8_t value = src->ambient_temperature;
        raw.ambient_temperature = (int8_t)value;
    }
    {
        int8_t value = src->heatsink_temperature;
        raw.heatsink_temperature = (int8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(577),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_status_unpack(mpptcan_mpptc_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(577) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_status_t raw = {0};
    if (mpptcan_raw_mpptc_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_status_t result = {0};
    result.operating_mode = (uint8_t)raw.operating_mode;
    result.fault = (uint8_t)raw.fault;
    result.enabled = (uint8_t)raw.enabled;
    result.ambient_temperature = (int8_t)raw.ambient_temperature;
    result.heatsink_temperature = (int8_t)raw.heatsink_temperature;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_sweep_data_pack(can_frame_t* dst, const mpptcan_mpptc_sweep_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_sweep_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->sample_index;
        raw.sample_index = (uint8_t)value;
    }
    {
        double value = (double)src->current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.current = (int16_t)encoded;
    }
    {
        double value = (double)src->voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.voltage = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(578),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_sweep_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_sweep_data_unpack(mpptcan_mpptc_sweep_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(578) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_sweep_data_t raw = {0};
    if (mpptcan_raw_mpptc_sweep_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_sweep_data_t result = {0};
    result.sample_index = (uint8_t)raw.sample_index;
    result.current = (double)((double)raw.current * 0.0005 + 0.0);
    result.voltage = (double)((double)raw.voltage * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_fill_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptc_fill_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_fill_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(581),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_fill_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_fill_rx_buffer_unpack(mpptcan_mpptc_fill_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(581) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_fill_rx_buffer_t raw = {0};
    if (mpptcan_raw_mpptc_fill_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_fill_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_fill_rx_buffer_long_pack(can_frame_t* dst, const mpptcan_mpptc_fill_rx_buffer_long_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_fill_rx_buffer_long_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(582),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_fill_rx_buffer_long_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_fill_rx_buffer_long_unpack(mpptcan_mpptc_fill_rx_buffer_long_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(582) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_fill_rx_buffer_long_t raw = {0};
    if (mpptcan_raw_mpptc_fill_rx_buffer_long_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_fill_rx_buffer_long_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_process_rx_buffer_pack(can_frame_t* dst, const mpptcan_mpptc_process_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_process_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(583),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_process_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_process_rx_buffer_unpack(mpptcan_mpptc_process_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(583) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_process_rx_buffer_t raw = {0};
    if (mpptcan_raw_mpptc_process_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_process_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_process_short_buffer_pack(can_frame_t* dst, const mpptcan_mpptc_process_short_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_process_short_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(584),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_process_short_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_process_short_buffer_unpack(mpptcan_mpptc_process_short_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(584) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_process_short_buffer_t raw = {0};
    if (mpptcan_raw_mpptc_process_short_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_process_short_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_set_mode_pack(can_frame_t* dst, const mpptcan_mpptc_set_mode_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_set_mode_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        raw.mode = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(585),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_set_mode_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_set_mode_unpack(mpptcan_mpptc_set_mode_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(585) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_set_mode_t raw = {0};
    if (mpptcan_raw_mpptc_set_mode_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_set_mode_t result = {0};
    result.mode = (uint8_t)raw.mode;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_perform_sweep_pack(can_frame_t* dst, const mpptcan_mpptc_perform_sweep_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_perform_sweep_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->start_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.start_voltage = (int16_t)encoded;
    }
    {
        double value = (double)src->end_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.end_voltage = (int16_t)encoded;
    }
    {
        uint8_t value = src->sample_count;
        if (value < INT64_C(4)) {
            value = (uint8_t)INT64_C(4); status = CAN_CLAMPED;
        }
        if (value > INT64_C(128)) {
            value = (uint8_t)INT64_C(128); status = CAN_CLAMPED;
        }
        raw.sample_count = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(586),
        .len = 5, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_perform_sweep_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_perform_sweep_unpack(mpptcan_mpptc_perform_sweep_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(586) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_perform_sweep_t raw = {0};
    if (mpptcan_raw_mpptc_perform_sweep_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_perform_sweep_t result = {0};
    result.start_voltage = (double)((double)raw.start_voltage * 0.01 + 0.0);
    result.end_voltage = (double)((double)raw.end_voltage * 0.01 + 0.0);
    if (raw.sample_count < INT64_C(4)) return CAN_RANGE;
    if (raw.sample_count > INT64_C(128)) return CAN_RANGE;
    result.sample_count = (uint8_t)raw.sample_count;
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_set_output_voltage_limit_pack(can_frame_t* dst, const mpptcan_mpptc_set_output_voltage_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_set_output_voltage_limit_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->voltage_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -327.68) { value = -327.68; status = CAN_CLAMPED; }
        if (value > 327.67) { value = 327.67; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.voltage_limit = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(587),
        .len = 2, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_set_output_voltage_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_set_output_voltage_limit_unpack(mpptcan_mpptc_set_output_voltage_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(587) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_set_output_voltage_limit_t raw = {0};
    if (mpptcan_raw_mpptc_set_output_voltage_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_set_output_voltage_limit_t result = {0};
    result.voltage_limit = (double)((double)raw.voltage_limit * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t mpptcan_mpptc_set_output_current_limit_pack(can_frame_t* dst, const mpptcan_mpptc_set_output_current_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct mpptcan_raw_mpptc_set_output_current_limit_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->current_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.3835) { value = 16.3835; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.0005);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.current_limit = (int16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(588),
        .len = 2, .is_extended = false,
        .is_fd = false };
    if (mpptcan_raw_mpptc_set_output_current_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t mpptcan_mpptc_set_output_current_limit_unpack(mpptcan_mpptc_set_output_current_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(588) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct mpptcan_raw_mpptc_set_output_current_limit_t raw = {0};
    if (mpptcan_raw_mpptc_set_output_current_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    mpptcan_mpptc_set_output_current_limit_t result = {0};
    result.current_limit = (double)((double)raw.current_limit * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}
