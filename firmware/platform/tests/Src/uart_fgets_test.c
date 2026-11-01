// A simple terminal application with three commands the user can pass in for different outputs; meant to illustrate
// how this could be extended to car-specific operations
#include "projdefs.h"
#include "stm32xx_hal.h"
#include "tusb_port.h"
#include "printf.h"
#include <string.h>

StaticTask_t termTaskBuffer;
StackType_t termTaskStack[configMINIMAL_STACK_SIZE];

/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}

// Reads a line from CDC port 0, blocks until enter is hit or the buffer fills
static char* cdc_fgets(char* buffer, size_t maxsz) {
    size_t i = 0;
    while (i < maxsz - 1) {
        while (!tud_cdc_n_available(0)) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        char c = (char)tud_cdc_n_read_char(0);

        // We check for \r instead of \n because that's what picocom and putty output on hitting enter key
        if (c == '\r') {
            break;
        } else if (c == '\b' || c == 0x7F) { // handle backspace and DEL
            if (i > 0)
                i--; // just move index back, will be overwritten
        } else {
            buffer[i++] = c;
        }
    }

    buffer[i] = 0;
    return buffer;
}

static bool contactor_state = true;
void TermTask(void* argument) {
    static char buf[128];

    while (1) {
        cdc_fgets(buf, 128); // blocks; waiting for either 128 characters or a \r

        printf("COMMAND: %s\n\r", buf);

        if (strcmp(buf, "hello_world") == 0) {
            printf("Hello World!");
        } else if (strcmp(buf, "contactors_get") == 0) {
            printf(contactor_state ? "CLOSED" : "OPEN");
        } else if (strcmp(buf, "contactors_toggle") == 0) {
            contactor_state = !contactor_state;
            printf(contactor_state ? "CLOSED" : "OPEN");
        }
        printf("\n\r");

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_USB_PCD_Init();
    // tusb must be up before the poll task runs, tud_task() returns immediately otherwise
    printf_init();

    // clang-format off
    xTaskCreateStatic(
        poll_events,
        "Poll Event Queue",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 3,
        pollEventsStack,
        &pollEventsTaskBuffer);
    // clang-format on

    // clang-format off
    xTaskCreateStatic(
        TermTask,
        "TermTask",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        termTaskStack,
        &termTaskBuffer);
    // clang-format on

    vTaskStartScheduler();

    while (1) {
    }
}
