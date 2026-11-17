/** @file telemetry_link.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Named raw values for CellularConnected; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_TELEMETRY_STATUS_CELLULAR_CONNECTED_DISCONNECTED = 0,
    TELEMETRY_LINK_TELEMETRY_STATUS_CELLULAR_CONNECTED_CONNECTED = 1,
} telemetry_link_telemetry_status_cellular_connected_choice_t;
/** Named raw values for CellularTechnology; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_NONE = 0,
    TELEMETRY_LINK_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_LTE = 1,
    TELEMETRY_LINK_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_LTE_M = 2,
    TELEMETRY_LINK_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_NB_IO_T = 3,
    TELEMETRY_LINK_TELEMETRY_STATUS_CELLULAR_TECHNOLOGY_OTHER = 4,
} telemetry_link_telemetry_status_cellular_technology_choice_t;
/** Named raw values for Roaming; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_TELEMETRY_STATUS_ROAMING_OFF = 0,
    TELEMETRY_LINK_TELEMETRY_STATUS_ROAMING_ON = 1,
} telemetry_link_telemetry_status_roaming_choice_t;
/** Named raw values for InternetConnected; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_TELEMETRY_STATUS_INTERNET_CONNECTED_DISCONNECTED = 0,
    TELEMETRY_LINK_TELEMETRY_STATUS_INTERNET_CONNECTED_CONNECTED = 1,
} telemetry_link_telemetry_status_internet_connected_choice_t;
/** Named raw values for ServerConnected; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_TELEMETRY_STATUS_SERVER_CONNECTED_DISCONNECTED__RETRYING = 0,
    TELEMETRY_LINK_TELEMETRY_STATUS_SERVER_CONNECTED_CONNECTED = 1,
} telemetry_link_telemetry_status_server_connected_choice_t;
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
    /** ServerConnected; unit=none; physical=raw*1+0; raw=[0,1]. Connection to the telemetry server; independent of internet connectivity. */
    uint8_t server_connected;
    /** RSSI; unit=dBm; physical=raw*1+0; raw=[-128,127]. RSSI */
    int8_t rssi;
    /** RSRP; unit=dBm; physical=raw*1+0; raw=[-128,127]. RSRP */
    int8_t rsrp;
    /** RSRQ; unit=dB; physical=raw*0.5+0; raw=[-128,127]. RSRQ */
    double rsrq;
    /** SINR; unit=dB; physical=raw*0.5+0; raw=[-128,127]. SINR */
    double sinr;
} telemetry_link_telemetry_status_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t telemetry_link_telemetry_status_pack(can_frame_t* dst, const telemetry_link_telemetry_status_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t telemetry_link_telemetry_status_unpack(telemetry_link_telemetry_status_t* dst, const can_frame_t* src);

/** Named raw values for GPSAntennaState; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_GPS_DATA_GPS_ANTENNA_STATE_OK = 0,
    TELEMETRY_LINK_GPS_DATA_GPS_ANTENNA_STATE_OPEN_CIRCUIT = 1,
    TELEMETRY_LINK_GPS_DATA_GPS_ANTENNA_STATE_SHORT_CIRCUIT = 2,
    TELEMETRY_LINK_GPS_DATA_GPS_ANTENNA_STATE_UNKNOWN = 3,
} telemetry_link_gps_data_gps_antenna_state_choice_t;
/** Named raw values for GPSFix; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_GPS_DATA_GPS_FIX_NO_FIX = 0,
    TELEMETRY_LINK_GPS_DATA_GPS_FIX_FIX2_D = 1,
    TELEMETRY_LINK_GPS_DATA_GPS_FIX_FIX3_D = 2,
} telemetry_link_gps_data_gps_fix_choice_t;
/** GPS receiver state and measurements. */
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
} telemetry_link_gps_data_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t telemetry_link_gps_data_pack(can_frame_t* dst, const telemetry_link_gps_data_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t telemetry_link_gps_data_unpack(telemetry_link_gps_data_t* dst, const can_frame_t* src);

/** Named raw values for BPSWatchdog; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_BPS_WATCHDOG_ALIVE = 0,
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_BPS_WATCHDOG_TIMED_OUT = 1,
} telemetry_link_on_board_watchdog_bps_watchdog_choice_t;
/** Named raw values for VCUWatchdog; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_VCU_WATCHDOG_ALIVE = 0,
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_VCU_WATCHDOG_TIMED_OUT = 1,
} telemetry_link_on_board_watchdog_vcu_watchdog_choice_t;
/** Named raw values for DEIWatchdog; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_DEI_WATCHDOG_ALIVE = 0,
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_DEI_WATCHDOG_TIMED_OUT = 1,
} telemetry_link_on_board_watchdog_dei_watchdog_choice_t;
/** Named raw values for PedalsWatchdog; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_PEDALS_WATCHDOG_ALIVE = 0,
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_PEDALS_WATCHDOG_TIMED_OUT = 1,
} telemetry_link_on_board_watchdog_pedals_watchdog_choice_t;
/** Named raw values for PDUWatchdog; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_PDU_WATCHDOG_ALIVE = 0,
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_PDU_WATCHDOG_TIMED_OUT = 1,
} telemetry_link_on_board_watchdog_pdu_watchdog_choice_t;
/** Named raw values for LVCWatchdog; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_LVC_WATCHDOG_ALIVE = 0,
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_LVC_WATCHDOG_TIMED_OUT = 1,
} telemetry_link_on_board_watchdog_lvc_watchdog_choice_t;
/** Named raw values for MSVWatchdog; unknown values remain integers. */
typedef enum {
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_MSV_WATCHDOG_ALIVE = 0,
    TELEMETRY_LINK_ON_BOARD_WATCHDOG_MSV_WATCHDOG_TIMED_OUT = 1,
} telemetry_link_on_board_watchdog_msv_watchdog_choice_t;
/** CM5 reports watchdog state of other boards: 0 alive, 1 timed out. Former CM5 self-watchdog removed; bit 7 is reserved. */
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
    /** MSVWatchdog; unit=none; physical=raw*1+0; raw=[0,1]. MSVWatchdog */
    uint8_t msv_watchdog;
} telemetry_link_on_board_watchdog_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t telemetry_link_on_board_watchdog_pack(can_frame_t* dst, const telemetry_link_on_board_watchdog_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t telemetry_link_on_board_watchdog_unpack(telemetry_link_on_board_watchdog_t* dst, const can_frame_t* src);

/** On-demand telemetry test frame; no assumed periodic rate. */
typedef struct {
    /** Sine; unit=none; IEEE-754 binary32. */
    float sine;
    /** Cosine; unit=none; IEEE-754 binary32. */
    float cosine;
} telemetry_link_sig_wave_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t telemetry_link_sig_wave_pack(can_frame_t* dst, const telemetry_link_sig_wave_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t telemetry_link_sig_wave_unpack(telemetry_link_sig_wave_t* dst, const can_frame_t* src);

/** On-demand telemetry test frame; no assumed periodic rate. */
typedef struct {
    /** sg0; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg0;
    /** sg1; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg1;
    /** sg2; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg2;
    /** sg3; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg3;
    /** sg4; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg4;
    /** sg5; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg5;
    /** sg6; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg6;
    /** sg7; unit=unit; physical=raw*1+0; raw=[0,1]. */
    uint8_t sg7;
} telemetry_link_max_message_classical_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t telemetry_link_max_message_classical_pack(can_frame_t* dst, const telemetry_link_max_message_classical_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t telemetry_link_max_message_classical_unpack(telemetry_link_max_message_classical_t* dst, const can_frame_t* src);

/** On-demand telemetry test frame; no assumed periodic rate. */
typedef struct {
    /** sg0; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg0;
    /** sg1; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg1;
    /** sg2; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg2;
    /** sg3; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg3;
    /** sg4; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg4;
    /** sg5; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg5;
    /** sg6; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg6;
    /** sg7; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg7;
    /** sg8; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg8;
    /** sg9; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg9;
    /** sg10; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg10;
    /** sg11; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg11;
    /** sg12; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg12;
    /** sg13; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg13;
    /** sg14; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg14;
    /** sg15; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg15;
    /** sg16; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg16;
    /** sg17; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg17;
    /** sg18; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg18;
    /** sg19; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg19;
    /** sg20; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg20;
    /** sg21; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg21;
    /** sg22; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg22;
    /** sg23; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg23;
    /** sg24; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg24;
    /** sg25; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg25;
    /** sg26; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg26;
    /** sg27; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg27;
    /** sg28; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg28;
    /** sg29; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg29;
    /** sg30; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg30;
    /** sg31; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg31;
    /** sg32; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg32;
    /** sg33; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg33;
    /** sg34; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg34;
    /** sg35; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg35;
    /** sg36; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg36;
    /** sg37; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg37;
    /** sg38; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg38;
    /** sg39; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg39;
    /** sg40; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg40;
    /** sg41; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg41;
    /** sg42; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg42;
    /** sg43; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg43;
    /** sg44; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg44;
    /** sg45; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg45;
    /** sg46; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg46;
    /** sg47; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg47;
    /** sg48; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg48;
    /** sg49; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg49;
    /** sg50; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg50;
    /** sg51; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg51;
    /** sg52; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg52;
    /** sg53; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg53;
    /** sg54; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg54;
    /** sg55; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg55;
    /** sg56; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg56;
    /** sg57; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg57;
    /** sg58; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg58;
    /** sg59; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg59;
    /** sg60; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg60;
    /** sg61; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg61;
    /** sg62; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg62;
    /** sg63; unit=unit; physical=raw*1+0; raw=[0,255]. */
    uint8_t sg63;
} telemetry_link_max_message_fd_t;

/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.
 * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */
can_status_t telemetry_link_max_message_fd_pack(can_frame_t* dst, const telemetry_link_max_message_fd_t* src);
/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.
 * Negative status preserves dst; unknown value-table entries remain numeric. */
can_status_t telemetry_link_max_message_fd_unpack(telemetry_link_max_message_fd_t* dst, const can_frame_t* src);

typedef struct {
    telemetry_link_telemetry_status_t telemetry_status;
    telemetry_link_gps_data_t gps_data;
    telemetry_link_on_board_watchdog_t on_board_watchdog;
    telemetry_link_sig_wave_t sig_wave;
    telemetry_link_max_message_classical_t max_message_classical;
    telemetry_link_max_message_fd_t max_message_fd;
} telemetry_link_cm5_t;

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    struct {
        can_message_info_t telemetry_status;
        can_message_info_t gps_data;
        can_message_info_t on_board_watchdog;
        can_message_info_t sig_wave;
        can_message_info_t max_message_classical;
        can_message_info_t max_message_fd;
    } cm5;
} telemetry_link_metadata_t;
extern const telemetry_link_metadata_t telemetry_link;
