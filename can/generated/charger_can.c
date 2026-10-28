#include "charger_can.h"
#include "detail/charger_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const charger_can_metadata_t charger_can = {
    .bps = {
        .bps_status = { .id = UINT32_C(96), .payload_bytes = 16, .wire_bytes = 16, .dlc = 10, .is_extended = false, .is_fd = true },
        .bps_pack_data = { .id = UINT32_C(112), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
        .bps_voltage_data = { .id = UINT32_C(113), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
        .bps_temperature_data = { .id = UINT32_C(114), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
    },
    .elcon_interface = {
        .elcon_interface_status = { .id = UINT32_C(1536), .payload_bytes = 6, .wire_bytes = 6, .dlc = 6, .is_extended = false, .is_fd = true },
    },
};

can_status_t charger_can_bps_status_pack(can_frame_t* dst, const charger_can_bps_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct charger_can_raw_bps_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->status;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.status = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_summary;
        if (value > INT64_C(7)) {
            value = (uint8_t)INT64_C(7); status = CAN_CLAMPED;
        }
        raw.fault_summary = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_temperature;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_temperature = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_under_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_under_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_charge_over_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_charge_over_current = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_discharge_over_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_discharge_over_current = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_hv_positive_contactor_mismatch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_hv_positive_contactor_mismatch = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_motor_contactor_mismatch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_motor_contactor_mismatch = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_hv_negative_contactor_mismatch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_hv_negative_contactor_mismatch = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_main_precharge_relay_mismatch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_main_precharge_relay_mismatch = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_motor_precharge_relay_mismatch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_motor_precharge_relay_mismatch = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_driver_estop;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_driver_estop = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_external_estop;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_external_estop = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_battery_estop;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_battery_estop = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_charger_estop;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_charger_estop = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_main_precharge_timeout;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_main_precharge_timeout = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_motor_precharge_timeout;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_motor_precharge_timeout = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_main_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_main_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_motor_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_motor_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_main_precharge_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_main_precharge_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_motor_precharge_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_motor_precharge_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_smb_communication;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_smb_communication = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_under_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_under_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_over_voltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_over_voltage = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_charge_over_temperature;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_charge_over_temperature = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_discharge_over_temperature;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_discharge_over_temperature = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_battery_fan1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_battery_fan1 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_battery_fan2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_battery_fan2 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_bps_fan;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_bps_fan = (uint8_t)value;
    }
    {
        uint8_t value = src->drive_ok;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.drive_ok = (uint8_t)value;
    }
    {
        uint8_t value = src->charge_ok;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.charge_ok = (uint8_t)value;
    }
    {
        uint8_t value = src->mppt_set_mode;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.mppt_set_mode = (uint8_t)value;
    }
    {
        uint8_t value = src->elcon_charge_ok;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.elcon_charge_ok = (uint8_t)value;
    }
    {
        uint8_t value = src->regen_charge_ok;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.regen_charge_ok = (uint8_t)value;
    }
    {
        uint8_t value = src->hv_positive_contactor;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.hv_positive_contactor = (uint8_t)value;
    }
    {
        uint8_t value = src->motor_contactor;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.motor_contactor = (uint8_t)value;
    }
    {
        uint8_t value = src->hv_negative_contactor;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.hv_negative_contactor = (uint8_t)value;
    }
    {
        uint8_t value = src->main_precharge_relay;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.main_precharge_relay = (uint8_t)value;
    }
    {
        uint8_t value = src->motor_precharge_relay;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.motor_precharge_relay = (uint8_t)value;
    }
    {
        double value = (double)src->mppt_voltage_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 175.0) { value = 175.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 3500.0) encoded = 3500.0;
        raw.mppt_voltage_limit = (uint16_t)encoded;
    }
    {
        double value = (double)src->mppt_current_limit;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 16.38) { value = 16.38; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1638.0) encoded = 1638.0;
        raw.mppt_current_limit = (uint16_t)encoded;
    }
    {
        double value = (double)src->battery_fan_tach1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 20475.0) { value = 20475.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 5.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.battery_fan_tach1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->battery_fan_tach2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 20475.0) { value = 20475.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 5.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.battery_fan_tach2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->bps_fan_tach;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 20475.0) { value = 20475.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 5.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.bps_fan_tach = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(96),
        .len = 16, .is_extended = false,
        .is_fd = true };
    if (charger_can_raw_bps_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t charger_can_bps_status_unpack(charger_can_bps_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(96) || src->len != 16 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct charger_can_raw_bps_status_t raw = {0};
    if (charger_can_raw_bps_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    charger_can_bps_status_t result = {0};
    if (raw.status > INT64_C(3)) return CAN_RANGE;
    result.status = (uint8_t)raw.status;
    if (raw.fault_summary > INT64_C(7)) return CAN_RANGE;
    result.fault_summary = (uint8_t)raw.fault_summary;
    if (raw.fault_over_temperature > INT64_C(1)) return CAN_RANGE;
    result.fault_over_temperature = (uint8_t)raw.fault_over_temperature;
    if (raw.fault_over_voltage > INT64_C(1)) return CAN_RANGE;
    result.fault_over_voltage = (uint8_t)raw.fault_over_voltage;
    if (raw.fault_under_voltage > INT64_C(1)) return CAN_RANGE;
    result.fault_under_voltage = (uint8_t)raw.fault_under_voltage;
    if (raw.fault_charge_over_current > INT64_C(1)) return CAN_RANGE;
    result.fault_charge_over_current = (uint8_t)raw.fault_charge_over_current;
    if (raw.fault_discharge_over_current > INT64_C(1)) return CAN_RANGE;
    result.fault_discharge_over_current = (uint8_t)raw.fault_discharge_over_current;
    if (raw.fault_hv_positive_contactor_mismatch > INT64_C(1)) return CAN_RANGE;
    result.fault_hv_positive_contactor_mismatch = (uint8_t)raw.fault_hv_positive_contactor_mismatch;
    if (raw.fault_motor_contactor_mismatch > INT64_C(1)) return CAN_RANGE;
    result.fault_motor_contactor_mismatch = (uint8_t)raw.fault_motor_contactor_mismatch;
    if (raw.fault_hv_negative_contactor_mismatch > INT64_C(1)) return CAN_RANGE;
    result.fault_hv_negative_contactor_mismatch = (uint8_t)raw.fault_hv_negative_contactor_mismatch;
    if (raw.fault_main_precharge_relay_mismatch > INT64_C(1)) return CAN_RANGE;
    result.fault_main_precharge_relay_mismatch = (uint8_t)raw.fault_main_precharge_relay_mismatch;
    if (raw.fault_motor_precharge_relay_mismatch > INT64_C(1)) return CAN_RANGE;
    result.fault_motor_precharge_relay_mismatch = (uint8_t)raw.fault_motor_precharge_relay_mismatch;
    if (raw.fault_driver_estop > INT64_C(1)) return CAN_RANGE;
    result.fault_driver_estop = (uint8_t)raw.fault_driver_estop;
    if (raw.fault_external_estop > INT64_C(1)) return CAN_RANGE;
    result.fault_external_estop = (uint8_t)raw.fault_external_estop;
    if (raw.fault_battery_estop > INT64_C(1)) return CAN_RANGE;
    result.fault_battery_estop = (uint8_t)raw.fault_battery_estop;
    if (raw.fault_charger_estop > INT64_C(1)) return CAN_RANGE;
    result.fault_charger_estop = (uint8_t)raw.fault_charger_estop;
    if (raw.fault_main_precharge_timeout > INT64_C(1)) return CAN_RANGE;
    result.fault_main_precharge_timeout = (uint8_t)raw.fault_main_precharge_timeout;
    if (raw.fault_motor_precharge_timeout > INT64_C(1)) return CAN_RANGE;
    result.fault_motor_precharge_timeout = (uint8_t)raw.fault_motor_precharge_timeout;
    if (raw.fault_main_voltage > INT64_C(1)) return CAN_RANGE;
    result.fault_main_voltage = (uint8_t)raw.fault_main_voltage;
    if (raw.fault_motor_voltage > INT64_C(1)) return CAN_RANGE;
    result.fault_motor_voltage = (uint8_t)raw.fault_motor_voltage;
    if (raw.fault_main_precharge_voltage > INT64_C(1)) return CAN_RANGE;
    result.fault_main_precharge_voltage = (uint8_t)raw.fault_main_precharge_voltage;
    if (raw.fault_motor_precharge_voltage > INT64_C(1)) return CAN_RANGE;
    result.fault_motor_precharge_voltage = (uint8_t)raw.fault_motor_precharge_voltage;
    if (raw.warning_smb_communication > INT64_C(1)) return CAN_RANGE;
    result.warning_smb_communication = (uint8_t)raw.warning_smb_communication;
    if (raw.warning_under_voltage > INT64_C(1)) return CAN_RANGE;
    result.warning_under_voltage = (uint8_t)raw.warning_under_voltage;
    if (raw.warning_over_voltage > INT64_C(1)) return CAN_RANGE;
    result.warning_over_voltage = (uint8_t)raw.warning_over_voltage;
    if (raw.warning_charge_over_temperature > INT64_C(1)) return CAN_RANGE;
    result.warning_charge_over_temperature = (uint8_t)raw.warning_charge_over_temperature;
    if (raw.warning_discharge_over_temperature > INT64_C(1)) return CAN_RANGE;
    result.warning_discharge_over_temperature = (uint8_t)raw.warning_discharge_over_temperature;
    if (raw.warning_battery_fan1 > INT64_C(1)) return CAN_RANGE;
    result.warning_battery_fan1 = (uint8_t)raw.warning_battery_fan1;
    if (raw.warning_battery_fan2 > INT64_C(1)) return CAN_RANGE;
    result.warning_battery_fan2 = (uint8_t)raw.warning_battery_fan2;
    if (raw.warning_bps_fan > INT64_C(1)) return CAN_RANGE;
    result.warning_bps_fan = (uint8_t)raw.warning_bps_fan;
    if (raw.drive_ok > INT64_C(1)) return CAN_RANGE;
    result.drive_ok = (uint8_t)raw.drive_ok;
    if (raw.charge_ok > INT64_C(1)) return CAN_RANGE;
    result.charge_ok = (uint8_t)raw.charge_ok;
    if (raw.mppt_set_mode > INT64_C(3)) return CAN_RANGE;
    result.mppt_set_mode = (uint8_t)raw.mppt_set_mode;
    if (raw.elcon_charge_ok > INT64_C(1)) return CAN_RANGE;
    result.elcon_charge_ok = (uint8_t)raw.elcon_charge_ok;
    if (raw.regen_charge_ok > INT64_C(1)) return CAN_RANGE;
    result.regen_charge_ok = (uint8_t)raw.regen_charge_ok;
    if (raw.hv_positive_contactor > INT64_C(1)) return CAN_RANGE;
    result.hv_positive_contactor = (uint8_t)raw.hv_positive_contactor;
    if (raw.motor_contactor > INT64_C(1)) return CAN_RANGE;
    result.motor_contactor = (uint8_t)raw.motor_contactor;
    if (raw.hv_negative_contactor > INT64_C(1)) return CAN_RANGE;
    result.hv_negative_contactor = (uint8_t)raw.hv_negative_contactor;
    if (raw.main_precharge_relay > INT64_C(1)) return CAN_RANGE;
    result.main_precharge_relay = (uint8_t)raw.main_precharge_relay;
    if (raw.motor_precharge_relay > INT64_C(1)) return CAN_RANGE;
    result.motor_precharge_relay = (uint8_t)raw.motor_precharge_relay;
    if (raw.mppt_voltage_limit > INT64_C(3500)) return CAN_RANGE;
    result.mppt_voltage_limit = (double)((double)raw.mppt_voltage_limit * 0.05 + 0.0);
    if (raw.mppt_current_limit > INT64_C(1638)) return CAN_RANGE;
    result.mppt_current_limit = (double)((double)raw.mppt_current_limit * 0.01 + 0.0);
    if (raw.battery_fan_tach1 > INT64_C(4095)) return CAN_RANGE;
    result.battery_fan_tach1 = (uint16_t)((double)raw.battery_fan_tach1 * 5.0 + 0.0);
    if (raw.battery_fan_tach2 > INT64_C(4095)) return CAN_RANGE;
    result.battery_fan_tach2 = (uint16_t)((double)raw.battery_fan_tach2 * 5.0 + 0.0);
    if (raw.bps_fan_tach > INT64_C(4095)) return CAN_RANGE;
    result.bps_fan_tach = (uint16_t)((double)raw.bps_fan_tach * 5.0 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t charger_can_bps_pack_data_pack(can_frame_t* dst, const charger_can_bps_pack_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct charger_can_raw_bps_pack_data_t raw = {0};
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
        double value = (double)src->motor_battery_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 204.75) { value = 204.75; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.motor_battery_voltage = (uint16_t)encoded;
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
        double value = (double)src->motor_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 204.75) { value = 204.75; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.motor_voltage = (uint16_t)encoded;
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
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(112),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (charger_can_raw_bps_pack_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t charger_can_bps_pack_data_unpack(charger_can_bps_pack_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(112) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct charger_can_raw_bps_pack_data_t raw = {0};
    if (charger_can_raw_bps_pack_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    charger_can_bps_pack_data_t result = {0};
    if (raw.cell_sum_voltage > INT64_C(4095)) return CAN_RANGE;
    result.cell_sum_voltage = (double)((double)raw.cell_sum_voltage * 0.05 + 0.0);
    if (raw.main_battery_voltage > INT64_C(4095)) return CAN_RANGE;
    result.main_battery_voltage = (double)((double)raw.main_battery_voltage * 0.05 + 0.0);
    if (raw.motor_battery_voltage > INT64_C(4095)) return CAN_RANGE;
    result.motor_battery_voltage = (double)((double)raw.motor_battery_voltage * 0.05 + 0.0);
    if (raw.main_voltage > INT64_C(4095)) return CAN_RANGE;
    result.main_voltage = (double)((double)raw.main_voltage * 0.05 + 0.0);
    if (raw.motor_voltage > INT64_C(4095)) return CAN_RANGE;
    result.motor_voltage = (double)((double)raw.motor_voltage * 0.05 + 0.0);
    if (raw.battery_current > INT64_C(16383)) return CAN_RANGE;
    result.battery_current = (double)((double)raw.battery_current * 0.01 + -50.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t charger_can_bps_voltage_data_pack(can_frame_t* dst, const charger_can_bps_voltage_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct charger_can_raw_bps_voltage_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->cell_voltage01;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage01 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage02;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage02 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage03;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage03 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage04;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage04 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage05;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage05 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage06;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage06 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage07;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage07 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage08;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage08 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage09;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage09 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage10;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage10 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage11;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage11 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage12;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage12 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage13;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage13 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage14;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage14 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage15;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage15 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage16;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage16 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage17;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage17 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage18;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage18 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage19;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage19 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage20;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage20 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage21;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage21 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage22;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage22 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage23;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage23 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage24;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage24 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage25;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage25 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage26;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage26 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage27;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage27 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage28;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage28 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage29;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage29 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage30;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage30 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage31;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage31 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_voltage32;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 2000.0) { value = 2000.0; status = CAN_CLAMPED; }
        if (value > 6095.0) { value = 6095.0; status = CAN_CLAMPED; }
        double encoded = round((value - 2000.0) / 1.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cell_voltage32 = (uint16_t)encoded;
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
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(113),
        .len = 64, .is_extended = false,
        .is_fd = true };
    if (charger_can_raw_bps_voltage_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t charger_can_bps_voltage_data_unpack(charger_can_bps_voltage_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(113) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct charger_can_raw_bps_voltage_data_t raw = {0};
    if (charger_can_raw_bps_voltage_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    charger_can_bps_voltage_data_t result = {0};
    if (raw.cell_voltage01 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage01 = (uint16_t)((double)raw.cell_voltage01 * 1.0 + 2000.0);
    if (raw.cell_voltage02 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage02 = (uint16_t)((double)raw.cell_voltage02 * 1.0 + 2000.0);
    if (raw.cell_voltage03 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage03 = (uint16_t)((double)raw.cell_voltage03 * 1.0 + 2000.0);
    if (raw.cell_voltage04 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage04 = (uint16_t)((double)raw.cell_voltage04 * 1.0 + 2000.0);
    if (raw.cell_voltage05 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage05 = (uint16_t)((double)raw.cell_voltage05 * 1.0 + 2000.0);
    if (raw.cell_voltage06 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage06 = (uint16_t)((double)raw.cell_voltage06 * 1.0 + 2000.0);
    if (raw.cell_voltage07 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage07 = (uint16_t)((double)raw.cell_voltage07 * 1.0 + 2000.0);
    if (raw.cell_voltage08 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage08 = (uint16_t)((double)raw.cell_voltage08 * 1.0 + 2000.0);
    if (raw.cell_voltage09 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage09 = (uint16_t)((double)raw.cell_voltage09 * 1.0 + 2000.0);
    if (raw.cell_voltage10 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage10 = (uint16_t)((double)raw.cell_voltage10 * 1.0 + 2000.0);
    if (raw.cell_voltage11 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage11 = (uint16_t)((double)raw.cell_voltage11 * 1.0 + 2000.0);
    if (raw.cell_voltage12 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage12 = (uint16_t)((double)raw.cell_voltage12 * 1.0 + 2000.0);
    if (raw.cell_voltage13 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage13 = (uint16_t)((double)raw.cell_voltage13 * 1.0 + 2000.0);
    if (raw.cell_voltage14 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage14 = (uint16_t)((double)raw.cell_voltage14 * 1.0 + 2000.0);
    if (raw.cell_voltage15 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage15 = (uint16_t)((double)raw.cell_voltage15 * 1.0 + 2000.0);
    if (raw.cell_voltage16 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage16 = (uint16_t)((double)raw.cell_voltage16 * 1.0 + 2000.0);
    if (raw.cell_voltage17 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage17 = (uint16_t)((double)raw.cell_voltage17 * 1.0 + 2000.0);
    if (raw.cell_voltage18 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage18 = (uint16_t)((double)raw.cell_voltage18 * 1.0 + 2000.0);
    if (raw.cell_voltage19 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage19 = (uint16_t)((double)raw.cell_voltage19 * 1.0 + 2000.0);
    if (raw.cell_voltage20 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage20 = (uint16_t)((double)raw.cell_voltage20 * 1.0 + 2000.0);
    if (raw.cell_voltage21 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage21 = (uint16_t)((double)raw.cell_voltage21 * 1.0 + 2000.0);
    if (raw.cell_voltage22 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage22 = (uint16_t)((double)raw.cell_voltage22 * 1.0 + 2000.0);
    if (raw.cell_voltage23 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage23 = (uint16_t)((double)raw.cell_voltage23 * 1.0 + 2000.0);
    if (raw.cell_voltage24 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage24 = (uint16_t)((double)raw.cell_voltage24 * 1.0 + 2000.0);
    if (raw.cell_voltage25 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage25 = (uint16_t)((double)raw.cell_voltage25 * 1.0 + 2000.0);
    if (raw.cell_voltage26 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage26 = (uint16_t)((double)raw.cell_voltage26 * 1.0 + 2000.0);
    if (raw.cell_voltage27 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage27 = (uint16_t)((double)raw.cell_voltage27 * 1.0 + 2000.0);
    if (raw.cell_voltage28 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage28 = (uint16_t)((double)raw.cell_voltage28 * 1.0 + 2000.0);
    if (raw.cell_voltage29 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage29 = (uint16_t)((double)raw.cell_voltage29 * 1.0 + 2000.0);
    if (raw.cell_voltage30 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage30 = (uint16_t)((double)raw.cell_voltage30 * 1.0 + 2000.0);
    if (raw.cell_voltage31 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage31 = (uint16_t)((double)raw.cell_voltage31 * 1.0 + 2000.0);
    if (raw.cell_voltage32 > INT64_C(4095)) return CAN_RANGE;
    result.cell_voltage32 = (uint16_t)((double)raw.cell_voltage32 * 1.0 + 2000.0);
    if (raw.min_cell_voltage > INT64_C(4095)) return CAN_RANGE;
    result.min_cell_voltage = (uint16_t)((double)raw.min_cell_voltage * 1.0 + 2000.0);
    if (raw.max_cell_voltage > INT64_C(4095)) return CAN_RANGE;
    result.max_cell_voltage = (uint16_t)((double)raw.max_cell_voltage * 1.0 + 2000.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t charger_can_bps_temperature_data_pack(can_frame_t* dst, const charger_can_bps_temperature_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct charger_can_raw_bps_temperature_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->cell_temperature01;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature01 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature02;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature02 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature03;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature03 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature04;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature04 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature05;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature05 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature06;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature06 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature07;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature07 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature08;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature08 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature09;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature09 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature10;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature10 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature11;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature11 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature12;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature12 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature13;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature13 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature14;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature14 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature15;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature15 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature16;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature16 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature17;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature17 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature18;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature18 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature19;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature19 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature20;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature20 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature21;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature21 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature22;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature22 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature23;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature23 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature24;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature24 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature25;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature25 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature26;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature26 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature27;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature27 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature28;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature28 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature29;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature29 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature30;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature30 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature31;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature31 = (uint16_t)encoded;
    }
    {
        double value = (double)src->cell_temperature32;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 102.35) { value = 102.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.cell_temperature32 = (uint16_t)encoded;
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
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(114),
        .len = 64, .is_extended = false,
        .is_fd = true };
    if (charger_can_raw_bps_temperature_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t charger_can_bps_temperature_data_unpack(charger_can_bps_temperature_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(114) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct charger_can_raw_bps_temperature_data_t raw = {0};
    if (charger_can_raw_bps_temperature_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    charger_can_bps_temperature_data_t result = {0};
    if (raw.cell_temperature01 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature01 = (double)((double)raw.cell_temperature01 * 0.05 + 0.0);
    if (raw.cell_temperature02 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature02 = (double)((double)raw.cell_temperature02 * 0.05 + 0.0);
    if (raw.cell_temperature03 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature03 = (double)((double)raw.cell_temperature03 * 0.05 + 0.0);
    if (raw.cell_temperature04 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature04 = (double)((double)raw.cell_temperature04 * 0.05 + 0.0);
    if (raw.cell_temperature05 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature05 = (double)((double)raw.cell_temperature05 * 0.05 + 0.0);
    if (raw.cell_temperature06 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature06 = (double)((double)raw.cell_temperature06 * 0.05 + 0.0);
    if (raw.cell_temperature07 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature07 = (double)((double)raw.cell_temperature07 * 0.05 + 0.0);
    if (raw.cell_temperature08 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature08 = (double)((double)raw.cell_temperature08 * 0.05 + 0.0);
    if (raw.cell_temperature09 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature09 = (double)((double)raw.cell_temperature09 * 0.05 + 0.0);
    if (raw.cell_temperature10 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature10 = (double)((double)raw.cell_temperature10 * 0.05 + 0.0);
    if (raw.cell_temperature11 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature11 = (double)((double)raw.cell_temperature11 * 0.05 + 0.0);
    if (raw.cell_temperature12 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature12 = (double)((double)raw.cell_temperature12 * 0.05 + 0.0);
    if (raw.cell_temperature13 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature13 = (double)((double)raw.cell_temperature13 * 0.05 + 0.0);
    if (raw.cell_temperature14 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature14 = (double)((double)raw.cell_temperature14 * 0.05 + 0.0);
    if (raw.cell_temperature15 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature15 = (double)((double)raw.cell_temperature15 * 0.05 + 0.0);
    if (raw.cell_temperature16 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature16 = (double)((double)raw.cell_temperature16 * 0.05 + 0.0);
    if (raw.cell_temperature17 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature17 = (double)((double)raw.cell_temperature17 * 0.05 + 0.0);
    if (raw.cell_temperature18 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature18 = (double)((double)raw.cell_temperature18 * 0.05 + 0.0);
    if (raw.cell_temperature19 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature19 = (double)((double)raw.cell_temperature19 * 0.05 + 0.0);
    if (raw.cell_temperature20 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature20 = (double)((double)raw.cell_temperature20 * 0.05 + 0.0);
    if (raw.cell_temperature21 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature21 = (double)((double)raw.cell_temperature21 * 0.05 + 0.0);
    if (raw.cell_temperature22 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature22 = (double)((double)raw.cell_temperature22 * 0.05 + 0.0);
    if (raw.cell_temperature23 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature23 = (double)((double)raw.cell_temperature23 * 0.05 + 0.0);
    if (raw.cell_temperature24 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature24 = (double)((double)raw.cell_temperature24 * 0.05 + 0.0);
    if (raw.cell_temperature25 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature25 = (double)((double)raw.cell_temperature25 * 0.05 + 0.0);
    if (raw.cell_temperature26 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature26 = (double)((double)raw.cell_temperature26 * 0.05 + 0.0);
    if (raw.cell_temperature27 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature27 = (double)((double)raw.cell_temperature27 * 0.05 + 0.0);
    if (raw.cell_temperature28 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature28 = (double)((double)raw.cell_temperature28 * 0.05 + 0.0);
    if (raw.cell_temperature29 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature29 = (double)((double)raw.cell_temperature29 * 0.05 + 0.0);
    if (raw.cell_temperature30 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature30 = (double)((double)raw.cell_temperature30 * 0.05 + 0.0);
    if (raw.cell_temperature31 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature31 = (double)((double)raw.cell_temperature31 * 0.05 + 0.0);
    if (raw.cell_temperature32 > INT64_C(2047)) return CAN_RANGE;
    result.cell_temperature32 = (double)((double)raw.cell_temperature32 * 0.05 + 0.0);
    if (raw.max_battery_temperature > INT64_C(2047)) return CAN_RANGE;
    result.max_battery_temperature = (double)((double)raw.max_battery_temperature * 0.05 + 0.0);
    if (raw.average_battery_temperature > INT64_C(2047)) return CAN_RANGE;
    result.average_battery_temperature = (double)((double)raw.average_battery_temperature * 0.05 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
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
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1536),
        .len = 6, .is_extended = false,
        .is_fd = true };
    if (charger_can_raw_elcon_interface_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t charger_can_elcon_interface_status_unpack(charger_can_elcon_interface_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1536) || src->len != 6 ||
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
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}
