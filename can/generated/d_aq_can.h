/** @file d_aq_can.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Named raw values for PreviousTimestampValid; unknown values remain integers. */
typedef enum {
    D_AQ_CAN_TIMESTAMP_SYNC_PREVIOUS_TIMESTAMP_VALID_INVALID = 0,
    D_AQ_CAN_TIMESTAMP_SYNC_PREVIOUS_TIMESTAMP_VALID_VALID = 1,
} d_aq_can_timestamp_sync_previous_timestamp_valid_choice_t;
/** Two-step 100 Hz sync. CM5 sends the actual hardware transmit SOF timestamp of a previous sync. Nodes match PreviousSequence to a hardware receive SOF; timestamps use 0.1 ms ticks modulo 65536. A DBC does not implement the synchronization. */
typedef struct {
    /** Sequence; unit=none; physical=raw*1+0; raw=[0,255]. Sequence number of this sync frame; wraps modulo 256. */
    uint8_t sequence;
    /** PreviousSequence; unit=none; physical=raw*1+0; raw=[0,255]. Sequence identifying the frame whose transmit SOF is reported. */
    uint8_t previous_sequence;
    /** PreviousTimestampValid; unit=none; physical=raw*1+0; raw=[0,1]. Valid only when the preceding transmit SOF timestamp was captured; 0 at startup or capture failure. */
    uint8_t previous_timestamp_valid;
    /** PreviousTxTimestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Actual leader transmit SOF time of PreviousSequence in 0.1 ms ticks; wraps every 6553.6 ms. Ignore when PreviousTimestampValid is invalid. */
    double previous_tx_timestamp;
} d_aq_can_timestamp_sync_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_timestamp_sync_pack(can_frame_t* dst, const d_aq_can_timestamp_sync_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_timestamp_sync_unpack(d_aq_can_timestamp_sync_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 208 Hz; one frame per four samples is 52 Hz (19.2308 ms). DBC 19 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-4096,4095]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
} d_aq_can_chassis_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_chassis_acceleration_data_pack(can_frame_t* dst, const d_aq_can_chassis_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_chassis_acceleration_data_unpack(d_aq_can_chassis_acceleration_data_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 208 Hz; one frame per four samples is 52 Hz (19.2308 ms). DBC 19 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
} d_aq_can_chassis_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_chassis_gyro_data_pack(can_frame_t* dst, const d_aq_can_chassis_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_chassis_gyro_data_unpack(d_aq_can_chassis_gyro_data_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 416 Hz; one frame per four samples is 104 Hz (9.6154 ms). DBC 10 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
} d_aq_can_upright_fr_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fr_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fr_acceleration_data_unpack(d_aq_can_upright_fr_acceleration_data_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 416 Hz; one frame per four samples is 104 Hz (9.6154 ms). DBC 10 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
} d_aq_can_upright_fr_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fr_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fr_gyro_data_unpack(d_aq_can_upright_fr_gyro_data_t* dst, const can_frame_t* src);

/** Five chronological wheel-speed samples per message at a 100 Hz sample rate (20 Hz message rate). Timestamp identifies the first sample. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** WheelRPM1; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 1 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm1;
    /** WheelRPM2; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 2 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm2;
    /** WheelRPM3; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 3 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm3;
    /** WheelRPM4; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 4 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm4;
    /** WheelRPM5; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 5 of WheelRPM; 100 Hz sampling, five samples per message. */
    double wheel_rpm5;
} d_aq_can_upright_fr_wheel_speed_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fr_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_fr_wheel_speed_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fr_wheel_speed_data_unpack(d_aq_can_upright_fr_wheel_speed_data_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 416 Hz; one frame per four samples is 104 Hz (9.6154 ms). DBC 10 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
} d_aq_can_upright_fl_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fl_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fl_acceleration_data_unpack(d_aq_can_upright_fl_acceleration_data_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 416 Hz; one frame per four samples is 104 Hz (9.6154 ms). DBC 10 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
} d_aq_can_upright_fl_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fl_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fl_gyro_data_unpack(d_aq_can_upright_fl_gyro_data_t* dst, const can_frame_t* src);

/** Five chronological wheel-speed samples per message at a 100 Hz sample rate (20 Hz message rate). Timestamp identifies the first sample. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** WheelRPM1; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 1 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm1;
    /** WheelRPM2; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 2 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm2;
    /** WheelRPM3; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 3 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm3;
    /** WheelRPM4; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 4 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm4;
    /** WheelRPM5; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 5 of WheelRPM; 100 Hz sampling, five samples per message. */
    double wheel_rpm5;
} d_aq_can_upright_fl_wheel_speed_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_fl_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_fl_wheel_speed_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_fl_wheel_speed_data_unpack(d_aq_can_upright_fl_wheel_speed_data_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 416 Hz; one frame per four samples is 104 Hz (9.6154 ms). DBC 10 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** AccelerationX1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x1;
    /** AccelerationX2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x2;
    /** AccelerationX3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x3;
    /** AccelerationX4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationX; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_x4;
    /** AccelerationY1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y1;
    /** AccelerationY2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y2;
    /** AccelerationY3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y3;
    /** AccelerationY4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationY; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_y4;
    /** AccelerationZ1; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 1 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z1;
    /** AccelerationZ2; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 2 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z2;
    /** AccelerationZ3; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 3 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z3;
    /** AccelerationZ4; unit=g; physical=raw*0.001+0; raw=[-16384,16383]. Chronological sample 4 of AccelerationZ; indices 1..4 align across axes within this batch. Physical unit g. */
    double acceleration_z4;
} d_aq_can_upright_rear_acceleration_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_rear_acceleration_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_acceleration_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_rear_acceleration_data_unpack(d_aq_can_upright_rear_acceleration_data_t* dst, const can_frame_t* src);

/** Four consecutive samples per axis at 416 Hz; one frame per four samples is 104 Hz (9.6154 ms). DBC 10 ms is a nominal integer-millisecond estimate. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** GyroX1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x1;
    /** GyroX2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x2;
    /** GyroX3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x3;
    /** GyroX4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroX; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_x4;
    /** GyroY1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y1;
    /** GyroY2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y2;
    /** GyroY3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y3;
    /** GyroY4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroY; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_y4;
    /** GyroZ1; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 1 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z1;
    /** GyroZ2; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 2 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z2;
    /** GyroZ3; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 3 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z3;
    /** GyroZ4; unit=deg/s; physical=raw*0.02+0; raw=[-32768,32767]. Chronological sample 4 of GyroZ; indices 1..4 align across axes within this batch. Physical unit deg/s. */
    double gyro_z4;
} d_aq_can_upright_rear_gyro_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_rear_gyro_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_gyro_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_rear_gyro_data_unpack(d_aq_can_upright_rear_gyro_data_t* dst, const can_frame_t* src);

/** Five chronological wheel-speed samples per message at a 100 Hz sample rate (20 Hz message rate). Timestamp identifies the first sample. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** WheelRPM1; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 1 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm1;
    /** WheelRPM2; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 2 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm2;
    /** WheelRPM3; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 3 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm3;
    /** WheelRPM4; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 4 of WheelRPM; indices 1..4 align across axes within this batch. Physical unit rpm. */
    double wheel_rpm4;
    /** WheelRPM5; unit=rpm; physical=raw*0.5+0; raw=[0,4095]. Chronological sample 5 of WheelRPM; 100 Hz sampling, five samples per message. */
    double wheel_rpm5;
} d_aq_can_upright_rear_wheel_speed_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_upright_rear_wheel_speed_data_pack(can_frame_t* dst, const d_aq_can_upright_rear_wheel_speed_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_upright_rear_wheel_speed_data_unpack(d_aq_can_upright_rear_wheel_speed_data_t* dst, const can_frame_t* src);

/** One linpot sampled at 500 Hz; five consecutive samples per frame at 100 Hz. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. Legacy 12-bit ADC calibration retained. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** LinpotPosition1; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 1 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position1;
    /** LinpotPosition2; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 2 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position2;
    /** LinpotPosition3; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 3 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position3;
    /** LinpotPosition4; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 4 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position4;
    /** LinpotPosition5; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 5 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position5;
} d_aq_can_suspension_fr_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_suspension_fr_data_pack(can_frame_t* dst, const d_aq_can_suspension_fr_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_suspension_fr_data_unpack(d_aq_can_suspension_fr_data_t* dst, const can_frame_t* src);

/** One linpot sampled at 500 Hz; five consecutive samples per frame at 100 Hz. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. Legacy 12-bit ADC calibration retained. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** LinpotPosition1; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 1 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position1;
    /** LinpotPosition2; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 2 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position2;
    /** LinpotPosition3; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 3 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position3;
    /** LinpotPosition4; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 4 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position4;
    /** LinpotPosition5; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 5 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position5;
} d_aq_can_suspension_fl_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t d_aq_can_suspension_fl_data_pack(can_frame_t* dst, const d_aq_can_suspension_fl_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t d_aq_can_suspension_fl_data_unpack(d_aq_can_suspension_fl_data_t* dst, const can_frame_t* src);

/** One linpot sampled at 500 Hz; five consecutive samples per frame at 100 Hz. Timestamp identifies sample 1; timestamp uses 0.1 ms ticks modulo 65536. Legacy 12-bit ADC calibration retained. */
typedef struct {
    /** Timestamp; unit=ms; physical=raw*0.1+0; raw=[0,65535]. Corrected CM5 time of sample 1, in 0.1 ms ticks modulo 65536; 0 is a valid timestamp. Validity and stale-sync handling are firmware responsibilities. */
    double timestamp;
    /** LinpotPosition1; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 1 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position1;
    /** LinpotPosition2; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 2 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position2;
    /** LinpotPosition3; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 3 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position3;
    /** LinpotPosition4; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 4 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position4;
    /** LinpotPosition5; unit=mm; physical=raw*0.0188885+1; raw=[0,4095]. Chronological sample 5 from the board linpot; legacy 12-bit ADC scale and offset. */
    double linpot_position5;
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
    d_aq_can_timestamp_sync_t timestamp_sync;
} d_aq_can_cm5_t;

typedef struct {
    d_aq_can_chassis_acceleration_data_t chassis_acceleration_data;
    d_aq_can_chassis_gyro_data_t chassis_gyro_data;
} d_aq_can_chassis_imu_t;

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
        can_message_info_t timestamp_sync;
    } cm5;
    struct {
        can_message_info_t chassis_acceleration_data;
        can_message_info_t chassis_gyro_data;
    } chassis_imu;
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
