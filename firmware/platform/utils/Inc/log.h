/**
 * @file log.h
 * @brief Leveled, colored, rate-limitable logging on top of the platform printf.
 *
 * @details
 *   #include "printf.h"   // platform printf, must come first
 *   #include "log.h"
 *
 * Usage:
 *   LOG_DEBUG / LOG_INFO / LOG_ERROR(fmt, ...)       cyan / green / red, one line per call.
 *   LOG_*_EVERY_N(n, fmt, ...)    prints on the 1st call, then every nth call, per call site.
 *   LOG_*_EVERY_MS(ms, fmt, ...)  prints on the 1st call, then at most once per ms milliseconds,
 *                                 per call site.
 *   Output format: "[<rtos tick>] LEVEL message\r\n".
 *
 * Notes:
 *  - NOT ISR-safe, including the rate-limited variants: they take mutexes and can block.
 *    Defer to a task (flag, queue, xTaskNotifyFromISR) and log from there.
 *  - fmt must be a string literal.
 *  - Raw printf( is a compile error in any file that includes this header.
 *  - Set the floor with -DLOG_LEVEL=LOG_LEVEL_INFO or #define LOG_LEVEL LOG_LEVEL_ERROR; lower levels compile to nothing.
 */

#pragma once

#include <stdint.h>

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_NONE  3

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#ifndef LOG_USE_COLOR
#define LOG_USE_COLOR 1
#endif

#if LOG_USE_COLOR
#define LOG_CLR_DEBUG "\x1b[36m" // cyan
#define LOG_CLR_INFO  "\x1b[32m" // green
#define LOG_CLR_ERROR "\x1b[31m" // red
#define LOG_CLR_RESET "\x1b[0m"
#else
#define LOG_CLR_DEBUG ""
#define LOG_CLR_INFO  ""
#define LOG_CLR_ERROR ""
#define LOG_CLR_RESET ""
#endif

#ifndef LOG_TIMESTAMP
#define LOG_TIMESTAMP() ((unsigned long)xTaskGetTickCount())
#endif

// (printf) in parens suppresses the function-like ban macro defined at the bottom.
#define LOG_WRITE(clr, tag, fmt, ...) \
    ((printf)(clr "[%lu] " tag " " fmt LOG_CLR_RESET "\r\n", LOG_TIMESTAMP(), ##__VA_ARGS__))

#define LOG_NOP(fmt, ...) ((void)0)

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) LOG_WRITE(LOG_CLR_DEBUG, "DEBUG", fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) LOG_NOP(fmt, ##__VA_ARGS__)
#endif
#if LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) LOG_WRITE(LOG_CLR_INFO, "INFO ", fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) LOG_NOP(fmt, ##__VA_ARGS__)
#endif
#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...) LOG_WRITE(LOG_CLR_ERROR, "ERROR", fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) LOG_NOP(fmt, ##__VA_ARGS__)
#endif

/**
 * Rate limiting: emits on the 1st call and every Nth call after, per call site
 * (each macro use gets its own static counter). The counter is not atomic; two
 * tasks racing on one call site can skew the cadence by one, never corrupt output.
 */
#define LOG_EVERY_N_IMPL(LOGFN, n, fmt, ...)                          \
    do {                                                              \
        static uint32_t prv_log_cnt = 0;                              \
        if (prv_log_cnt == 0) LOGFN(fmt, ##__VA_ARGS__);              \
        if (++prv_log_cnt >= (uint32_t)(n)) prv_log_cnt = 0;          \
    } while (0)

#define LOG_DEBUG_EVERY_N(n, fmt, ...) LOG_EVERY_N_IMPL(LOG_DEBUG, n, fmt, ##__VA_ARGS__)
#define LOG_INFO_EVERY_N(n, fmt, ...)  LOG_EVERY_N_IMPL(LOG_INFO, n, fmt, ##__VA_ARGS__)
#define LOG_ERROR_EVERY_N(n, fmt, ...) LOG_EVERY_N_IMPL(LOG_ERROR, n, fmt, ##__VA_ARGS__)
#define LOG_EVERY_N(n, fmt, ...)       LOG_INFO_EVERY_N(n, fmt, ##__VA_ARGS__)

// ---- enforcement: raw printf( is a hard error in files that include this header ----
#ifndef LOG_ALLOW_RAW_PRINTF
void log_raw_printf_is_banned(void) __attribute__((error("raw printf is banned: use LOG_DEBUG/LOG_INFO/LOG_ERROR")));
#define printf(...) (log_raw_printf_is_banned(), 0)
#endif