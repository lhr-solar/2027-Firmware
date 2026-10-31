#include "BPS_tasks.h"

// task description goes here

// defines go here

void Task_Current_Monitor(void* pvParameters) {

    // task variables go here 

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true) {
    
        // code go here :)

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK_CURRENT_MONITOR_DELAY_MS));
    }
}
