#include "lvc_can.h"

#define FDCAN_MODE FDCAN_MODE_INTERNAL_LOOPBACK

static StaticTask_t can_task_buffer;
static StackType_t can_task_stack[512];

static void can_task(void* pvParameters) {

    FDCAN_TxHeaderTypeDef tx_header = {
        .Identifier = 0x001,
        .IdType = FDCAN_STANDARD_ID,
        .TxFrameType = FDCAN_DATA_FRAME,
        .DataLength = FDCAN_DLC_BYTES_8,
        // ErrorStateIndicator
        .BitRateSwitch = FDCAN_BRS_OFF,
        .FDFormat = FDCAN_CLASSIC_CAN,
        .TxEventFifoControl = FDCAN_STORE_TX_EVENTS,       
    };
    // send x1234 to 0x001
    uint8_t tx_data[8] = {0};
    tx_data[0] = 0x12;
    tx_data[1] = 0x34;
    tx_data[2] = 0x56;
    tx_data[3] = 0x78;
    tx_data[4] = 0x9A;
    tx_data[5] = 0xBC;
    tx_data[6] = 0xDE;
    tx_data[7] = 0xFF;

    FDCAN_RxHeaderTypeDef rx_header = {0};
    uint8_t rx_data[8] = {0};

    while (1) {
        if (can_fd_send(hfdcan2, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR) {
            Error_Handler();
        }

        if (can_fd_recv(hfdcan2, 0x001, &rx_header, rx_data, portMAX_DELAY) != CAN_OK) {
            Error_Handler();
        }

        for (int i = 0; i < 8; i++) {
            if (rx_data[i] != tx_data[i]) {
                Error_Handler();
            }
        }
        statusLEDs_toggle(GREEN_LED);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
    HAL_Init();

    SystemClock_Config();

    LEDs_Init();

    CAN_Init(FDCAN_MODE);

    xTaskCreateStatic(
        can_task,
        "Can_Task",
        sizeof(can_task_stack),
        NULL,
        tskIDLE_PRIORITY + 2,
        can_task_stack,
        &can_task_buffer
    );

    vTaskStartScheduler();
    while (1) {
    }

    Error_Handler();
    return 0;
}