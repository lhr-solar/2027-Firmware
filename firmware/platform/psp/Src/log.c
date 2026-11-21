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
#ifndef LOG_MTX_DELAY_TICKS
    #define LOG_MTX_DELAY_TICKS (pdMS_TO_TICKS(200))
#endif

// Max time to wait for room in the USB TX buffer, what doesn't fit after this is dropped
#ifndef LOG_TX_TIMEOUT_TICKS
    #define LOG_TX_TIMEOUT_TICKS (pdMS_TO_TICKS(250))
#endif

static const char* log_level_name(loggingLevel_t level) {
    switch (level) {
        case L_DEBUG:
            return "DEBUG";
        case L_INFO:
            return "INFO";
        case L_WARN:
            return "WARN";
        case L_ERROR:
            return "ERROR";
        default:
            return NULL;
    }
    /* Should be unreachable */
    return NULL;
}

typedef struct {
    char buffer[MAX_LOG_SIZE];
    SemaphoreHandle_t buffer_mtx;
    StaticSemaphore_t buffer_mtx_buffer;
} log_buffer_t;

typedef struct {
    // pool of buffers for tasks logging to this port, one handed out per task
    log_buffer_t cdc_pool[NUM_LOG_BUFFERS];
    uint8_t cdc_pool_next;
    SemaphoreHandle_t cdc_pool_mtx;
    StaticSemaphore_t cdc_pool_mtx_buf;
    // mutex on calling tusb write to FIFO, per CDC port
    SemaphoreHandle_t cdc_port_mtx;
    StaticSemaphore_t cdc_port_mtx_buf;
    // semaphore given on tx complete callback, per CDC port
    SemaphoreHandle_t cdc_tx_sem;
    StaticSemaphore_t cdc_tx_sem_buf;
} cdc_port_t;

cdc_port_t cdc_ports[NUM_CDC_INSTANCES];

// defaults
uint16_t cdc_slots_needed[NUM_CDC_INSTANCES] = {0};

// signal completed init 
static bool log_init_done = false;

/**
 * @brief Initializes logging over USB CDC
 * @return bool true if success
 */
bool log_init(void) {
    if (log_init_done) return false;

    for (int i = 0; i < NUM_CDC_INSTANCES; i++) {
        cdc_port_t* p = &cdc_ports[i];
        for (int j = 0; j < NUM_LOG_BUFFERS; j++) {
            p->cdc_pool[j].buffer_mtx =
                xSemaphoreCreateMutexStatic(&p->cdc_pool[j].buffer_mtx_buffer);
        }
        p->cdc_pool_mtx = xSemaphoreCreateMutexStatic(&p->cdc_pool_mtx_buf);
        p->cdc_port_mtx = xSemaphoreCreateMutexStatic(&p->cdc_port_mtx_buf);
        p->cdc_tx_sem = xSemaphoreCreateBinaryStatic(&p->cdc_tx_sem_buf);
    }

    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO,
    };
    log_init_done = tusb_init(BOARD_TUD_RHPORT, &dev_init);

    return log_init_done;
}

// called by TinyUSB when cdc_slots_needed fits in the TX FIFO
void cdc_space_available_cb(uint8_t itf) {
    if (cdc_slots_needed[itf] > 0 && tud_cdc_n_write_available(itf) >= cdc_slots_needed[itf]) {
        // give sem for that specific CDC port (itf)
        if (cdc_ports[itf].cdc_tx_sem != NULL) {
            // not an ISR
            xSemaphoreGive(cdc_ports[itf].cdc_tx_sem);
            // reset so semaphore doesn't give when not needed
            cdc_slots_needed[itf] = 0;
        }
    }
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
int LOG(uint8_t port, loggingLevel_t level, const char* fmt, ...) {
    // exit early if no init, cdc unconnected, or invalid port
    if (!log_init_done || port > NUM_CDC_INSTANCES - 1 || !tud_cdc_n_connected(port))
        return -1;

    // "[LEVEL] " prefix, then the message after it
    const char* name = log_level_name(level);
    // if invalid log level passed, checked before taking any lock
    if (name == NULL)
        return -1;

    cdc_port_t* cdc = &cdc_ports[port];

    log_buffer_t* pbuf = pvTaskGetThreadLocalStoragePointer(NULL, port);
    if (pbuf == NULL) {
        // only if this task has no buffer for this port yet, assign it one from the port's pool
        // we must use a separate lock here for the "next pool entry" variable
        if (xSemaphoreTake(cdc->cdc_pool_mtx, LOG_MTX_DELAY_TICKS) != pdTRUE)
            return -1;
        pbuf = &cdc->cdc_pool[cdc->cdc_pool_next];
        vTaskSetThreadLocalStoragePointer(NULL, port, (void*)pbuf);
        cdc->cdc_pool_next = (cdc->cdc_pool_next + 1) % NUM_LOG_BUFFERS;
        xSemaphoreGive(cdc->cdc_pool_mtx);
    }

    va_list val;
    va_start(val, fmt);
    // Lock on the specific buffer's mutex (if it's shared between threads)
    if (xSemaphoreTake(pbuf->buffer_mtx, LOG_MTX_DELAY_TICKS) != pdTRUE) {
        va_end(val);
        return -1;
    }

    int const prefix = npf_snprintf(pbuf->buffer, MAX_LOG_SIZE, "[%s] ", name);
    int const body = npf_vsnprintf(pbuf->buffer + prefix, MAX_LOG_SIZE - prefix, fmt, val);
    va_end(val);
    if (body < 0) {
        xSemaphoreGive(pbuf->buffer_mtx);
        return body;
    }
    int const rv = prefix + body;

    // claim access to port hw
    if (xSemaphoreTake(cdc->cdc_port_mtx, LOG_MTX_DELAY_TICKS) != pdTRUE) {
        xSemaphoreGive(pbuf->buffer_mtx);
        return -1;
    }

    uint8_t* buf = (uint8_t*)pbuf->buffer;
    // "-1" for null char space
    uint32_t count = (rv >= MAX_LOG_SIZE) ? MAX_LOG_SIZE - 1 : rv;

    int ret  = rv; // return val

    // cdc_n_connected checks if a DTR bit was sent
    // happens for any serial terminals that claim a COM port
    if (tud_cdc_n_connected(port)) {
        // clear any leftover semphr give if happened after previous calls timeout
        xSemaphoreTake(cdc->cdc_tx_sem, 0);
        cdc_slots_needed[port] = count;
        BaseType_t fifo_space_available = pdTRUE;
        if (tud_cdc_n_write_available(port) < count) {
            // pend on slots available
            fifo_space_available = xSemaphoreTake(cdc->cdc_tx_sem, LOG_TX_TIMEOUT_TICKS);
            // space is now available on that CDC's Tx FIFO
        }

        if (fifo_space_available) {
            // write buff into tx fifo
            tud_cdc_n_write(port, buf, count);
            // force fifo -> endpoint xfer if possible
            tud_cdc_n_write_flush(port);
        } else {
            // cannot write, semphr timed out
            ret = -1
        }
    } else {
        // cdc port unconnected
        ret = -1;
    }

    xSemaphoreGive(pbuf->buffer_mtx);
    xSemaphoreGive(cdc->cdc_port_mtx);
    return ret;
}