#include "charger_can.h"
#include "detail/charger_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const charger_can_metadata_t charger_can = {
    .bps = {
        .bps_charge_data = { .id = UINT32_C(121), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
    },
    .elcon_interface = {
        .elcon_interface_status = { .id = UINT32_C(1536), .payload_bytes = 4, .wire_bytes = 4, .dlc = 4, .is_extended = false, .is_fd = true },
    },
};

can_status_t charger_can_bps_charge_data_pack(can_frame_t* dst, const charger_can_bps_charge_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct charger_can_raw_bps_charge_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->cell_sum_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 204.75) { value = 204.75; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_sum_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->main_battery_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 204.75) { value = 204.75; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.main_battery_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->main_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 204.75) { value = 204.75; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.main_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->min_cell_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.min_cell_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->max_cell_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.max_cell_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->max_battery_temperature;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.max_battery_temperature = (uint16_t)encoded;
    }
    {
        double value = (double)src->average_battery_temperature;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.average_battery_temperature = (uint16_t)encoded;
    }
    {
        double value = (double)src->battery_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -50.0) { value = -50.0; status = CAN_CLAMPED; }
        if (value > 113.83) { value = 113.83; status = CAN_CLAMPED; }
        double encoded = round((value - -50.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.battery_current = (uint16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(121),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (charger_can_raw_bps_charge_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t charger_can_bps_charge_data_unpack(charger_can_bps_charge_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(121) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct charger_can_raw_bps_charge_data_t raw = {0};
    if (charger_can_raw_bps_charge_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    charger_can_bps_charge_data_t result = {0};
    if (raw.cell_sum_voltage > INT64_C(4095)) return CAN_RANGE;
    result.cell_sum_voltage = (double)((double)raw.cell_sum_voltage * 0.05 + 0.0);
    if (raw.main_battery_voltage > INT64_C(4095)) return CAN_RANGE;
    result.main_battery_voltage = (double)((double)raw.main_battery_voltage * 0.05 + 0.0);
    if (raw.main_voltage > INT64_C(4095)) return CAN_RANGE;
    result.main_voltage = (double)((double)raw.main_voltage * 0.05 + 0.0);
    if (raw.min_cell_voltage > INT64_C(4095)) return CAN_RANGE;
    result.min_cell_voltage = (uint16_t)((double)raw.min_cell_voltage * 1.0 + 2000.0);
    if (raw.max_cell_voltage > INT64_C(4095)) return CAN_RANGE;
    result.max_cell_voltage = (uint16_t)((double)raw.max_cell_voltage * 1.0 + 2000.0);
    if (raw.max_battery_temperature > INT64_C(2047)) return CAN_RANGE;
    result.max_battery_temperature = (double)((double)raw.max_battery_temperature * 0.05 + 0.0);
    if (raw.average_battery_temperature > INT64_C(2047)) return CAN_RANGE;
    result.average_battery_temperature = (double)((double)raw.average_battery_temperature * 0.05 + 0.0);
    if (raw.battery_current > INT64_C(16383)) return CAN_RANGE;
    result.battery_current = (double)((double)raw.battery_current * 0.01 + -50.0);
    *dst = result;
    return CAN_OK;
}

can_status_t charger_can_elcon_interface_status_pack(can_frame_t* dst, const charger_can_elcon_interface_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct charger_can_raw_elcon_interface_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->status;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.status = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_elcon_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_elcon_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_bps_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_bps_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_elcon;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_elcon = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_bps;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_bps = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_evse;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_evse = (uint8_t)value;
    }
    {
        uint8_t value = src->estop;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.estop = (uint8_t)value;
    }
    {
        uint8_t value = src->charge_state;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.charge_state = (uint8_t)value;
    }
    {
        uint8_t value = src->evse_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.evse_connected = (uint8_t)value;
    }
    {
        uint8_t value = src->evse_latch_pressed;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.evse_latch_pressed = (uint8_t)value;
    }
    {
        uint8_t value = src->evse_state;
        if (value > INT64_C(7)) {
            value = (uint8_t)INT64_C(7); status = CAN_CLAMPED;
        }
        raw.evse_state = (uint8_t)value;
    }
    {
        uint8_t value = src->evsecp_valid;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.evsecp_valid = (uint8_t)value;
    }
    {
        double value = (double)src->evse_available_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 25.5) { value = 25.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2550.0) encoded = 2550.0;
        raw.evse_available_current = (uint16_t)encoded;
    }
    can_frame_t result = { .id = UINT32_C(1536),
        .len = 4, .is_extended = false,
        .is_fd = true };
    if (charger_can_raw_elcon_interface_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t charger_can_elcon_interface_status_unpack(charger_can_elcon_interface_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1536) || src->len != 4 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct charger_can_raw_elcon_interface_status_t raw = {0};
    if (charger_can_raw_elcon_interface_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    charger_can_elcon_interface_status_t result = {0};
    if (raw.status > INT64_C(1)) return CAN_RANGE;
    result.status = (uint8_t)raw.status;
    if (raw.fault_elcon_watchdog > INT64_C(1)) return CAN_RANGE;
    result.fault_elcon_watchdog = (uint8_t)raw.fault_elcon_watchdog;
    if (raw.fault_bps_watchdog > INT64_C(1)) return CAN_RANGE;
    result.fault_bps_watchdog = (uint8_t)raw.fault_bps_watchdog;
    if (raw.fault_elcon > INT64_C(1)) return CAN_RANGE;
    result.fault_elcon = (uint8_t)raw.fault_elcon;
    if (raw.fault_bps > INT64_C(1)) return CAN_RANGE;
    result.fault_bps = (uint8_t)raw.fault_bps;
    if (raw.fault_evse > INT64_C(1)) return CAN_RANGE;
    result.fault_evse = (uint8_t)raw.fault_evse;
    if (raw.estop > INT64_C(1)) return CAN_RANGE;
    result.estop = (uint8_t)raw.estop;
    if (raw.charge_state > INT64_C(3)) return CAN_RANGE;
    result.charge_state = (uint8_t)raw.charge_state;
    if (raw.evse_connected > INT64_C(1)) return CAN_RANGE;
    result.evse_connected = (uint8_t)raw.evse_connected;
    if (raw.evse_latch_pressed > INT64_C(1)) return CAN_RANGE;
    result.evse_latch_pressed = (uint8_t)raw.evse_latch_pressed;
    if (raw.evse_state > INT64_C(7)) return CAN_RANGE;
    result.evse_state = (uint8_t)raw.evse_state;
    if (raw.evsecp_valid > INT64_C(1)) return CAN_RANGE;
    result.evsecp_valid = (uint8_t)raw.evsecp_valid;
    if (raw.evse_available_current > INT64_C(2550)) return CAN_RANGE;
    result.evse_available_current = (double)((double)raw.evse_available_current * 0.01 + 0.0);
    *dst = result;
    return CAN_OK;
}
