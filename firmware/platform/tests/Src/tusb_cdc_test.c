// TinyUSB Dual Port CDC Example
// interfaces directly with tinyUSB CDC functions
#include "stm32xx_hal.h"
#include "tusb_port.h"

static void MX_GPIO_Init(void);
void SystemClock_Config(void);

StaticTask_t pollEventsTaskBuffer;
StaticTask_t cdcTaskBuffer;
StackType_t pollEventsStack[configMINIMAL_STACK_SIZE];
StackType_t cdcStack[configMINIMAL_STACK_SIZE];

// Echo chars on one part to the other
static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        tud_cdc_n_write_char(itf, buf[i]);
    }
    tud_cdc_n_write_flush(itf);
}

// Mount
void tud_mount_cb(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
}

// Unmount
void tud_umount_cb(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
}

static void cdc_task(void* pvParameters) {
    uint8_t itf;

    while (1) {
        for (itf = 0; itf < CFG_TUD_CDC; itf++) {
            // connected() check for DTR bit
            // Most but not all terminal client set this when making connection
            // if ( tud_cdc_n_connected(itf) )
            {
                if (tud_cdc_n_available(itf)) {
                    uint8_t buf[64];

                    uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

                    // echo back to both serial ports
                    echo_serial_port(0, buf, count);
                    echo_serial_port(1, buf, count);
                }
            }
        }

        // block
        vTaskDelay(100);
    }
}

static void poll_events(void* pvParameters) {
    while (1) {
        // polls event queue
        tud_task();
    }
}

int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USB_PCD_Init();

    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO,
    };
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    // clang-format off
    xTaskCreateStatic(
        poll_events,
        "Poll Event Queue",
        configMINIMAL_STACK_SIZE,
        (void*)1,
        tskIDLE_PRIORITY + 3,
        pollEventsStack,
        &pollEventsTaskBuffer);
    // clang-format on

    // clang-format off
    xTaskCreateStatic(
        cdc_task,
        "CDC Task",
        configMINIMAL_STACK_SIZE,
        (void*)1,
        tskIDLE_PRIORITY + 2,
        cdcStack,
        &cdcTaskBuffer);
    // clang-format on

    vTaskStartScheduler();

    /* Random msg */
    while (1) {
    }
}

static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // heartbeat
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_2,
    };
    HAL_GPIO_Init(GPIOA, &led_config);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
}
