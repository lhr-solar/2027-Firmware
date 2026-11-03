#include "telemetry_link.h"
#include "detail/telemetry_link_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const telemetry_link_metadata_t telemetry_link = {
    .cm5 = {
        .telemetry_status = { .id = UINT32_C(1569), .payload_bytes = 5, .wire_bytes = 5, .dlc = 5, .is_extended = false, .is_fd = true },
        .gps_data = { .id = UINT32_C(1570), .payload_bytes = 16, .wire_bytes = 16, .dlc = 10, .is_extended = false, .is_fd = true },
        .on_board_watchdog = { .id = UINT32_C(1571), .payload_bytes = 1, .wire_bytes = 1, .dlc = 1, .is_extended = false, .is_fd = true },
        .sig_wave = { .id = UINT32_C(2045), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .max_message_classical = { .id = UINT32_C(2046), .payload_bytes = 8, .wire_bytes = 8, .dlc = 8, .is_extended = false, .is_fd = false },
        .max_message_fd = { .id = UINT32_C(2047), .payload_bytes = 64, .wire_bytes = 64, .dlc = 15, .is_extended = false, .is_fd = true },
    },
};

can_status_t telemetry_link_telemetry_status_pack(can_frame_t* dst, const telemetry_link_telemetry_status_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct telemetry_link_raw_telemetry_status_t raw = {0};
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
        uint8_t value = src->server_connected;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.server_connected = (uint8_t)value;
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
    can_frame_t result = { .id = UINT32_C(1569),
        .len = 5, .is_extended = false,
        .is_fd = true };
    if (telemetry_link_raw_telemetry_status_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t telemetry_link_telemetry_status_unpack(telemetry_link_telemetry_status_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1569) || src->len != 5 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct telemetry_link_raw_telemetry_status_t raw = {0};
    if (telemetry_link_raw_telemetry_status_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    telemetry_link_telemetry_status_t result = {0};
    if (raw.cellular_connected > INT64_C(1)) return CAN_RANGE;
    result.cellular_connected = (uint8_t)raw.cellular_connected;
    if (raw.cellular_technology > INT64_C(7)) return CAN_RANGE;
    result.cellular_technology = (uint8_t)raw.cellular_technology;
    if (raw.roaming > INT64_C(1)) return CAN_RANGE;
    result.roaming = (uint8_t)raw.roaming;
    if (raw.internet_connected > INT64_C(1)) return CAN_RANGE;
    result.internet_connected = (uint8_t)raw.internet_connected;
    if (raw.server_connected > INT64_C(1)) return CAN_RANGE;
    result.server_connected = (uint8_t)raw.server_connected;
    result.rssi = (int8_t)raw.rssi;
    result.rsrp = (int8_t)raw.rsrp;
    result.rsrq = (double)((double)raw.rsrq * 0.5 + 0.0);
    result.sinr = (double)((double)raw.sinr * 0.5 + 0.0);
    *dst = result;
    return CAN_OK;
}

can_status_t telemetry_link_gps_data_pack(can_frame_t* dst, const telemetry_link_gps_data_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct telemetry_link_raw_gps_data_t raw = {0};
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
    can_frame_t result = { .id = UINT32_C(1570),
        .len = 16, .is_extended = false,
        .is_fd = true };
    if (telemetry_link_raw_gps_data_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t telemetry_link_gps_data_unpack(telemetry_link_gps_data_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1570) || src->len != 16 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct telemetry_link_raw_gps_data_t raw = {0};
    if (telemetry_link_raw_gps_data_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    telemetry_link_gps_data_t result = {0};
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
    *dst = result;
    return CAN_OK;
}

can_status_t telemetry_link_on_board_watchdog_pack(can_frame_t* dst, const telemetry_link_on_board_watchdog_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct telemetry_link_raw_on_board_watchdog_t raw = {0};
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
        uint8_t value = src->msv_watchdog;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.msv_watchdog = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(1571),
        .len = 1, .is_extended = false,
        .is_fd = true };
    if (telemetry_link_raw_on_board_watchdog_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t telemetry_link_on_board_watchdog_unpack(telemetry_link_on_board_watchdog_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(1571) || src->len != 1 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct telemetry_link_raw_on_board_watchdog_t raw = {0};
    if (telemetry_link_raw_on_board_watchdog_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    telemetry_link_on_board_watchdog_t result = {0};
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
    if (raw.msv_watchdog > INT64_C(1)) return CAN_RANGE;
    result.msv_watchdog = (uint8_t)raw.msv_watchdog;
    *dst = result;
    return CAN_OK;
}

can_status_t telemetry_link_sig_wave_pack(can_frame_t* dst, const telemetry_link_sig_wave_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct telemetry_link_raw_sig_wave_t raw = {0};
    can_status_t status = CAN_OK;
    {
        float value = src->sine;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -1.0) { value = (float)-1.0; status = CAN_CLAMPED; }
        if (value > 1.0) { value = (float)1.0; status = CAN_CLAMPED; }
        memcpy(&raw.sine, &value, sizeof(value));
    }
    {
        float value = src->cosine;
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -1.0) { value = (float)-1.0; status = CAN_CLAMPED; }
        if (value > 1.0) { value = (float)1.0; status = CAN_CLAMPED; }
        memcpy(&raw.cosine, &value, sizeof(value));
    }
    can_frame_t result = { .id = UINT32_C(2045),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (telemetry_link_raw_sig_wave_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t telemetry_link_sig_wave_unpack(telemetry_link_sig_wave_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(2045) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct telemetry_link_raw_sig_wave_t raw = {0};
    if (telemetry_link_raw_sig_wave_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    telemetry_link_sig_wave_t result = {0};
    {
        float value;
        memcpy(&value, &raw.sine, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -1.0) return CAN_RANGE;
        if (value > 1.0) return CAN_RANGE;
        result.sine = value;
    }
    {
        float value;
        memcpy(&value, &raw.cosine, sizeof(value));
        if (!isfinite(value)) return CAN_INVALID;
        if (value < -1.0) return CAN_RANGE;
        if (value > 1.0) return CAN_RANGE;
        result.cosine = value;
    }
    *dst = result;
    return CAN_OK;
}

can_status_t telemetry_link_max_message_classical_pack(can_frame_t* dst, const telemetry_link_max_message_classical_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct telemetry_link_raw_max_message_classical_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->sg0;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg0 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg1;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg1 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg2;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg2 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg3;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg3 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg4;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg4 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg5;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg5 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg6;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg6 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg7;
        if (value > INT64_C(1)) {
            value = (uint8_t)INT64_C(1); status = CAN_CLAMPED;
        }
        raw.sg7 = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(2046),
        .len = 8, .is_extended = false,
        .is_fd = false };
    if (telemetry_link_raw_max_message_classical_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t telemetry_link_max_message_classical_unpack(telemetry_link_max_message_classical_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(2046) || src->len != 8 ||
        src->is_extended != false ||
        src->is_fd != false ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct telemetry_link_raw_max_message_classical_t raw = {0};
    if (telemetry_link_raw_max_message_classical_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    telemetry_link_max_message_classical_t result = {0};
    if (raw.sg0 > INT64_C(1)) return CAN_RANGE;
    result.sg0 = (uint8_t)raw.sg0;
    if (raw.sg1 > INT64_C(1)) return CAN_RANGE;
    result.sg1 = (uint8_t)raw.sg1;
    if (raw.sg2 > INT64_C(1)) return CAN_RANGE;
    result.sg2 = (uint8_t)raw.sg2;
    if (raw.sg3 > INT64_C(1)) return CAN_RANGE;
    result.sg3 = (uint8_t)raw.sg3;
    if (raw.sg4 > INT64_C(1)) return CAN_RANGE;
    result.sg4 = (uint8_t)raw.sg4;
    if (raw.sg5 > INT64_C(1)) return CAN_RANGE;
    result.sg5 = (uint8_t)raw.sg5;
    if (raw.sg6 > INT64_C(1)) return CAN_RANGE;
    result.sg6 = (uint8_t)raw.sg6;
    if (raw.sg7 > INT64_C(1)) return CAN_RANGE;
    result.sg7 = (uint8_t)raw.sg7;
    *dst = result;
    return CAN_OK;
}

can_status_t telemetry_link_max_message_fd_pack(can_frame_t* dst, const telemetry_link_max_message_fd_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    struct telemetry_link_raw_max_message_fd_t raw = {0};
    can_status_t status = CAN_OK;
    {
        uint8_t value = src->sg0;
        raw.sg0 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg1;
        raw.sg1 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg2;
        raw.sg2 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg3;
        raw.sg3 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg4;
        raw.sg4 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg5;
        raw.sg5 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg6;
        raw.sg6 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg7;
        raw.sg7 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg8;
        raw.sg8 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg9;
        raw.sg9 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg10;
        raw.sg10 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg11;
        raw.sg11 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg12;
        raw.sg12 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg13;
        raw.sg13 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg14;
        raw.sg14 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg15;
        raw.sg15 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg16;
        raw.sg16 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg17;
        raw.sg17 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg18;
        raw.sg18 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg19;
        raw.sg19 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg20;
        raw.sg20 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg21;
        raw.sg21 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg22;
        raw.sg22 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg23;
        raw.sg23 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg24;
        raw.sg24 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg25;
        raw.sg25 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg26;
        raw.sg26 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg27;
        raw.sg27 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg28;
        raw.sg28 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg29;
        raw.sg29 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg30;
        raw.sg30 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg31;
        raw.sg31 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg32;
        raw.sg32 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg33;
        raw.sg33 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg34;
        raw.sg34 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg35;
        raw.sg35 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg36;
        raw.sg36 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg37;
        raw.sg37 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg38;
        raw.sg38 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg39;
        raw.sg39 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg40;
        raw.sg40 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg41;
        raw.sg41 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg42;
        raw.sg42 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg43;
        raw.sg43 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg44;
        raw.sg44 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg45;
        raw.sg45 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg46;
        raw.sg46 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg47;
        raw.sg47 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg48;
        raw.sg48 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg49;
        raw.sg49 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg50;
        raw.sg50 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg51;
        raw.sg51 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg52;
        raw.sg52 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg53;
        raw.sg53 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg54;
        raw.sg54 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg55;
        raw.sg55 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg56;
        raw.sg56 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg57;
        raw.sg57 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg58;
        raw.sg58 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg59;
        raw.sg59 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg60;
        raw.sg60 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg61;
        raw.sg61 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg62;
        raw.sg62 = (uint8_t)value;
    }
    {
        uint8_t value = src->sg63;
        raw.sg63 = (uint8_t)value;
    }
    can_frame_t result = { .id = UINT32_C(2047),
        .len = 64, .is_extended = false,
        .is_fd = true };
    if (telemetry_link_raw_max_message_fd_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;
    *dst = result;
    return status;
}

can_status_t telemetry_link_max_message_fd_unpack(telemetry_link_max_message_fd_t* dst, const can_frame_t* src) {
    if (dst == NULL || src == NULL) return CAN_INVALID;
    if (src->id != UINT32_C(2047) || src->len != 64 ||
        src->is_extended != false ||
        src->is_fd != true ||
        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;
    struct telemetry_link_raw_max_message_fd_t raw = {0};
    if (telemetry_link_raw_max_message_fd_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;
    telemetry_link_max_message_fd_t result = {0};
    result.sg0 = (uint8_t)raw.sg0;
    result.sg1 = (uint8_t)raw.sg1;
    result.sg2 = (uint8_t)raw.sg2;
    result.sg3 = (uint8_t)raw.sg3;
    result.sg4 = (uint8_t)raw.sg4;
    result.sg5 = (uint8_t)raw.sg5;
    result.sg6 = (uint8_t)raw.sg6;
    result.sg7 = (uint8_t)raw.sg7;
    result.sg8 = (uint8_t)raw.sg8;
    result.sg9 = (uint8_t)raw.sg9;
    result.sg10 = (uint8_t)raw.sg10;
    result.sg11 = (uint8_t)raw.sg11;
    result.sg12 = (uint8_t)raw.sg12;
    result.sg13 = (uint8_t)raw.sg13;
    result.sg14 = (uint8_t)raw.sg14;
    result.sg15 = (uint8_t)raw.sg15;
    result.sg16 = (uint8_t)raw.sg16;
    result.sg17 = (uint8_t)raw.sg17;
    result.sg18 = (uint8_t)raw.sg18;
    result.sg19 = (uint8_t)raw.sg19;
    result.sg20 = (uint8_t)raw.sg20;
    result.sg21 = (uint8_t)raw.sg21;
    result.sg22 = (uint8_t)raw.sg22;
    result.sg23 = (uint8_t)raw.sg23;
    result.sg24 = (uint8_t)raw.sg24;
    result.sg25 = (uint8_t)raw.sg25;
    result.sg26 = (uint8_t)raw.sg26;
    result.sg27 = (uint8_t)raw.sg27;
    result.sg28 = (uint8_t)raw.sg28;
    result.sg29 = (uint8_t)raw.sg29;
    result.sg30 = (uint8_t)raw.sg30;
    result.sg31 = (uint8_t)raw.sg31;
    result.sg32 = (uint8_t)raw.sg32;
    result.sg33 = (uint8_t)raw.sg33;
    result.sg34 = (uint8_t)raw.sg34;
    result.sg35 = (uint8_t)raw.sg35;
    result.sg36 = (uint8_t)raw.sg36;
    result.sg37 = (uint8_t)raw.sg37;
    result.sg38 = (uint8_t)raw.sg38;
    result.sg39 = (uint8_t)raw.sg39;
    result.sg40 = (uint8_t)raw.sg40;
    result.sg41 = (uint8_t)raw.sg41;
    result.sg42 = (uint8_t)raw.sg42;
    result.sg43 = (uint8_t)raw.sg43;
    result.sg44 = (uint8_t)raw.sg44;
    result.sg45 = (uint8_t)raw.sg45;
    result.sg46 = (uint8_t)raw.sg46;
    result.sg47 = (uint8_t)raw.sg47;
    result.sg48 = (uint8_t)raw.sg48;
    result.sg49 = (uint8_t)raw.sg49;
    result.sg50 = (uint8_t)raw.sg50;
    result.sg51 = (uint8_t)raw.sg51;
    result.sg52 = (uint8_t)raw.sg52;
    result.sg53 = (uint8_t)raw.sg53;
    result.sg54 = (uint8_t)raw.sg54;
    result.sg55 = (uint8_t)raw.sg55;
    result.sg56 = (uint8_t)raw.sg56;
    result.sg57 = (uint8_t)raw.sg57;
    result.sg58 = (uint8_t)raw.sg58;
    result.sg59 = (uint8_t)raw.sg59;
    result.sg60 = (uint8_t)raw.sg60;
    result.sg61 = (uint8_t)raw.sg61;
    result.sg62 = (uint8_t)raw.sg62;
    result.sg63 = (uint8_t)raw.sg63;
    *dst = result;
    return CAN_OK;
}
