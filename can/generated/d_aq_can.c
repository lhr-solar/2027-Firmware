#include "d_aq_can.h"
#include "detail/d_aq_can_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const d_aq_can_metadata_t d_aq_can = {
    .blackbody1 = {
        .blackbody1_data = { .id = UINT32_C(1664), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = true },
    },
    .blackbody2 = {
        .blackbody2_data = { .id = UINT32_C(1665), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = true },
    },
    .blackbody3 = {
        .blackbody3_data = { .id = UINT32_C(1666), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = true },
    },
    .chassis_imu = {
        .chassis_acceleration_data = { .id = UINT32_C(1584), .payload_bytes = 32, .wire_bytes = 32, .dlc = 13, .is_extended = false, .is_fd = true },
        .chassis_gyro_data = { .id = UINT32_C(1585), .payload_bytes = 48, .wire_bytes = 48, .dlc = 14, .is_extended = false, .is_fd = true },
        .chassis_magnetometer_data = { .id = UINT32_C(1586), .payload_bytes = 32, .wire_bytes = 32, .dlc = 13, .is_extended = false, .is_fd = true },
    },
    .mika = {
        .timestamp_sync = { .id = UINT32_C(1568), .payload_bytes = 2, .wire_bytes = 2, .dlc = 2, .is_extended = false, .is_fd = true },
        .telemetry_status = { .id = UINT32_C(1569), .payload_bytes = 7, .wire_bytes = 7, .dlc = 7, .is_extended = false, .is_fd = true },
        .gps_data = { .id = UINT32_C(1570), .payload_bytes = 20, .wire_bytes = 20, .dlc = 11, .is_extended = false, .is_fd = true },
        .on_board_watchdog = { .id = UINT32_C(1571), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = true },
    },
    .strain_gauge_front = {
        .strain_gauge_front_data = { .id = UINT32_C(1680), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = true },
    },
    .strain_gauge_rear = {
        .strain_gauge_rear_data = { .id = UINT32_C(1681), .payload_bytes = 0, .wire_bytes = 0, .dlc = 0, .is_extended = false, .is_fd = true },
    },
    .suspension_fl = {
        .suspension_fl_data = { .id = UINT32_C(1649), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
    },
    .suspension_fr = {
        .suspension_fr_data = { .id = UINT32_C(1648), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
    },
    .suspension_rear = {
        .suspension_rear_data = { .id = UINT32_C(1650), .payload_bytes = 12, .wire_bytes = 12, .dlc = 9, .is_extended = false, .is_fd = true },
    },
    .upright_fl = {
        .upright_fl_acceleration_data = { .id = UINT32_C(1616), .payload_bytes = 48, .wire_bytes = 48, .dlc = 14, .is_extended = false, .is_fd = true },
        .upright_fl_gyro_data = { .id = UINT32_C(1617), .payload_bytes = 48, .wire_bytes = 48, .dlc = 14, .is_extended = false, .is_fd = true },
        .upright_fl_wheel_speed_data = { .id = UINT32_C(1618), .payload_bytes = 16, .wire_bytes = 16, .dlc = 10, .is_extended = false, .is_fd = true },
    },
    .upright_fr = {
        .upright_fr_acceleration_data = { .id = UINT32_C(1600), .payload_bytes = 48, .wire_bytes = 48, .dlc = 14, .is_extended = false, .is_fd = true },
        .upright_fr_gyro_data = { .id = UINT32_C(1601), .payload_bytes = 48, .wire_bytes = 48, .dlc = 14, .is_extended = false, .is_fd = true },
        .upright_fr_wheel_speed_data = { .id = UINT32_C(1602), .payload_bytes = 16, .wire_bytes = 16, .dlc = 10, .is_extended = false, .is_fd = true },
    },
    .upright_rear = {
        .upright_rear_acceleration_data = { .id = UINT32_C(1632), .payload_bytes = 48, .wire_bytes = 48, .dlc = 14, .is_extended = false, .is_fd = true },
        .upright_rear_gyro_data = { .id = UINT32_C(1633), .payload_bytes = 48, .wire_bytes = 48, .dlc = 14, .is_extended = false, .is_fd = true },
        .upright_rear_wheel_speed_data = { .id = UINT32_C(1634), .payload_bytes = 16, .wire_bytes = 16, .dlc = 10, .is_extended = false, .is_fd = true },
    },
};

can_status_t d_aq_can_timestamp_sync_pack(can_frame_t* dst, const d_aq_can_timestamp_sync_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_timestamp_sync_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1568),
        .len = 2, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_timestamp_sync_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_timestamp_sync_unpack(d_aq_can_timestamp_sync_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1568) || src->len != 2 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_timestamp_sync_t raw = {0};
    if (d_aq_can_raw_timestamp_sync_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_timestamp_sync_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_telemetry_status_pack(can_frame_t* dst, const d_aq_can_telemetry_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_telemetry_status_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->cellular_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.cellular_connected = (uint8_t)value;
    }
    {
        uint8_t value = src->cellular_technology;
        if (value > INT64_C(7)) {
            value = (uint8_t)INT64_C(7); status = CAN_CLAMPED;
        }
        raw.cellular_technology = (uint8_t)value;
    }
    {
        uint8_t value = src->roaming;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.roaming = (uint8_t)value;
    }
    {
        uint8_t value = src->internet_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.internet_connected = (uint8_t)value;
    }
    {
        int8_t value = src->rssi;
        raw.rssi = (int8_t)value;
    }
    {
        int8_t value = src->rsrp;
        raw.rsrp = (int8_t)value;
    }
    {
        double value = (double)src->rsrq;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -64.0) { value = -64.0; status = CAN_CLAMPED; }
        if (value > 63.5) { value = 63.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < -128.0) encoded = -128.0;
        if (encoded > 127.0) encoded = 127.0;
        raw.rsrq = (int8_t)encoded;
    }
    {
        double value = (double)src->sinr;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -64.0) { value = -64.0; status = CAN_CLAMPED; }
        if (value > 63.5) { value = 63.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < -128.0) encoded = -128.0;
        if (encoded > 127.0) encoded = 127.0;
        raw.sinr = (int8_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1569),
        .len = 7, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_telemetry_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_telemetry_status_unpack(d_aq_can_telemetry_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1569) || src->len != 7 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_telemetry_status_t raw = {0};
    if (d_aq_can_raw_telemetry_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_telemetry_status_t result = {0};
    if (raw.cellular_connected > INT64_C(1)) return CAN_RANGE;
    result.cellular_connected = (uint8_t)raw.cellular_connected;
    if (raw.cellular_technology > INT64_C(7)) return CAN_RANGE;
    result.cellular_technology = (uint8_t)raw.cellular_technology;
    if (raw.roaming > INT64_C(1)) return CAN_RANGE;
    result.roaming = (uint8_t)raw.roaming;
    if (raw.internet_connected > INT64_C(1)) return CAN_RANGE;
    result.internet_connected = (uint8_t)raw.internet_connected;
    result.rssi = (int8_t)raw.rssi;
    result.rsrp = (int8_t)raw.rsrp;
    result.rsrq = (double)((double)raw.rsrq * 0.5 + 0.0);
    result.sinr = (double)((double)raw.sinr * 0.5 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_gps_data_pack(can_frame_t* dst, const d_aq_can_gps_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_gps_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->gps_antenna_state;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.gps_antenna_state = (uint8_t)value;
    }
    {
        uint8_t value = src->gps_fix;
        if (value > INT64_C(3)) {
            value = (uint8_t)INT64_C(3); status = CAN_CLAMPED;
        }
        raw.gps_fix = (uint8_t)value;
    }
    {
        double value = (double)src->gps_longitude;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -180.0) { value = -180.0; status = CAN_CLAMPED; }
        if (value > 180.0) { value = 180.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 1e-06);
        if (encoded < -180000000.0) encoded = -180000000.0;
        if (encoded > 180000000.0) encoded = 180000000.0;
        raw.gps_longitude = (int32_t)encoded;
    }
    {
        double value = (double)src->gps_latitude;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -90.0) { value = -90.0; status = CAN_CLAMPED; }
        if (value > 90.0) { value = 90.0; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 1e-06);
        if (encoded < -90000000.0) encoded = -90000000.0;
        if (encoded > 90000000.0) encoded = 90000000.0;
        raw.gps_latitude = (int32_t)encoded;
    }
    {
        double value = (double)src->gps_speed;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 655.35) { value = 655.35; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 65535.0) encoded = 65535.0;
        raw.gps_speed = (uint16_t)encoded;
    }
    {
        double value = (double)src->gps_heading;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 359.99) { value = 359.99; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.01);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 35999.0) encoded = 35999.0;
        raw.gps_heading = (uint16_t)encoded;
    }
    {
        double value = (double)src->gps_altitude;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -3276.8) { value = -3276.8; status = CAN_CLAMPED; }
        if (value > 3276.7) { value = 3276.7; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.1);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gps_altitude = (int16_t)encoded;
    }
    {
        uint8_t value = src->satellites_in_use;
        raw.satellites_in_use = (uint8_t)value;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1570),
        .len = 20, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_gps_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_gps_data_unpack(d_aq_can_gps_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1570) || src->len != 20 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_gps_data_t raw = {0};
    if (d_aq_can_raw_gps_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_gps_data_t result = {0};
    if (raw.gps_antenna_state > INT64_C(3)) return CAN_RANGE;
    result.gps_antenna_state = (uint8_t)raw.gps_antenna_state;
    if (raw.gps_fix > INT64_C(3)) return CAN_RANGE;
    result.gps_fix = (uint8_t)raw.gps_fix;
    if (raw.gps_longitude < (-INT64_C(180000000))) return CAN_RANGE;
    if (raw.gps_longitude > INT64_C(180000000)) return CAN_RANGE;
    result.gps_longitude = (double)((double)raw.gps_longitude * 1e-06 + 0.0);
    if (raw.gps_latitude < (-INT64_C(90000000))) return CAN_RANGE;
    if (raw.gps_latitude > INT64_C(90000000)) return CAN_RANGE;
    result.gps_latitude = (double)((double)raw.gps_latitude * 1e-06 + 0.0);
    result.gps_speed = (double)((double)raw.gps_speed * 0.01 + 0.0);
    if (raw.gps_heading > INT64_C(35999)) return CAN_RANGE;
    result.gps_heading = (double)((double)raw.gps_heading * 0.01 + 0.0);
    result.gps_altitude = (double)((double)raw.gps_altitude * 0.1 + 0.0);
    result.satellites_in_use = (uint8_t)raw.satellites_in_use;
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_on_board_watchdog_pack(can_frame_t* dst, const d_aq_can_on_board_watchdog_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_on_board_watchdog_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->bps_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.bps_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->vcu_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.vcu_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->dei_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.dei_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->pedals_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.pedals_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->pdu_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.pdu_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->lvc_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.lvc_watchdog = (uint8_t)value;
    }
    {
        uint8_t value = src->mppt_interface_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.mppt_interface_watchdog = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1571),
        .len = 1, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_on_board_watchdog_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_on_board_watchdog_unpack(d_aq_can_on_board_watchdog_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1571) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_on_board_watchdog_t raw = {0};
    if (d_aq_can_raw_on_board_watchdog_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_on_board_watchdog_t result = {0};
    if (raw.bps_watchdog > INT64_C(1)) return CAN_RANGE;
    result.bps_watchdog = (uint8_t)raw.bps_watchdog;
    if (raw.vcu_watchdog > INT64_C(1)) return CAN_RANGE;
    result.vcu_watchdog = (uint8_t)raw.vcu_watchdog;
    if (raw.dei_watchdog > INT64_C(1)) return CAN_RANGE;
    result.dei_watchdog = (uint8_t)raw.dei_watchdog;
    if (raw.pedals_watchdog > INT64_C(1)) return CAN_RANGE;
    result.pedals_watchdog = (uint8_t)raw.pedals_watchdog;
    if (raw.pdu_watchdog > INT64_C(1)) return CAN_RANGE;
    result.pdu_watchdog = (uint8_t)raw.pdu_watchdog;
    if (raw.lvc_watchdog > INT64_C(1)) return CAN_RANGE;
    result.lvc_watchdog = (uint8_t)raw.lvc_watchdog;
    if (raw.mppt_interface_watchdog > INT64_C(1)) return CAN_RANGE;
    result.mppt_interface_watchdog = (uint8_t)raw.mppt_interface_watchdog;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_chassis_acceleration_data_pack(can_frame_t* dst, const d_aq_can_chassis_acceleration_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_chassis_acceleration_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->acceleration_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -4.096) { value = -4.096; status = CAN_CLAMPED; }
        if (value > 4.095) { value = 4.095; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -4096.0) encoded = -4096.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.acceleration_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1584),
        .len = 32, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_chassis_acceleration_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_chassis_acceleration_data_unpack(d_aq_can_chassis_acceleration_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1584) || src->len != 32 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_chassis_acceleration_data_t raw = {0};
    if (d_aq_can_raw_chassis_acceleration_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_chassis_acceleration_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.acceleration_x1 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_x1 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_x1 = (double)((double)raw.acceleration_x1 * 0.001 + 0.0);
    if (raw.acceleration_x2 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_x2 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_x2 = (double)((double)raw.acceleration_x2 * 0.001 + 0.0);
    if (raw.acceleration_x3 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_x3 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_x3 = (double)((double)raw.acceleration_x3 * 0.001 + 0.0);
    if (raw.acceleration_x4 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_x4 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_x4 = (double)((double)raw.acceleration_x4 * 0.001 + 0.0);
    if (raw.acceleration_x_average < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_x_average > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_x_average = (double)((double)raw.acceleration_x_average * 0.001 + 0.0);
    if (raw.acceleration_y1 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_y1 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_y1 = (double)((double)raw.acceleration_y1 * 0.001 + 0.0);
    if (raw.acceleration_y2 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_y2 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_y2 = (double)((double)raw.acceleration_y2 * 0.001 + 0.0);
    if (raw.acceleration_y3 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_y3 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_y3 = (double)((double)raw.acceleration_y3 * 0.001 + 0.0);
    if (raw.acceleration_y4 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_y4 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_y4 = (double)((double)raw.acceleration_y4 * 0.001 + 0.0);
    if (raw.acceleration_y_average < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_y_average > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_y_average = (double)((double)raw.acceleration_y_average * 0.001 + 0.0);
    if (raw.acceleration_z1 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_z1 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_z1 = (double)((double)raw.acceleration_z1 * 0.001 + 0.0);
    if (raw.acceleration_z2 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_z2 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_z2 = (double)((double)raw.acceleration_z2 * 0.001 + 0.0);
    if (raw.acceleration_z3 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_z3 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_z3 = (double)((double)raw.acceleration_z3 * 0.001 + 0.0);
    if (raw.acceleration_z4 < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_z4 > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_z4 = (double)((double)raw.acceleration_z4 * 0.001 + 0.0);
    if (raw.acceleration_z_average < (-INT64_C(4096))) return CAN_RANGE;
    if (raw.acceleration_z_average > INT64_C(4095)) return CAN_RANGE;
    result.acceleration_z_average = (double)((double)raw.acceleration_z_average * 0.001 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_chassis_gyro_data_pack(can_frame_t* dst, const d_aq_can_chassis_gyro_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_chassis_gyro_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->gyro_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1585),
        .len = 48, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_chassis_gyro_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_chassis_gyro_data_unpack(d_aq_can_chassis_gyro_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1585) || src->len != 48 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_chassis_gyro_data_t raw = {0};
    if (d_aq_can_raw_chassis_gyro_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_chassis_gyro_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    result.gyro_x1 = (double)((double)raw.gyro_x1 * 0.02 + 0.0);
    result.gyro_x2 = (double)((double)raw.gyro_x2 * 0.02 + 0.0);
    result.gyro_x3 = (double)((double)raw.gyro_x3 * 0.02 + 0.0);
    result.gyro_x4 = (double)((double)raw.gyro_x4 * 0.02 + 0.0);
    result.gyro_x_average = (double)((double)raw.gyro_x_average * 0.02 + 0.0);
    result.gyro_y1 = (double)((double)raw.gyro_y1 * 0.02 + 0.0);
    result.gyro_y2 = (double)((double)raw.gyro_y2 * 0.02 + 0.0);
    result.gyro_y3 = (double)((double)raw.gyro_y3 * 0.02 + 0.0);
    result.gyro_y4 = (double)((double)raw.gyro_y4 * 0.02 + 0.0);
    result.gyro_y_average = (double)((double)raw.gyro_y_average * 0.02 + 0.0);
    result.gyro_z1 = (double)((double)raw.gyro_z1 * 0.02 + 0.0);
    result.gyro_z2 = (double)((double)raw.gyro_z2 * 0.02 + 0.0);
    result.gyro_z3 = (double)((double)raw.gyro_z3 * 0.02 + 0.0);
    result.gyro_z4 = (double)((double)raw.gyro_z4 * 0.02 + 0.0);
    result.gyro_z_average = (double)((double)raw.gyro_z_average * 0.02 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_chassis_magnetometer_data_pack(can_frame_t* dst, const d_aq_can_chassis_magnetometer_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_chassis_magnetometer_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->magnetic_field_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->magnetic_field_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -307.2) { value = -307.2; status = CAN_CLAMPED; }
        if (value > 307.05) { value = 307.05; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.15);
        if (encoded < -2048.0) encoded = -2048.0;
        if (encoded > 2047.0) encoded = 2047.0;
        raw.magnetic_field_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1586),
        .len = 32, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_chassis_magnetometer_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_chassis_magnetometer_data_unpack(d_aq_can_chassis_magnetometer_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1586) || src->len != 32 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_chassis_magnetometer_data_t raw = {0};
    if (d_aq_can_raw_chassis_magnetometer_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_chassis_magnetometer_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.magnetic_field_x1 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_x1 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_x1 = (double)((double)raw.magnetic_field_x1 * 0.15 + 0.0);
    if (raw.magnetic_field_x2 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_x2 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_x2 = (double)((double)raw.magnetic_field_x2 * 0.15 + 0.0);
    if (raw.magnetic_field_x3 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_x3 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_x3 = (double)((double)raw.magnetic_field_x3 * 0.15 + 0.0);
    if (raw.magnetic_field_x4 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_x4 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_x4 = (double)((double)raw.magnetic_field_x4 * 0.15 + 0.0);
    if (raw.magnetic_field_x_average < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_x_average > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_x_average = (double)((double)raw.magnetic_field_x_average * 0.15 + 0.0);
    if (raw.magnetic_field_y1 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_y1 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_y1 = (double)((double)raw.magnetic_field_y1 * 0.15 + 0.0);
    if (raw.magnetic_field_y2 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_y2 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_y2 = (double)((double)raw.magnetic_field_y2 * 0.15 + 0.0);
    if (raw.magnetic_field_y3 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_y3 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_y3 = (double)((double)raw.magnetic_field_y3 * 0.15 + 0.0);
    if (raw.magnetic_field_y4 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_y4 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_y4 = (double)((double)raw.magnetic_field_y4 * 0.15 + 0.0);
    if (raw.magnetic_field_y_average < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_y_average > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_y_average = (double)((double)raw.magnetic_field_y_average * 0.15 + 0.0);
    if (raw.magnetic_field_z1 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_z1 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_z1 = (double)((double)raw.magnetic_field_z1 * 0.15 + 0.0);
    if (raw.magnetic_field_z2 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_z2 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_z2 = (double)((double)raw.magnetic_field_z2 * 0.15 + 0.0);
    if (raw.magnetic_field_z3 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_z3 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_z3 = (double)((double)raw.magnetic_field_z3 * 0.15 + 0.0);
    if (raw.magnetic_field_z4 < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_z4 > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_z4 = (double)((double)raw.magnetic_field_z4 * 0.15 + 0.0);
    if (raw.magnetic_field_z_average < (-INT64_C(2048))) return CAN_RANGE;
    if (raw.magnetic_field_z_average > INT64_C(2047)) return CAN_RANGE;
    result.magnetic_field_z_average = (double)((double)raw.magnetic_field_z_average * 0.15 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_fr_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_acceleration_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_fr_acceleration_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->acceleration_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1600),
        .len = 48, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_fr_acceleration_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_fr_acceleration_data_unpack(d_aq_can_upright_fr_acceleration_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1600) || src->len != 48 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_fr_acceleration_data_t raw = {0};
    if (d_aq_can_raw_upright_fr_acceleration_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_fr_acceleration_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.acceleration_x1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x1 = (double)((double)raw.acceleration_x1 * 0.001 + 0.0);
    if (raw.acceleration_x2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x2 = (double)((double)raw.acceleration_x2 * 0.001 + 0.0);
    if (raw.acceleration_x3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x3 = (double)((double)raw.acceleration_x3 * 0.001 + 0.0);
    if (raw.acceleration_x4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x4 = (double)((double)raw.acceleration_x4 * 0.001 + 0.0);
    if (raw.acceleration_x_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x_average = (double)((double)raw.acceleration_x_average * 0.001 + 0.0);
    if (raw.acceleration_y1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y1 = (double)((double)raw.acceleration_y1 * 0.001 + 0.0);
    if (raw.acceleration_y2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y2 = (double)((double)raw.acceleration_y2 * 0.001 + 0.0);
    if (raw.acceleration_y3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y3 = (double)((double)raw.acceleration_y3 * 0.001 + 0.0);
    if (raw.acceleration_y4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y4 = (double)((double)raw.acceleration_y4 * 0.001 + 0.0);
    if (raw.acceleration_y_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y_average = (double)((double)raw.acceleration_y_average * 0.001 + 0.0);
    if (raw.acceleration_z1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z1 = (double)((double)raw.acceleration_z1 * 0.001 + 0.0);
    if (raw.acceleration_z2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z2 = (double)((double)raw.acceleration_z2 * 0.001 + 0.0);
    if (raw.acceleration_z3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z3 = (double)((double)raw.acceleration_z3 * 0.001 + 0.0);
    if (raw.acceleration_z4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z4 = (double)((double)raw.acceleration_z4 * 0.001 + 0.0);
    if (raw.acceleration_z_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z_average = (double)((double)raw.acceleration_z_average * 0.001 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_fr_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_gyro_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_fr_gyro_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->gyro_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1601),
        .len = 48, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_fr_gyro_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_fr_gyro_data_unpack(d_aq_can_upright_fr_gyro_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1601) || src->len != 48 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_fr_gyro_data_t raw = {0};
    if (d_aq_can_raw_upright_fr_gyro_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_fr_gyro_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    result.gyro_x1 = (double)((double)raw.gyro_x1 * 0.02 + 0.0);
    result.gyro_x2 = (double)((double)raw.gyro_x2 * 0.02 + 0.0);
    result.gyro_x3 = (double)((double)raw.gyro_x3 * 0.02 + 0.0);
    result.gyro_x4 = (double)((double)raw.gyro_x4 * 0.02 + 0.0);
    result.gyro_x_average = (double)((double)raw.gyro_x_average * 0.02 + 0.0);
    result.gyro_y1 = (double)((double)raw.gyro_y1 * 0.02 + 0.0);
    result.gyro_y2 = (double)((double)raw.gyro_y2 * 0.02 + 0.0);
    result.gyro_y3 = (double)((double)raw.gyro_y3 * 0.02 + 0.0);
    result.gyro_y4 = (double)((double)raw.gyro_y4 * 0.02 + 0.0);
    result.gyro_y_average = (double)((double)raw.gyro_y_average * 0.02 + 0.0);
    result.gyro_z1 = (double)((double)raw.gyro_z1 * 0.02 + 0.0);
    result.gyro_z2 = (double)((double)raw.gyro_z2 * 0.02 + 0.0);
    result.gyro_z3 = (double)((double)raw.gyro_z3 * 0.02 + 0.0);
    result.gyro_z4 = (double)((double)raw.gyro_z4 * 0.02 + 0.0);
    result.gyro_z_average = (double)((double)raw.gyro_z_average * 0.02 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_fr_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_wheel_speed_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_fr_wheel_speed_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->wheel_rpm1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm_average = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1602),
        .len = 16, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_fr_wheel_speed_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_fr_wheel_speed_data_unpack(d_aq_can_upright_fr_wheel_speed_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1602) || src->len != 16 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_fr_wheel_speed_data_t raw = {0};
    if (d_aq_can_raw_upright_fr_wheel_speed_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_fr_wheel_speed_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.wheel_rpm1 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm1 = (double)((double)raw.wheel_rpm1 * 0.5 + 0.0);
    if (raw.wheel_rpm2 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm2 = (double)((double)raw.wheel_rpm2 * 0.5 + 0.0);
    if (raw.wheel_rpm3 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm3 = (double)((double)raw.wheel_rpm3 * 0.5 + 0.0);
    if (raw.wheel_rpm4 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm4 = (double)((double)raw.wheel_rpm4 * 0.5 + 0.0);
    if (raw.wheel_rpm_average > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm_average = (double)((double)raw.wheel_rpm_average * 0.5 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_fl_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_acceleration_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_fl_acceleration_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->acceleration_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1616),
        .len = 48, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_fl_acceleration_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_fl_acceleration_data_unpack(d_aq_can_upright_fl_acceleration_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1616) || src->len != 48 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_fl_acceleration_data_t raw = {0};
    if (d_aq_can_raw_upright_fl_acceleration_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_fl_acceleration_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.acceleration_x1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x1 = (double)((double)raw.acceleration_x1 * 0.001 + 0.0);
    if (raw.acceleration_x2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x2 = (double)((double)raw.acceleration_x2 * 0.001 + 0.0);
    if (raw.acceleration_x3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x3 = (double)((double)raw.acceleration_x3 * 0.001 + 0.0);
    if (raw.acceleration_x4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x4 = (double)((double)raw.acceleration_x4 * 0.001 + 0.0);
    if (raw.acceleration_x_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x_average = (double)((double)raw.acceleration_x_average * 0.001 + 0.0);
    if (raw.acceleration_y1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y1 = (double)((double)raw.acceleration_y1 * 0.001 + 0.0);
    if (raw.acceleration_y2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y2 = (double)((double)raw.acceleration_y2 * 0.001 + 0.0);
    if (raw.acceleration_y3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y3 = (double)((double)raw.acceleration_y3 * 0.001 + 0.0);
    if (raw.acceleration_y4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y4 = (double)((double)raw.acceleration_y4 * 0.001 + 0.0);
    if (raw.acceleration_y_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y_average = (double)((double)raw.acceleration_y_average * 0.001 + 0.0);
    if (raw.acceleration_z1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z1 = (double)((double)raw.acceleration_z1 * 0.001 + 0.0);
    if (raw.acceleration_z2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z2 = (double)((double)raw.acceleration_z2 * 0.001 + 0.0);
    if (raw.acceleration_z3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z3 = (double)((double)raw.acceleration_z3 * 0.001 + 0.0);
    if (raw.acceleration_z4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z4 = (double)((double)raw.acceleration_z4 * 0.001 + 0.0);
    if (raw.acceleration_z_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z_average = (double)((double)raw.acceleration_z_average * 0.001 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_fl_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_gyro_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_fl_gyro_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->gyro_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1617),
        .len = 48, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_fl_gyro_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_fl_gyro_data_unpack(d_aq_can_upright_fl_gyro_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1617) || src->len != 48 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_fl_gyro_data_t raw = {0};
    if (d_aq_can_raw_upright_fl_gyro_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_fl_gyro_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    result.gyro_x1 = (double)((double)raw.gyro_x1 * 0.02 + 0.0);
    result.gyro_x2 = (double)((double)raw.gyro_x2 * 0.02 + 0.0);
    result.gyro_x3 = (double)((double)raw.gyro_x3 * 0.02 + 0.0);
    result.gyro_x4 = (double)((double)raw.gyro_x4 * 0.02 + 0.0);
    result.gyro_x_average = (double)((double)raw.gyro_x_average * 0.02 + 0.0);
    result.gyro_y1 = (double)((double)raw.gyro_y1 * 0.02 + 0.0);
    result.gyro_y2 = (double)((double)raw.gyro_y2 * 0.02 + 0.0);
    result.gyro_y3 = (double)((double)raw.gyro_y3 * 0.02 + 0.0);
    result.gyro_y4 = (double)((double)raw.gyro_y4 * 0.02 + 0.0);
    result.gyro_y_average = (double)((double)raw.gyro_y_average * 0.02 + 0.0);
    result.gyro_z1 = (double)((double)raw.gyro_z1 * 0.02 + 0.0);
    result.gyro_z2 = (double)((double)raw.gyro_z2 * 0.02 + 0.0);
    result.gyro_z3 = (double)((double)raw.gyro_z3 * 0.02 + 0.0);
    result.gyro_z4 = (double)((double)raw.gyro_z4 * 0.02 + 0.0);
    result.gyro_z_average = (double)((double)raw.gyro_z_average * 0.02 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_fl_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_wheel_speed_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_fl_wheel_speed_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->wheel_rpm1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm_average = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1618),
        .len = 16, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_fl_wheel_speed_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_fl_wheel_speed_data_unpack(d_aq_can_upright_fl_wheel_speed_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1618) || src->len != 16 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_fl_wheel_speed_data_t raw = {0};
    if (d_aq_can_raw_upright_fl_wheel_speed_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_fl_wheel_speed_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.wheel_rpm1 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm1 = (double)((double)raw.wheel_rpm1 * 0.5 + 0.0);
    if (raw.wheel_rpm2 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm2 = (double)((double)raw.wheel_rpm2 * 0.5 + 0.0);
    if (raw.wheel_rpm3 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm3 = (double)((double)raw.wheel_rpm3 * 0.5 + 0.0);
    if (raw.wheel_rpm4 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm4 = (double)((double)raw.wheel_rpm4 * 0.5 + 0.0);
    if (raw.wheel_rpm_average > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm_average = (double)((double)raw.wheel_rpm_average * 0.5 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_rear_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_acceleration_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_rear_acceleration_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->acceleration_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->acceleration_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -16.384) { value = -16.384; status = CAN_CLAMPED; }
        if (value > 16.383) { value = 16.383; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.001);
        if (encoded < -16384.0) encoded = -16384.0;
        if (encoded > 16383.0) encoded = 16383.0;
        raw.acceleration_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1632),
        .len = 48, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_rear_acceleration_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_rear_acceleration_data_unpack(d_aq_can_upright_rear_acceleration_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1632) || src->len != 48 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_rear_acceleration_data_t raw = {0};
    if (d_aq_can_raw_upright_rear_acceleration_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_rear_acceleration_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.acceleration_x1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x1 = (double)((double)raw.acceleration_x1 * 0.001 + 0.0);
    if (raw.acceleration_x2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x2 = (double)((double)raw.acceleration_x2 * 0.001 + 0.0);
    if (raw.acceleration_x3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x3 = (double)((double)raw.acceleration_x3 * 0.001 + 0.0);
    if (raw.acceleration_x4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x4 = (double)((double)raw.acceleration_x4 * 0.001 + 0.0);
    if (raw.acceleration_x_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_x_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_x_average = (double)((double)raw.acceleration_x_average * 0.001 + 0.0);
    if (raw.acceleration_y1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y1 = (double)((double)raw.acceleration_y1 * 0.001 + 0.0);
    if (raw.acceleration_y2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y2 = (double)((double)raw.acceleration_y2 * 0.001 + 0.0);
    if (raw.acceleration_y3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y3 = (double)((double)raw.acceleration_y3 * 0.001 + 0.0);
    if (raw.acceleration_y4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y4 = (double)((double)raw.acceleration_y4 * 0.001 + 0.0);
    if (raw.acceleration_y_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_y_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_y_average = (double)((double)raw.acceleration_y_average * 0.001 + 0.0);
    if (raw.acceleration_z1 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z1 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z1 = (double)((double)raw.acceleration_z1 * 0.001 + 0.0);
    if (raw.acceleration_z2 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z2 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z2 = (double)((double)raw.acceleration_z2 * 0.001 + 0.0);
    if (raw.acceleration_z3 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z3 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z3 = (double)((double)raw.acceleration_z3 * 0.001 + 0.0);
    if (raw.acceleration_z4 < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z4 > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z4 = (double)((double)raw.acceleration_z4 * 0.001 + 0.0);
    if (raw.acceleration_z_average < (-INT64_C(16384))) return CAN_RANGE;
    if (raw.acceleration_z_average > INT64_C(16383)) return CAN_RANGE;
    result.acceleration_z_average = (double)((double)raw.acceleration_z_average * 0.001 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_rear_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_gyro_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_rear_gyro_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->gyro_x1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_x_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_x_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_y_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_y_average = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z1 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z2 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z3 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z4 = (int16_t)encoded;
    }
    {
        double value = (double)src->gyro_z_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -655.36) { value = -655.36; status = CAN_CLAMPED; }
        if (value > 655.34) { value = 655.34; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.02);
        if (encoded < -32768.0) encoded = -32768.0;
        if (encoded > 32767.0) encoded = 32767.0;
        raw.gyro_z_average = (int16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1633),
        .len = 48, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_rear_gyro_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_rear_gyro_data_unpack(d_aq_can_upright_rear_gyro_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1633) || src->len != 48 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_rear_gyro_data_t raw = {0};
    if (d_aq_can_raw_upright_rear_gyro_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_rear_gyro_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    result.gyro_x1 = (double)((double)raw.gyro_x1 * 0.02 + 0.0);
    result.gyro_x2 = (double)((double)raw.gyro_x2 * 0.02 + 0.0);
    result.gyro_x3 = (double)((double)raw.gyro_x3 * 0.02 + 0.0);
    result.gyro_x4 = (double)((double)raw.gyro_x4 * 0.02 + 0.0);
    result.gyro_x_average = (double)((double)raw.gyro_x_average * 0.02 + 0.0);
    result.gyro_y1 = (double)((double)raw.gyro_y1 * 0.02 + 0.0);
    result.gyro_y2 = (double)((double)raw.gyro_y2 * 0.02 + 0.0);
    result.gyro_y3 = (double)((double)raw.gyro_y3 * 0.02 + 0.0);
    result.gyro_y4 = (double)((double)raw.gyro_y4 * 0.02 + 0.0);
    result.gyro_y_average = (double)((double)raw.gyro_y_average * 0.02 + 0.0);
    result.gyro_z1 = (double)((double)raw.gyro_z1 * 0.02 + 0.0);
    result.gyro_z2 = (double)((double)raw.gyro_z2 * 0.02 + 0.0);
    result.gyro_z3 = (double)((double)raw.gyro_z3 * 0.02 + 0.0);
    result.gyro_z4 = (double)((double)raw.gyro_z4 * 0.02 + 0.0);
    result.gyro_z_average = (double)((double)raw.gyro_z_average * 0.02 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_upright_rear_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_wheel_speed_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_upright_rear_wheel_speed_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        uint16_t value = src->sample_interval_ticks;
        if (value < INT64_C(1)) {
            value = (uint16_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sample_interval_ticks = (uint16_t)value;
    }
    {
        double value = (double)src->wheel_rpm1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->wheel_rpm_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 0.0) { value = 0.0; status = CAN_CLAMPED; }
        if (value > 2047.5) { value = 2047.5; status = CAN_CLAMPED; }
        double encoded = round((value - 0.0) / 0.5);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.wheel_rpm_average = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1634),
        .len = 16, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_upright_rear_wheel_speed_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_upright_rear_wheel_speed_data_unpack(d_aq_can_upright_rear_wheel_speed_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1634) || src->len != 16 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_upright_rear_wheel_speed_data_t raw = {0};
    if (d_aq_can_raw_upright_rear_wheel_speed_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_upright_rear_wheel_speed_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.sample_interval_ticks < INT64_C(1)) return CAN_RANGE;
    result.sample_interval_ticks = (uint16_t)raw.sample_interval_ticks;
    if (raw.wheel_rpm1 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm1 = (double)((double)raw.wheel_rpm1 * 0.5 + 0.0);
    if (raw.wheel_rpm2 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm2 = (double)((double)raw.wheel_rpm2 * 0.5 + 0.0);
    if (raw.wheel_rpm3 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm3 = (double)((double)raw.wheel_rpm3 * 0.5 + 0.0);
    if (raw.wheel_rpm4 > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm4 = (double)((double)raw.wheel_rpm4 * 0.5 + 0.0);
    if (raw.wheel_rpm_average > INT64_C(4095)) return CAN_RANGE;
    result.wheel_rpm_average = (double)((double)raw.wheel_rpm_average * 0.5 + 0.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_suspension_fr_data_pack(can_frame_t* dst, const d_aq_can_suspension_fr_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_suspension_fr_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        double value = (double)src->linpot_position1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position_average = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1648),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_suspension_fr_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_suspension_fr_data_unpack(d_aq_can_suspension_fr_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1648) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_suspension_fr_data_t raw = {0};
    if (d_aq_can_raw_suspension_fr_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_suspension_fr_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.linpot_position1 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position1 = (double)((double)raw.linpot_position1 * 0.0188885 + 1.0);
    if (raw.linpot_position2 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position2 = (double)((double)raw.linpot_position2 * 0.0188885 + 1.0);
    if (raw.linpot_position3 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position3 = (double)((double)raw.linpot_position3 * 0.0188885 + 1.0);
    if (raw.linpot_position4 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position4 = (double)((double)raw.linpot_position4 * 0.0188885 + 1.0);
    if (raw.linpot_position_average > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position_average = (double)((double)raw.linpot_position_average * 0.0188885 + 1.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_suspension_fl_data_pack(can_frame_t* dst, const d_aq_can_suspension_fl_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_suspension_fl_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        double value = (double)src->linpot_position1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position_average = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1649),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_suspension_fl_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_suspension_fl_data_unpack(d_aq_can_suspension_fl_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1649) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_suspension_fl_data_t raw = {0};
    if (d_aq_can_raw_suspension_fl_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_suspension_fl_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.linpot_position1 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position1 = (double)((double)raw.linpot_position1 * 0.0188885 + 1.0);
    if (raw.linpot_position2 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position2 = (double)((double)raw.linpot_position2 * 0.0188885 + 1.0);
    if (raw.linpot_position3 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position3 = (double)((double)raw.linpot_position3 * 0.0188885 + 1.0);
    if (raw.linpot_position4 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position4 = (double)((double)raw.linpot_position4 * 0.0188885 + 1.0);
    if (raw.linpot_position_average > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position_average = (double)((double)raw.linpot_position_average * 0.0188885 + 1.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_suspension_rear_data_pack(can_frame_t* dst, const d_aq_can_suspension_rear_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_suspension_rear_data_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint16_t value = src->timestamp;
        raw.timestamp = (uint16_t)value;
    }
    {
        double value = (double)src->linpot_position1;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position1 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position2;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position2 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position3;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position3 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position4;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position4 = (uint16_t)encoded;
    }
    {
        double value = (double)src->linpot_position_average;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < 1.0) { value = 1.0; status = CAN_CLAMPED; }
        if (value > 78.3484075) { value = 78.3484075; status = CAN_CLAMPED; }
        double encoded = round((value - 1.0) / 0.0188885);
        if (encoded < 0.0) encoded = 0.0;
        if (encoded > 4095.0) encoded = 4095.0;
        raw.linpot_position_average = (uint16_t)encoded;
    }
    {
        uint16_t value = src->sample_age_ms;
        raw.sample_age_ms = (uint16_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1650),
        .len = 12, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_suspension_rear_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_suspension_rear_data_unpack(d_aq_can_suspension_rear_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1650) || src->len != 12 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_suspension_rear_data_t raw = {0};
    if (d_aq_can_raw_suspension_rear_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_suspension_rear_data_t result = {0};
    result.timestamp = (uint16_t)raw.timestamp;
    if (raw.linpot_position1 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position1 = (double)((double)raw.linpot_position1 * 0.0188885 + 1.0);
    if (raw.linpot_position2 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position2 = (double)((double)raw.linpot_position2 * 0.0188885 + 1.0);
    if (raw.linpot_position3 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position3 = (double)((double)raw.linpot_position3 * 0.0188885 + 1.0);
    if (raw.linpot_position4 > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position4 = (double)((double)raw.linpot_position4 * 0.0188885 + 1.0);
    if (raw.linpot_position_average > INT64_C(4095)) return CAN_RANGE;
    result.linpot_position_average = (double)((double)raw.linpot_position_average * 0.0188885 + 1.0);
    result.sample_age_ms = (uint16_t)raw.sample_age_ms;
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_blackbody1_data_pack(can_frame_t* dst, const d_aq_can_blackbody1_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_blackbody1_data_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1664),
        .len = 0, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_blackbody1_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_blackbody1_data_unpack(d_aq_can_blackbody1_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1664) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_blackbody1_data_t raw = {0};
    if (d_aq_can_raw_blackbody1_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_blackbody1_data_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_blackbody2_data_pack(can_frame_t* dst, const d_aq_can_blackbody2_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_blackbody2_data_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1665),
        .len = 0, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_blackbody2_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_blackbody2_data_unpack(d_aq_can_blackbody2_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1665) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_blackbody2_data_t raw = {0};
    if (d_aq_can_raw_blackbody2_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_blackbody2_data_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_blackbody3_data_pack(can_frame_t* dst, const d_aq_can_blackbody3_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_blackbody3_data_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1666),
        .len = 0, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_blackbody3_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_blackbody3_data_unpack(d_aq_can_blackbody3_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1666) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_blackbody3_data_t raw = {0};
    if (d_aq_can_raw_blackbody3_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_blackbody3_data_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_strain_gauge_front_data_pack(can_frame_t* dst, const d_aq_can_strain_gauge_front_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_strain_gauge_front_data_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1680),
        .len = 0, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_strain_gauge_front_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_strain_gauge_front_data_unpack(d_aq_can_strain_gauge_front_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1680) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_strain_gauge_front_data_t raw = {0};
    if (d_aq_can_raw_strain_gauge_front_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_strain_gauge_front_data_t result = {0};
    *dst = result;
    return CAN_OK;
}

can_status_t d_aq_can_strain_gauge_rear_data_pack(can_frame_t* dst, const d_aq_can_strain_gauge_rear_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct d_aq_can_raw_strain_gauge_rear_data_t raw = {0};
    can_status_t status = CAN_OK;
    can_frame_t result = { .id = UINT32_C(1681),
        .len = 0, .is_extended = false,
        .is_fd = true };
    if (d_aq_can_raw_strain_gauge_rear_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t d_aq_can_strain_gauge_rear_data_unpack(d_aq_can_strain_gauge_rear_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1681) || src->len != 0 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct d_aq_can_raw_strain_gauge_rear_data_t raw = {0};
    if (d_aq_can_raw_strain_gauge_rear_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    d_aq_can_strain_gauge_rear_data_t result = {0};
    *dst = result;
    return CAN_OK;
}
