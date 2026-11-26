#include "stm32xx_hal.h"
#include <string.h>
#include "CAN_FD.h"
#include "can_msgs.h"
#include "projdefs.h"
#include "printf.h"
#include "UART.h"
#include "task.h"
#include "tusb_port.h"


static StaticTask_t motordrivetask_buffer;
static StackType_t motordrivetask_stack[512];

static StaticTask_t motorpowertask_buffer;
static StackType_t motorpowertask_stack[512];

static StaticTask_t heartbeat_buffer;
static StackType_t heartbeat_stack[configMINIMAL_STACK_SIZE];

/* tusb polling task */
StaticTask_t pollEventsTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];

/*
configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY is the maximum FreeRTOS priority
for an interrupt
*/
#define FDCAN_NVIC_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3

// LSOM has heartbeat pin defined as PC3
// Most other nucleos have a heartbeat for A5
#define LED_PIN  GPIO_PIN_4
#define LED_PORT GPIOD

#define LED_PIN_1  GPIO_PIN_5
#define LED_PORT_1 GPIOD

#define LED_PIN_2  GPIO_PIN_3
#define LED_PORT_2 GPIOD

#define LED_PIN_3  GPIO_PIN_2
#define LED_PORT_3 GPIOD

// STM32G473VETx (LQFP100): PD5 drives SW_LED1 independently of CAN reception.
static void heartbeat_task(void* argument) {
    (void)argument;

    while (1) {
        HAL_GPIO_TogglePin(LED_PORT_1, LED_PIN_1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Some pins broken out by LSOM
#define CAN_TX_PROFILE_PORT GPIOA
#define CAN_TX_PROFILE_PIN  GPIO_PIN_4

#define CAN_RX_PROFILE_PORT GPIOA
#define CAN_RX_PROFILE_PIN  GPIO_PIN_5

#define DRIVER_CONTROL_ADDRESS (0x400) //change MOCO address add it here
#define MOTOR_DRIVE_COMMAND (DRIVER_CONTROL_ADDRESS + 0x01)
#define MOTOR_POWER_COMMAND (DRIVER_CONTROL_ADDRESS + 0x02)

void can_profile_pins_init() {
    GPIO_InitTypeDef tx_pin_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = CAN_TX_PROFILE_PIN,
    };

    switch ((uint32_t)CAN_TX_PROFILE_PORT) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case (uint32_t)GPIOD:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
    }

    HAL_GPIO_Init(CAN_TX_PROFILE_PORT, &tx_pin_config);

    GPIO_InitTypeDef rx_pin_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = CAN_RX_PROFILE_PIN,
    };

    switch ((uint32_t)CAN_RX_PROFILE_PORT) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case (uint32_t)GPIOD:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
    }
    HAL_GPIO_Init(CAN_RX_PROFILE_PORT, &rx_pin_config);
}

// Initialize clock for heartbeat LED port
void Heartbeat_Init() {
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Pin = LED_PIN | LED_PIN_1 | LED_PIN_2 | LED_PIN_3};

    switch ((uint32_t)LED_PORT) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case (uint32_t)GPIOD:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
    }

    HAL_GPIO_Init(GPIOD, &led_config);
}

void G474_SystemClockConfig() {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
   */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
   */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

void G473_SystemClockConfig(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
   */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 12;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
   */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
}

void Error_Handler() {
    while (1) {
        HAL_GPIO_TogglePin(LED_PORT_3, LED_PIN_3);
        HAL_Delay(700);
    }
}

// void Success_Handler() {
//     HAL_GPIO_WritePin(LED_PORT_3, LED_PIN_3, GPIO_PIN_SET);
// }

static void poll_events(void* argument) {
    while (1) {
        // polls event queue
        tud_task();
    }
}


static void motordrivetask(void* pvParameters) {

    FDCAN_TxHeaderTypeDef tx_header = {0};
    tx_header.Identifier = MOTOR_DRIVE_COMMAND;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
    tx_header.MessageMarker = 0;

    uint8_t motor_drive_command_data[8] = {0};
    
    float motor_current = 0.05f; //%
    float motor_velocity= 67; //rpm

    memcpy(&motor_drive_command_data[4], &motor_current, 4);
    memcpy(&motor_drive_command_data[0], &motor_velocity, 4);

    while (1) {
        #ifdef FDCAN1
            if (can_fd_send(hfdcan1, &tx_header, motor_drive_command_data,
                            pdMS_TO_TICKS(100)) == CAN_ERR) {
                Error_Handler();
            }
        // Only blink when a message is received
        HAL_GPIO_TogglePin(LED_PORT_2, LED_PIN_2);
        HAL_Delay(100);
        vTaskDelay(pdMS_TO_TICKS(50));
        #endif
    }

}

static void motorpowertask(void* pvParameters) {

    FDCAN_TxHeaderTypeDef tx_header = {0};
    tx_header.Identifier = MOTOR_POWER_COMMAND;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
    tx_header.MessageMarker = 0;

    uint8_t motor_power_command_data[8] = {0};
    
    float bus_current = 0.05f; //%

    memcpy(&motor_power_command_data[4], &bus_current, 4);

    while (1) {
        #ifdef FDCAN1
            if (can_fd_send(hfdcan1, &tx_header, motor_power_command_data,
                            pdMS_TO_TICKS(100)) == CAN_ERR) {
                Error_Handler();
            }
        // Only blink when a message is received
        HAL_GPIO_TogglePin(LED_PORT_3, LED_PIN_3);
        HAL_Delay(100);
        vTaskDelay(pdMS_TO_TICKS(50));
        #endif
    }

}


void can_fd_tx_callback_hook(FDCAN_HandleTypeDef* hfdcan, const can_tx_payload_t* payload) {
    HAL_GPIO_TogglePin(CAN_TX_PROFILE_PORT, CAN_TX_PROFILE_PIN);
}

void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
                             can_rx_payload_t recv_payload) {
    HAL_GPIO_TogglePin(CAN_RX_PROFILE_PORT, CAN_RX_PROFILE_PIN);
}

int main(void) {
    HAL_Init();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // System clock config can change depending on the target MCU, since the clock
    // tree can be different If you need to use a different MCU, go to cubemx and
    // generate a new system clock config function with the system clock being 80
    // Mhz It especially varies with nucleo vs direct MCU G473_SystemClockConfig();
    #ifdef STM32G474xx
    G474_SystemClockConfig();
    #elif defined(STM32G473xx)
    G473_SystemClockConfig();
    #else
    SystemClock_Config();
    #endif

    Heartbeat_Init();        // enable LED for LED_PORT
    can_profile_pins_init(); // enable pins we toggle during interrupts

    #ifdef FDCAN1
    hfdcan1->Instance = FDCAN1;
    hfdcan1->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

    // internal loopback shorts the CAN RX and TX internally.
    // In production, this should be set to FDCAN_MODE_NORMAL
    hfdcan1->Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan1->Init.AutoRetransmission = ENABLE;
    hfdcan1->Init.TransmitPause = DISABLE;
    hfdcan1->Init.ProtocolException = DISABLE;
    hfdcan1->Init.NominalPrescaler = 12;
    hfdcan1->Init.NominalSyncJumpWidth = 1;
    hfdcan1->Init.NominalTimeSeg1 = 13;
    hfdcan1->Init.NominalTimeSeg2 = 2;
    hfdcan1->Init.DataPrescaler = 1;
    hfdcan1->Init.DataSyncJumpWidth = 1;
    hfdcan1->Init.DataTimeSeg1 = 1;
    hfdcan1->Init.DataTimeSeg2 = 1;
    hfdcan1->Init.StdFiltersNbr = 1;

    // ExtFiltersNB bits sets # of ID allowed
    // 0 = no extended ID allowed (all extended ID rejected)
    // 1 =  filter enabled to match to ID
    // each CAN needs its own filter
    hfdcan1->Init.ExtFiltersNbr = 0;
    hfdcan1->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    // FDCAN1 Filter Config

    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0

    // this accepts all CAN IDs
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x000;

    if (can_fd_init(hfdcan1, &sFilterConfig) != CAN_OK) {
        Error_Handler();
    }

    if (can_fd_start(hfdcan1) != CAN_OK) {
        Error_Handler();
    }
    #endif

    MX_USB_PCD_Init();
        if (!printf_init()) {
            Error_Handler();
        }

    xTaskCreateStatic(
        poll_events, 
        "Poll Event Queue", 
        configMINIMAL_STACK_SIZE, 
        NULL,
        tskIDLE_PRIORITY + 3, 
        pollEventsStack, 
        &pollEventsTaskBuffer);

    // you can only send CAN messages within a FreeRTOS task
    // clang-format off
    xTaskCreateStatic(
        motordrivetask,
        "motordrivetask",
        512,
        NULL,
        tskIDLE_PRIORITY + 2,
        motordrivetask_stack,
        &motordrivetask_buffer);
    // clang-format on

    xTaskCreateStatic(
        motorpowertask,
        "motorpowertask",
        512,
        NULL,
        tskIDLE_PRIORITY + 2,
        motorpowertask_stack,
        &motorpowertask_buffer);

    xTaskCreateStatic(heartbeat_task, "Heartbeat", configMINIMAL_STACK_SIZE, NULL,
                      tskIDLE_PRIORITY + 1, heartbeat_stack, &heartbeat_buffer);

    vTaskStartScheduler();
    while (1) {
    }

    Error_Handler();
    return 0;
}

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    // FDCAN1

    #ifdef FDCAN1
    if (hfdcan->Instance == FDCAN1) {

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* FDCAN1 clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**FDCAN1 GPIO Configuration
    PD0     ------> FDCAN1_RX
    PD1     ------> FDCAN1_TX
    */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* FDCAN1 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
    }
    #endif 
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan) {
    if (hfdcan->Instance == FDCAN1) {
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1);

        /* FDCAN1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
        /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

        /* USER CODE END FDCAN1_MspDeInit 1 */
    }
}

// Toggle LED on CAN error
void can_fd_error_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t ErrorStatusITs) {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}