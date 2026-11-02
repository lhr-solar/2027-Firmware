#include "PDU_Clock.h"
#include "PDU_GPIO.h"
#include "PDU_SPI.h"
#include "PDU_USB.h"

// TODO: HSS device setup. One HSS task owns all six chips and makes every TPS_* call
// (driver is not thread-safe); other tasks send it requests through a queue.
//
// static TPS2HCS08_t hss[6] = {
//     { .hspi = &hspi1, .cs_port = HSS1_CS_PORT, .cs_pin = HSS1_CS_PIN },
//     ...
//     { .hspi = &hspi1, .cs_port = HSS6_CS_PORT, .cs_pin = HSS6_CS_PIN },
// };

int main(void) {
    HAL_Init();
    PDU_Clock_Init();
    PDU_GPIO_Init();
    if (PDU_SPI_Init() != HAL_OK) {
        // No SPI = no control of any TPS2HCS08. HSS outputs stay off: CS lines
        // are deasserted and LIMP_HOME/DI held low by PDU_GPIO_Init.
        // TODO: report the fault (LED / USB / CAN) and skip all HSS commands
    }
    PDU_USB_Init();

    while (1) {
    }
}
