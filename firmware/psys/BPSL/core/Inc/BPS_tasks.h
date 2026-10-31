#pragma once

#include "common.h"

// Creates a task from its definitions (preprossors can't change case :( )
#define TASK_CREATE(TASK, Task)                                                               \
    xTaskCreateStatic(Task_##Task, #Task, TASK_##TASK##_STACK_SIZE, NULL, TASK_##TASK##_PRIO, \
                      Task_##Task##_Stack_Array, &Task_##Task##_Buffer)

// Task Priorities
#define TASK_INIT_PRIO            (tskIDLE_PRIORITY + 1)
#define TASK_SMB_MONITOR_PRIO     (tskIDLE_PRIORITY + 3)
#define TASK_CURRENT_MONITOR_PRIO (tskIDLE_PRIORITY + 2)
#define TASK_TEST_PRIO            (tskIDLE_PRIORITY + 4)

// Task Stack Sizes
#define TASK_INIT_STACK_SIZE            (configMINIMAL_STACK_SIZE * 2)
#define TASK_SMB_MONITOR_STACK_SIZE     (configMINIMAL_STACK_SIZE * 4)
#define TASK_CURRENT_MONITOR_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define TASK_TEST_STACK_SIZE            (configMINIMAL_STACK_SIZE * 2)

// Task Delays
#define TASK_SMB_MONITOR_DELAY_MS     290
#define TASK_CURRENT_MONITOR_DELAY_MS 90

// (exposed so that tests can init tasks)
// Task Stack Arrays
extern StackType_t Task_Init_Stack_Array[TASK_INIT_STACK_SIZE];
extern StackType_t Task_SMB_Monitor_Stack_Array[TASK_SMB_MONITOR_STACK_SIZE];
extern StackType_t Task_Current_Monitor_Stack_Array[TASK_CURRENT_MONITOR_STACK_SIZE];
extern StackType_t Task_Test_Stack_Array[TASK_TEST_STACK_SIZE];

// Task Buffers
extern StaticTask_t Task_Init_Buffer;
extern StaticTask_t Task_SMB_Monitor_Buffer;
extern StaticTask_t Task_Current_Monitor_Buffer;
extern StaticTask_t Task_Test_Buffer;

// Task Inits
void Task_Init(void* pvParameters);
void Task_SMB_Monitor(void* pvParameters);
void Task_Current_Monitor(void* pvParameters);
void Task_Test(void* pvParameters);
