#include "cotscan.h"
#include "detail/cotscan_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const cotscan_metadata_t cotscan = {
    .dei = {
        .lws_config = { .id = UINT32_C(1984), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = false },
    },
    .lws = {
        .lws_standard = { .id = UINT32_C(688), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = false },
    },
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
    .vcu = {
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
        .mc_drive_command = { .id = UINT32_C(1025), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_power_command = { .id = UINT32_C(1026), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_reset_command = { .id = UINT32_C(1027), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
    },
    .ws22 = {
        .mc_info = { .id = UINT32_C(1056), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_status = { .id = UINT32_C(1057), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_bus_measurement = { .id = UINT32_C(1058), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_velocity_measurement = { .id = UINT32_C(1059), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_phase_current_measurement = { .id = UINT32_C(1060), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_motor_voltage_vector_measurement = { .id = UINT32_C(1061), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_motor_current_vector_measurement = { .id = UINT32_C(1062), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_back_emf_measurement_prediction = { .id = UINT32_C(1063), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_15_v_rail_measurement = { .id = UINT32_C(1064), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_3_v3_19_v_rail_measurement = { .id = UINT32_C(1065), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .ws22_reserved0_a = { .id = UINT32_C(1066), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_motor_temp_measurement = { .id = UINT32_C(1067), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_dsp_board_temp_measurement = { .id = UINT32_C(1068), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .ws22_reserved0_d = { .id = UINT32_C(1069), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_odometer_bus_ah_measurement = { .id = UINT32_C(1070), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .mc_slip_speed_measurement = { .id = UINT32_C(1079), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
    },
};

can_status_t cotscan_mppta_power_measurements_pack(can_frame_t* dst, const cotscan_mppta_power_measurements_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_power_measurements_t raw = {0};
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
    if (cotscan_raw_mppta_power_measurements_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_power_measurements_unpack(cotscan_mppta_power_measurements_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(512) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_power_measurements_t raw = {0};
    if (cotscan_raw_mppta_power_measurements_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_power_measurements_t result = {0};
    result.input_voltage = (double)((double)raw.input_voltage * 0.01 + 0.0);
    result.input_current = (double)((double)raw.input_current * 0.0005 + 0.0);
    result.output_voltage = (double)((double)raw.output_voltage * 0.01 + 0.0);
    result.output_current = (double)((double)raw.output_current * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_status_pack(can_frame_t* dst, const cotscan_mppta_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_status_t raw = {0};
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
    if (cotscan_raw_mppta_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_status_unpack(cotscan_mppta_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(513) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_status_t raw = {0};
    if (cotscan_raw_mppta_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_status_t result = {0};
    result.operating_mode = (uint8_t)raw.operating_mode;
    result.fault = (uint8_t)raw.fault;
    result.enabled = (uint8_t)raw.enabled;
    result.ambient_temperature = (int8_t)raw.ambient_temperature;
    result.heatsink_temperature = (int8_t)raw.heatsink_temperature;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_sweep_data_pack(can_frame_t* dst, const cotscan_mppta_sweep_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_sweep_data_t raw = {0};
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
    if (cotscan_raw_mppta_sweep_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_sweep_data_unpack(cotscan_mppta_sweep_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(514) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_sweep_data_t raw = {0};
    if (cotscan_raw_mppta_sweep_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_sweep_data_t result = {0};
    result.sample_index = (uint8_t)raw.sample_index;
    result.current = (double)((double)raw.current * 0.0005 + 0.0);
    result.voltage = (double)((double)raw.voltage * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_fill_rx_buffer_pack(can_frame_t* dst, const cotscan_mppta_fill_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_fill_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(517),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mppta_fill_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_fill_rx_buffer_unpack(cotscan_mppta_fill_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(517) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_fill_rx_buffer_t raw = {0};
    if (cotscan_raw_mppta_fill_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_fill_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_fill_rx_buffer_long_pack(can_frame_t* dst, const cotscan_mppta_fill_rx_buffer_long_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_fill_rx_buffer_long_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(518),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mppta_fill_rx_buffer_long_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_fill_rx_buffer_long_unpack(cotscan_mppta_fill_rx_buffer_long_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(518) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_fill_rx_buffer_long_t raw = {0};
    if (cotscan_raw_mppta_fill_rx_buffer_long_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_fill_rx_buffer_long_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_process_rx_buffer_pack(can_frame_t* dst, const cotscan_mppta_process_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_process_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(519),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mppta_process_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_process_rx_buffer_unpack(cotscan_mppta_process_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(519) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_process_rx_buffer_t raw = {0};
    if (cotscan_raw_mppta_process_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_process_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_process_short_buffer_pack(can_frame_t* dst, const cotscan_mppta_process_short_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_process_short_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(520),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mppta_process_short_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_process_short_buffer_unpack(cotscan_mppta_process_short_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(520) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_process_short_buffer_t raw = {0};
    if (cotscan_raw_mppta_process_short_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_process_short_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_set_mode_pack(can_frame_t* dst, const cotscan_mppta_set_mode_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_set_mode_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        raw.mode = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(521),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mppta_set_mode_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_set_mode_unpack(cotscan_mppta_set_mode_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(521) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_set_mode_t raw = {0};
    if (cotscan_raw_mppta_set_mode_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_set_mode_t result = {0};
    result.mode = (uint8_t)raw.mode;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_perform_sweep_pack(can_frame_t* dst, const cotscan_mppta_perform_sweep_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_perform_sweep_t raw = {0};
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
    if (cotscan_raw_mppta_perform_sweep_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_perform_sweep_unpack(cotscan_mppta_perform_sweep_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(522) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_perform_sweep_t raw = {0};
    if (cotscan_raw_mppta_perform_sweep_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_perform_sweep_t result = {0};
    result.start_voltage = (double)((double)raw.start_voltage * 0.01 + 0.0);
    result.end_voltage = (double)((double)raw.end_voltage * 0.01 + 0.0);
    if (raw.sample_count < INT64_C(4)) return CAN_RANGE;
    if (raw.sample_count > INT64_C(128)) return CAN_RANGE;
    result.sample_count = (uint8_t)raw.sample_count;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_set_output_voltage_limit_pack(can_frame_t* dst, const cotscan_mppta_set_output_voltage_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_set_output_voltage_limit_t raw = {0};
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
    if (cotscan_raw_mppta_set_output_voltage_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_set_output_voltage_limit_unpack(cotscan_mppta_set_output_voltage_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(523) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_set_output_voltage_limit_t raw = {0};
    if (cotscan_raw_mppta_set_output_voltage_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_set_output_voltage_limit_t result = {0};
    result.voltage_limit = (double)((double)raw.voltage_limit * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mppta_set_output_current_limit_pack(can_frame_t* dst, const cotscan_mppta_set_output_current_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mppta_set_output_current_limit_t raw = {0};
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
    if (cotscan_raw_mppta_set_output_current_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mppta_set_output_current_limit_unpack(cotscan_mppta_set_output_current_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(524) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mppta_set_output_current_limit_t raw = {0};
    if (cotscan_raw_mppta_set_output_current_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mppta_set_output_current_limit_t result = {0};
    result.current_limit = (double)((double)raw.current_limit * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_power_measurements_pack(can_frame_t* dst, const cotscan_mpptb_power_measurements_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_power_measurements_t raw = {0};
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
    if (cotscan_raw_mpptb_power_measurements_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_power_measurements_unpack(cotscan_mpptb_power_measurements_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(544) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_power_measurements_t raw = {0};
    if (cotscan_raw_mpptb_power_measurements_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_power_measurements_t result = {0};
    result.input_voltage = (double)((double)raw.input_voltage * 0.01 + 0.0);
    result.input_current = (double)((double)raw.input_current * 0.0005 + 0.0);
    result.output_voltage = (double)((double)raw.output_voltage * 0.01 + 0.0);
    result.output_current = (double)((double)raw.output_current * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_status_pack(can_frame_t* dst, const cotscan_mpptb_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_status_t raw = {0};
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
    if (cotscan_raw_mpptb_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_status_unpack(cotscan_mpptb_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(545) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_status_t raw = {0};
    if (cotscan_raw_mpptb_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_status_t result = {0};
    result.operating_mode = (uint8_t)raw.operating_mode;
    result.fault = (uint8_t)raw.fault;
    result.enabled = (uint8_t)raw.enabled;
    result.ambient_temperature = (int8_t)raw.ambient_temperature;
    result.heatsink_temperature = (int8_t)raw.heatsink_temperature;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_sweep_data_pack(can_frame_t* dst, const cotscan_mpptb_sweep_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_sweep_data_t raw = {0};
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
    if (cotscan_raw_mpptb_sweep_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_sweep_data_unpack(cotscan_mpptb_sweep_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(546) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_sweep_data_t raw = {0};
    if (cotscan_raw_mpptb_sweep_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_sweep_data_t result = {0};
    result.sample_index = (uint8_t)raw.sample_index;
    result.current = (double)((double)raw.current * 0.0005 + 0.0);
    result.voltage = (double)((double)raw.voltage * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_fill_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptb_fill_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_fill_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(549),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptb_fill_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_fill_rx_buffer_unpack(cotscan_mpptb_fill_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(549) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_fill_rx_buffer_t raw = {0};
    if (cotscan_raw_mpptb_fill_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_fill_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_fill_rx_buffer_long_pack(can_frame_t* dst, const cotscan_mpptb_fill_rx_buffer_long_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_fill_rx_buffer_long_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(550),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptb_fill_rx_buffer_long_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_fill_rx_buffer_long_unpack(cotscan_mpptb_fill_rx_buffer_long_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(550) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_fill_rx_buffer_long_t raw = {0};
    if (cotscan_raw_mpptb_fill_rx_buffer_long_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_fill_rx_buffer_long_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_process_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptb_process_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_process_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(551),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptb_process_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_process_rx_buffer_unpack(cotscan_mpptb_process_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(551) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_process_rx_buffer_t raw = {0};
    if (cotscan_raw_mpptb_process_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_process_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_process_short_buffer_pack(can_frame_t* dst, const cotscan_mpptb_process_short_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_process_short_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(552),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptb_process_short_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_process_short_buffer_unpack(cotscan_mpptb_process_short_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(552) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_process_short_buffer_t raw = {0};
    if (cotscan_raw_mpptb_process_short_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_process_short_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_set_mode_pack(can_frame_t* dst, const cotscan_mpptb_set_mode_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_set_mode_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        raw.mode = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(553),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptb_set_mode_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_set_mode_unpack(cotscan_mpptb_set_mode_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(553) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_set_mode_t raw = {0};
    if (cotscan_raw_mpptb_set_mode_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_set_mode_t result = {0};
    result.mode = (uint8_t)raw.mode;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_perform_sweep_pack(can_frame_t* dst, const cotscan_mpptb_perform_sweep_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_perform_sweep_t raw = {0};
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
    if (cotscan_raw_mpptb_perform_sweep_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_perform_sweep_unpack(cotscan_mpptb_perform_sweep_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(554) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_perform_sweep_t raw = {0};
    if (cotscan_raw_mpptb_perform_sweep_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_perform_sweep_t result = {0};
    result.start_voltage = (double)((double)raw.start_voltage * 0.01 + 0.0);
    result.end_voltage = (double)((double)raw.end_voltage * 0.01 + 0.0);
    if (raw.sample_count < INT64_C(4)) return CAN_RANGE;
    if (raw.sample_count > INT64_C(128)) return CAN_RANGE;
    result.sample_count = (uint8_t)raw.sample_count;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_set_output_voltage_limit_pack(can_frame_t* dst, const cotscan_mpptb_set_output_voltage_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_set_output_voltage_limit_t raw = {0};
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
    if (cotscan_raw_mpptb_set_output_voltage_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_set_output_voltage_limit_unpack(cotscan_mpptb_set_output_voltage_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(555) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_set_output_voltage_limit_t raw = {0};
    if (cotscan_raw_mpptb_set_output_voltage_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_set_output_voltage_limit_t result = {0};
    result.voltage_limit = (double)((double)raw.voltage_limit * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptb_set_output_current_limit_pack(can_frame_t* dst, const cotscan_mpptb_set_output_current_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptb_set_output_current_limit_t raw = {0};
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
    if (cotscan_raw_mpptb_set_output_current_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptb_set_output_current_limit_unpack(cotscan_mpptb_set_output_current_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(556) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptb_set_output_current_limit_t raw = {0};
    if (cotscan_raw_mpptb_set_output_current_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptb_set_output_current_limit_t result = {0};
    result.current_limit = (double)((double)raw.current_limit * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_power_measurements_pack(can_frame_t* dst, const cotscan_mpptc_power_measurements_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_power_measurements_t raw = {0};
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
    if (cotscan_raw_mpptc_power_measurements_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_power_measurements_unpack(cotscan_mpptc_power_measurements_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(576) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_power_measurements_t raw = {0};
    if (cotscan_raw_mpptc_power_measurements_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_power_measurements_t result = {0};
    result.input_voltage = (double)((double)raw.input_voltage * 0.01 + 0.0);
    result.input_current = (double)((double)raw.input_current * 0.0005 + 0.0);
    result.output_voltage = (double)((double)raw.output_voltage * 0.01 + 0.0);
    result.output_current = (double)((double)raw.output_current * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_status_pack(can_frame_t* dst, const cotscan_mpptc_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_status_t raw = {0};
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
    if (cotscan_raw_mpptc_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_status_unpack(cotscan_mpptc_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(577) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_status_t raw = {0};
    if (cotscan_raw_mpptc_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_status_t result = {0};
    result.operating_mode = (uint8_t)raw.operating_mode;
    result.fault = (uint8_t)raw.fault;
    result.enabled = (uint8_t)raw.enabled;
    result.ambient_temperature = (int8_t)raw.ambient_temperature;
    result.heatsink_temperature = (int8_t)raw.heatsink_temperature;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_sweep_data_pack(can_frame_t* dst, const cotscan_mpptc_sweep_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_sweep_data_t raw = {0};
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
    if (cotscan_raw_mpptc_sweep_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_sweep_data_unpack(cotscan_mpptc_sweep_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(578) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_sweep_data_t raw = {0};
    if (cotscan_raw_mpptc_sweep_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_sweep_data_t result = {0};
    result.sample_index = (uint8_t)raw.sample_index;
    result.current = (double)((double)raw.current * 0.0005 + 0.0);
    result.voltage = (double)((double)raw.voltage * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_fill_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptc_fill_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_fill_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(581),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptc_fill_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_fill_rx_buffer_unpack(cotscan_mpptc_fill_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(581) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_fill_rx_buffer_t raw = {0};
    if (cotscan_raw_mpptc_fill_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_fill_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_fill_rx_buffer_long_pack(can_frame_t* dst, const cotscan_mpptc_fill_rx_buffer_long_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_fill_rx_buffer_long_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(582),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptc_fill_rx_buffer_long_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_fill_rx_buffer_long_unpack(cotscan_mpptc_fill_rx_buffer_long_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(582) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_fill_rx_buffer_long_t raw = {0};
    if (cotscan_raw_mpptc_fill_rx_buffer_long_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_fill_rx_buffer_long_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_process_rx_buffer_pack(can_frame_t* dst, const cotscan_mpptc_process_rx_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_process_rx_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(583),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptc_process_rx_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_process_rx_buffer_unpack(cotscan_mpptc_process_rx_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(583) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_process_rx_buffer_t raw = {0};
    if (cotscan_raw_mpptc_process_rx_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_process_rx_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_process_short_buffer_pack(can_frame_t* dst, const cotscan_mpptc_process_short_buffer_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_process_short_buffer_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(584),
        .len = 0, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptc_process_short_buffer_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_process_short_buffer_unpack(cotscan_mpptc_process_short_buffer_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(584) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_process_short_buffer_t raw = {0};
    if (cotscan_raw_mpptc_process_short_buffer_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_process_short_buffer_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_set_mode_pack(can_frame_t* dst, const cotscan_mpptc_set_mode_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_set_mode_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        raw.mode = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(585),
        .len = 1, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mpptc_set_mode_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_set_mode_unpack(cotscan_mpptc_set_mode_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(585) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_set_mode_t raw = {0};
    if (cotscan_raw_mpptc_set_mode_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_set_mode_t result = {0};
    result.mode = (uint8_t)raw.mode;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_perform_sweep_pack(can_frame_t* dst, const cotscan_mpptc_perform_sweep_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_perform_sweep_t raw = {0};
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
    if (cotscan_raw_mpptc_perform_sweep_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_perform_sweep_unpack(cotscan_mpptc_perform_sweep_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(586) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_perform_sweep_t raw = {0};
    if (cotscan_raw_mpptc_perform_sweep_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_perform_sweep_t result = {0};
    result.start_voltage = (double)((double)raw.start_voltage * 0.01 + 0.0);
    result.end_voltage = (double)((double)raw.end_voltage * 0.01 + 0.0);
    if (raw.sample_count < INT64_C(4)) return CAN_RANGE;
    if (raw.sample_count > INT64_C(128)) return CAN_RANGE;
    result.sample_count = (uint8_t)raw.sample_count;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_set_output_voltage_limit_pack(can_frame_t* dst, const cotscan_mpptc_set_output_voltage_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_set_output_voltage_limit_t raw = {0};
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
    if (cotscan_raw_mpptc_set_output_voltage_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_set_output_voltage_limit_unpack(cotscan_mpptc_set_output_voltage_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(587) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_set_output_voltage_limit_t raw = {0};
    if (cotscan_raw_mpptc_set_output_voltage_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_set_output_voltage_limit_t result = {0};
    result.voltage_limit = (double)((double)raw.voltage_limit * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mpptc_set_output_current_limit_pack(can_frame_t* dst, const cotscan_mpptc_set_output_current_limit_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mpptc_set_output_current_limit_t raw = {0};
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
    if (cotscan_raw_mpptc_set_output_current_limit_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mpptc_set_output_current_limit_unpack(cotscan_mpptc_set_output_current_limit_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(588) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mpptc_set_output_current_limit_t raw = {0};
    if (cotscan_raw_mpptc_set_output_current_limit_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mpptc_set_output_current_limit_t result = {0};
    result.current_limit = (double)((double)raw.current_limit * 0.0005 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_lws_standard_pack(can_frame_t* dst, const cotscan_lws_standard_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_lws_standard_t raw = {0};
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
    if (cotscan_raw_lws_standard_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_lws_standard_unpack(cotscan_lws_standard_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(688) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_lws_standard_t raw = {0};
    if (cotscan_raw_lws_standard_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_lws_standard_t result = {0};
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

can_status_t cotscan_mc_drive_command_pack(can_frame_t* dst, const cotscan_mc_drive_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_drive_command_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_motor_velocity_setpoint;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_motor_velocity_setpoint, &value, sizeof(value));
    }
    {
        float value = src->mc_motor_current_setpoint;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = (float)0.0; status = CAN_CLAMPED; }
        if (value > 1.0) { value = (float)1.0; status = CAN_CLAMPED; }
        memcpy(&raw.mc_motor_current_setpoint, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1025),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_drive_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_drive_command_unpack(cotscan_mc_drive_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1025) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_drive_command_t raw = {0};
    if (cotscan_raw_mc_drive_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_drive_command_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_motor_velocity_setpoint, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_motor_velocity_setpoint = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_motor_current_setpoint, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) return CAN_RANGE;
        if (value > 1.0) return CAN_RANGE;
        result.mc_motor_current_setpoint = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_power_command_pack(can_frame_t* dst, const cotscan_mc_power_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_power_command_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_motor_power_setpoint;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = (float)0.0; status = CAN_CLAMPED; }
        if (value > 1.0) { value = (float)1.0; status = CAN_CLAMPED; }
        memcpy(&raw.mc_motor_power_setpoint, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1026),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_power_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_power_command_unpack(cotscan_mc_power_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1026) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_power_command_t raw = {0};
    if (cotscan_raw_mc_power_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_power_command_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_motor_power_setpoint, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) return CAN_RANGE;
        if (value > 1.0) return CAN_RANGE;
        result.mc_motor_power_setpoint = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_reset_command_pack(can_frame_t* dst, const cotscan_mc_reset_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_reset_command_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1027),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_reset_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_reset_command_unpack(cotscan_mc_reset_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1027) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_reset_command_t raw = {0};
    if (cotscan_raw_mc_reset_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_reset_command_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_info_pack(can_frame_t* dst, const cotscan_mc_info_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_info_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint32_t value = src->mc_tritium_id;
        raw.mc_tritium_id = (uint32_t)value;
    }
    {
        uint32_t value = src->mc_serial_number;
        raw.mc_serial_number = (uint32_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1056),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_info_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_info_unpack(cotscan_mc_info_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1056) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_info_t raw = {0};
    if (cotscan_raw_mc_info_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_info_t result = {0};
    result.mc_tritium_id = (uint32_t)raw.mc_tritium_id;
    result.mc_serial_number = (uint32_t)raw.mc_serial_number;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_status_pack(can_frame_t* dst, const cotscan_mc_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mc_limit_output_voltage_pwm;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_limit_output_voltage_pwm = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_limit_motor_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_limit_motor_current = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_limit_velocity;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_limit_velocity = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_limit_bus_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_limit_bus_current = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_limit_bus_voltage_upper;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_limit_bus_voltage_upper = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_limit_bus_voltage_lower;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_limit_bus_voltage_lower = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_limit_motor_temp;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_limit_motor_temp = (uint8_t)value;
    }
    {
        uint16_t value = src->mc_limit_reserved;
        if (value > INT64_C(511)) {
            value = (uint16_t)INT64_C(511); status = CAN_CLAMPED;
        }
        raw.mc_limit_reserved = (uint16_t)value;
    }
    {
        uint8_t value = src->mc_fault_hardware_over_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_hardware_over_current = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_software_over_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_software_over_current = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_dc_bus_over_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_dc_bus_over_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_bad_motor_position_hall_seq;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_bad_motor_position_hall_seq = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_watchdog_caused_last_reset;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_watchdog_caused_last_reset = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_config_read;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_config_read = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_15v_rail_under_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_15v_rail_under_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_desaturation_fault;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_desaturation_fault = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_motor_over_speed;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mc_fault_motor_over_speed = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_fault_reserved;
        if (value > INT64_C(127)) {
            value = (uint8_t)INT64_C(127); status = CAN_CLAMPED;
        }
        raw.mc_fault_reserved = (uint8_t)value;
    }
    {
        uint16_t value = src->mc_active_motor;
        raw.mc_active_motor = (uint16_t)value;
    }
    {
        uint8_t value = src->mc_tx_error_count;
        raw.mc_tx_error_count = (uint8_t)value;
    }
    {
        uint8_t value = src->mc_rx_error_count;
        raw.mc_rx_error_count = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1057),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_status_unpack(cotscan_mc_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1057) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_status_t raw = {0};
    if (cotscan_raw_mc_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_status_t result = {0};
    if (raw.mc_limit_output_voltage_pwm > INT64_C(1)) return CAN_RANGE;
    result.mc_limit_output_voltage_pwm = (uint8_t)raw.mc_limit_output_voltage_pwm;
    if (raw.mc_limit_motor_current > INT64_C(1)) return CAN_RANGE;
    result.mc_limit_motor_current = (uint8_t)raw.mc_limit_motor_current;
    if (raw.mc_limit_velocity > INT64_C(1)) return CAN_RANGE;
    result.mc_limit_velocity = (uint8_t)raw.mc_limit_velocity;
    if (raw.mc_limit_bus_current > INT64_C(1)) return CAN_RANGE;
    result.mc_limit_bus_current = (uint8_t)raw.mc_limit_bus_current;
    if (raw.mc_limit_bus_voltage_upper > INT64_C(1)) return CAN_RANGE;
    result.mc_limit_bus_voltage_upper = (uint8_t)raw.mc_limit_bus_voltage_upper;
    if (raw.mc_limit_bus_voltage_lower > INT64_C(1)) return CAN_RANGE;
    result.mc_limit_bus_voltage_lower = (uint8_t)raw.mc_limit_bus_voltage_lower;
    if (raw.mc_limit_motor_temp > INT64_C(1)) return CAN_RANGE;
    result.mc_limit_motor_temp = (uint8_t)raw.mc_limit_motor_temp;
    if (raw.mc_limit_reserved > INT64_C(511)) return CAN_RANGE;
    result.mc_limit_reserved = (uint16_t)raw.mc_limit_reserved;
    if (raw.mc_fault_hardware_over_current > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_hardware_over_current = (uint8_t)raw.mc_fault_hardware_over_current;
    if (raw.mc_fault_software_over_current > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_software_over_current = (uint8_t)raw.mc_fault_software_over_current;
    if (raw.mc_fault_dc_bus_over_voltage > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_dc_bus_over_voltage = (uint8_t)raw.mc_fault_dc_bus_over_voltage;
    if (raw.mc_fault_bad_motor_position_hall_seq > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_bad_motor_position_hall_seq = (uint8_t)raw.mc_fault_bad_motor_position_hall_seq;
    if (raw.mc_fault_watchdog_caused_last_reset > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_watchdog_caused_last_reset = (uint8_t)raw.mc_fault_watchdog_caused_last_reset;
    if (raw.mc_fault_config_read > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_config_read = (uint8_t)raw.mc_fault_config_read;
    if (raw.mc_fault_15v_rail_under_voltage > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_15v_rail_under_voltage = (uint8_t)raw.mc_fault_15v_rail_under_voltage;
    if (raw.mc_fault_desaturation_fault > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_desaturation_fault = (uint8_t)raw.mc_fault_desaturation_fault;
    if (raw.mc_fault_motor_over_speed > INT64_C(1)) return CAN_RANGE;
    result.mc_fault_motor_over_speed = (uint8_t)raw.mc_fault_motor_over_speed;
    if (raw.mc_fault_reserved > INT64_C(127)) return CAN_RANGE;
    result.mc_fault_reserved = (uint8_t)raw.mc_fault_reserved;
    result.mc_active_motor = (uint16_t)raw.mc_active_motor;
    result.mc_tx_error_count = (uint8_t)raw.mc_tx_error_count;
    result.mc_rx_error_count = (uint8_t)raw.mc_rx_error_count;
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_bus_measurement_pack(can_frame_t* dst, const cotscan_mc_bus_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_bus_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_bus_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_bus_voltage, &value, sizeof(value));
    }
    {
        float value = src->mc_bus_current;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_bus_current, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1058),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_bus_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_bus_measurement_unpack(cotscan_mc_bus_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1058) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_bus_measurement_t raw = {0};
    if (cotscan_raw_mc_bus_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_bus_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_bus_voltage, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_bus_voltage = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_bus_current, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_bus_current = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_velocity_measurement_pack(can_frame_t* dst, const cotscan_mc_velocity_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_velocity_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_motor_velocity;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_motor_velocity, &value, sizeof(value));
    }
    {
        float value = src->mc_vehicle_velocity;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_vehicle_velocity, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1059),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_velocity_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_velocity_measurement_unpack(cotscan_mc_velocity_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1059) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_velocity_measurement_t raw = {0};
    if (cotscan_raw_mc_velocity_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_velocity_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_motor_velocity, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_motor_velocity = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_vehicle_velocity, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_vehicle_velocity = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_phase_current_measurement_pack(can_frame_t* dst, const cotscan_mc_phase_current_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_phase_current_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_phase_current_b;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_phase_current_b, &value, sizeof(value));
    }
    {
        float value = src->mc_phase_current_c;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_phase_current_c, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1060),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_phase_current_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_phase_current_measurement_unpack(cotscan_mc_phase_current_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1060) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_phase_current_measurement_t raw = {0};
    if (cotscan_raw_mc_phase_current_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_phase_current_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_phase_current_b, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_phase_current_b = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_phase_current_c, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_phase_current_c = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_motor_voltage_vector_measurement_pack(can_frame_t* dst, const cotscan_mc_motor_voltage_vector_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_motor_voltage_vector_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_vq;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_vq, &value, sizeof(value));
    }
    {
        float value = src->mc_vd;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_vd, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1061),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_motor_voltage_vector_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_motor_voltage_vector_measurement_unpack(cotscan_mc_motor_voltage_vector_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1061) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_motor_voltage_vector_measurement_t raw = {0};
    if (cotscan_raw_mc_motor_voltage_vector_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_motor_voltage_vector_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_vq, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_vq = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_vd, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_vd = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_motor_current_vector_measurement_pack(can_frame_t* dst, const cotscan_mc_motor_current_vector_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_motor_current_vector_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_iq;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_iq, &value, sizeof(value));
    }
    {
        float value = src->mc_id;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_id, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1062),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_motor_current_vector_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_motor_current_vector_measurement_unpack(cotscan_mc_motor_current_vector_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1062) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_motor_current_vector_measurement_t raw = {0};
    if (cotscan_raw_mc_motor_current_vector_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_motor_current_vector_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_iq, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_iq = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_id, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_id = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_back_emf_measurement_prediction_pack(can_frame_t* dst, const cotscan_mc_back_emf_measurement_prediction_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_back_emf_measurement_prediction_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_bem_fq;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_bem_fq, &value, sizeof(value));
    }
    {
        float value = src->mc_bem_fd;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_bem_fd, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1063),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_back_emf_measurement_prediction_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_back_emf_measurement_prediction_unpack(cotscan_mc_back_emf_measurement_prediction_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1063) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_back_emf_measurement_prediction_t raw = {0};
    if (cotscan_raw_mc_back_emf_measurement_prediction_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_back_emf_measurement_prediction_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_bem_fq, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_bem_fq = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_bem_fd, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_bem_fd = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_15_v_rail_measurement_pack(can_frame_t* dst, const cotscan_mc_15_v_rail_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_15_v_rail_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_supply15_v;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_supply15_v, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1064),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_15_v_rail_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_15_v_rail_measurement_unpack(cotscan_mc_15_v_rail_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1064) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_15_v_rail_measurement_t raw = {0};
    if (cotscan_raw_mc_15_v_rail_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_15_v_rail_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_supply15_v, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_supply15_v = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_3_v3_19_v_rail_measurement_pack(can_frame_t* dst, const cotscan_mc_3_v3_19_v_rail_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_3_v3_19_v_rail_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_supply1_v9;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_supply1_v9, &value, sizeof(value));
    }
    {
        float value = src->mc_supply3_v3;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_supply3_v3, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1065),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_3_v3_19_v_rail_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_3_v3_19_v_rail_measurement_unpack(cotscan_mc_3_v3_19_v_rail_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1065) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_3_v3_19_v_rail_measurement_t raw = {0};
    if (cotscan_raw_mc_3_v3_19_v_rail_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_3_v3_19_v_rail_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_supply1_v9, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_supply1_v9 = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_supply3_v3, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_supply3_v3 = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_ws22_reserved0_a_pack(can_frame_t* dst, const cotscan_ws22_reserved0_a_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_ws22_reserved0_a_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1066),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_ws22_reserved0_a_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_ws22_reserved0_a_unpack(cotscan_ws22_reserved0_a_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1066) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_ws22_reserved0_a_t raw = {0};
    if (cotscan_raw_ws22_reserved0_a_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_ws22_reserved0_a_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_motor_temp_measurement_pack(can_frame_t* dst, const cotscan_mc_motor_temp_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_motor_temp_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_motor_temp;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_motor_temp, &value, sizeof(value));
    }
    {
        float value = src->mc_heatsink_temp;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_heatsink_temp, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1067),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_motor_temp_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_motor_temp_measurement_unpack(cotscan_mc_motor_temp_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1067) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_motor_temp_measurement_t raw = {0};
    if (cotscan_raw_mc_motor_temp_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_motor_temp_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_motor_temp, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_motor_temp = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_heatsink_temp, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_heatsink_temp = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_dsp_board_temp_measurement_pack(can_frame_t* dst, const cotscan_mc_dsp_board_temp_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_dsp_board_temp_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_dsp_board_temp;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_dsp_board_temp, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1068),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_dsp_board_temp_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_dsp_board_temp_measurement_unpack(cotscan_mc_dsp_board_temp_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1068) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_dsp_board_temp_measurement_t raw = {0};
    if (cotscan_raw_mc_dsp_board_temp_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_dsp_board_temp_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_dsp_board_temp, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_dsp_board_temp = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_ws22_reserved0_d_pack(can_frame_t* dst, const cotscan_ws22_reserved0_d_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_ws22_reserved0_d_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1069),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_ws22_reserved0_d_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_ws22_reserved0_d_unpack(cotscan_ws22_reserved0_d_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1069) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_ws22_reserved0_d_t raw = {0};
    if (cotscan_raw_ws22_reserved0_d_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_ws22_reserved0_d_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_odometer_bus_ah_measurement_pack(can_frame_t* dst, const cotscan_mc_odometer_bus_ah_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_odometer_bus_ah_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_trip_odometer;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_trip_odometer, &value, sizeof(value));
    }
    {
        float value = src->mc_dc_bus_ah;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_dc_bus_ah, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1070),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_odometer_bus_ah_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_odometer_bus_ah_measurement_unpack(cotscan_mc_odometer_bus_ah_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1070) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_odometer_bus_ah_measurement_t raw = {0};
    if (cotscan_raw_mc_odometer_bus_ah_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_odometer_bus_ah_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_trip_odometer, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_trip_odometer = value;
    }
    {
        float value;
        memcpy(&value, &raw.mc_dc_bus_ah, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_dc_bus_ah = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_mc_slip_speed_measurement_pack(can_frame_t* dst, const cotscan_mc_slip_speed_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_mc_slip_speed_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_slip_speed;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_slip_speed, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1079),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (cotscan_raw_mc_slip_speed_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_mc_slip_speed_measurement_unpack(cotscan_mc_slip_speed_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1079) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_mc_slip_speed_measurement_t raw = {0};
    if (cotscan_raw_mc_slip_speed_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_mc_slip_speed_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_slip_speed, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_slip_speed = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t cotscan_lws_config_pack(can_frame_t* dst, const cotscan_lws_config_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct cotscan_raw_lws_config_t raw = {0};
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
    if (cotscan_raw_lws_config_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t cotscan_lws_config_unpack(cotscan_lws_config_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1984) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct cotscan_raw_lws_config_t raw = {0};
    if (cotscan_raw_lws_config_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    cotscan_lws_config_t result = {0};
    if (raw.command > INT64_C(15)) return CAN_RANGE;
    result.command = (uint8_t)raw.command;
    *dst = result;
    return CAN_OK;
}
