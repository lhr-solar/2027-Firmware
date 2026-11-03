#include "Contactors.h"
#include "pinDefs.h"

static SemaphoreHandle_t contactors_mutex = NULL;
static StaticSemaphore_t contactors_mutex_buffer;

bool contactor_is_initialized = false;

// designated initializers keep names tied to the enum even if its order changes
const char* CONTACTOR_NAMES[NUM_CONTACTORS] = {[HV_PLUS_CONTACTOR] = "HV Plus Contactor",
                                               [HV_MINUS_CONTACTOR] = "HV Minus Contactor",
                                               [MOTOR_CONTACTOR] = "Motor Contactor"};

// array to hold the contactor structs
static contactor_t contactors[NUM_CONTACTORS];

// get physical state
contactor_state_t Contactors_Get(contactor_num_t contactor_num) {

    // check that contactor exists
    if ((contactor_num < 0) || (contactor_num >= NUM_CONTACTORS)) {
        return CONTACTOR_ERROR;
    }

    contactor_t* contactor = &contactors[contactor_num];
    return (HAL_GPIO_ReadPin(contactor->sense_pin.port, contactor->sense_pin.pin) ? CONTACTOR_CLOSED
                                                                                  : CONTACTOR_OPEN);
}

// determines if the contactor physical state matches the expected state
contactor_state_t Contactors_Verify(contactor_num_t contactor_num) {

    // check that contactor exists
    if ((contactor_num < 0) || (contactor_num >= NUM_CONTACTORS)) {
        return CONTACTOR_ERROR;
    }

    contactor_t* contactor = &contactors[contactor_num];

    // if timer is active contactor could be in either state
    if (xTimerIsTimerActive(contactor->sense_timer) == pdTRUE) {
        return CONTACTOR_OK;
    }

    return (Contactors_Get(contactor_num) == contactor->state) ? CONTACTOR_OK : CONTACTOR_ERROR;
}

static void prvContactorCallback(TimerHandle_t sense_timer) {

    contactor_num_t contactor_num = (contactor_num_t)pvTimerGetTimerID(sense_timer);
    contactor_t* contactor = &contactors[contactor_num];

    if (contactor->state != Contactors_Get(contactor_num)) {
        contactor->callback_faulted = true;
        // TODO: Change when we determine how faults/errors should be handled
    }
}

estop_status_t Contactors_CheckEstops() {

    if (HAL_GPIO_ReadPin(ESTOP_1_SENSE_PORT, ESTOP_1_SENSE_PIN) == GPIO_PIN_SET) {
        return ESTOP_1_FAULT;
    } else if (HAL_GPIO_ReadPin(ESTOP_2_SENSE_PORT, ESTOP_2_SENSE_PIN) == GPIO_PIN_SET) {
        return ESTOP_2_FAULT;
    } else if (HAL_GPIO_ReadPin(ESTOP_3_SENSE_PORT, ESTOP_3_SENSE_PIN) == GPIO_PIN_SET) {
        return ESTOP_3_FAULT;
    } else {
        return ESTOP_OK;
    }
}

bool Contactors_GetFaultedStatus(contactor_num_t contactor_num) {
    if (!contactor_is_initialized)
        return false;
    // check that contactor exists
    if ((contactor_num < 0) || (contactor_num >= NUM_CONTACTORS)) {
        return false;
    }

    contactor_t* contactor = &contactors[contactor_num];
    return contactor->callback_faulted;
}

// sets contactor, updates state value, then starts timer to check expected state matches actual state.
contactor_state_t Contactors_Set(contactor_num_t contactor_num, contactor_state_t state,
                                 TickType_t wait_ms, bool emergency) {

    // check that contactor exists
    if ((contactor_num < 0) || (contactor_num >= NUM_CONTACTORS)) {
        return CONTACTOR_ERROR;
    }

    if ((contactors_mutex == NULL) && (!emergency))
        return CONTACTOR_ERROR;

    contactor_t* contactor = &contactors[contactor_num];

    // if its emergency, dont bother with semaphore
    if ((!emergency) && xSemaphoreTake(contactors_mutex, pdMS_TO_TICKS(wait_ms)) == pdFALSE) {
        return CONTACTOR_ERROR;
    };

    // critical section:
    HAL_GPIO_WritePin(contactor->control_pin.port, contactor->control_pin.pin,
                      ((state == CONTACTOR_CLOSED) ? GPIO_PIN_SET : GPIO_PIN_RESET));
    contactor->state = state;

    /* start timer to check if the state of the contactor makes expected state, the exit critical section. Timer resets
    when the contactor is set to another value, so no possible error with expected value changing from when timer is called
    the 0 param in xTimerStart indicates to start the timer immediately, not wait any ticks*/
    if ((!emergency)) {
        xTimerStart(contactor->sense_timer, 0);
        xSemaphoreGive(contactors_mutex);
    }

    return CONTACTOR_OK;
}

void Contactors_Init() {

    if (contactor_is_initialized)
        return;

    // Enable clocks for every GPIO port this driver touches.
    // Contactors: B (HV+ sense), C (motor control/sense), E (HV+ control, HV- control/sense).
    // E-stops: D (PD2, PD3, PD4).
    // A port whose clock is off ignores writes and reads 0, which this driver reads as
    // "contactor open" / "no e-stop", so every port used here must appear below.
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    // initializing the pindef into contactor structs
    contactors[HV_PLUS_CONTACTOR] =
        (contactor_t){.control_pin = {HV_PLUS_CTRL_PORT, HV_PLUS_CTRL_PIN},
                      .sense_pin = {HV_PLUS_SENSE_PORT, HV_PLUS_SENSE_PIN}};

    contactors[HV_MINUS_CONTACTOR] =
        (contactor_t){.control_pin = {HV_MINUS_CTRL_PORT, HV_MINUS_CTRL_PIN},
                      .sense_pin = {HV_MINUS_SENSE_PORT, HV_MINUS_SENSE_PIN}};

    contactors[MOTOR_CONTACTOR] = (contactor_t){.control_pin = {MOTOR_CTRL_PORT, MOTOR_CTRL_PIN},
                                                .sense_pin = {MOTOR_SENSE_PORT, MOTOR_SENSE_PIN}};

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // init estops
    GPIO_InitStruct.Pin = ESTOP_1_SENSE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ESTOP_1_SENSE_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ESTOP_2_SENSE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ESTOP_2_SENSE_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ESTOP_3_SENSE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ESTOP_3_SENSE_PORT, &GPIO_InitStruct);

    // loop to intialize contactor GPIO and timers
    for (uint8_t contactor_num = 0; contactor_num < NUM_CONTACTORS; contactor_num++) {

        contactor_t* contactor = &contactors[contactor_num];

        // set output level before the pin becomes an output so it never glitches high
        HAL_GPIO_WritePin(contactor->control_pin.port, contactor->control_pin.pin, GPIO_PIN_RESET);

        // init contactor control pins
        GPIO_InitStruct.Pin = contactor->control_pin.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(contactor->control_pin.port, &GPIO_InitStruct);

        // init contactor sense pins
        GPIO_InitStruct.Pin = contactor->sense_pin.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(contactor->sense_pin.port, &GPIO_InitStruct);

        contactor->state = Contactors_Get(contactor_num);

        contactor->callback_faulted = false;

        // making timers and putting them into contactor structs
        contactor->sense_timer =
            xTimerCreateStatic(CONTACTOR_NAMES[contactor_num],  /* Name of the timer */
                               CONTACTOR_SENSE_DELAY_TICKS,     /* Timer period in ticks */
                               pdFALSE,                         /* Don't auto-reload */
                               (void*)(uint32_t)contactor_num,  /* Timer ID */
                               prvContactorCallback,            /* Callback function */
                               &(contactor->sense_timer_buffer) /* Buffer to hold timer data */
            );
    }

    // INITIALIZE MUTEX
    contactors_mutex = xSemaphoreCreateMutexStatic(&contactors_mutex_buffer);

    contactor_is_initialized = true;
}

// Opens all contactors in case of fault
void Contactors_EmergencyOpenAll(void) {

    Contactors_Init();

    for (uint8_t contactor_num = 0; contactor_num < NUM_CONTACTORS; contactor_num++) {
        Contactors_Set(contactor_num, CONTACTOR_OPEN, pdTICKS_TO_MS(portMAX_DELAY), true);
    }
}