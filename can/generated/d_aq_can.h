/** @file d_aq_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** MIKA timestamp synchronization: legacy two-byte unsigned little-endian raw count. New allocation ID 0x620 and CAN-FD retained; time unit/epoch/period are not specified. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Legacy unsigned 16-bit raw time count. Tick period and epoch are unresolved; do not interpret as milliseconds or seconds. Wraps modulo 65536 ticks. */
    uint16_t timestamp;
} d_aq_can_timestamp_sync_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_timestamp_sync_pack(can_frame_t* dst, const d_aq_can_timestamp_sync_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_timestamp_sync_unpack(d_aq_can_timestamp_sync_t* dst, const can_frame_t* src);

/** Named raw values for CellularConnected; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_TELEMETRY_STATUS_CELLULAR_CONNECTED_DISCONNECTED = 0,
    D_AQ_CAN_TELEMETRY_STATUS_CELLULAR_CONNECTED_CONNECTED = 1,
} d_aq_can_telemetry_status_cellular_connected_choice_t;
/** Named raw values for CellularTechnology; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_NONE = 0,
    D_AQ_CAN_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_LTE = 1,
    D_AQ_CAN_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_LTE_M = 2,
    D_AQ_CAN_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_NB_IO_T = 3,
    D_AQ_CAN_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_OTHER = 4,
} d_aq_can_telemetry_status_cellular_technology_choice_t;
/** Named raw values for Roaming; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_TELEMETRY_STATUS_ROAMING_OFF = 0,
    D_AQ_CAN_TELEMETRY_STATUS_ROAMING_ON = 1,
} d_aq_can_telemetry_status_roaming_choice_t;
/** Named raw values for InternetConnected; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_TELEMETRY_STATUS_INTERNET_CONNECTED_DISCONNECTED = 0,
    D_AQ_CAN_TELEMETRY_STATUS_INTERNET_CONNECTED_CONNECTED = 1,
} d_aq_can_telemetry_status_internet_connected_choice_t;
/** 2027 team-defined layout; physical values use the shared scaling conventions. */
typedef struct {
    /** CellularConnected; unit=none; physical=raw*1+0; raw=[0,1]. CellularConnected */
    uint8_t cellular_connected;
    /** CellularTechnology; unit=none; physical=raw*1+0; raw=[0,7]. CellularTechnology */
    uint8_t cellular_technology;
    /** Roaming; unit=none; physical=raw*1+0; raw=[0,1]. Roaming */
    uint8_t roaming;
    /** InternetConnected; unit=none; physical=raw*1+0; raw=[0,1]. InternetConnected */
    uint8_t internet_connected;
    /** RSSI; unit=dBm; physical=raw*1+0; raw=[-128,127]. RSSI */
    int8_t rssi;
    /** RSRP; unit=dBm; physical=raw*1+0; raw=[-128,127]. RSRP */
    int8_t rsrp;
    /** RSRQ; unit=dB; physical=raw*0.5+0; raw=[-128,127]. RSRQ */
    double rsrq;
    /** SINR; unit=dB; physical=raw*0.5+0; raw=[-128,127]. SINR */
    double sinr;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_telemetry_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_telemetry_status_pack(can_frame_t* dst, const d_aq_can_telemetry_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_telemetry_status_unpack(d_aq_can_telemetry_status_t* dst, const can_frame_t* src);

/** Named raw values for GPSAntennaState; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_GPS_DATA_GPS_ANTENNA_STATE_OK = 0,
    D_AQ_CAN_GPS_DATA_GPS_ANTENNA_STATE_OPEN_CIRCUIT = 1,
    D_AQ_CAN_GPS_DATA_GPS_ANTENNA_STATE_SHORT_CIRCUIT = 2,
    D_AQ_CAN_GPS_DATA_GPS_ANTENNA_STATE_UNKNOWN = 3,
} d_aq_can_gps_data_gps_antenna_state_choice_t;
/** Named raw values for GPSFix; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_GPS_DATA_GPS_FIX_NO_FIX = 0,
    D_AQ_CAN_GPS_DATA_GPS_FIX_FIX2_D = 1,
    D_AQ_CAN_GPS_DATA_GPS_FIX_FIX3_D = 2,
} d_aq_can_gps_data_gps_fix_choice_t;
/** GPS receiver state and measurements. SampleAgeMs is the age of the oldest included GPS measurement, saturated at 65535 ms. */
typedef struct {
    /** GPSAntennaState; unit=none; physical=raw*1+0; raw=[0,3]. GPSAntennaState */
    uint8_t gps_antenna_state;
    /** GPSFix; unit=none; physical=raw*1+0; raw=[0,3]. GPSFix */
    uint8_t gps_fix;
    /** GPSLongitude; unit=deg; physical=raw*0.000001+0; raw=[-180000000,180000000]. Longitude range corrected from swapped draft coordinates. */
    double gps_longitude;
    /** GPSLatitude; unit=deg; physical=raw*0.000001+0; raw=[-90000000,90000000]. GPSLatitude */
    double gps_latitude;
    /** GPSSpeed; unit=m/s; physical=raw*0.01+0; raw=[0,65535]. GPSSpeed */
    double gps_speed;
    /** GPSHeading; unit=deg; physical=raw*0.01+0; raw=[0,35999]. GPSHeading */
    double gps_heading;
    /** GPSAltitude; unit=m; physical=raw*0.1+0; raw=[-32768,32767]. GPSAltitude */
    double gps_altitude;
    /** SatellitesInUse; unit=none; physical=raw*1+0; raw=[0,255]. SatellitesInUse */
    uint8_t satellites_in_use;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_gps_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_gps_data_pack(can_frame_t* dst, const d_aq_can_gps_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_gps_data_unpack(d_aq_can_gps_data_t* dst, const can_frame_t* src);

/** Named raw values for BPSWatchdog; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_ON_BOARD_WATCHDOG_BPS_WATCHDOG_ALIVE = 0,
    D_AQ_CAN_ON_BOARD_WATCHDOG_BPS_WATCHDOG_TIMED_OUT = 1,
} d_aq_can_on_board_watchdog_bps_watchdog_choice_t;
/** Named raw values for VCUWatchdog; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_ON_BOARD_WATCHDOG_VCU_WATCHDOG_ALIVE = 0,
    D_AQ_CAN_ON_BOARD_WATCHDOG_VCU_WATCHDOG_TIMED_OUT = 1,
} d_aq_can_on_board_watchdog_vcu_watchdog_choice_t;
/** Named raw values for DEIWatchdog; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_ON_BOARD_WATCHDOG_DEI_WATCHDOG_ALIVE = 0,
    D_AQ_CAN_ON_BOARD_WATCHDOG_DEI_WATCHDOG_TIMED_OUT = 1,
} d_aq_can_on_board_watchdog_dei_watchdog_choice_t;
/** Named raw values for PedalsWatchdog; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_ON_BOARD_WATCHDOG_PEDALS_WATCHDOG_ALIVE = 0,
    D_AQ_CAN_ON_BOARD_WATCHDOG_PEDALS_WATCHDOG_TIMED_OUT = 1,
} d_aq_can_on_board_watchdog_pedals_watchdog_choice_t;
/** Named raw values for PDUWatchdog; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_ON_BOARD_WATCHDOG_PDU_WATCHDOG_ALIVE = 0,
    D_AQ_CAN_ON_BOARD_WATCHDOG_PDU_WATCHDOG_TIMED_OUT = 1,
} d_aq_can_on_board_watchdog_pdu_watchdog_choice_t;
/** Named raw values for LVCWatchdog; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_ON_BOARD_WATCHDOG_LVC_WATCHDOG_ALIVE = 0,
    D_AQ_CAN_ON_BOARD_WATCHDOG_LVC_WATCHDOG_TIMED_OUT = 1,
} d_aq_can_on_board_watchdog_lvc_watchdog_choice_t;
/** Named raw values for MPPTInterfaceWatchdog; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_ON_BOARD_WATCHDOG_MPPT_INTERFACE_WATCHDOG_ALIVE = 0,
    D_AQ_CAN_ON_BOARD_WATCHDOG_MPPT_INTERFACE_WATCHDOG_TIMED_OUT = 1,
} d_aq_can_on_board_watchdog_mppt_interface_watchdog_choice_t;
/** MIKA reports watchdog state of other boards: 0 alive, 1 timed out. Former MIKA self-watchdog removed; bit 7 is reserved. */
typedef struct {
    /** BPSWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. BPSWatchdog */
    uint8_t bps_watchdog;
    /** VCUWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. VCUWatchdog */
    uint8_t vcu_watchdog;
    /** DEIWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. DEIWatchdog */
    uint8_t dei_watchdog;
    /** PedalsWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. PedalsWatchdog */
    uint8_t pedals_watchdog;
    /** PDUWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. PDUWatchdog */
    uint8_t pdu_watchdog;
    /** LVCWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. LVCWatchdog */
    uint8_t lvc_watchdog;
    /** MPPTInterfaceWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. MPPTInterfaceWatchdog */
    uint8_t mppt_interface_watchdog;
} d_aq_can_on_board_watchdog_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_on_board_watchdog_pack(can_frame_t* dst, const d_aq_can_on_board_watchdog_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_on_board_watchdog_unpack(d_aq_can_on_board_watchdog_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationXAverage; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Sender-populated arithmetic mean of AccelerationX1..AccelerationX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_x_average;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationYAverage; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Sender-populated arithmetic mean of AccelerationY1..AccelerationY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_y_average;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
    /** AccelerationZAverage; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Sender-populated arithmetic mean of AccelerationZ1..AccelerationZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_chassis_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_chassis_acceleration_data_pack(can_frame_t* dst, const d_aq_can_chassis_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_chassis_acceleration_data_unpack(d_aq_can_chassis_acceleration_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroXAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroX1..GyroX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_x_average;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroYAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroY1..GyroY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_y_average;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
    /** GyroZAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroZ1..GyroZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_chassis_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_chassis_gyro_data_pack(can_frame_t* dst, const d_aq_can_chassis_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_chassis_gyro_data_unpack(d_aq_can_chassis_gyro_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** MagneticFieldX1; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 1 of MagneticFieldX; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_x1;
    /** MagneticFieldX2; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 2 of MagneticFieldX; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_x2;
    /** MagneticFieldX3; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 3 of MagneticFieldX; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_x3;
    /** MagneticFieldX4; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 4 of MagneticFieldX; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_x4;
    /** MagneticFieldXAverage; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Sender-populated arithmetic mean of MagneticFieldX1..MagneticFieldX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double magnetic_field_x_average;
    /** MagneticFieldY1; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 1 of MagneticFieldY; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_y1;
    /** MagneticFieldY2; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 2 of MagneticFieldY; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_y2;
    /** MagneticFieldY3; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 3 of MagneticFieldY; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_y3;
    /** MagneticFieldY4; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 4 of MagneticFieldY; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_y4;
    /** MagneticFieldYAverage; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Sender-populated arithmetic mean of MagneticFieldY1..MagneticFieldY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double magnetic_field_y_average;
    /** MagneticFieldZ1; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 1 of MagneticFieldZ; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_z1;
    /** MagneticFieldZ2; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 2 of MagneticFieldZ; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_z2;
    /** MagneticFieldZ3; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 3 of MagneticFieldZ; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_z3;
    /** MagneticFieldZ4; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Chronological sample 4 of MagneticFieldZ; indices 1..4 align across axes within this batch. Physical unit uT. */
    double magnetic_field_z4;
    /** MagneticFieldZAverage; unit=uT; physical=raw*0.15+0; raw=[-2048,2047]. Sender-populated arithmetic mean of MagneticFieldZ1..MagneticFieldZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double magnetic_field_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_chassis_magnetometer_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_chassis_magnetometer_data_pack(can_frame_t* dst, const d_aq_can_chassis_magnetometer_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_chassis_magnetometer_data_unpack(d_aq_can_chassis_magnetometer_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationXAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationX1..AccelerationX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_x_average;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationYAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationY1..AccelerationY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_y_average;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
    /** AccelerationZAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationZ1..AccelerationZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_fr_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fr_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fr_acceleration_data_unpack(d_aq_can_upright_fr_acceleration_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroXAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroX1..GyroX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_x_average;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroYAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroY1..GyroY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_y_average;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
    /** GyroZAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroZ1..GyroZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_fr_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fr_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fr_gyro_data_unpack(d_aq_can_upright_fr_gyro_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** WheelRPM1; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 1 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm1;
    /** WheelRPM2; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 2 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm2;
    /** WheelRPM3; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 3 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm3;
    /** WheelRPM4; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 4 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm4;
    /** WheelRPMAverage; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Sender-populated arithmetic mean of WheelRPM1..WheelRPM4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double wheel_rpm_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_fr_wheel_speed_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fr_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_wheel_speed_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fr_wheel_speed_data_unpack(d_aq_can_upright_fr_wheel_speed_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationXAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationX1..AccelerationX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_x_average;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationYAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationY1..AccelerationY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_y_average;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
    /** AccelerationZAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationZ1..AccelerationZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_fl_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fl_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fl_acceleration_data_unpack(d_aq_can_upright_fl_acceleration_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroXAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroX1..GyroX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_x_average;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroYAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroY1..GyroY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_y_average;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
    /** GyroZAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroZ1..GyroZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_fl_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fl_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fl_gyro_data_unpack(d_aq_can_upright_fl_gyro_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** WheelRPM1; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 1 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm1;
    /** WheelRPM2; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 2 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm2;
    /** WheelRPM3; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 3 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm3;
    /** WheelRPM4; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 4 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm4;
    /** WheelRPMAverage; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Sender-populated arithmetic mean of WheelRPM1..WheelRPM4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double wheel_rpm_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_fl_wheel_speed_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fl_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_wheel_speed_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fl_wheel_speed_data_unpack(d_aq_can_upright_fl_wheel_speed_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationXAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationX1..AccelerationX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_x_average;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationYAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationY1..AccelerationY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_y_average;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
    /** AccelerationZAverage; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Sender-populated arithmetic mean of AccelerationZ1..AccelerationZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double acceleration_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_rear_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_rear_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_rear_acceleration_data_unpack(d_aq_can_upright_rear_acceleration_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroXAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroX1..GyroX4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_x_average;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroYAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroY1..GyroY4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_y_average;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
    /** GyroZAverage; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Sender-populated arithmetic mean of GyroZ1..GyroZ4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double gyro_z_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_rear_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_rear_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_rear_gyro_data_unpack(d_aq_can_upright_rear_gyro_data_t* dst, const can_frame_t* src);

/** Four chronological samples per axis/value and their sender-computed averages. Little-endian CAN-FD; raw timestamp/interval tick duration remains unresolved. Emit only four valid samples, do not substitute stale/invalid values. Each sensor modality has its own sampling interval; magnetometer batches need not align with IMU batches. Sensor saturation must be handled by firmware. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Time count of sample 1. Same unresolved raw tick timebase as TimestampSync; wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** SampleIntervalTicks; unit=none; physical=raw*1+0; raw=[1,65535]. Uniform interval between samples 1..4, in the same unresolved ticks as Timestamp. Must be positive; send only complete four-sample batches. Matching accel/gyro batches from one board share timing. */
    uint16_t sample_interval_ticks;
    /** WheelRPM1; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 1 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm1;
    /** WheelRPM2; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 2 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm2;
    /** WheelRPM3; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 3 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm3;
    /** WheelRPM4; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 4 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm4;
    /** WheelRPMAverage; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Sender-populated arithmetic mean of WheelRPM1..WheelRPM4. Round to nearest wire step, exact ties away from zero in raw space. Codec does not compute this field. */
    double wheel_rpm_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_upright_rear_wheel_speed_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_rear_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_wheel_speed_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_rear_wheel_speed_data_unpack(d_aq_can_upright_rear_wheel_speed_data_t* dst, const can_frame_t* src);

/** Four simultaneous physical linpot channels, plus their sender-computed arithmetic mean. Legacy 16-bit raw timestamp and 12-bit ADC calibration retained; all little-endian. The average extends the payload from 8 to 12 bytes. The four channels are not chronological samples. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Legacy unsigned 16-bit raw time count. Tick period and epoch are unresolved; do not interpret as milliseconds or seconds. Wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** LinpotPosition1; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position1;
    /** LinpotPosition2; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position2;
    /** LinpotPosition3; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position3;
    /** LinpotPosition4; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position4;
    /** LinpotPositionAverage; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Sender-populated arithmetic mean of LinpotPosition1..4 in mm. Same scale/offset as individual channels: average the four raw ADC counts, round to nearest raw count (half upward), then apply mm = raw * 0.0188885 + 1. Codec does not compute this field. */
    double linpot_position_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_suspension_fr_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_suspension_fr_data_pack(can_frame_t* dst, const d_aq_can_suspension_fr_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_suspension_fr_data_unpack(d_aq_can_suspension_fr_data_t* dst, const can_frame_t* src);

/** Four simultaneous physical linpot channels, plus their sender-computed arithmetic mean. Legacy 16-bit raw timestamp and 12-bit ADC calibration retained; all little-endian. The average extends the payload from 8 to 12 bytes. The four channels are not chronological samples. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Legacy unsigned 16-bit raw time count. Tick period and epoch are unresolved; do not interpret as milliseconds or seconds. Wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** LinpotPosition1; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position1;
    /** LinpotPosition2; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position2;
    /** LinpotPosition3; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position3;
    /** LinpotPosition4; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position4;
    /** LinpotPositionAverage; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Sender-populated arithmetic mean of LinpotPosition1..4 in mm. Same scale/offset as individual channels: average the four raw ADC counts, round to nearest raw count (half upward), then apply mm = raw * 0.0188885 + 1. Codec does not compute this field. */
    double linpot_position_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_suspension_fl_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_suspension_fl_data_pack(can_frame_t* dst, const d_aq_can_suspension_fl_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_suspension_fl_data_unpack(d_aq_can_suspension_fl_data_t* dst, const can_frame_t* src);

/** Four simultaneous physical linpot channels, plus their sender-computed arithmetic mean. Legacy 16-bit raw timestamp and 12-bit ADC calibration retained; all little-endian. The average extends the payload from 8 to 12 bytes. The four channels are not chronological samples. */
typedef struct {
    /** Timestamp; unit=none; physical=raw*1+0; raw=[0,65535]. Legacy unsigned 16-bit raw time count. Tick period and epoch are unresolved; do not interpret as milliseconds or seconds. Wraps modulo 65536 ticks. */
    uint16_t timestamp;
    /** LinpotPosition1; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position1;
    /** LinpotPosition2; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position2;
    /** LinpotPosition3; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position3;
    /** LinpotPosition4; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Legacy linpot calibration: wire raw is 12-bit ADC count (0..4095); physical mm = raw * 0.0188885 + 1. Same sensor/measuring scheme as last year. */
    double linpot_position4;
    /** LinpotPositionAverage; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Sender-populated arithmetic mean of LinpotPosition1..4 in mm. Same scale/offset as individual channels: average the four raw ADC counts, round to nearest raw count (half upward), then apply mm = raw * 0.0188885 + 1. Codec does not compute this field. */
    double linpot_position_average;
    /** SampleAgeMs; unit=ms; physical=raw*1+0; raw=[0,65535]. Age in milliseconds of the oldest continuous sensor sample included in this frame when assembled; saturates at 65535. Age must advance for cached samples. In a fault snapshot, freeze age at fault detection; per-channel captured ages, if present, remain authoritative for individual trip records. */
    uint16_t sample_age_ms;
} d_aq_can_suspension_rear_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_suspension_rear_data_pack(can_frame_t* dst, const d_aq_can_suspension_rear_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_suspension_rear_data_unpack(d_aq_can_suspension_rear_data_t* dst, const can_frame_t* src);

/** PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} d_aq_can_blackbody1_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_blackbody1_data_pack(can_frame_t* dst, const d_aq_can_blackbody1_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_blackbody1_data_unpack(d_aq_can_blackbody1_data_t* dst, const can_frame_t* src);

/** PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} d_aq_can_blackbody2_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_blackbody2_data_pack(can_frame_t* dst, const d_aq_can_blackbody2_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_blackbody2_data_unpack(d_aq_can_blackbody2_data_t* dst, const can_frame_t* src);

/** PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} d_aq_can_blackbody3_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_blackbody3_data_pack(can_frame_t* dst, const d_aq_can_blackbody3_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_blackbody3_data_unpack(d_aq_can_blackbody3_data_t* dst, const can_frame_t* src);

/** PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} d_aq_can_strain_gauge_front_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_strain_gauge_front_data_pack(can_frame_t* dst, const d_aq_can_strain_gauge_front_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_strain_gauge_front_data_unpack(d_aq_can_strain_gauge_front_data_t* dst, const can_frame_t* src);

/** PLACEHOLDER: signals and payload length are not defined; zero DLC is a drafting marker, not an approved transmit layout. */
typedef struct {
    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */
} d_aq_can_strain_gauge_rear_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_strain_gauge_rear_data_pack(can_frame_t* dst, const d_aq_can_strain_gauge_rear_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_strain_gauge_rear_data_unpack(d_aq_can_strain_gauge_rear_data_t* dst, const can_frame_t* src);

typedef struct {
    d_aq_can_blackbody1_data_t blackbody1_data;
} d_aq_can_blackbody1_t;

typedef struct {
    d_aq_can_blackbody2_data_t blackbody2_data;
} d_aq_can_blackbody2_t;

typedef struct {
    d_aq_can_blackbody3_data_t blackbody3_data;
} d_aq_can_blackbody3_t;

typedef struct {
    d_aq_can_chassis_acceleration_data_t chassis_acceleration_data;
    d_aq_can_chassis_gyro_data_t chassis_gyro_data;
    d_aq_can_chassis_magnetometer_data_t chassis_magnetometer_data;
} d_aq_can_chassis_imu_t;

typedef struct {
    d_aq_can_timestamp_sync_t timestamp_sync;
    d_aq_can_telemetry_status_t telemetry_status;
    d_aq_can_gps_data_t gps_data;
    d_aq_can_on_board_watchdog_t on_board_watchdog;
} d_aq_can_mika_t;

typedef struct {
    d_aq_can_strain_gauge_front_data_t strain_gauge_front_data;
} d_aq_can_strain_gauge_front_t;

typedef struct {
    d_aq_can_strain_gauge_rear_data_t strain_gauge_rear_data;
} d_aq_can_strain_gauge_rear_t;

typedef struct {
    d_aq_can_suspension_fl_data_t suspension_fl_data;
} d_aq_can_suspension_fl_t;

typedef struct {
    d_aq_can_suspension_fr_data_t suspension_fr_data;
} d_aq_can_suspension_fr_t;

typedef struct {
    d_aq_can_suspension_rear_data_t suspension_rear_data;
} d_aq_can_suspension_rear_t;

typedef struct {
    d_aq_can_upright_fl_acceleration_data_t upright_fl_acceleration_data;
    d_aq_can_upright_fl_gyro_data_t upright_fl_gyro_data;
    d_aq_can_upright_fl_wheel_speed_data_t upright_fl_wheel_speed_data;
} d_aq_can_upright_fl_t;

typedef struct {
    d_aq_can_upright_fr_acceleration_data_t upright_fr_acceleration_data;
    d_aq_can_upright_fr_gyro_data_t upright_fr_gyro_data;
    d_aq_can_upright_fr_wheel_speed_data_t upright_fr_wheel_speed_data;
} d_aq_can_upright_fr_t;

typedef struct {
    d_aq_can_upright_rear_acceleration_data_t upright_rear_acceleration_data;
    d_aq_can_upright_rear_gyro_data_t upright_rear_gyro_data;
    d_aq_can_upright_rear_wheel_speed_data_t upright_rear_wheel_speed_data;
} d_aq_can_upright_rear_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t blackbody1_data;
    } blackbody1;
    struct {
        can_message_info_t blackbody2_data;
    } blackbody2;
    struct {
        can_message_info_t blackbody3_data;
    } blackbody3;
    struct {
        can_message_info_t chassis_acceleration_data;
        can_message_info_t chassis_gyro_data;
        can_message_info_t chassis_magnetometer_data;
    } chassis_imu;
    struct {
        can_message_info_t timestamp_sync;
        can_message_info_t telemetry_status;
        can_message_info_t gps_data;
        can_message_info_t on_board_watchdog;
    } mika;
    struct {
        can_message_info_t strain_gauge_front_data;
    } strain_gauge_front;
    struct {
        can_message_info_t strain_gauge_rear_data;
    } strain_gauge_rear;
    struct {
        can_message_info_t suspension_fl_data;
    } suspension_fl;
    struct {
        can_message_info_t suspension_fr_data;
    } suspension_fr;
    struct {
        can_message_info_t suspension_rear_data;
    } suspension_rear;
    struct {
        can_message_info_t upright_fl_acceleration_data;
        can_message_info_t upright_fl_gyro_data;
        can_message_info_t upright_fl_wheel_speed_data;
    } upright_fl;
    struct {
        can_message_info_t upright_fr_acceleration_data;
        can_message_info_t upright_fr_gyro_data;
        can_message_info_t upright_fr_wheel_speed_data;
    } upright_fr;
    struct {
        can_message_info_t upright_rear_acceleration_data;
        can_message_info_t upright_rear_gyro_data;
        can_message_info_t upright_rear_wheel_speed_data;
    } upright_rear;
} d_aq_can_metadata_t;
extern const d_aq_can_metadata_t d_aq_can;
