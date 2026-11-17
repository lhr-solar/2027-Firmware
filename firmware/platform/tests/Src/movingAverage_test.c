#include "movingAverage.h"
#include "stm32xx_hal.h"
#include "tusb_port.h"
#include "printf.h"

StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}

// printf drops what doesn't fit in the USB TX buffer, give it time to drain between lines
#define PRINT_DRAIN_DELAY pdMS_TO_TICKS(5)

DEFINE_MOVING_AVG(MA_u16, uint16_t, uint32_t, 10)
DEFINE_MOVING_AVG(MA_u32, uint32_t, uint64_t, 10)
DEFINE_MOVING_AVG(MA_s32, int32_t, int64_t, 10)

void test_u16() {
    printf("\n--- uint16_t test ---\n\r");

    MA_u16 ma;
    MA_u16_init(&ma);

    uint16_t inputs[] = {2, 4, 6, 8, 10, 12};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        uint16_t avg = MA_u16_update(&ma, inputs[i]);
        printf("input=%u -> avg=%u\n\r", inputs[i], avg);
        vTaskDelay(PRINT_DRAIN_DELAY);
    }
}

void test_u32() {
    printf("\n--- uint32_t test ---\n\r");

    MA_u32 ma;
    MA_u32_init(&ma);

    uint32_t inputs[] = {100, 200, 300, 400, 500, 600};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        uint32_t avg = MA_u32_update(&ma, inputs[i]);
        printf("input=%lu -> avg=%lu\n\r", inputs[i], avg);
        vTaskDelay(PRINT_DRAIN_DELAY);
    }
}

void test_s32() {
    printf("\n--- int32_t test ---\n\r");

    MA_s32 ma;
    MA_s32_init(&ma);

    int32_t inputs[] = {-2, -4, 6, 8, 10, -10};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        int32_t avg = MA_s32_update(&ma, inputs[i]);
        printf("input=%ld -> avg=%ld\n\r", inputs[i], avg);
        vTaskDelay(PRINT_DRAIN_DELAY);
    }
}

void TxTask(void* argument) {
    // output is dropped until a terminal opens the port
    while (!tud_cdc_n_connected(0)) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    printf("Running moving average tests...\n\r");
    vTaskDelay(PRINT_DRAIN_DELAY);

    test_u16();
    test_u32();
    test_s32();

    while (1) {
    }
}


int main() {

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
        TxTask,
        "TX",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        txTaskStack,
        &txTaskBuffer);
    // clang-format on

    vTaskStartScheduler();


    while (1) {
    }


    return 0;
}
