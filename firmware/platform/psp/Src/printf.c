#include "printf.h"
#include "UART.h"
#include "portmacro.h"
#include "stm32xx_hal.h"
#include "tusb_port.h"
#include <string.h>

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

#ifndef MAX_PRINTF_SIZE
    #define MAX_PRINTF_SIZE (256)
#endif

#ifndef NUM_PRINTF_BUFFERS
    #define NUM_PRINTF_BUFFERS (5)
#endif

// How long to wait for a printf buffer to open up
#ifndef PRINTF_MTX_DELAY
    #define PRINTF_MTX_DELAY (pdMS_TO_TICKS(100))
#endif

// Max time to wait for room in the USB TX buffer, what doesn't fit after this is dropped
#ifndef PRINTF_TX_TIMEOUT
    #define PRINTF_TX_TIMEOUT (pdMS_TO_TICKS(2))
#endif

typedef struct {
    char buffer[MAX_PRINTF_SIZE];
    SemaphoreHandle_t buffer_mtx;
    StaticSemaphore_t buffer_mtx_buffer;
} printf_buffer_t;

printf_buffer_t printf_pool[NUM_PRINTF_BUFFERS];
uint8_t printf_pool_next = 0;

SemaphoreHandle_t printf_pool_mtx;
StaticSemaphore_t printf_pool_mtx_buf;

/**
 * @brief Initializes printf
 * @param huart pointer to the UART handle
 * @return bool true if success
 */
bool printf_init(void) {
    printf_pool_mtx = xSemaphoreCreateMutexStatic(&printf_pool_mtx_buf);

    for (int i = 0; i < NUM_PRINTF_BUFFERS; i++) {
        printf_pool[i].buffer_mtx = xSemaphoreCreateMutexStatic(&printf_pool[i].buffer_mtx_buffer);
    }

    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO,
    };
    return tusb_init(BOARD_TUD_RHPORT, &dev_init);
}

// Print to a buffer
int snprintf(char* buffer, size_t bufsz, char const* fmt, ...) {
    va_list val;
    va_start(val, fmt);
    int const rv = npf_vsnprintf(buffer, bufsz, fmt, val);
    va_end(val);
    return rv;
}

// printf but on a port :P
int printf_port(uint8_t port, const char* fmt, ...) {
    // exit early if port specified greater than num cdc ports
    if (port > CFG_TUD_CDC - 1) return -1;

    printf_buffer_t* pbuf = pvTaskGetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0);
    if (pbuf == NULL) {
        // only if this task is not assigned a buffer, assign it a buffer
        // we must use a separate lock here for the "next pool entry" variable
        if (xSemaphoreTake(printf_pool_mtx, portMAX_DELAY) != pdTRUE)
            return -1;
        vTaskSetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0,
                                          (void*)(&printf_pool[printf_pool_next]));

        pbuf = &printf_pool[printf_pool_next];
        printf_pool_next = (printf_pool_next + 1) % NUM_PRINTF_BUFFERS;
        xSemaphoreGive(printf_pool_mtx);
    }

    va_list val;
    va_start(val, fmt);
    // Lock on the specific buffer's mutex (if it's shared between threads)
    if (xSemaphoreTake(pbuf->buffer_mtx, PRINTF_MTX_DELAY) != pdTRUE) {
        va_end(val);
        return -1;
    }

    int const rv = npf_vsnprintf(pbuf->buffer, MAX_PRINTF_SIZE, fmt, val);
    va_end(val);
    if (rv <= 0) {
        xSemaphoreGive(pbuf->buffer_mtx);
        return rv;
    }

    uint8_t* buf = (uint8_t*)pbuf->buffer;
    // "-1" for null char space
    uint32_t count = (rv >= MAX_PRINTF_SIZE) ? MAX_PRINTF_SIZE - 1 : rv;
    TickType_t start = xTaskGetTickCount();

    // cdc_n_connected checks if a DTR bit was sent
    // happens for any serial terminals that claim a COM port
    if (tud_cdc_n_connected(port)) {
        while (tud_cdc_n_write_available(port) < count &&
                xTaskGetTickCount() - start < PRINTF_TX_TIMEOUT) {
            vTaskDelay(1);
        }
        // write buff into tx fifo
        tud_cdc_n_write(port, buf, count);
        // force fifo -> endpoint xfer is possible
        tud_cdc_n_write_flush(port);
    }
    
    xSemaphoreGive(pbuf->buffer_mtx);
    return rv;
}