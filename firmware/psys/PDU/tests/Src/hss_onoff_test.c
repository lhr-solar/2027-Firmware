#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "PDU_Clock.h"
#include "PDU_GPIO.h"
#include "PDU_SPI.h"
#include "TPS2HCS08.h"

// HSS output on constant

//   solid red              -> PDU_SPI_Init failed
//   solid blue             -> channel is ON, no faults
//   red, 1 blink + pause   -> TPS_Init failed 
//   red, 2 blinks + pause  -> SPI error or SW_STATE read-back mismatch
//   red, 3 blinks + pause  -> chip reported a protection fault while ON

#define TEST_CS_PORT  HSS1_CS_PORT
#define TEST_CS_PIN   HSS1_CS_PIN

#define TEST_CHANNEL  0

#define FAULT_CHECK_MS 500

#define TRIP_FAULTS   (TPS_FLT_ILIMIT | TPS_FLT_THERMAL_SD | TPS_FLT_I2T_FLT)

static TPS2HCS08_t hss = { .hspi = &hspi1, .cs_port = TEST_CS_PORT, .cs_pin = TEST_CS_PIN };

static volatile TPS_Status_t last_status;
static volatile uint16_t     global_fault;
static volatile uint16_t     ch_fault;    

static StaticTask_t hssTaskBuffer;
static StackType_t  hssStack[configMINIMAL_STACK_SIZE * 2];

static void fail_forever(uint32_t n)
{
    (void)TPS_WriteReg(&hss, TPS_REG_SW_STATE, 0);
    hss.sw_state = 0;
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);

    while (1) {
        for (uint32_t i = 0; i < n; i++) {
            HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
            vTaskDelay(pdMS_TO_TICKS(200));
            HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

static void hss_task(void *pvParameters)
{
    (void)pvParameters;

    if ((last_status = TPS_Init(&hss)) != TPS_OK) {
        fail_forever(1);
    }

    if ((last_status = TPS_SetChannel(&hss, TEST_CHANNEL, true)) != TPS_OK) {
        fail_forever(2);
    }
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(FAULT_CHECK_MS));

        uint16_t g, f1, f2;
        if ((last_status = TPS_ReadFaults(&hss, &g, &f1, &f2)) != TPS_OK) {
            fail_forever(2);
        }
        global_fault = g;
        ch_fault = (TEST_CHANNEL == 0) ? f1 : f2;
        if (ch_fault & TRIP_FAULTS) {
            fail_forever(3);
        }
    }
}

int main(void)
{
    HAL_Init();
    PDU_Clock_Init();
    PDU_GPIO_Init();  

    if (PDU_SPI_Init() != HAL_OK) {
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
        while (1) {
        }
    }

    xTaskCreateStatic(hss_task,
        "HSS On Test",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        tskIDLE_PRIORITY + 1,
        hssStack,
        &hssTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
