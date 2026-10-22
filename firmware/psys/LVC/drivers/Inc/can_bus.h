#pragma once
#include "CAN_FD.h"
#include "pinDefs.h"

// G473 has no bxCAN; the LVC CAN pins (PB5/PB6) are FDCAN2
#define CAN_INTERRUPT_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

// CAN function declarations
can_status_t CANbus_init(void);
/**
 * @brief Send a classic CAN frame
 * @param id CAN message ID
 * @param data Data bytes to send
 * @param length Number of data bytes (max 8)
 * @param timeout RTOS ticks to block if the send queue is full
 * @return can_status_t status
 */
can_status_t CANbus_send(uint16_t id, uint8_t data[], uint8_t length, TickType_t timeout);
/**
 * @brief Receive CAN message matching ID
 * @param id Expected STD ID
 * @param header Rx header output
 * @param data Rx data output (8 bytes)
 * @param timeout RTOS ticks to block
 * @return can_status_t CAN_OK / CAN_ERR / CAN_EMPTY
 */
can_status_t CANbus_recv(uint16_t id, FDCAN_RxHeaderTypeDef *header, uint8_t data[], TickType_t timeout);
