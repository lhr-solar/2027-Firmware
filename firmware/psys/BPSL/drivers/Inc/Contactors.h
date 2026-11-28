#pragma once
#include "common.h"
#include "pinDefs.h"

/* Timing Definitions */

/** * @brief Time to wait for the physical contactor to settle before reading feedback. 
 */
#define CONTACTOR_SENSE_DELAY_TICKS pdMS_TO_TICKS(1000)

/** * @brief Maximum blocking time in milliseconds for contactor callbacks. 
 */
#define CONTACTOR_CALLBACK_BLOCKING_TIME_MS 20

/**
 * @brief Represents the logical and physical state of a contactor.
 */
typedef enum {
    CONTACTOR_OPEN = 0, /**< Circuit is disconnected */
    CONTACTOR_CLOSED,   /**< Circuit is connected */
    CONTACTOR_ERROR,    /**< Error getting contactor state (used for function return) */
    CONTACTOR_OK        /**< Contactor action completed successfully (used for function return) */
} contactor_state_t;

/**
 * @brief Represents the possible states of the ESTOP buttons.
 */
typedef enum { ESTOP_OK = 0, ESTOP_1_FAULT, ESTOP_2_FAULT, ESTOP_3_FAULT } estop_status_t;

/**
 * @brief Identification for specific contactors within the HV system.
 */
typedef enum {
    HV_PLUS_CONTACTOR = 0, /**< Main Positive Bus */
    HV_MINUS_CONTACTOR,    /**< Main Negative Bus */
    MOTOR_CONTACTOR,       /**< Motor controller supply */
    NUM_CONTACTORS         /**< Total count helper */
} contactor_num_t;

extern const char* CONTACTOR_NAMES[NUM_CONTACTORS];

/**
 * @brief Contactor hardware abstraction object.
 */
typedef struct {
    contactor_state_t state; /**< Current commanded state */
    gpio_pin_t sense_pin;    /**< Digital input for auxiliary feedback loop */
    gpio_pin_t control_pin;  /**< Digital output to coil driver/relay */
    bool callback_faulted;   /**< Whether or not this Contactor had a callback fault */

    /* RTOS Resources */
    TimerHandle_t sense_timer;        /**< Handle for non-blocking state verification */
    StaticTimer_t sense_timer_buffer; /**< Memory buffer for static timer allocation */
} contactor_t;

/** * @brief Hardware/RTOS initialization for all contactors, timers, and mutexes. 
 */
void Contactors_Init(void);

/** * @brief Reads the physical sense pin for a specific contactor.
 * * @param contactor_num The specific contactor to read from the hardware.
 * @return contactor_state_t Returns CONTACTOR_CLOSED if connected, CONTACTOR_OPEN if disconnected, or CONTACTOR_ERROR.
 */
contactor_state_t Contactors_Get(contactor_num_t contactor_num);

/**
 * @brief Verifies if the physical state of a specific contactor matches its expected software state. 
 * * @param contactor_num The specific contactor to check (e.g., HV_PLUS_CONTACTOR, MOTOR_CONTACTOR).
 * * @return contactor_state_t Returns CONTACTOR_OK if they match (or the sense timer is still running), CONTACTOR_ERROR if they don't
 */
contactor_state_t Contactors_Verify(contactor_num_t contactor_num);

/** * @brief Commands a contactor state change with safety verification.
 * * @param contactor_num The specific contactor to switch.
 * @param state         The desired state (CONTACTOR_OPEN or CONTACTOR_CLOSED).
 * @param wait_ms       Maximum time to wait for the contactor mutex, in ms. Ignored when emergency is true.
 * @param emergency     Pass true to write the pin immediately, skipping the mutex and the sense timer.
 * @return contactor_state_t  Returns CONTACTOR_OK if command was accepted, CONTACTOR_ERROR on hardware or RTOS failure.
 */
contactor_state_t Contactors_Set(contactor_num_t contactor_num, contactor_state_t state,
                                 TickType_t wait_ms, bool emergency);

/**
 * @brief Checks if that Contactor had a contactor callback fault
 * * @param contactor_num The specific contactor to check (e.g., HV_PLUS_CONTACTOR, MOTOR_CONTACTOR).
 * * @return true if the Contactor had a callback fault, false if not
 */
bool Contactors_GetFaultedStatus(contactor_num_t contactor_num);

/** * @brief Determines if any ESTOPs are tripped
 * @return estop_status_t Returns ESTOP_OK if no estops are tripped, else it returns a code for each estop
 */
estop_status_t Contactors_CheckEstops();

/** * @brief RTOS Timer callback for contactor state verification.
 * @note This function is called when the sense timer for the contactor times out. 
 * It confirms that the physical contactor state changed successfully and matches the expected value.
 * * @param sense_timer Handle of the timer that expired.
 */
/* static void prvContactorCallback( TimerHandle_t sense_timer ); */

/** * @brief Opens all contactors immediately in case of a system fault.
 * @note Initializes the contactors first if Contactors_Init() has not run yet.
 */
void Contactors_EmergencyOpenAll(void);