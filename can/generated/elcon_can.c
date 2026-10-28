#include "elcon_can.h"
#include "detail/elcon_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const elcon_can_metadata_t elcon_can = {
    .elcon = {
        .elcon_ccs_status = { .id = UINT32_C(419385573), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = true, .is_fd = false },
    },
    .elcon_interface = {
        .elcon_control = { .id = UINT32_C(403105268), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = true, .is_fd = false },
        .elcon_control_mux = { .id = UINT32_C(403105524), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = true, .is_fd = false },
    },
};

can_status_t elcon_can_elcon_control_pack(can_frame_t* dst, const elcon_can_elcon_control_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct elcon_can_raw_elcon_control_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->elcon_vol_set;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.1);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.elcon_vol_set = (uint16_t)encoded;
    }
    {
        double value = (double)src->elcon_cur_set;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.1);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.elcon_cur_set = (uint16_t)encoded;
    }
    {
        uint8_t value = src->elcon_control_flag;
        raw.elcon_control_flag = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(403105268),
        .len = 5, .is_extended = true,
        .is_fd = false };
    if (elcon_can_raw_elcon_control_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t elcon_can_elcon_control_unpack(elcon_can_elcon_control_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(403105268) || src->len != 5 ||
        src->is_extended != true ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct elcon_can_raw_elcon_control_t raw = {0};
    if (elcon_can_raw_elcon_control_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    elcon_can_elcon_control_t result = {0};
    result.elcon_vol_set = (double)((double)raw.elcon_vol_set * 0.1 + 0.0);
    result.elcon_cur_set = (double)((double)raw.elcon_cur_set * 0.1 + 0.0);
    result.elcon_control_flag = (uint8_t)raw.elcon_control_flag;
    *dst = result;
    return CAN_OK;
}

can_status_t elcon_can_elcon_control_mux_pack(can_frame_t* dst, const elcon_can_elcon_control_mux_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct elcon_can_raw_elcon_control_mux_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->elcon_mux_page;
        raw.elcon_mux_page = (uint8_t)value;
    }
    switch (raw.elcon_mux_page) {
    case INT64_C(1):
        {
            double value = (double)src->elcon_vol_set;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.1);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_vol_set = (uint16_t)encoded;
        }
        {
            double value = (double)src->elcon_cur_set;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.1);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_cur_set = (uint16_t)encoded;
        }
        {
            uint8_t value = src->elcon_control_flag;
            raw.elcon_control_flag = (uint8_t)value;
        }
        {
            double value = (double)src->elcon_discur_max;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 2550.0) { value = 2550.0; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 10.0);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 255.0) encoded = 255.0;
            raw.elcon_discur_max = (uint8_t)encoded;
        }
        break;
    case INT64_C(2):
        {
            double value = (double)src->elcon_ah_nominal;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.1);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_ah_nominal = (uint16_t)encoded;
        }
        {
            double value = (double)src->elcon_ah_actual;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.1);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_ah_actual = (uint16_t)encoded;
        }
        {
            double value = (double)src->elcon_cell_ov_thresh;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 65.535) { value = 65.535; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.001);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_cell_ov_thresh = (uint16_t)encoded;
        }
        {
            uint8_t value = src->elcon_num_cells;
            raw.elcon_num_cells = (uint8_t)value;
        }
        break;
    case INT64_C(3):
        {
            double value = (double)src->elcon_max_cell_vol;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 65.535) { value = 65.535; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.001);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_max_cell_vol = (uint16_t)encoded;
        }
        {
            double value = (double)src->elcon_min_cell_vol;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 65.535) { value = 65.535; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.001);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_min_cell_vol = (uint16_t)encoded;
        }
        {
            double value = (double)src->elcon_cell_uv_thresh;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 65.535) { value = 65.535; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.001);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_cell_uv_thresh = (uint16_t)encoded;
        }
        {
            uint8_t value = src->elcon_battery_state_uv;
            if (value > INT64_C(1)) {
                value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
            }
            raw.elcon_battery_state_uv = (uint8_t)value;
        }
        {
            uint8_t value = src->elcon_battery_state_ov;
            if (value > INT64_C(1)) {
                value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
            }
            raw.elcon_battery_state_ov = (uint8_t)value;
        }
        break;
    case INT64_C(4):
        {
            double value = (double)src->elcon_battery_vol;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.1);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_battery_vol = (uint16_t)encoded;
        }
        {
            double value = (double)src->elcon_battery_charge_cur;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
            if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
            double encoded = round((value - 0.0) / 0.1);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 65535.0) encoded = 65535.0;
            raw.elcon_battery_charge_cur = (uint16_t)encoded;
        }
        {
            uint8_t value = src->elcon_battery_soc;
            if (value > INT64_C(100)) {
                value = (uint8_t)INT64_C(100); status = CAN_CLAMPED;
            }
            raw.elcon_battery_soc = (uint8_t)value;
        }
        {
            double value = (double)src->elcon_temp_max;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < -100.0) { value = -100.0; status = CAN_CLAMPED; }
            if (value > 155.0) { value = 155.0; status = CAN_CLAMPED; }
            double encoded = round((value - -100.0) / 1.0);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 255.0) encoded = 255.0;
            raw.elcon_temp_max = (uint8_t)encoded;
        }
        {
            double value = (double)src->elcon_temp_min;
            if (!isfinite(value)) return CAN_INVALID;
            if (value < -100.0) { value = -100.0; status = CAN_CLAMPED; }
            if (value > 155.0) { value = 155.0; status = CAN_CLAMPED; }
            double encoded = round((value - -100.0) / 1.0);
            if (encoded < 0.0) encoded = 0.0;
            if (encoded > 255.0) encoded = 255.0;
            raw.elcon_temp_min = (uint8_t)encoded;
        }
        break;
    case INT64_C(5):
        {
            uint8_t value = src->elcon_num_cells_ext;
            raw.elcon_num_cells_ext = (uint8_t)value;
        }
        break;
    default: return CAN_INVALID;
    }
    can_frame_t result = { .id = UINT32_C(403105524),
        .len = 8, .is_extended = true,
        .is_fd = false };
    if (elcon_can_raw_elcon_control_mux_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t elcon_can_elcon_control_mux_unpack(elcon_can_elcon_control_mux_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(403105524) || src->len != 8 ||
        src->is_extended != true ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct elcon_can_raw_elcon_control_mux_t raw = {0};
    if (elcon_can_raw_elcon_control_mux_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    elcon_can_elcon_control_mux_t result = {0};
    result.elcon_mux_page = (uint8_t)raw.elcon_mux_page;
    switch (raw.elcon_mux_page) {
    case INT64_C(1):
        result.elcon_vol_set = (double)((double)raw.elcon_vol_set * 0.1 + 0.0);
        result.elcon_cur_set = (double)((double)raw.elcon_cur_set * 0.1 + 0.0);
        result.elcon_control_flag = (uint8_t)raw.elcon_control_flag;
        result.elcon_discur_max = (uint16_t)((double)raw.elcon_discur_max * 10.0 + 0.0);
        break;
    case INT64_C(2):
        result.elcon_ah_nominal = (double)((double)raw.elcon_ah_nominal * 0.1 + 0.0);
        result.elcon_ah_actual = (double)((double)raw.elcon_ah_actual * 0.1 + 0.0);
        result.elcon_cell_ov_thresh = (double)((double)raw.elcon_cell_ov_thresh * 0.001 + 0.0);
        result.elcon_num_cells = (uint8_t)raw.elcon_num_cells;
        break;
    case INT64_C(3):
        result.elcon_max_cell_vol = (double)((double)raw.elcon_max_cell_vol * 0.001 + 0.0);
        result.elcon_min_cell_vol = (double)((double)raw.elcon_min_cell_vol * 0.001 + 0.0);
        result.elcon_cell_uv_thresh = (double)((double)raw.elcon_cell_uv_thresh * 0.001 + 0.0);
        if (raw.elcon_battery_state_uv > INT64_C(1)) return CAN_RANGE;
        result.elcon_battery_state_uv = (uint8_t)raw.elcon_battery_state_uv;
        if (raw.elcon_battery_state_ov > INT64_C(1)) return CAN_RANGE;
        result.elcon_battery_state_ov = (uint8_t)raw.elcon_battery_state_ov;
        break;
    case INT64_C(4):
        result.elcon_battery_vol = (double)((double)raw.elcon_battery_vol * 0.1 + 0.0);
        result.elcon_battery_charge_cur = (double)((double)raw.elcon_battery_charge_cur * 0.1 + 0.0);
        if (raw.elcon_battery_soc > INT64_C(100)) return CAN_RANGE;
        result.elcon_battery_soc = (uint8_t)raw.elcon_battery_soc;
        result.elcon_temp_max = (int16_t)((double)raw.elcon_temp_max * 1.0 + -100.0);
        result.elcon_temp_min = (int16_t)((double)raw.elcon_temp_min * 1.0 + -100.0);
        break;
    case INT64_C(5):
        result.elcon_num_cells_ext = (uint8_t)raw.elcon_num_cells_ext;
        break;
    default: return CAN_INVALID;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t elcon_can_elcon_ccs_status_pack(can_frame_t* dst, const elcon_can_elcon_ccs_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct elcon_can_raw_elcon_ccs_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->elcon_vol_out;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.1);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.elcon_vol_out = (uint16_t)encoded;
    }
    {
        double value = (double)src->elcon_cur_out;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 6553.5) { value = 6553.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.1);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.elcon_cur_out = (uint16_t)encoded;
    }
    {
        uint8_t value = src->elcon_cmd_timeout;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.elcon_cmd_timeout = (uint8_t)value;
    }
    {
        uint8_t value = src->elcon_start_state;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.elcon_start_state = (uint8_t)value;
    }
    {
        uint8_t value = src->elcon_input_vol_error;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.elcon_input_vol_error = (uint8_t)value;
    }
    {
        uint8_t value = src->elcon_over_temperature;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.elcon_over_temperature = (uint8_t)value;
    }
    {
        uint8_t value = src->elcon_hw_fault;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.elcon_hw_fault = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(419385573),
        .len = 5, .is_extended = true,
        .is_fd = false };
    if (elcon_can_raw_elcon_ccs_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t elcon_can_elcon_ccs_status_unpack(elcon_can_elcon_ccs_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(419385573) || src->len != 5 ||
        src->is_extended != true ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct elcon_can_raw_elcon_ccs_status_t raw = {0};
    if (elcon_can_raw_elcon_ccs_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    elcon_can_elcon_ccs_status_t result = {0};
    result.elcon_vol_out = (double)((double)raw.elcon_vol_out * 0.1 + 0.0);
    result.elcon_cur_out = (double)((double)raw.elcon_cur_out * 0.1 + 0.0);
    if (raw.elcon_cmd_timeout > INT64_C(1)) return CAN_RANGE;
    result.elcon_cmd_timeout = (uint8_t)raw.elcon_cmd_timeout;
    if (raw.elcon_start_state > INT64_C(1)) return CAN_RANGE;
    result.elcon_start_state = (uint8_t)raw.elcon_start_state;
    if (raw.elcon_input_vol_error > INT64_C(1)) return CAN_RANGE;
    result.elcon_input_vol_error = (uint8_t)raw.elcon_input_vol_error;
    if (raw.elcon_over_temperature > INT64_C(1)) return CAN_RANGE;
    result.elcon_over_temperature = (uint8_t)raw.elcon_over_temperature;
    if (raw.elcon_hw_fault > INT64_C(1)) return CAN_RANGE;
    result.elcon_hw_fault = (uint8_t)raw.elcon_hw_fault;
    *dst = result;
    return CAN_OK;
}
