#include "printf.h"
#include "log.h"          // after printf.h

#define LOG_PERIOD_MS 1000   // log every second

StaticTask_t loggingTaskBuffer;
StackType_t loggingTaskStack[configMINIMAL_STACK_SIZE];

int val = 0;

static void loggingTask(void* arg) {
    
    LOG_DEBUG("every pass=%d", val);                       // every pass (compiled out in race builds)
    LOG_INFO_EVERY_MS(LOG_PERIOD_MS, "every sec = %d mV", mv);   // every second
    LOG_INFO_EVERY_N(100, "still alive");          // every 100th pass
    if (val > 100) {
        LOG_ERROR_EVERY_MS(1000, "whoa that's a big value there... %d", val);
        val = 0;
    }
     val++;
    vTaskDelay(pdMS_TO_TICKS(1));                       // 1 kHz
}

int main(void) {

    xTaskCreateStatic(loggingTask,
        "Logging Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        initTaskStack,
        &initTaskBuffer);
}