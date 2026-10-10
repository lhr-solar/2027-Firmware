#include "PDU_Clock.h"
#include "PDU_GPIO.h"
#include "PDU_SPI.h"
#include "PDU_USB.h"

int main(void) {
    HAL_Init();
    PDU_Clock_Init();
    PDU_GPIO_Init();
    PDU_SPI_Init();
    PDU_USB_Init();

    while (1) {
    }
}
