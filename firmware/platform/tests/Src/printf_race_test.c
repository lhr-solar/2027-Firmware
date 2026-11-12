#include "stm32xx_hal.h"
#include "tusb_port.h"
#include "printf.h"
#include <string.h>

// Two same-priority tasks on one port: S sends 20 B lines often, L sends 500 B lines.
// L needs the 512 B FIFO almost empty, so it's the one at risk of being starved/cut.
// Host side: every line must be its full length and end in |END, counters gap-free.
// Needs MAX_PRINTF_SIZE >= 501.

#define TEST_PORT 0

#define TX_PRIO   (tskIDLE_PRIORITY + 2)
#define TX_STACK  (configMINIMAL_STACK_SIZE * 2)

// "S#000000: " + payload + "|END\r\n"
#define LINE_FMT "%c#%06u: %s|END\r\n"
#define HDR_LEN  10
#define TAIL_LEN 6

#define SMALL_ID        'S'
#define SMALL_LEN       20
#define SMALL_FILL      (SMALL_LEN - HDR_LEN - TAIL_LEN)
#define SMALL_PERIOD_MS 1

#define LARGE_ID        'L'
#define LARGE_LEN       500
#define LARGE_FILL      (LARGE_LEN - HDR_LEN - TAIL_LEN)
#define LARGE_PERIOD_MS 10

static char small_payload[SMALL_FILL + 1];
static char large_payload[LARGE_FILL + 1];

StaticTask_t smallTaskBuffer;
StackType_t smallTaskStack[TX_STACK];
StaticTask_t largeTaskBuffer;
StackType_t largeTaskStack[TX_STACK];
/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}


void SmallPayloadTask(void* argument) {
    // fill payload with buncha S's
    memset(small_payload, 'S', SMALL_FILL);
    uint32_t counter = 0;
    while (1) {
        LOG(TEST_PORT, LINE_FMT, SMALL_ID, (unsigned)counter++, small_payload);
        vTaskDelay(pdMS_TO_TICKS(SMALL_PERIOD_MS));
    }
}

void LargePayloadTask(void* argument) {
    // fill payload with buncha L's
    memset(large_payload, 'L', LARGE_FILL);
    uint32_t counter = 0;
    while (1) {
        LOG(TEST_PORT, LINE_FMT, LARGE_ID, (unsigned)counter++, large_payload);
        vTaskDelay(pdMS_TO_TICKS(LARGE_PERIOD_MS));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_USB_PCD_Init();
    // tusb must be up before the poll task runs, tud_task() returns immediately otherwise
    log_init();

    xTaskCreateStatic(poll_events, "Poll Event Queue", configMINIMAL_STACK_SIZE, NULL,
                      tskIDLE_PRIORITY + 3, pollEventsStack, &pollEventsTaskBuffer);

    xTaskCreateStatic(SmallPayloadTask, "TX_S", TX_STACK, NULL, TX_PRIO, smallTaskStack,
                      &smallTaskBuffer);
    xTaskCreateStatic(LargePayloadTask, "TX_L", TX_STACK, NULL, TX_PRIO, largeTaskStack,
                      &largeTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
