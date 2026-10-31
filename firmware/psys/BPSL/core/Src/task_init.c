// Startup task: brings up what the BPS tasks share, creates them, then deletes itself.
#include "BPS_tasks.h"
#include "task.h"

#define STARTUP_DELAY_MS (10)

// Task Stack Arrays
StackType_t Task_Init_Stack_Array[TASK_INIT_STACK_SIZE];
StackType_t Task_SMB_Monitor_Stack_Array[TASK_SMB_MONITOR_STACK_SIZE];
StackType_t Task_Current_Monitor_Stack_Array[TASK_CURRENT_MONITOR_STACK_SIZE];
StackType_t Task_Test_Stack_Array[TASK_TEST_STACK_SIZE];

// Task Buffers
StaticTask_t Task_Init_Buffer;
StaticTask_t Task_SMB_Monitor_Buffer;
StaticTask_t Task_Current_Monitor_Buffer;
StaticTask_t Task_Test_Buffer;

void Task_Init(void* pvParameters) {
    
    (void)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(STARTUP_DELAY_MS));

    TASK_CREATE(SMB_MONITOR, SMB_Monitor);
    TASK_CREATE(CURRENT_MONITOR, Current_Monitor);

    // Task deletes itself after all other tasks are init'd
    vTaskDelete(NULL);
}
