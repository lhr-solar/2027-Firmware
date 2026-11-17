#include "motor_can.h"
#include "detail/motor_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const motor_can_metadata_t motor_can = {
    .vcu = {
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

can_status_t motor_can_mc_drive_command_pack(can_frame_t* dst, const motor_can_mc_drive_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_drive_command_t raw = {0};
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
    if (motor_can_raw_mc_drive_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_drive_command_unpack(motor_can_mc_drive_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1025) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_drive_command_t raw = {0};
    if (motor_can_raw_mc_drive_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_drive_command_t result = {0};
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

can_status_t motor_can_mc_power_command_pack(can_frame_t* dst, const motor_can_mc_power_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_power_command_t raw = {0};
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
    if (motor_can_raw_mc_power_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_power_command_unpack(motor_can_mc_power_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1026) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_power_command_t raw = {0};
    if (motor_can_raw_mc_power_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_power_command_t result = {0};
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

can_status_t motor_can_mc_reset_command_pack(can_frame_t* dst, const motor_can_mc_reset_command_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_reset_command_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1027),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (motor_can_raw_mc_reset_command_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_reset_command_unpack(motor_can_mc_reset_command_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1027) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_reset_command_t raw = {0};
    if (motor_can_raw_mc_reset_command_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_reset_command_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t motor_can_mc_info_pack(can_frame_t* dst, const motor_can_mc_info_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_info_t raw = {0};
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
    if (motor_can_raw_mc_info_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_info_unpack(motor_can_mc_info_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1056) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_info_t raw = {0};
    if (motor_can_raw_mc_info_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_info_t result = {0};
    result.mc_tritium_id = (uint32_t)raw.mc_tritium_id;
    result.mc_serial_number = (uint32_t)raw.mc_serial_number;
    *dst = result;
    return CAN_OK;
}

can_status_t motor_can_mc_status_pack(can_frame_t* dst, const motor_can_mc_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_status_t raw = {0};
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
    if (motor_can_raw_mc_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_status_unpack(motor_can_mc_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1057) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_status_t raw = {0};
    if (motor_can_raw_mc_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_status_t result = {0};
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

can_status_t motor_can_mc_bus_measurement_pack(can_frame_t* dst, const motor_can_mc_bus_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_bus_measurement_t raw = {0};
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
    if (motor_can_raw_mc_bus_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_bus_measurement_unpack(motor_can_mc_bus_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1058) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_bus_measurement_t raw = {0};
    if (motor_can_raw_mc_bus_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_bus_measurement_t result = {0};
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

can_status_t motor_can_mc_velocity_measurement_pack(can_frame_t* dst, const motor_can_mc_velocity_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_velocity_measurement_t raw = {0};
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
    if (motor_can_raw_mc_velocity_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_velocity_measurement_unpack(motor_can_mc_velocity_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1059) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_velocity_measurement_t raw = {0};
    if (motor_can_raw_mc_velocity_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_velocity_measurement_t result = {0};
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

can_status_t motor_can_mc_phase_current_measurement_pack(can_frame_t* dst, const motor_can_mc_phase_current_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_phase_current_measurement_t raw = {0};
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
    if (motor_can_raw_mc_phase_current_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_phase_current_measurement_unpack(motor_can_mc_phase_current_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1060) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_phase_current_measurement_t raw = {0};
    if (motor_can_raw_mc_phase_current_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_phase_current_measurement_t result = {0};
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

can_status_t motor_can_mc_motor_voltage_vector_measurement_pack(can_frame_t* dst, const motor_can_mc_motor_voltage_vector_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_motor_voltage_vector_measurement_t raw = {0};
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
    if (motor_can_raw_mc_motor_voltage_vector_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_motor_voltage_vector_measurement_unpack(motor_can_mc_motor_voltage_vector_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1061) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_motor_voltage_vector_measurement_t raw = {0};
    if (motor_can_raw_mc_motor_voltage_vector_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_motor_voltage_vector_measurement_t result = {0};
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

can_status_t motor_can_mc_motor_current_vector_measurement_pack(can_frame_t* dst, const motor_can_mc_motor_current_vector_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_motor_current_vector_measurement_t raw = {0};
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
    if (motor_can_raw_mc_motor_current_vector_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_motor_current_vector_measurement_unpack(motor_can_mc_motor_current_vector_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1062) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_motor_current_vector_measurement_t raw = {0};
    if (motor_can_raw_mc_motor_current_vector_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_motor_current_vector_measurement_t result = {0};
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

can_status_t motor_can_mc_back_emf_measurement_prediction_pack(can_frame_t* dst, const motor_can_mc_back_emf_measurement_prediction_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_back_emf_measurement_prediction_t raw = {0};
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
    if (motor_can_raw_mc_back_emf_measurement_prediction_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_back_emf_measurement_prediction_unpack(motor_can_mc_back_emf_measurement_prediction_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1063) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_back_emf_measurement_prediction_t raw = {0};
    if (motor_can_raw_mc_back_emf_measurement_prediction_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_back_emf_measurement_prediction_t result = {0};
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

can_status_t motor_can_mc_15_v_rail_measurement_pack(can_frame_t* dst, const motor_can_mc_15_v_rail_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_15_v_rail_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_supply15_v;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_supply15_v, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1064),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (motor_can_raw_mc_15_v_rail_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_15_v_rail_measurement_unpack(motor_can_mc_15_v_rail_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1064) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_15_v_rail_measurement_t raw = {0};
    if (motor_can_raw_mc_15_v_rail_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_15_v_rail_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_supply15_v, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_supply15_v = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t motor_can_mc_3_v3_19_v_rail_measurement_pack(can_frame_t* dst, const motor_can_mc_3_v3_19_v_rail_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_3_v3_19_v_rail_measurement_t raw = {0};
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
    if (motor_can_raw_mc_3_v3_19_v_rail_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_3_v3_19_v_rail_measurement_unpack(motor_can_mc_3_v3_19_v_rail_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1065) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_3_v3_19_v_rail_measurement_t raw = {0};
    if (motor_can_raw_mc_3_v3_19_v_rail_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_3_v3_19_v_rail_measurement_t result = {0};
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

can_status_t motor_can_ws22_reserved0_a_pack(can_frame_t* dst, const motor_can_ws22_reserved0_a_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_ws22_reserved0_a_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1066),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (motor_can_raw_ws22_reserved0_a_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_ws22_reserved0_a_unpack(motor_can_ws22_reserved0_a_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1066) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_ws22_reserved0_a_t raw = {0};
    if (motor_can_raw_ws22_reserved0_a_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_ws22_reserved0_a_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t motor_can_mc_motor_temp_measurement_pack(can_frame_t* dst, const motor_can_mc_motor_temp_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_motor_temp_measurement_t raw = {0};
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
    if (motor_can_raw_mc_motor_temp_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_motor_temp_measurement_unpack(motor_can_mc_motor_temp_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1067) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_motor_temp_measurement_t raw = {0};
    if (motor_can_raw_mc_motor_temp_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_motor_temp_measurement_t result = {0};
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

can_status_t motor_can_mc_dsp_board_temp_measurement_pack(can_frame_t* dst, const motor_can_mc_dsp_board_temp_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_dsp_board_temp_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_dsp_board_temp;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_dsp_board_temp, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1068),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (motor_can_raw_mc_dsp_board_temp_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_dsp_board_temp_measurement_unpack(motor_can_mc_dsp_board_temp_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1068) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_dsp_board_temp_measurement_t raw = {0};
    if (motor_can_raw_mc_dsp_board_temp_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_dsp_board_temp_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_dsp_board_temp, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_dsp_board_temp = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t motor_can_ws22_reserved0_d_pack(can_frame_t* dst, const motor_can_ws22_reserved0_d_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_ws22_reserved0_d_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1069),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (motor_can_raw_ws22_reserved0_d_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_ws22_reserved0_d_unpack(motor_can_ws22_reserved0_d_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1069) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_ws22_reserved0_d_t raw = {0};
    if (motor_can_raw_ws22_reserved0_d_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_ws22_reserved0_d_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t motor_can_mc_odometer_bus_ah_measurement_pack(can_frame_t* dst, const motor_can_mc_odometer_bus_ah_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_odometer_bus_ah_measurement_t raw = {0};
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
    if (motor_can_raw_mc_odometer_bus_ah_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_odometer_bus_ah_measurement_unpack(motor_can_mc_odometer_bus_ah_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1070) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_odometer_bus_ah_measurement_t raw = {0};
    if (motor_can_raw_mc_odometer_bus_ah_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_odometer_bus_ah_measurement_t result = {0};
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

can_status_t motor_can_mc_slip_speed_measurement_pack(can_frame_t* dst, const motor_can_mc_slip_speed_measurement_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct motor_can_raw_mc_slip_speed_measurement_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->mc_slip_speed;
        if (!isfinite(value)) return CAN_INVALID;
        memcpy(&raw.mc_slip_speed, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(1079),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (motor_can_raw_mc_slip_speed_measurement_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t motor_can_mc_slip_speed_measurement_unpack(motor_can_mc_slip_speed_measurement_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1079) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct motor_can_raw_mc_slip_speed_measurement_t raw = {0};
    if (motor_can_raw_mc_slip_speed_measurement_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    motor_can_mc_slip_speed_measurement_t result = {0};
    {
        float value;
        memcpy(&value, &raw.mc_slip_speed, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        result.mc_slip_speed = value;
    }
    *dst = result;
    return CAN_OK;
}
