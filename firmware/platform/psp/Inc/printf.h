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

// delete later if we only want logging funcs
#define LOG(port, fmt, ...) printf_port(port, fmt, ##__VA_ARGS__)
#define log_init()          printf_init()

int printf_port(uint8_t port, const char* fmt, ...);
int snprintf(char* buffer, size_t bufsz, char const* fmt, ...);
// char* uart_fgets(char* buffer, size_t maxsz);

bool printf_init(void);