#include "log.h"
#include "UART.h"
#include "portmacro.h"
#include "stm32xx_hal.h"
#include <string.h>

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

#ifndef MAX_LOG_SIZE
    #define MAX_LOG_SIZE (512)
#endif

#ifndef NUM_LOG_BUFFERS
    #define NUM_LOG_BUFFERS (5)
#endif

// How long to wait for a log buffer to open up
#ifndef LOG_MTX_DELAY
    #define LOG_MTX_DELAY (pdMS_TO_TICKS(200))
#endif

// Max time to wait for room in the USB TX buffer, what doesn't fit after this is dropped
#ifndef LOG_TX_TIMEOUT
    #define LOG_TX_TIMEOUT (pdMS_TO_TICKS(250))
#endif

static const char* log_level_name(LoggingLevel level) {
    switch (level) {
        case L_DEBUG:
            return "DEBUG";
        case L_INFO:
            return "INFO";
        case L_WARN:
            return "WARN";
        case L_ERROR:
            return "ERROR";
    }
    /* Should be unreachable */
    return NULL;
}

typedef struct {
    char buffer[MAX_LOG_SIZE];
    SemaphoreHandle_t buffer_mtx;
    StaticSemaphore_t buffer_mtx_buffer;
} log_buffer_t;

log_buffer_t log_pool[NUM_LOG_BUFFERS];
uint8_t log_pool_next = 0;

SemaphoreHandle_t log_pool_mtx;
StaticSemaphore_t log_pool_mtx_buf;

// mutex on calling tusb write to FIFO, per CDC port
SemaphoreHandle_t cdc_port_mtx[CFG_TUD_CDC];
StaticSemaphore_t cdc_port_mtx_buf[CFG_TUD_CDC];

// semaphore given on tx complete callback, per CDC port
SemaphoreHandle_t cdc_tx_sem[CFG_TUD_CDC];
StaticSemaphore_t cdc_tx_sem_buf[CFG_TUD_CDC];

// defaults
uint16_t cdc_slots_needed[CFG_TUD_CDC] = {0};

/**
 * @brief Initializes logging over USB CDC
 * @return bool true if success
 */
bool log_init(void) {
    log_pool_mtx = xSemaphoreCreateMutexStatic(&log_pool_mtx_buf);

    for (int i = 0; i < NUM_LOG_BUFFERS; i++) {
        log_pool[i].buffer_mtx = xSemaphoreCreateMutexStatic(&log_pool[i].buffer_mtx_buffer);
    }

    for (int i = 0; i < CFG_TUD_CDC; i++) {
        cdc_port_mtx[i] = xSemaphoreCreateMutexStatic(&cdc_port_mtx_buf[i]);
        cdc_tx_sem[i] = xSemaphoreCreateBinaryStatic(&cdc_tx_sem_buf[i]);
    }

    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO,
    };
    return tusb_init(BOARD_TUD_RHPORT, &dev_init);
}

// called by TinyUSB when cdc_slots_needed fits in the TX FIFO
void cdc_space_available_cb(uint8_t itf) {
    // give sem for that specific CDC port (itf)
    if (cdc_tx_sem[itf] != NULL)
        // not an ISR
        xSemaphoreGive(cdc_tx_sem[itf]);
}

// Print to a buffer
int snprintf(char* buffer, size_t bufsz, char const* fmt, ...) {
    va_list val;
    va_start(val, fmt);
    int const rv = npf_vsnprintf(buffer, bufsz, fmt, val);
    va_end(val);
    return rv;
}

// formatted log to a CDC port
int LOG(uint8_t port, LoggingLevel level, const char* fmt, ...) {
    // exit early if port specified greater than num cdc ports
    if (port > CFG_TUD_CDC - 1 || log_pool_mtx == NULL)
        return -1;

    log_buffer_t* pbuf = pvTaskGetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0);
    if (pbuf == NULL) {
        // only if this task is not assigned a buffer, assign it a buffer
        // we must use a separate lock here for the "next pool entry" variable
        if (xSemaphoreTake(log_pool_mtx, LOG_MTX_DELAY) != pdTRUE)
            return -1;
        vTaskSetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0,
                                          (void*)(&log_pool[log_pool_next]));

        pbuf = &log_pool[log_pool_next];
        log_pool_next = (log_pool_next + 1) % NUM_LOG_BUFFERS;
        xSemaphoreGive(log_pool_mtx);
    }

    va_list val;
    va_start(val, fmt);
    // Lock on the specific buffer's mutex (if it's shared between threads)
    if (xSemaphoreTake(pbuf->buffer_mtx, LOG_MTX_DELAY) != pdTRUE) {
        va_end(val);
        return -1;
    }
    // "[LEVEL] " prefix, then the message after it
    const char* name = log_level_name(level);
    int const prefix =
        npf_snprintf(pbuf->buffer, MAX_LOG_SIZE, "[%s] ", (name != NULL) ? name : "?");
    int const body = npf_vsnprintf(pbuf->buffer + prefix, MAX_LOG_SIZE - prefix, fmt, val);
    va_end(val);
    if (body < 0) {
        xSemaphoreGive(pbuf->buffer_mtx);
        return body;
    }
    int const rv = prefix + body;

    // claim access to port hw
    if (xSemaphoreTake(cdc_port_mtx[port], LOG_MTX_DELAY) != pdTRUE) {
        xSemaphoreGive(pbuf->buffer_mtx);
        return -1;
    }

    uint8_t* buf = (uint8_t*)pbuf->buffer;
    // "-1" for null char space
    uint32_t count = (rv >= MAX_LOG_SIZE) ? MAX_LOG_SIZE - 1 : rv;

    // cdc_n_connected checks if a DTR bit was sent
    // happens for any serial terminals that claim a COM port
    if (tud_cdc_n_connected(port)) {
        // clear any leftover semphr give if happened after previous calls timeout
        xSemaphoreTake(cdc_tx_sem[port], 0);
        cdc_slots_needed[port] = count;
        if (tud_cdc_n_write_available(port) < count) {
            // pend on slots available
            xSemaphoreTake(cdc_tx_sem[port], LOG_TX_TIMEOUT);
            // space is now available on that CDC's Tx FIFO
        }
        // write buff into tx fifo
        tud_cdc_n_write(port, buf, count);
        // force fifo -> endpoint xfer if possible
        tud_cdc_n_write_flush(port);
    }

    // reset so semaphore doesn't give when not needed
    cdc_slots_needed[port] = 0;

    xSemaphoreGive(pbuf->buffer_mtx);
    xSemaphoreGive(cdc_port_mtx[port]);
    return rv;
}