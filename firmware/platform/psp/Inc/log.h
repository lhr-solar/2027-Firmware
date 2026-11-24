#pragma once

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS   1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS      1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS   0

#include <stdbool.h>
#include <stdint.h>
#include "UART.h"
#include "tusb_port.h"

// thank you ramesh
typedef enum { L_ERROR = 0, L_WARN, L_INFO, L_DEBUG, NUM_LOGGING_LEVELS } loggingLevel_t;

// mask bit for a level, OR together
#define LOG_BIT(l) (1u << (l))

// sends "[LEVEL] <message>" to a CDC port
int LOG(uint8_t port, loggingLevel_t level, const char* fmt, ...);
int snprintf(char* buffer, size_t bufsz, char const* fmt, ...);

typedef struct {
    // LOG_BIT(L_x) | ... -> only those levels print, others return -1. 0 -> nothing prints.
    uint8_t levels;
    // USB product string and CDC port string (for udev rules ltr).
    const char* board_name;
    const char* port_names[NUM_CDC_INSTANCES];
} log_init_t;

// Call once from main before the scheduler starts. NULL config -> nothing prints, default names.
bool log_init(const log_init_t* config);