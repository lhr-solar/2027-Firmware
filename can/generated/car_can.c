#include "car_can.h"
#include "detail/car_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const car_can_metadata_t car_can = {
    .bps = {
        .bps_status = { .id = UINT32_C(96), .payload_bytes = 16, .wire_bytes = 16, .dlc = 10, .is_extended = false, .is_fd = true },
        .bps_pack_data = { .id = UINT32_C(112), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
        .bps_voltage_data = { .id = UINT32_C(113), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
        .bps_temperature_data = { .id = UINT32_C(114), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
        .smd_status = { .id = UINT32_C(115), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = true },
        .bps_command_echo = { .id = UINT32_C(116), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
        .bps_fault_snapshot = { .id = UINT32_C(117), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
        .bps_fault_voltage_snapshot = { .id = UINT32_C(118), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
        .bps_fault_temperature_snapshot = { .id = UINT32_C(119), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
    },
    .dei = {
        .dei_status = { .id = UINT32_C(98), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = true },
    },
    .external = {
        .bps_command = { .id = UINT32_C(81), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
        .pdu_command = { .id = UINT32_C(82), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = true },
        .motor_controller_command = { .id = UINT32_C(83), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = true },
        .sweep_mpp_ts = { .id = UINT32_C(84), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = true },
    },
    .lvc = {
        .lvc_status = { .id = UINT32_C(144), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = true },
        .lvc_current_data = { .id = UINT32_C(145), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
        .lvc_voltage_data = { .id = UINT32_C(146), .payload_bytes = 7, .wire_bytes = 7, .dlc = 7, .is_extended = false, .is_fd = true },
        .lvc_temperature_data = { .id = UINT32_C(147), .payload_bytes = 4, .wire_bytes = 4, .dlc = 4, .is_extended = false, .is_fd = true },
        .lvc_over_current_snapshot = { .id = UINT32_C(148), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
    },
    .mika = {
        .mika_status = { .id = UINT32_C(130), .payload_bytes = 7, .wire_bytes = 7, .dlc = 7, .is_extended = false, .is_fd = true },
    },
    .mppt_interface = {
        .mppt_interface_status = { .id = UINT32_C(121), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = true },
    },
    .pdu = {
        .pdu_status = { .id = UINT32_C(160), .payload_bytes = 16, .wire_bytes = 16, .dlc = 10, .is_extended = false, .is_fd = true },
        .pdu_current_data = { .id = UINT32_C(161), .payload_bytes = 24, .wire_bytes = 24, .dlc = 12, .is_extended = false, .is_fd = true },
        .pdu_voltage_data = { .id = UINT32_C(162), .payload_bytes = 20, .wire_bytes = 20, .dlc = 11, .is_extended = false, .is_fd = true },
        .pdu_temperature_data = { .id = UINT32_C(163), .payload_bytes = 24, .wire_bytes = 24, .dlc = 12, .is_extended = false, .is_fd = true },
        .pdu_over_current_snapshot_a = { .id = UINT32_C(164), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
        .pdu_over_current_snapshot_b = { .id = UINT32_C(165), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
    },
    .pedals = {
        .pedals_status = { .id = UINT32_C(97), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = true },
    },
    .vcu = {
        .vcu_status = { .id = UINT32_C(99), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = true },
    },
};

can_status_t car_can_bps_command_pack(can_frame_t* dst, const car_can_bps_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_command_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mode = (uint8_t)value;
    }
    {
        uint8_t value = src->mppt_charge_scheme;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mppt_charge_scheme = (uint8_t)value;
    }
    {
        uint32_t value = src->voltage_override_mask;
        raw.voltage_override_mask = (uint32_t)value;
    }
    {
        uint32_t value = src->temperature_override_mask;
        raw.temperature_override_mask = (uint32_t)value;
    }
    can_frame_t result = { .id = UINT32_C(81),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_bps_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_command_unpack(car_can_bps_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(81) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_command_t raw = {0};
    if (car_can_raw_bps_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_command_t result = {0};
    if (raw.mode > INT64_C(1)) return CAN_RANGE;
    result.mode = (uint8_t)raw.mode;
    if (raw.mppt_charge_scheme > INT64_C(1)) return CAN_RANGE;
    result.mppt_charge_scheme = (uint8_t)raw.mppt_charge_scheme;
    result.voltage_override_mask = (uint32_t)raw.voltage_override_mask;
    result.temperature_override_mask = (uint32_t)raw.temperature_override_mask;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_pdu_command_pack(can_frame_t* dst, const car_can_pdu_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pdu_command_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->channel_index;
        if (value > INT64_C(11)) {
            value = (uint8_t)INT64_C(11); status = CAN_CLAMPED;
        }
        raw.channel_index = (uint8_t)value;
    }
    {
        uint8_t value = src->enable;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.enable = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(82),
        .len = 1, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pdu_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pdu_command_unpack(car_can_pdu_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(82) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pdu_command_t raw = {0};
    if (car_can_raw_pdu_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pdu_command_t result = {0};
    if (raw.channel_index > INT64_C(11)) return CAN_RANGE;
    result.channel_index = (uint8_t)raw.channel_index;
    if (raw.enable > INT64_C(1)) return CAN_RANGE;
    result.enable = (uint8_t)raw.enable;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_motor_controller_command_pack(can_frame_t* dst, const car_can_motor_controller_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_motor_controller_command_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->command_source;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.command_source = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(83),
        .len = 1, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_motor_controller_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_motor_controller_command_unpack(car_can_motor_controller_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(83) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_motor_controller_command_t raw = {0};
    if (car_can_raw_motor_controller_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_motor_controller_command_t result = {0};
    if (raw.command_source > INT64_C(1)) return CAN_RANGE;
    result.command_source = (uint8_t)raw.command_source;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_sweep_mpp_ts_pack(can_frame_t* dst, const car_can_sweep_mpp_ts_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_sweep_mpp_ts_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(84),
        .len = 0, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_sweep_mpp_ts_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_sweep_mpp_ts_unpack(car_can_sweep_mpp_ts_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(84) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_sweep_mpp_ts_t raw = {0};
    if (car_can_raw_sweep_mpp_ts_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_sweep_mpp_ts_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_bps_status_pack(can_frame_t* dst, const car_can_bps_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_status_t raw = {0};
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
    if (car_can_raw_bps_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_status_unpack(car_can_bps_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(96) || src->len != 16 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_status_t raw = {0};
    if (car_can_raw_bps_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_status_t result = {0};
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

can_status_t car_can_pedals_status_pack(can_frame_t* dst, const car_can_pedals_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pedals_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->status;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.status = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_no_accelerator;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_no_accelerator = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_main_accelerator_disconnected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_main_accelerator_disconnected = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_redundant_accelerator_disconnected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_redundant_accelerator_disconnected = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_pressure_switch_mismatch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_pressure_switch_mismatch = (uint8_t)value;
    }
    {
        uint8_t value = src->brake_active;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.brake_active = (uint8_t)value;
    }
    {
        uint8_t value = src->front_pressure_switch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.front_pressure_switch = (uint8_t)value;
    }
    {
        uint8_t value = src->handbrake_pressure_switch;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.handbrake_pressure_switch = (uint8_t)value;
    }
    {
        double value = (double)src->accelerator_main_position;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 100.0) { value = 100.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.39215686274509803);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 255.0) encoded = 255.0;
        raw.accelerator_main_position = (uint8_t)encoded;
    }
    {
        double value = (double)src->accelerator_redundant_position;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 100.0) { value = 100.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.39215686274509803);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 255.0) encoded = 255.0;
        raw.accelerator_redundant_position = (uint8_t)encoded;
    }
    {
        uint16_t value = src->front_pressure;
        if (value > INT64_C(3000)) {
            value = (uint16_t)INT64_C(3000); status = CAN_CLAMPED;
        }
        raw.front_pressure = (uint16_t)value;
    }
    {
        uint16_t value = src->rear_pressure;
        if (value > INT64_C(3000)) {
            value = (uint16_t)INT64_C(3000); status = CAN_CLAMPED;
        }
        raw.rear_pressure = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(97),
        .len = 8, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pedals_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pedals_status_unpack(car_can_pedals_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(97) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pedals_status_t raw = {0};
    if (car_can_raw_pedals_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pedals_status_t result = {0};
    if (raw.status > INT64_C(1)) return CAN_RANGE;
    result.status = (uint8_t)raw.status;
    if (raw.fault_no_accelerator > INT64_C(1)) return CAN_RANGE;
    result.fault_no_accelerator = (uint8_t)raw.fault_no_accelerator;
    if (raw.warning_main_accelerator_disconnected > INT64_C(1)) return CAN_RANGE;
    result.warning_main_accelerator_disconnected = (uint8_t)raw.warning_main_accelerator_disconnected;
    if (raw.warning_redundant_accelerator_disconnected > INT64_C(1)) return CAN_RANGE;
    result.warning_redundant_accelerator_disconnected = (uint8_t)raw.warning_redundant_accelerator_disconnected;
    if (raw.warning_pressure_switch_mismatch > INT64_C(1)) return CAN_RANGE;
    result.warning_pressure_switch_mismatch = (uint8_t)raw.warning_pressure_switch_mismatch;
    if (raw.brake_active > INT64_C(1)) return CAN_RANGE;
    result.brake_active = (uint8_t)raw.brake_active;
    if (raw.front_pressure_switch > INT64_C(1)) return CAN_RANGE;
    result.front_pressure_switch = (uint8_t)raw.front_pressure_switch;
    if (raw.handbrake_pressure_switch > INT64_C(1)) return CAN_RANGE;
    result.handbrake_pressure_switch = (uint8_t)raw.handbrake_pressure_switch;
    result.accelerator_main_position = (double)((double)raw.accelerator_main_position * 0.39215686274509803 + 0.0);
    result.accelerator_redundant_position = (double)((double)raw.accelerator_redundant_position * 0.39215686274509803 + 0.0);
    if (raw.front_pressure > INT64_C(3000)) return CAN_RANGE;
    result.front_pressure = (uint16_t)raw.front_pressure;
    if (raw.rear_pressure > INT64_C(3000)) return CAN_RANGE;
    result.rear_pressure = (uint16_t)raw.rear_pressure;
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_dei_status_pack(can_frame_t* dst, const car_can_dei_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_dei_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->ignition_lv;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.ignition_lv = (uint8_t)value;
    }
    {
        uint8_t value = src->ignition_charge;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.ignition_charge = (uint8_t)value;
    }
    {
        uint8_t value = src->ignition_drive;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.ignition_drive = (uint8_t)value;
    }
    {
        uint8_t value = src->gear;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.gear = (uint8_t)value;
    }
    {
        uint8_t value = src->hazards;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.hazards = (uint8_t)value;
    }
    {
        uint8_t value = src->left_turn_signal;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.left_turn_signal = (uint8_t)value;
    }
    {
        uint8_t value = src->right_turn_signal;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.right_turn_signal = (uint8_t)value;
    }
    {
        uint8_t value = src->regen_enable;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.regen_enable = (uint8_t)value;
    }
    {
        uint8_t value = src->regen_activate;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.regen_activate = (uint8_t)value;
    }
    {
        uint8_t value = src->horn;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.horn = (uint8_t)value;
    }
    {
        uint8_t value = src->push_to_talk;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.push_to_talk = (uint8_t)value;
    }
    {
        uint8_t value = src->headlights;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.headlights = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(98),
        .len = 2, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_dei_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_dei_status_unpack(car_can_dei_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(98) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_dei_status_t raw = {0};
    if (car_can_raw_dei_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_dei_status_t result = {0};
    if (raw.ignition_lv > INT64_C(1)) return CAN_RANGE;
    result.ignition_lv = (uint8_t)raw.ignition_lv;
    if (raw.ignition_charge > INT64_C(1)) return CAN_RANGE;
    result.ignition_charge = (uint8_t)raw.ignition_charge;
    if (raw.ignition_drive > INT64_C(1)) return CAN_RANGE;
    result.ignition_drive = (uint8_t)raw.ignition_drive;
    if (raw.gear > INT64_C(3)) return CAN_RANGE;
    result.gear = (uint8_t)raw.gear;
    if (raw.hazards > INT64_C(1)) return CAN_RANGE;
    result.hazards = (uint8_t)raw.hazards;
    if (raw.left_turn_signal > INT64_C(1)) return CAN_RANGE;
    result.left_turn_signal = (uint8_t)raw.left_turn_signal;
    if (raw.right_turn_signal > INT64_C(1)) return CAN_RANGE;
    result.right_turn_signal = (uint8_t)raw.right_turn_signal;
    if (raw.regen_enable > INT64_C(1)) return CAN_RANGE;
    result.regen_enable = (uint8_t)raw.regen_enable;
    if (raw.regen_activate > INT64_C(1)) return CAN_RANGE;
    result.regen_activate = (uint8_t)raw.regen_activate;
    if (raw.horn > INT64_C(1)) return CAN_RANGE;
    result.horn = (uint8_t)raw.horn;
    if (raw.push_to_talk > INT64_C(1)) return CAN_RANGE;
    result.push_to_talk = (uint8_t)raw.push_to_talk;
    if (raw.headlights > INT64_C(1)) return CAN_RANGE;
    result.headlights = (uint8_t)raw.headlights;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_vcu_status_pack(can_frame_t* dst, const car_can_vcu_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_vcu_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->status;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.status = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_motor;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_motor = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_bps;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_bps = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_pedals;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_pedals = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_motor_controller_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_motor_controller_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_bps_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_bps_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_driver_input_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_driver_input_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_pedals_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_pedals_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_tipping;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_tipping = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_direction_change_lockout;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_direction_change_lockout = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_motor_controller_fan;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_motor_controller_fan = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_motor_fan;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_motor_fan = (uint8_t)value;
    }
    {
        uint8_t value = src->regen_enabled;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.regen_enabled = (uint8_t)value;
    }
    {
        uint8_t value = src->state;
        if (value > INT64_C(7)) {
            value = (uint8_t)INT64_C(7); status = CAN_CLAMPED;
        }
        raw.state = (uint8_t)value;
    }
    {
        uint8_t value = src->fsm_brake;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fsm_brake = (uint8_t)value;
    }
    {
        uint8_t value = src->fsmbps_drive_ok;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fsmbps_drive_ok = (uint8_t)value;
    }
    {
        uint8_t value = src->fsmbps_regen_ok;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fsmbps_regen_ok = (uint8_t)value;
    }
    {
        uint8_t value = src->fsm_driver_regen_enable;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fsm_driver_regen_enable = (uint8_t)value;
    }
    {
        uint8_t value = src->fsm_driver_regen_request;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fsm_driver_regen_request = (uint8_t)value;
    }
    {
        uint8_t value = src->fsm_gear;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.fsm_gear = (uint8_t)value;
    }
    {
        uint8_t value = src->motor_command_source;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.motor_command_source = (uint8_t)value;
    }
    {
        double value = (double)src->motor_controller_fan_tach;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 20475.0) { value = 20475.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 5.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.motor_controller_fan_tach = (uint16_t)encoded;
    }
    {
        double value = (double)src->motor_fan_tach;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 20475.0) { value = 20475.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 5.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.motor_fan_tach = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(99),
        .len = 8, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_vcu_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_vcu_status_unpack(car_can_vcu_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(99) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_vcu_status_t raw = {0};
    if (car_can_raw_vcu_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_vcu_status_t result = {0};
    if (raw.status > INT64_C(1)) return CAN_RANGE;
    result.status = (uint8_t)raw.status;
    if (raw.fault_motor > INT64_C(1)) return CAN_RANGE;
    result.fault_motor = (uint8_t)raw.fault_motor;
    if (raw.fault_bps > INT64_C(1)) return CAN_RANGE;
    result.fault_bps = (uint8_t)raw.fault_bps;
    if (raw.fault_pedals > INT64_C(1)) return CAN_RANGE;
    result.fault_pedals = (uint8_t)raw.fault_pedals;
    if (raw.fault_motor_controller_watchdog > INT64_C(1)) return CAN_RANGE;
    result.fault_motor_controller_watchdog = (uint8_t)raw.fault_motor_controller_watchdog;
    if (raw.fault_bps_watchdog > INT64_C(1)) return CAN_RANGE;
    result.fault_bps_watchdog = (uint8_t)raw.fault_bps_watchdog;
    if (raw.fault_driver_input_watchdog > INT64_C(1)) return CAN_RANGE;
    result.fault_driver_input_watchdog = (uint8_t)raw.fault_driver_input_watchdog;
    if (raw.fault_pedals_watchdog > INT64_C(1)) return CAN_RANGE;
    result.fault_pedals_watchdog = (uint8_t)raw.fault_pedals_watchdog;
    if (raw.warning_tipping > INT64_C(1)) return CAN_RANGE;
    result.warning_tipping = (uint8_t)raw.warning_tipping;
    if (raw.warning_direction_change_lockout > INT64_C(1)) return CAN_RANGE;
    result.warning_direction_change_lockout = (uint8_t)raw.warning_direction_change_lockout;
    if (raw.warning_motor_controller_fan > INT64_C(1)) return CAN_RANGE;
    result.warning_motor_controller_fan = (uint8_t)raw.warning_motor_controller_fan;
    if (raw.warning_motor_fan > INT64_C(1)) return CAN_RANGE;
    result.warning_motor_fan = (uint8_t)raw.warning_motor_fan;
    if (raw.regen_enabled > INT64_C(1)) return CAN_RANGE;
    result.regen_enabled = (uint8_t)raw.regen_enabled;
    if (raw.state > INT64_C(7)) return CAN_RANGE;
    result.state = (uint8_t)raw.state;
    if (raw.fsm_brake > INT64_C(1)) return CAN_RANGE;
    result.fsm_brake = (uint8_t)raw.fsm_brake;
    if (raw.fsmbps_drive_ok > INT64_C(1)) return CAN_RANGE;
    result.fsmbps_drive_ok = (uint8_t)raw.fsmbps_drive_ok;
    if (raw.fsmbps_regen_ok > INT64_C(1)) return CAN_RANGE;
    result.fsmbps_regen_ok = (uint8_t)raw.fsmbps_regen_ok;
    if (raw.fsm_driver_regen_enable > INT64_C(1)) return CAN_RANGE;
    result.fsm_driver_regen_enable = (uint8_t)raw.fsm_driver_regen_enable;
    if (raw.fsm_driver_regen_request > INT64_C(1)) return CAN_RANGE;
    result.fsm_driver_regen_request = (uint8_t)raw.fsm_driver_regen_request;
    if (raw.fsm_gear > INT64_C(3)) return CAN_RANGE;
    result.fsm_gear = (uint8_t)raw.fsm_gear;
    if (raw.motor_command_source > INT64_C(1)) return CAN_RANGE;
    result.motor_command_source = (uint8_t)raw.motor_command_source;
    if (raw.motor_controller_fan_tach > INT64_C(4095)) return CAN_RANGE;
    result.motor_controller_fan_tach = (uint16_t)((double)raw.motor_controller_fan_tach * 5.0 + 0.0);
    if (raw.motor_fan_tach > INT64_C(4095)) return CAN_RANGE;
    result.motor_fan_tach = (uint16_t)((double)raw.motor_fan_tach * 5.0 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_bps_pack_data_pack(can_frame_t* dst, const car_can_bps_pack_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_pack_data_t raw = {0};
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
    if (car_can_raw_bps_pack_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_pack_data_unpack(car_can_bps_pack_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(112) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_pack_data_t raw = {0};
    if (car_can_raw_bps_pack_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_pack_data_t result = {0};
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

can_status_t car_can_bps_voltage_data_pack(can_frame_t* dst, const car_can_bps_voltage_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_voltage_data_t raw = {0};
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
    if (car_can_raw_bps_voltage_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_voltage_data_unpack(car_can_bps_voltage_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(113) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_voltage_data_t raw = {0};
    if (car_can_raw_bps_voltage_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_voltage_data_t result = {0};
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

can_status_t car_can_bps_temperature_data_pack(can_frame_t* dst, const car_can_bps_temperature_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_temperature_data_t raw = {0};
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
    if (car_can_raw_bps_temperature_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_temperature_data_unpack(car_can_bps_temperature_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(114) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_temperature_data_t raw = {0};
    if (car_can_raw_bps_temperature_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_temperature_data_t result = {0};
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

can_status_t car_can_smd_status_pack(can_frame_t* dst, const car_can_smd_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_smd_status_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(115),
        .len = 0, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_smd_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_smd_status_unpack(car_can_smd_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(115) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_smd_status_t raw = {0};
    if (car_can_raw_smd_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_smd_status_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_bps_command_echo_pack(can_frame_t* dst, const car_can_bps_command_echo_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_command_echo_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->mode;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mode = (uint8_t)value;
    }
    {
        uint8_t value = src->mppt_charge_scheme;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mppt_charge_scheme = (uint8_t)value;
    }
    {
        uint32_t value = src->voltage_override_mask;
        raw.voltage_override_mask = (uint32_t)value;
    }
    {
        uint32_t value = src->temperature_override_mask;
        raw.temperature_override_mask = (uint32_t)value;
    }
    can_frame_t result = { .id = UINT32_C(116),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_bps_command_echo_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_command_echo_unpack(car_can_bps_command_echo_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(116) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_command_echo_t raw = {0};
    if (car_can_raw_bps_command_echo_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_command_echo_t result = {0};
    if (raw.mode > INT64_C(1)) return CAN_RANGE;
    result.mode = (uint8_t)raw.mode;
    if (raw.mppt_charge_scheme > INT64_C(1)) return CAN_RANGE;
    result.mppt_charge_scheme = (uint8_t)raw.mppt_charge_scheme;
    result.voltage_override_mask = (uint32_t)raw.voltage_override_mask;
    result.temperature_override_mask = (uint32_t)raw.temperature_override_mask;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_bps_fault_snapshot_pack(can_frame_t* dst, const car_can_bps_fault_snapshot_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_fault_snapshot_t raw = {0};
    can_status_t status = CAN_OK;
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
        uint8_t value = src->current_valid;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.current_valid = (uint8_t)value;
    }
    {
        uint32_t value = src->capture_time_ms;
        raw.capture_time_ms = (uint32_t)value;
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
    can_frame_t result = { .id = UINT32_C(117),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_bps_fault_snapshot_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_fault_snapshot_unpack(car_can_bps_fault_snapshot_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(117) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_fault_snapshot_t raw = {0};
    if (car_can_raw_bps_fault_snapshot_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_fault_snapshot_t result = {0};
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
    if (raw.current_valid > INT64_C(1)) return CAN_RANGE;
    result.current_valid = (uint8_t)raw.current_valid;
    result.capture_time_ms = (uint32_t)raw.capture_time_ms;
    if (raw.battery_current > INT64_C(16383)) return CAN_RANGE;
    result.battery_current = (double)((double)raw.battery_current * 0.01 + -50.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_bps_fault_voltage_snapshot_pack(can_frame_t* dst, const car_can_bps_fault_voltage_snapshot_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_fault_voltage_snapshot_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint32_t value = src->sample_valid_mask;
        raw.sample_valid_mask = (uint32_t)value;
    }
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
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(118),
        .len = 64, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_bps_fault_voltage_snapshot_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_fault_voltage_snapshot_unpack(car_can_bps_fault_voltage_snapshot_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(118) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_fault_voltage_snapshot_t raw = {0};
    if (car_can_raw_bps_fault_voltage_snapshot_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_fault_voltage_snapshot_t result = {0};
    result.sample_valid_mask = (uint32_t)raw.sample_valid_mask;
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
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_bps_fault_temperature_snapshot_pack(can_frame_t* dst, const car_can_bps_fault_temperature_snapshot_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_bps_fault_temperature_snapshot_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint32_t value = src->sample_valid_mask;
        raw.sample_valid_mask = (uint32_t)value;
    }
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
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(119),
        .len = 64, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_bps_fault_temperature_snapshot_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_bps_fault_temperature_snapshot_unpack(car_can_bps_fault_temperature_snapshot_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(119) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_bps_fault_temperature_snapshot_t raw = {0};
    if (car_can_raw_bps_fault_temperature_snapshot_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_bps_fault_temperature_snapshot_t result = {0};
    result.sample_valid_mask = (uint32_t)raw.sample_valid_mask;
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
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_mppt_interface_status_pack(can_frame_t* dst, const car_can_mppt_interface_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_mppt_interface_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->warning_bps_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_bps_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_mppta_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_mppta_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_mpptb_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_mpptb_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_mpptc_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_mpptc_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_fan;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_fan = (uint8_t)value;
    }
    {
        uint8_t value = src->mppt_set_mode;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.mppt_set_mode = (uint8_t)value;
    }
    {
        uint8_t value = src->sweeping_active;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sweeping_active = (uint8_t)value;
    }
    {
        double value = (double)src->fan_tach;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 20475.0) { value = 20475.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 5.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.fan_tach = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(121),
        .len = 5, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_mppt_interface_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_mppt_interface_status_unpack(car_can_mppt_interface_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(121) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_mppt_interface_status_t raw = {0};
    if (car_can_raw_mppt_interface_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_mppt_interface_status_t result = {0};
    if (raw.warning_bps_watchdog > INT64_C(1)) return CAN_RANGE;
    result.warning_bps_watchdog = (uint8_t)raw.warning_bps_watchdog;
    if (raw.warning_mppta_watchdog > INT64_C(1)) return CAN_RANGE;
    result.warning_mppta_watchdog = (uint8_t)raw.warning_mppta_watchdog;
    if (raw.warning_mpptb_watchdog > INT64_C(1)) return CAN_RANGE;
    result.warning_mpptb_watchdog = (uint8_t)raw.warning_mpptb_watchdog;
    if (raw.warning_mpptc_watchdog > INT64_C(1)) return CAN_RANGE;
    result.warning_mpptc_watchdog = (uint8_t)raw.warning_mpptc_watchdog;
    if (raw.warning_fan > INT64_C(1)) return CAN_RANGE;
    result.warning_fan = (uint8_t)raw.warning_fan;
    if (raw.mppt_set_mode > INT64_C(3)) return CAN_RANGE;
    result.mppt_set_mode = (uint8_t)raw.mppt_set_mode;
    if (raw.sweeping_active > INT64_C(1)) return CAN_RANGE;
    result.sweeping_active = (uint8_t)raw.sweeping_active;
    if (raw.fan_tach > INT64_C(4095)) return CAN_RANGE;
    result.fan_tach = (uint16_t)((double)raw.fan_tach * 5.0 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_mika_status_pack(can_frame_t* dst, const car_can_mika_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_mika_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->warning_temperature;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_temperature = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_over_temperature;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_over_temperature = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_display_disconnected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_display_disconnected = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_rear_camera_disconnected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_rear_camera_disconnected = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_router_disconnected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_router_disconnected = (uint8_t)value;
    }
    {
        uint8_t value = src->cellular_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.cellular_connected = (uint8_t)value;
    }
    {
        uint8_t value = src->internet_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.internet_connected = (uint8_t)value;
    }
    {
        uint8_t value = src->gps_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.gps_connected = (uint8_t)value;
    }
    {
        uint8_t value = src->driver_camera_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.driver_camera_connected = (uint8_t)value;
    }
    {
        uint8_t value = src->display_frame_rate;
        if (value > INT64_C(60)) {
            value = (uint8_t)INT64_C(60); status = CAN_CLAMPED;
        }
        raw.display_frame_rate = (uint8_t)value;
    }
    {
        double value = (double)src->so_c_temperature;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -20.0) { value = -20.0; status = CAN_CLAMPED; }
        if (value > 85.0) { value = 85.0; status = CAN_CLAMPED; }
        double encoded = round((value - -20.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 210.0) encoded = 210.0;
        raw.so_c_temperature = (uint8_t)encoded;
    }
    {
        double value = (double)src->cm5_fan_tach;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 20475.0) { value = 20475.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 5.0);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.cm5_fan_tach = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(130),
        .len = 7, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_mika_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_mika_status_unpack(car_can_mika_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(130) || src->len != 7 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_mika_status_t raw = {0};
    if (car_can_raw_mika_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_mika_status_t result = {0};
    if (raw.warning_temperature > INT64_C(1)) return CAN_RANGE;
    result.warning_temperature = (uint8_t)raw.warning_temperature;
    if (raw.warning_over_temperature > INT64_C(1)) return CAN_RANGE;
    result.warning_over_temperature = (uint8_t)raw.warning_over_temperature;
    if (raw.warning_display_disconnected > INT64_C(1)) return CAN_RANGE;
    result.warning_display_disconnected = (uint8_t)raw.warning_display_disconnected;
    if (raw.warning_rear_camera_disconnected > INT64_C(1)) return CAN_RANGE;
    result.warning_rear_camera_disconnected = (uint8_t)raw.warning_rear_camera_disconnected;
    if (raw.warning_router_disconnected > INT64_C(1)) return CAN_RANGE;
    result.warning_router_disconnected = (uint8_t)raw.warning_router_disconnected;
    if (raw.cellular_connected > INT64_C(1)) return CAN_RANGE;
    result.cellular_connected = (uint8_t)raw.cellular_connected;
    if (raw.internet_connected > INT64_C(1)) return CAN_RANGE;
    result.internet_connected = (uint8_t)raw.internet_connected;
    if (raw.gps_connected > INT64_C(1)) return CAN_RANGE;
    result.gps_connected = (uint8_t)raw.gps_connected;
    if (raw.driver_camera_connected > INT64_C(1)) return CAN_RANGE;
    result.driver_camera_connected = (uint8_t)raw.driver_camera_connected;
    if (raw.display_frame_rate > INT64_C(60)) return CAN_RANGE;
    result.display_frame_rate = (uint8_t)raw.display_frame_rate;
    if (raw.so_c_temperature > INT64_C(210)) return CAN_RANGE;
    result.so_c_temperature = (double)((double)raw.so_c_temperature * 0.5 + -20.0);
    if (raw.cm5_fan_tach > INT64_C(4095)) return CAN_RANGE;
    result.cm5_fan_tach = (uint16_t)((double)raw.cm5_fan_tach * 5.0 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_lvc_status_pack(can_frame_t* dst, const car_can_lvc_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_lvc_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->warning_hss_over_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hss_over_current = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_hssi2_t;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hssi2_t = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_hss_over_temperature;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hss_over_temperature = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_hss_open_load;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hss_open_load = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_hss_short_to_battery;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hss_short_to_battery = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_hssvbb_undervoltage;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hssvbb_undervoltage = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_hssspi_error;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hssspi_error = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_hss_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_hss_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_no_dcdc_output;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_no_dcdc_output = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_bpslv_blown_fuse;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_bpslv_blown_fuse = (uint8_t)value;
    }
    {
        uint8_t value = src->dcdc_enabled;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.dcdc_enabled = (uint8_t)value;
    }
    {
        uint8_t value = src->bpslv_enabled;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.bpslv_enabled = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(144),
        .len = 2, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_lvc_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_lvc_status_unpack(car_can_lvc_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(144) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_lvc_status_t raw = {0};
    if (car_can_raw_lvc_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_lvc_status_t result = {0};
    if (raw.warning_hss_over_current > INT64_C(1)) return CAN_RANGE;
    result.warning_hss_over_current = (uint8_t)raw.warning_hss_over_current;
    if (raw.warning_hssi2_t > INT64_C(1)) return CAN_RANGE;
    result.warning_hssi2_t = (uint8_t)raw.warning_hssi2_t;
    if (raw.warning_hss_over_temperature > INT64_C(1)) return CAN_RANGE;
    result.warning_hss_over_temperature = (uint8_t)raw.warning_hss_over_temperature;
    if (raw.warning_hss_open_load > INT64_C(1)) return CAN_RANGE;
    result.warning_hss_open_load = (uint8_t)raw.warning_hss_open_load;
    if (raw.warning_hss_short_to_battery > INT64_C(1)) return CAN_RANGE;
    result.warning_hss_short_to_battery = (uint8_t)raw.warning_hss_short_to_battery;
    if (raw.warning_hssvbb_undervoltage > INT64_C(1)) return CAN_RANGE;
    result.warning_hssvbb_undervoltage = (uint8_t)raw.warning_hssvbb_undervoltage;
    if (raw.warning_hssspi_error > INT64_C(1)) return CAN_RANGE;
    result.warning_hssspi_error = (uint8_t)raw.warning_hssspi_error;
    if (raw.warning_hss_watchdog > INT64_C(1)) return CAN_RANGE;
    result.warning_hss_watchdog = (uint8_t)raw.warning_hss_watchdog;
    if (raw.warning_no_dcdc_output > INT64_C(1)) return CAN_RANGE;
    result.warning_no_dcdc_output = (uint8_t)raw.warning_no_dcdc_output;
    if (raw.warning_bpslv_blown_fuse > INT64_C(1)) return CAN_RANGE;
    result.warning_bpslv_blown_fuse = (uint8_t)raw.warning_bpslv_blown_fuse;
    if (raw.dcdc_enabled > INT64_C(1)) return CAN_RANGE;
    result.dcdc_enabled = (uint8_t)raw.dcdc_enabled;
    if (raw.bpslv_enabled > INT64_C(1)) return CAN_RANGE;
    result.bpslv_enabled = (uint8_t)raw.bpslv_enabled;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_lvc_current_data_pack(can_frame_t* dst, const car_can_lvc_current_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_lvc_current_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->dcdc_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 163.83) { value = 163.83; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.dcdc_current = (uint16_t)encoded;
    }
    {
        double value = (double)src->lvb_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -50.0) { value = -50.0; status = CAN_CLAMPED; }
        if (value > 113.83) { value = 113.83; status = CAN_CLAMPED; }
        double encoded = round((value - -50.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.lvb_current = (uint16_t)encoded;
    }
    {
        double value = (double)src->total_output_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 163.83) { value = 163.83; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.total_output_current = (uint16_t)encoded;
    }
    {
        double value = (double)src->bpslv_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.bpslv_current = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(145),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_lvc_current_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_lvc_current_data_unpack(car_can_lvc_current_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(145) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_lvc_current_data_t raw = {0};
    if (car_can_raw_lvc_current_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_lvc_current_data_t result = {0};
    if (raw.dcdc_current > INT64_C(16383)) return CAN_RANGE;
    result.dcdc_current = (double)((double)raw.dcdc_current * 0.01 + 0.0);
    if (raw.lvb_current > INT64_C(16383)) return CAN_RANGE;
    result.lvb_current = (double)((double)raw.lvb_current * 0.01 + -50.0);
    if (raw.total_output_current > INT64_C(16383)) return CAN_RANGE;
    result.total_output_current = (double)((double)raw.total_output_current * 0.01 + 0.0);
    if (raw.bpslv_current > INT64_C(4095)) return CAN_RANGE;
    result.bpslv_current = (double)((double)raw.bpslv_current * 0.01 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_lvc_voltage_data_pack(can_frame_t* dst, const car_can_lvc_voltage_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_lvc_voltage_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->dcdc_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.dcdc_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->lvb_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.lvb_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->power_supply_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.power_supply_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->bpslv_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.bpslv_voltage = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(146),
        .len = 7, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_lvc_voltage_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_lvc_voltage_data_unpack(car_can_lvc_voltage_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(146) || src->len != 7 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_lvc_voltage_data_t raw = {0};
    if (car_can_raw_lvc_voltage_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_lvc_voltage_data_t result = {0};
    if (raw.dcdc_voltage > INT64_C(1023)) return CAN_RANGE;
    result.dcdc_voltage = (double)((double)raw.dcdc_voltage * 0.05 + 0.0);
    if (raw.lvb_voltage > INT64_C(1023)) return CAN_RANGE;
    result.lvb_voltage = (double)((double)raw.lvb_voltage * 0.05 + 0.0);
    if (raw.power_supply_voltage > INT64_C(1023)) return CAN_RANGE;
    result.power_supply_voltage = (double)((double)raw.power_supply_voltage * 0.05 + 0.0);
    if (raw.bpslv_voltage > INT64_C(1023)) return CAN_RANGE;
    result.bpslv_voltage = (double)((double)raw.bpslv_voltage * 0.05 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_lvc_temperature_data_pack(can_frame_t* dst, const car_can_lvc_temperature_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_lvc_temperature_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->bpsfet_temperature;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.bpsfet_temperature = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(147),
        .len = 4, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_lvc_temperature_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_lvc_temperature_data_unpack(car_can_lvc_temperature_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(147) || src->len != 4 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_lvc_temperature_data_t raw = {0};
    if (car_can_raw_lvc_temperature_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_lvc_temperature_data_t result = {0};
    if (raw.bpsfet_temperature > INT64_C(4800)) return CAN_RANGE;
    result.bpsfet_temperature = (double)((double)raw.bpsfet_temperature * 0.05 + -40.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_lvc_over_current_snapshot_pack(can_frame_t* dst, const car_can_lvc_over_current_snapshot_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_lvc_over_current_snapshot_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->fault_over_current;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated = (uint8_t)value;
    }
    {
        uint32_t value = src->capture_time_ms;
        raw.capture_time_ms = (uint32_t)value;
    }
    {
        double value = (double)src->trip_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(148),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_lvc_over_current_snapshot_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_lvc_over_current_snapshot_unpack(car_can_lvc_over_current_snapshot_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(148) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_lvc_over_current_snapshot_t raw = {0};
    if (car_can_raw_lvc_over_current_snapshot_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_lvc_over_current_snapshot_t result = {0};
    if (raw.fault_over_current > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current = (uint8_t)raw.fault_over_current;
    if (raw.fault_i2_t > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t = (uint8_t)raw.fault_i2_t;
    if (raw.trip_current_saturated > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated = (uint8_t)raw.trip_current_saturated;
    result.capture_time_ms = (uint32_t)raw.capture_time_ms;
    result.trip_current = (double)((double)raw.trip_current * 0.01 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_pdu_status_pack(can_frame_t* dst, const car_can_pdu_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pdu_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->fault_channel_over_current1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current7 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t7 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature7 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load7 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery7 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current8 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t8 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature8 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load8 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery8 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current9 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t9 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature9 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load9 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery9 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current10 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t10 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature10 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load10 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery10 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current11 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t11 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature11 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load11 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery11 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_current12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_current12 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_i2_t12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_i2_t12 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_over_temperature12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_over_temperature12 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_open_load12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_open_load12 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_channel_short_to_battery12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_channel_short_to_battery12 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_vbb_undervoltage1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_vbb_undervoltage1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_spi_error1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_spi_error1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_watchdog1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_watchdog1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_vbb_undervoltage2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_vbb_undervoltage2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_spi_error2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_spi_error2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_watchdog2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_watchdog2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_vbb_undervoltage3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_vbb_undervoltage3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_spi_error3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_spi_error3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_watchdog3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_watchdog3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_vbb_undervoltage4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_vbb_undervoltage4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_spi_error4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_spi_error4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_watchdog4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_watchdog4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_vbb_undervoltage5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_vbb_undervoltage5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_spi_error5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_spi_error5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_watchdog5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_watchdog5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_vbb_undervoltage6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_vbb_undervoltage6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_spi_error6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_spi_error6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_chip_watchdog6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_chip_watchdog6 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_blown_fuse;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_blown_fuse = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse1 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse2 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse3 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse4 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse5 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse6 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse7 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse8 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse9 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse10 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse11 = (uint8_t)value;
    }
    {
        uint8_t value = src->warning_channel_blown_fuse12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.warning_channel_blown_fuse12 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled1 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled2 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled3 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled4 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled5 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled6 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled7 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled8 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled9 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled10 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled11 = (uint8_t)value;
    }
    {
        uint8_t value = src->channel_enabled12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.channel_enabled12 = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(160),
        .len = 16, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pdu_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pdu_status_unpack(car_can_pdu_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(160) || src->len != 16 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pdu_status_t raw = {0};
    if (car_can_raw_pdu_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pdu_status_t result = {0};
    if (raw.fault_channel_over_current1 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current1 = (uint8_t)raw.fault_channel_over_current1;
    if (raw.fault_channel_i2_t1 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t1 = (uint8_t)raw.fault_channel_i2_t1;
    if (raw.fault_channel_over_temperature1 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature1 = (uint8_t)raw.fault_channel_over_temperature1;
    if (raw.fault_channel_open_load1 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load1 = (uint8_t)raw.fault_channel_open_load1;
    if (raw.fault_channel_short_to_battery1 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery1 = (uint8_t)raw.fault_channel_short_to_battery1;
    if (raw.fault_channel_over_current2 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current2 = (uint8_t)raw.fault_channel_over_current2;
    if (raw.fault_channel_i2_t2 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t2 = (uint8_t)raw.fault_channel_i2_t2;
    if (raw.fault_channel_over_temperature2 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature2 = (uint8_t)raw.fault_channel_over_temperature2;
    if (raw.fault_channel_open_load2 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load2 = (uint8_t)raw.fault_channel_open_load2;
    if (raw.fault_channel_short_to_battery2 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery2 = (uint8_t)raw.fault_channel_short_to_battery2;
    if (raw.fault_channel_over_current3 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current3 = (uint8_t)raw.fault_channel_over_current3;
    if (raw.fault_channel_i2_t3 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t3 = (uint8_t)raw.fault_channel_i2_t3;
    if (raw.fault_channel_over_temperature3 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature3 = (uint8_t)raw.fault_channel_over_temperature3;
    if (raw.fault_channel_open_load3 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load3 = (uint8_t)raw.fault_channel_open_load3;
    if (raw.fault_channel_short_to_battery3 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery3 = (uint8_t)raw.fault_channel_short_to_battery3;
    if (raw.fault_channel_over_current4 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current4 = (uint8_t)raw.fault_channel_over_current4;
    if (raw.fault_channel_i2_t4 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t4 = (uint8_t)raw.fault_channel_i2_t4;
    if (raw.fault_channel_over_temperature4 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature4 = (uint8_t)raw.fault_channel_over_temperature4;
    if (raw.fault_channel_open_load4 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load4 = (uint8_t)raw.fault_channel_open_load4;
    if (raw.fault_channel_short_to_battery4 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery4 = (uint8_t)raw.fault_channel_short_to_battery4;
    if (raw.fault_channel_over_current5 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current5 = (uint8_t)raw.fault_channel_over_current5;
    if (raw.fault_channel_i2_t5 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t5 = (uint8_t)raw.fault_channel_i2_t5;
    if (raw.fault_channel_over_temperature5 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature5 = (uint8_t)raw.fault_channel_over_temperature5;
    if (raw.fault_channel_open_load5 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load5 = (uint8_t)raw.fault_channel_open_load5;
    if (raw.fault_channel_short_to_battery5 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery5 = (uint8_t)raw.fault_channel_short_to_battery5;
    if (raw.fault_channel_over_current6 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current6 = (uint8_t)raw.fault_channel_over_current6;
    if (raw.fault_channel_i2_t6 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t6 = (uint8_t)raw.fault_channel_i2_t6;
    if (raw.fault_channel_over_temperature6 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature6 = (uint8_t)raw.fault_channel_over_temperature6;
    if (raw.fault_channel_open_load6 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load6 = (uint8_t)raw.fault_channel_open_load6;
    if (raw.fault_channel_short_to_battery6 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery6 = (uint8_t)raw.fault_channel_short_to_battery6;
    if (raw.fault_channel_over_current7 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current7 = (uint8_t)raw.fault_channel_over_current7;
    if (raw.fault_channel_i2_t7 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t7 = (uint8_t)raw.fault_channel_i2_t7;
    if (raw.fault_channel_over_temperature7 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature7 = (uint8_t)raw.fault_channel_over_temperature7;
    if (raw.fault_channel_open_load7 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load7 = (uint8_t)raw.fault_channel_open_load7;
    if (raw.fault_channel_short_to_battery7 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery7 = (uint8_t)raw.fault_channel_short_to_battery7;
    if (raw.fault_channel_over_current8 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current8 = (uint8_t)raw.fault_channel_over_current8;
    if (raw.fault_channel_i2_t8 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t8 = (uint8_t)raw.fault_channel_i2_t8;
    if (raw.fault_channel_over_temperature8 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature8 = (uint8_t)raw.fault_channel_over_temperature8;
    if (raw.fault_channel_open_load8 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load8 = (uint8_t)raw.fault_channel_open_load8;
    if (raw.fault_channel_short_to_battery8 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery8 = (uint8_t)raw.fault_channel_short_to_battery8;
    if (raw.fault_channel_over_current9 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current9 = (uint8_t)raw.fault_channel_over_current9;
    if (raw.fault_channel_i2_t9 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t9 = (uint8_t)raw.fault_channel_i2_t9;
    if (raw.fault_channel_over_temperature9 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature9 = (uint8_t)raw.fault_channel_over_temperature9;
    if (raw.fault_channel_open_load9 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load9 = (uint8_t)raw.fault_channel_open_load9;
    if (raw.fault_channel_short_to_battery9 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery9 = (uint8_t)raw.fault_channel_short_to_battery9;
    if (raw.fault_channel_over_current10 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current10 = (uint8_t)raw.fault_channel_over_current10;
    if (raw.fault_channel_i2_t10 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t10 = (uint8_t)raw.fault_channel_i2_t10;
    if (raw.fault_channel_over_temperature10 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature10 = (uint8_t)raw.fault_channel_over_temperature10;
    if (raw.fault_channel_open_load10 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load10 = (uint8_t)raw.fault_channel_open_load10;
    if (raw.fault_channel_short_to_battery10 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery10 = (uint8_t)raw.fault_channel_short_to_battery10;
    if (raw.fault_channel_over_current11 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current11 = (uint8_t)raw.fault_channel_over_current11;
    if (raw.fault_channel_i2_t11 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t11 = (uint8_t)raw.fault_channel_i2_t11;
    if (raw.fault_channel_over_temperature11 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature11 = (uint8_t)raw.fault_channel_over_temperature11;
    if (raw.fault_channel_open_load11 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load11 = (uint8_t)raw.fault_channel_open_load11;
    if (raw.fault_channel_short_to_battery11 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery11 = (uint8_t)raw.fault_channel_short_to_battery11;
    if (raw.fault_channel_over_current12 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_current12 = (uint8_t)raw.fault_channel_over_current12;
    if (raw.fault_channel_i2_t12 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_i2_t12 = (uint8_t)raw.fault_channel_i2_t12;
    if (raw.fault_channel_over_temperature12 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_over_temperature12 = (uint8_t)raw.fault_channel_over_temperature12;
    if (raw.fault_channel_open_load12 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_open_load12 = (uint8_t)raw.fault_channel_open_load12;
    if (raw.fault_channel_short_to_battery12 > INT64_C(1)) return CAN_RANGE;
    result.fault_channel_short_to_battery12 = (uint8_t)raw.fault_channel_short_to_battery12;
    if (raw.fault_chip_vbb_undervoltage1 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_vbb_undervoltage1 = (uint8_t)raw.fault_chip_vbb_undervoltage1;
    if (raw.fault_chip_spi_error1 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_spi_error1 = (uint8_t)raw.fault_chip_spi_error1;
    if (raw.fault_chip_watchdog1 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_watchdog1 = (uint8_t)raw.fault_chip_watchdog1;
    if (raw.fault_chip_vbb_undervoltage2 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_vbb_undervoltage2 = (uint8_t)raw.fault_chip_vbb_undervoltage2;
    if (raw.fault_chip_spi_error2 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_spi_error2 = (uint8_t)raw.fault_chip_spi_error2;
    if (raw.fault_chip_watchdog2 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_watchdog2 = (uint8_t)raw.fault_chip_watchdog2;
    if (raw.fault_chip_vbb_undervoltage3 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_vbb_undervoltage3 = (uint8_t)raw.fault_chip_vbb_undervoltage3;
    if (raw.fault_chip_spi_error3 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_spi_error3 = (uint8_t)raw.fault_chip_spi_error3;
    if (raw.fault_chip_watchdog3 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_watchdog3 = (uint8_t)raw.fault_chip_watchdog3;
    if (raw.fault_chip_vbb_undervoltage4 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_vbb_undervoltage4 = (uint8_t)raw.fault_chip_vbb_undervoltage4;
    if (raw.fault_chip_spi_error4 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_spi_error4 = (uint8_t)raw.fault_chip_spi_error4;
    if (raw.fault_chip_watchdog4 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_watchdog4 = (uint8_t)raw.fault_chip_watchdog4;
    if (raw.fault_chip_vbb_undervoltage5 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_vbb_undervoltage5 = (uint8_t)raw.fault_chip_vbb_undervoltage5;
    if (raw.fault_chip_spi_error5 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_spi_error5 = (uint8_t)raw.fault_chip_spi_error5;
    if (raw.fault_chip_watchdog5 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_watchdog5 = (uint8_t)raw.fault_chip_watchdog5;
    if (raw.fault_chip_vbb_undervoltage6 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_vbb_undervoltage6 = (uint8_t)raw.fault_chip_vbb_undervoltage6;
    if (raw.fault_chip_spi_error6 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_spi_error6 = (uint8_t)raw.fault_chip_spi_error6;
    if (raw.fault_chip_watchdog6 > INT64_C(1)) return CAN_RANGE;
    result.fault_chip_watchdog6 = (uint8_t)raw.fault_chip_watchdog6;
    if (raw.warning_blown_fuse > INT64_C(1)) return CAN_RANGE;
    result.warning_blown_fuse = (uint8_t)raw.warning_blown_fuse;
    if (raw.warning_channel_blown_fuse1 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse1 = (uint8_t)raw.warning_channel_blown_fuse1;
    if (raw.warning_channel_blown_fuse2 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse2 = (uint8_t)raw.warning_channel_blown_fuse2;
    if (raw.warning_channel_blown_fuse3 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse3 = (uint8_t)raw.warning_channel_blown_fuse3;
    if (raw.warning_channel_blown_fuse4 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse4 = (uint8_t)raw.warning_channel_blown_fuse4;
    if (raw.warning_channel_blown_fuse5 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse5 = (uint8_t)raw.warning_channel_blown_fuse5;
    if (raw.warning_channel_blown_fuse6 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse6 = (uint8_t)raw.warning_channel_blown_fuse6;
    if (raw.warning_channel_blown_fuse7 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse7 = (uint8_t)raw.warning_channel_blown_fuse7;
    if (raw.warning_channel_blown_fuse8 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse8 = (uint8_t)raw.warning_channel_blown_fuse8;
    if (raw.warning_channel_blown_fuse9 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse9 = (uint8_t)raw.warning_channel_blown_fuse9;
    if (raw.warning_channel_blown_fuse10 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse10 = (uint8_t)raw.warning_channel_blown_fuse10;
    if (raw.warning_channel_blown_fuse11 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse11 = (uint8_t)raw.warning_channel_blown_fuse11;
    if (raw.warning_channel_blown_fuse12 > INT64_C(1)) return CAN_RANGE;
    result.warning_channel_blown_fuse12 = (uint8_t)raw.warning_channel_blown_fuse12;
    if (raw.channel_enabled1 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled1 = (uint8_t)raw.channel_enabled1;
    if (raw.channel_enabled2 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled2 = (uint8_t)raw.channel_enabled2;
    if (raw.channel_enabled3 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled3 = (uint8_t)raw.channel_enabled3;
    if (raw.channel_enabled4 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled4 = (uint8_t)raw.channel_enabled4;
    if (raw.channel_enabled5 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled5 = (uint8_t)raw.channel_enabled5;
    if (raw.channel_enabled6 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled6 = (uint8_t)raw.channel_enabled6;
    if (raw.channel_enabled7 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled7 = (uint8_t)raw.channel_enabled7;
    if (raw.channel_enabled8 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled8 = (uint8_t)raw.channel_enabled8;
    if (raw.channel_enabled9 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled9 = (uint8_t)raw.channel_enabled9;
    if (raw.channel_enabled10 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled10 = (uint8_t)raw.channel_enabled10;
    if (raw.channel_enabled11 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled11 = (uint8_t)raw.channel_enabled11;
    if (raw.channel_enabled12 > INT64_C(1)) return CAN_RANGE;
    result.channel_enabled12 = (uint8_t)raw.channel_enabled12;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_pdu_current_data_pack(can_frame_t* dst, const car_can_pdu_current_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pdu_current_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->channel_current1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current5;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current5 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current6;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current6 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current7;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current7 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current8;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current8 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current9;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current9 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current10;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current10 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current11;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current11 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_current12;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 40.95) { value = 40.95; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.channel_current12 = (uint16_t)encoded;
    }
    {
        double value = (double)src->total_current;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 163.83) { value = 163.83; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.total_current = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(161),
        .len = 24, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pdu_current_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pdu_current_data_unpack(car_can_pdu_current_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(161) || src->len != 24 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pdu_current_data_t raw = {0};
    if (car_can_raw_pdu_current_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pdu_current_data_t result = {0};
    if (raw.channel_current1 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current1 = (double)((double)raw.channel_current1 * 0.01 + 0.0);
    if (raw.channel_current2 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current2 = (double)((double)raw.channel_current2 * 0.01 + 0.0);
    if (raw.channel_current3 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current3 = (double)((double)raw.channel_current3 * 0.01 + 0.0);
    if (raw.channel_current4 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current4 = (double)((double)raw.channel_current4 * 0.01 + 0.0);
    if (raw.channel_current5 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current5 = (double)((double)raw.channel_current5 * 0.01 + 0.0);
    if (raw.channel_current6 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current6 = (double)((double)raw.channel_current6 * 0.01 + 0.0);
    if (raw.channel_current7 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current7 = (double)((double)raw.channel_current7 * 0.01 + 0.0);
    if (raw.channel_current8 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current8 = (double)((double)raw.channel_current8 * 0.01 + 0.0);
    if (raw.channel_current9 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current9 = (double)((double)raw.channel_current9 * 0.01 + 0.0);
    if (raw.channel_current10 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current10 = (double)((double)raw.channel_current10 * 0.01 + 0.0);
    if (raw.channel_current11 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current11 = (double)((double)raw.channel_current11 * 0.01 + 0.0);
    if (raw.channel_current12 > INT64_C(4095)) return CAN_RANGE;
    result.channel_current12 = (double)((double)raw.channel_current12 * 0.01 + 0.0);
    if (raw.total_current > INT64_C(16383)) return CAN_RANGE;
    result.total_current = (double)((double)raw.total_current * 0.01 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_pdu_voltage_data_pack(can_frame_t* dst, const car_can_pdu_voltage_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pdu_voltage_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->input_voltage;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.input_voltage = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage5;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage5 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage6;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage6 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage7;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage7 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage8;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage8 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage9;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage9 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage10;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage10 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage11;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage11 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_voltage12;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 51.15) { value = 51.15; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 1023.0) encoded = 1023.0;
        raw.channel_voltage12 = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(162),
        .len = 20, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pdu_voltage_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pdu_voltage_data_unpack(car_can_pdu_voltage_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(162) || src->len != 20 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pdu_voltage_data_t raw = {0};
    if (car_can_raw_pdu_voltage_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pdu_voltage_data_t result = {0};
    if (raw.input_voltage > INT64_C(1023)) return CAN_RANGE;
    result.input_voltage = (double)((double)raw.input_voltage * 0.05 + 0.0);
    if (raw.channel_voltage1 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage1 = (double)((double)raw.channel_voltage1 * 0.05 + 0.0);
    if (raw.channel_voltage2 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage2 = (double)((double)raw.channel_voltage2 * 0.05 + 0.0);
    if (raw.channel_voltage3 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage3 = (double)((double)raw.channel_voltage3 * 0.05 + 0.0);
    if (raw.channel_voltage4 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage4 = (double)((double)raw.channel_voltage4 * 0.05 + 0.0);
    if (raw.channel_voltage5 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage5 = (double)((double)raw.channel_voltage5 * 0.05 + 0.0);
    if (raw.channel_voltage6 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage6 = (double)((double)raw.channel_voltage6 * 0.05 + 0.0);
    if (raw.channel_voltage7 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage7 = (double)((double)raw.channel_voltage7 * 0.05 + 0.0);
    if (raw.channel_voltage8 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage8 = (double)((double)raw.channel_voltage8 * 0.05 + 0.0);
    if (raw.channel_voltage9 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage9 = (double)((double)raw.channel_voltage9 * 0.05 + 0.0);
    if (raw.channel_voltage10 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage10 = (double)((double)raw.channel_voltage10 * 0.05 + 0.0);
    if (raw.channel_voltage11 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage11 = (double)((double)raw.channel_voltage11 * 0.05 + 0.0);
    if (raw.channel_voltage12 > INT64_C(1023)) return CAN_RANGE;
    result.channel_voltage12 = (double)((double)raw.channel_voltage12 * 0.05 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_pdu_temperature_data_pack(can_frame_t* dst, const car_can_pdu_temperature_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pdu_temperature_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        double value = (double)src->channel_temperature1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature5;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature5 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature6;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature6 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature7;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature7 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature8;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature8 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature9;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature9 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature10;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature10 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature11;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature11 = (uint16_t)encoded;
    }
    {
        double value = (double)src->channel_temperature12;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -40.0) { value = -40.0; status = CAN_CLAMPED; }
        if (value > 200.0) { value = 200.0; status = CAN_CLAMPED; }
        double encoded = round((value - -40.0) / 0.05);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4800.0) encoded = 4800.0;
        raw.channel_temperature12 = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(163),
        .len = 24, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pdu_temperature_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pdu_temperature_data_unpack(car_can_pdu_temperature_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(163) || src->len != 24 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pdu_temperature_data_t raw = {0};
    if (car_can_raw_pdu_temperature_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pdu_temperature_data_t result = {0};
    if (raw.channel_temperature1 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature1 = (double)((double)raw.channel_temperature1 * 0.05 + -40.0);
    if (raw.channel_temperature2 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature2 = (double)((double)raw.channel_temperature2 * 0.05 + -40.0);
    if (raw.channel_temperature3 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature3 = (double)((double)raw.channel_temperature3 * 0.05 + -40.0);
    if (raw.channel_temperature4 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature4 = (double)((double)raw.channel_temperature4 * 0.05 + -40.0);
    if (raw.channel_temperature5 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature5 = (double)((double)raw.channel_temperature5 * 0.05 + -40.0);
    if (raw.channel_temperature6 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature6 = (double)((double)raw.channel_temperature6 * 0.05 + -40.0);
    if (raw.channel_temperature7 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature7 = (double)((double)raw.channel_temperature7 * 0.05 + -40.0);
    if (raw.channel_temperature8 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature8 = (double)((double)raw.channel_temperature8 * 0.05 + -40.0);
    if (raw.channel_temperature9 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature9 = (double)((double)raw.channel_temperature9 * 0.05 + -40.0);
    if (raw.channel_temperature10 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature10 = (double)((double)raw.channel_temperature10 * 0.05 + -40.0);
    if (raw.channel_temperature11 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature11 = (double)((double)raw.channel_temperature11 * 0.05 + -40.0);
    if (raw.channel_temperature12 > INT64_C(4800)) return CAN_RANGE;
    result.channel_temperature12 = (double)((double)raw.channel_temperature12 * 0.05 + -40.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_pdu_over_current_snapshot_a_pack(can_frame_t* dst, const car_can_pdu_over_current_snapshot_a_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pdu_over_current_snapshot_a_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->fault_over_current1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current6 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t1 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t2 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t3 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t4 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t5 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t6 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated1 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated2 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated3 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated4 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated5 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated6 = (uint8_t)value;
    }
    {
        uint32_t value = src->capture_time_ms1;
        raw.capture_time_ms1 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms1;
        raw.current_sample_age_ms1 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current1 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms2;
        raw.capture_time_ms2 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms2;
        raw.current_sample_age_ms2 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current2 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms3;
        raw.capture_time_ms3 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms3;
        raw.current_sample_age_ms3 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current3 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms4;
        raw.capture_time_ms4 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms4;
        raw.current_sample_age_ms4 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current4 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms5;
        raw.capture_time_ms5 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms5;
        raw.current_sample_age_ms5 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current5;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current5 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms6;
        raw.capture_time_ms6 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms6;
        raw.current_sample_age_ms6 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current6;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current6 = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(164),
        .len = 64, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pdu_over_current_snapshot_a_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pdu_over_current_snapshot_a_unpack(car_can_pdu_over_current_snapshot_a_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(164) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pdu_over_current_snapshot_a_t raw = {0};
    if (car_can_raw_pdu_over_current_snapshot_a_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pdu_over_current_snapshot_a_t result = {0};
    if (raw.fault_over_current1 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current1 = (uint8_t)raw.fault_over_current1;
    if (raw.fault_over_current2 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current2 = (uint8_t)raw.fault_over_current2;
    if (raw.fault_over_current3 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current3 = (uint8_t)raw.fault_over_current3;
    if (raw.fault_over_current4 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current4 = (uint8_t)raw.fault_over_current4;
    if (raw.fault_over_current5 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current5 = (uint8_t)raw.fault_over_current5;
    if (raw.fault_over_current6 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current6 = (uint8_t)raw.fault_over_current6;
    if (raw.fault_i2_t1 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t1 = (uint8_t)raw.fault_i2_t1;
    if (raw.fault_i2_t2 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t2 = (uint8_t)raw.fault_i2_t2;
    if (raw.fault_i2_t3 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t3 = (uint8_t)raw.fault_i2_t3;
    if (raw.fault_i2_t4 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t4 = (uint8_t)raw.fault_i2_t4;
    if (raw.fault_i2_t5 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t5 = (uint8_t)raw.fault_i2_t5;
    if (raw.fault_i2_t6 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t6 = (uint8_t)raw.fault_i2_t6;
    if (raw.trip_current_saturated1 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated1 = (uint8_t)raw.trip_current_saturated1;
    if (raw.trip_current_saturated2 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated2 = (uint8_t)raw.trip_current_saturated2;
    if (raw.trip_current_saturated3 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated3 = (uint8_t)raw.trip_current_saturated3;
    if (raw.trip_current_saturated4 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated4 = (uint8_t)raw.trip_current_saturated4;
    if (raw.trip_current_saturated5 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated5 = (uint8_t)raw.trip_current_saturated5;
    if (raw.trip_current_saturated6 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated6 = (uint8_t)raw.trip_current_saturated6;
    result.capture_time_ms1 = (uint32_t)raw.capture_time_ms1;
    result.current_sample_age_ms1 = (uint16_t)raw.current_sample_age_ms1;
    result.trip_current1 = (double)((double)raw.trip_current1 * 0.01 + 0.0);
    result.capture_time_ms2 = (uint32_t)raw.capture_time_ms2;
    result.current_sample_age_ms2 = (uint16_t)raw.current_sample_age_ms2;
    result.trip_current2 = (double)((double)raw.trip_current2 * 0.01 + 0.0);
    result.capture_time_ms3 = (uint32_t)raw.capture_time_ms3;
    result.current_sample_age_ms3 = (uint16_t)raw.current_sample_age_ms3;
    result.trip_current3 = (double)((double)raw.trip_current3 * 0.01 + 0.0);
    result.capture_time_ms4 = (uint32_t)raw.capture_time_ms4;
    result.current_sample_age_ms4 = (uint16_t)raw.current_sample_age_ms4;
    result.trip_current4 = (double)((double)raw.trip_current4 * 0.01 + 0.0);
    result.capture_time_ms5 = (uint32_t)raw.capture_time_ms5;
    result.current_sample_age_ms5 = (uint16_t)raw.current_sample_age_ms5;
    result.trip_current5 = (double)((double)raw.trip_current5 * 0.01 + 0.0);
    result.capture_time_ms6 = (uint32_t)raw.capture_time_ms6;
    result.current_sample_age_ms6 = (uint16_t)raw.current_sample_age_ms6;
    result.trip_current6 = (double)((double)raw.trip_current6 * 0.01 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t car_can_pdu_over_current_snapshot_b_pack(can_frame_t* dst, const car_can_pdu_over_current_snapshot_b_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct car_can_raw_pdu_over_current_snapshot_b_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->fault_over_current7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current7 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current8 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current9 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current10 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current11 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_over_current12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_over_current12 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t7 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t8 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t9 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t10 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t11 = (uint8_t)value;
    }
    {
        uint8_t value = src->fault_i2_t12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.fault_i2_t12 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated7 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated8;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated8 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated9;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated9 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated10;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated10 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated11;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated11 = (uint8_t)value;
    }
    {
        uint8_t value = src->trip_current_saturated12;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.trip_current_saturated12 = (uint8_t)value;
    }
    {
        uint32_t value = src->capture_time_ms7;
        raw.capture_time_ms7 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms7;
        raw.current_sample_age_ms7 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current7;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current7 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms8;
        raw.capture_time_ms8 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms8;
        raw.current_sample_age_ms8 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current8;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current8 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms9;
        raw.capture_time_ms9 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms9;
        raw.current_sample_age_ms9 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current9;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current9 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms10;
        raw.capture_time_ms10 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms10;
        raw.current_sample_age_ms10 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current10;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current10 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms11;
        raw.capture_time_ms11 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms11;
        raw.current_sample_age_ms11 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current11;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current11 = (uint16_t)encoded;
    }
    {
        uint32_t value = src->capture_time_ms12;
        raw.capture_time_ms12 = (uint32_t)value;
    }
    {
        uint16_t value = src->current_sample_age_ms12;
        raw.current_sample_age_ms12 = (uint16_t)value;
    }
    {
        double value = (double)src->trip_current12;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.trip_current12 = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(165),
        .len = 64, .is_extended = false,
        .is_fd = true };
    if (car_can_raw_pdu_over_current_snapshot_b_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t car_can_pdu_over_current_snapshot_b_unpack(car_can_pdu_over_current_snapshot_b_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(165) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct car_can_raw_pdu_over_current_snapshot_b_t raw = {0};
    if (car_can_raw_pdu_over_current_snapshot_b_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    car_can_pdu_over_current_snapshot_b_t result = {0};
    if (raw.fault_over_current7 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current7 = (uint8_t)raw.fault_over_current7;
    if (raw.fault_over_current8 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current8 = (uint8_t)raw.fault_over_current8;
    if (raw.fault_over_current9 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current9 = (uint8_t)raw.fault_over_current9;
    if (raw.fault_over_current10 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current10 = (uint8_t)raw.fault_over_current10;
    if (raw.fault_over_current11 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current11 = (uint8_t)raw.fault_over_current11;
    if (raw.fault_over_current12 > INT64_C(1)) return CAN_RANGE;
    result.fault_over_current12 = (uint8_t)raw.fault_over_current12;
    if (raw.fault_i2_t7 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t7 = (uint8_t)raw.fault_i2_t7;
    if (raw.fault_i2_t8 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t8 = (uint8_t)raw.fault_i2_t8;
    if (raw.fault_i2_t9 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t9 = (uint8_t)raw.fault_i2_t9;
    if (raw.fault_i2_t10 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t10 = (uint8_t)raw.fault_i2_t10;
    if (raw.fault_i2_t11 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t11 = (uint8_t)raw.fault_i2_t11;
    if (raw.fault_i2_t12 > INT64_C(1)) return CAN_RANGE;
    result.fault_i2_t12 = (uint8_t)raw.fault_i2_t12;
    if (raw.trip_current_saturated7 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated7 = (uint8_t)raw.trip_current_saturated7;
    if (raw.trip_current_saturated8 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated8 = (uint8_t)raw.trip_current_saturated8;
    if (raw.trip_current_saturated9 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated9 = (uint8_t)raw.trip_current_saturated9;
    if (raw.trip_current_saturated10 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated10 = (uint8_t)raw.trip_current_saturated10;
    if (raw.trip_current_saturated11 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated11 = (uint8_t)raw.trip_current_saturated11;
    if (raw.trip_current_saturated12 > INT64_C(1)) return CAN_RANGE;
    result.trip_current_saturated12 = (uint8_t)raw.trip_current_saturated12;
    result.capture_time_ms7 = (uint32_t)raw.capture_time_ms7;
    result.current_sample_age_ms7 = (uint16_t)raw.current_sample_age_ms7;
    result.trip_current7 = (double)((double)raw.trip_current7 * 0.01 + 0.0);
    result.capture_time_ms8 = (uint32_t)raw.capture_time_ms8;
    result.current_sample_age_ms8 = (uint16_t)raw.current_sample_age_ms8;
    result.trip_current8 = (double)((double)raw.trip_current8 * 0.01 + 0.0);
    result.capture_time_ms9 = (uint32_t)raw.capture_time_ms9;
    result.current_sample_age_ms9 = (uint16_t)raw.current_sample_age_ms9;
    result.trip_current9 = (double)((double)raw.trip_current9 * 0.01 + 0.0);
    result.capture_time_ms10 = (uint32_t)raw.capture_time_ms10;
    result.current_sample_age_ms10 = (uint16_t)raw.current_sample_age_ms10;
    result.trip_current10 = (double)((double)raw.trip_current10 * 0.01 + 0.0);
    result.capture_time_ms11 = (uint32_t)raw.capture_time_ms11;
    result.current_sample_age_ms11 = (uint16_t)raw.current_sample_age_ms11;
    result.trip_current11 = (double)((double)raw.trip_current11 * 0.01 + 0.0);
    result.capture_time_ms12 = (uint32_t)raw.capture_time_ms12;
    result.current_sample_age_ms12 = (uint16_t)raw.current_sample_age_ms12;
    result.trip_current12 = (double)((double)raw.trip_current12 * 0.01 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}
