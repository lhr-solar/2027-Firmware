#pragma once

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS   1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS      1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS   0

#include <stdbool.h>
#include "UART.h"
#include "tusb_port.h"

// thank you ramesh
typedef enum { L_ERROR = 0, L_WARN, L_INFO, L_DEBUG, NUM_LOGGING_LEVELS } loggingLevel_t;

// sends "[LEVEL] <message>" to a CDC port
int LOG(uint8_t port, loggingLevel_t level, const char* fmt, ...);
int snprintf(char* buffer, size_t bufsz, char const* fmt, ...);

bool log_init(void);