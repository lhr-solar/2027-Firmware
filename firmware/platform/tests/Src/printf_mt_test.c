// A simple echo application to test input and output over serial
#include <string.h>
#include <math.h>
#include "stm32xx_hal.h"
#include "tusb_port.h"
#include "printf.h"

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];
/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

static void poll_events(void* argument) {
    while (1) {
        // blocks until there is a USB event
        tud_task();
    }
}

#define THREAD_COUNT 4
#define DUMP_SIZE    100

// static char *messages[] = {
//     "This is a message I expect to never interleave!\n\r",
//     "The quick brown fox jumped over the lazy dog!\n\r",
//     "5.5\n\r",
//     "3.777\n\r",
//     "I'm going to write a really long message to test long transmission lengths to confirm that interleaving is not a problem with these sorts of messages.\n\r"
// };

// If you wanna test with messages that all take the same base amount of time
static char* messages[] = {
    "This is a message I expect to never interleave!\n\r",
    "This is a message I expect to never interleave!\n\r",
    "This is a message I expect to never interleave!\n\r",
    "This is a message I expect to never interleave!\n\r",
    "This is a message I expect to never interleave!\n\r",
};

StaticTask_t txTaskBuffer[THREAD_COUNT];
StackType_t txTaskStack[THREAD_COUNT][configMINIMAL_STACK_SIZE];

static uint32_t time_dump[THREAD_COUNT][DUMP_SIZE] = {0};
static bool tx_done[THREAD_COUNT] = {};

void TxTask(void* argument) {
    uint8_t time_ind = 0;

    for (int i = 0; i < DUMP_SIZE; i++) {
        volatile uint32_t time_before = DWT->CYCCNT;
        printf(messages[((int)argument % (sizeof(messages) / sizeof(char*)))]);
        //uart_send(husart2, (const uint8_t *)messages[((int)argument % (sizeof(messages)/sizeof(char*)))], strlen(messages[((int)argument % (sizeof(messages)/sizeof(char*)))]), portMAX_DELAY);
        volatile uint32_t time_after = DWT->CYCCNT;
        uint32_t time_elapsed = (time_after - time_before);
        time_dump[(int)argument][time_ind++] = time_elapsed;

        time_ind = time_ind % DUMP_SIZE;

        portYIELD();
    }

    tx_done[(int)argument] = true;
    while (1) {
        vTaskDelay(1000);
    }
}

void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0UL;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void InitTask(void* argument) {
    // output is dropped until a terminal opens the port, so wait before timing
    while (!tud_cdc_n_connected(0)) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    DWT_Init();

    for (int i = 0; i < THREAD_COUNT; i++) {
        tx_done[i] = false;
        for (int j = 0; j < DUMP_SIZE; j++)
            time_dump[i][j] = 0;
        // clang-format off
        xTaskCreateStatic(
            TxTask,
            "TX",
            configMINIMAL_STACK_SIZE,
            ((void*)i),
            tskIDLE_PRIORITY + 2,
            txTaskStack[i],
            &txTaskBuffer[i]);
        // clang-format on
    }

    while (1) {
        bool all_done = true;
        for (int i = 0; i < THREAD_COUNT; i++) {
            all_done &= tx_done[i];
        }

        if (all_done) {
            float global_max = 0.0f;
            int num_messages = sizeof(messages) / sizeof(char*);

            for (int i = 0; i < THREAD_COUNT; i++) {
                const char* msg = messages[i % num_messages];
                int msg_len = strlen(msg);

                float t_min = 1e30f, t_max = 0.0f;
                float t_sum = 0.0f, t_sum_sq = 0.0f;

                printf("Times for TID %d (msg idx %d, len %d):\n\r", i, i % num_messages, msg_len);
                for (int j = 0; j < DUMP_SIZE; j++) {
                    float t = time_dump[i][j] / (SystemCoreClock / 1000.0f);
                    // printf("\t%f ms\n\r", t);

                    if (t < t_min)
                        t_min = t;
                    if (t > t_max)
                        t_max = t;
                    t_sum += t;
                    t_sum_sq += t * t;

                    if (t > global_max)
                        global_max = t;
                }

                float mean = t_sum / DUMP_SIZE;
                float stddev = sqrtf((t_sum_sq / DUMP_SIZE) - (mean * mean));

                printf("  Msg:    \"%s\"\n\r", msg);
                printf("  Min:    %f ms\n\r", t_min);
                printf("  Max:    %f ms\n\r", t_max);
                printf("  Mean:   %f ms\n\r", mean);
                printf("  Jitter: %f ms  (peak-to-peak)\n\r", t_max - t_min);
                printf("  Stddev: %f ms\n\r", stddev);
            }

            printf("Global max: %f ms\n\r", global_max);
            break;
        }
    }

    while (1) {
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
        InitTask,
        "Init",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        initTaskStack,
        &initTaskBuffer);
    // clang-format on

    vTaskStartScheduler();

    while (1) {
    }
}
