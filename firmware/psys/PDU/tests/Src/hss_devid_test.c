#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "PDU_Clock.h"
#include "PDU_GPIO.h"
#include "PDU_SPI.h"
#include "TPS2HCS08.h"

#define TEST_CS_PORT HSS1_CS_PORT
#define TEST_CS_PIN  HSS1_CS_PIN

#define NUM_READS    200

static TPS2HCS08_t hss = { .hspi = &hspi1, .cs_port = TEST_CS_PORT, .cs_pin = TEST_CS_PIN };

static volatile uint32_t good_reads;     // if dev_id read is 0xFFF0
static volatile uint32_t bad_reads;      
static volatile uint32_t spi_errors;     
static volatile uint16_t first_bad_id;   

static StaticTask_t hssTaskBuffer;
static StackType_t  hssStack[configMINIMAL_STACK_SIZE * 2];

//   solid red              -> PDU_SPI_Init failed
//   blue solid             -> test running (well under a second)
//   blue blinking          -> PASS: all 200 reads were 0xFFF0
//   red, 1 blink + pause   -> a few bad reads (1-10)      
//   red, 2 blinks + pause  -> many bad reads (11-199)  
//   red, 3 blinks + pause  -> every read was bad            
//   red, 4 blinks + pause  -> HAL SPI error                

static void blink_red_forever(uint32_t n)
{
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

    if (TPS_Wake(&hss) != TPS_OK) {
        HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
        blink_red_forever(4);
    }

    for (uint32_t i = 0; i < NUM_READS; i++) {
        uint16_t id = 0;

        if (TPS_ReadReg(&hss, TPS_REG_DEV_ID, &id) != TPS_OK) {
            spi_errors++;
        } else if (id == TPS_DEV_ID_A) {
            good_reads++;
        } else {
            if (bad_reads == 0) {
                first_bad_id = id;
            }
            bad_reads++;
        }

        vTaskDelay(pdMS_TO_TICKS(1));  
    }

    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);


    if (bad_reads > 0) {
        (void)TPS_Frame(&hss, false, TPS_NOP_REG, first_bad_id, NULL);
    }

    if (spi_errors > 0) {
        blink_red_forever(4);
    } else if (bad_reads == NUM_READS) {
        blink_red_forever(3);
    } else if (bad_reads > 10) {
        blink_red_forever(2);
    } else if (bad_reads > 0) {
        blink_red_forever(1);
    }

    while (1) {
        HAL_GPIO_TogglePin(LED_BLUE_PORT, LED_BLUE_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
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

    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);

    xTaskCreateStatic(hss_task,
        "HSS DEV_ID Test",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        tskIDLE_PRIORITY + 1,
        hssStack,
        &hssTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
