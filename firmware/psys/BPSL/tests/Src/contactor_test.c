// Cycles the RGB heartbeat LED through red, green, and blue, 500 ms each.
#include "stm32xx_hal.h"
#include "common.h"
#include "StatusLEDs.h"
#include "Contactors.h"

// Task configuration
#define CONTACTOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define CONTACTOR_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define CONTACTOR_CHECK_PERIOD    pdMS_TO_TICKS(500)

// Static task buffers
static StaticTask_t xContactorTaskBuffer;
static StackType_t xContactorStack[CONTACTOR_TASK_STACK_SIZE];

// close the contactors
static void vContactorTask(void* pvParameters) {
    (void)pvParameters;
    rgb_color_t color = RGB_RED;
    contactor_state_t contactor_state = CONTACTOR_OPEN;
    contactor_state_t contactor_state_next = CONTACTOR_CLOSED;

    while (true) {
        // HAL_GPIO_WritePin(HV_MINUS_CTRL_PORT, HV_MINUS_CTRL_PIN, GPIO_PIN_SET);


        Contactors_Set(HV_MINUS_CONTACTOR, contactor_state_next,
                       CONTACTOR_CALLBACK_BLOCKING_TIME_MS, true);

        StatusLEDs_SetHeartbeat(Contactors_Get(HV_MINUS_CONTACTOR) == CONTACTOR_CLOSED);


        if (StatusLEDs_SetHeartbeatColor(color) != LED_OK) {
            configASSERT(pdFALSE);
        }


        // contactor_state = Contactors_Get(HV_MINUS_CONTACTOR);
        if (contactor_state == CONTACTOR_CLOSED) {
            contactor_state_next = CONTACTOR_OPEN;
            StatusLEDs_SetHeartbeat(false);

        } else if (contactor_state == CONTACTOR_OPEN) {
            contactor_state_next = CONTACTOR_CLOSED;
            StatusLEDs_SetHeartbeat(true);

        } else {
            // error state, do nothing
            StatusLEDs_SetHeartbeat(false);
        }
        contactor_state = contactor_state_next;

        vTaskDelay(CONTACTOR_CHECK_PERIOD * 5);

        // move to the next color, wrapping back to red after blue
        // color = (color + 1) % RGB_NUM_COLORS;
    }
}

int main(void) {
    if (HAL_Init() != HAL_OK) {
        Error_Handler();
    }

    SystemClock_Config();

    if (StatusLEDs_Init() != LED_OK) {
        Error_Handler();
    }
    Contactors_Init();

    TaskHandle_t handle =
        xTaskCreateStatic(vContactorTask, "Contactor", CONTACTOR_TASK_STACK_SIZE, NULL,
                          CONTACTOR_TASK_PRIORITY, xContactorStack, &xContactorTaskBuffer);
    configASSERT(handle != NULL);
    (void)handle;

    vTaskStartScheduler();

    // only reached if the scheduler failed to start
    Error_Handler();
}
