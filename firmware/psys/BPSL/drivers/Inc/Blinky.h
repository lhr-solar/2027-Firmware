#pragma once
/**
 * @file Blinky.h
 * @brief FreeRTOS task that cycles the heartbeat LED through red, green, and
 *        blue, 500 ms each, forever.
 *
 * Call order: StatusLEDs_Init() -> Blinky_Init() -> vTaskStartScheduler()
 */

/** @brief Creates the blinky task (statically allocated). Calls Error_Handler()
 *         if the task can't be created.
 */
void Blinky_Init(void);
