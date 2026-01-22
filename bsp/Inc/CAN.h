/**
 * @file CAN.h
 * @brief CAN peripheral driver for LHRS embedded systems.
 *
 * @details
 * This driver provides initialization, deinitialization, start/stop, 
 * sending, and receiving functionality for CAN peripherals.
 *
 * Function Descriptions:
 *
 *  - Init: Initializes the CAN peripheral given the configuration
 *          (does not start the CAN peripheral).
 *
 *  - DeInit: Deinitializes the CAN peripheral; resets configuration to default values.
 *
 *  - Start: Starts the CAN peripheral, enabling interrupts and message handling.
 *
 *  - Stop: Stops the CAN peripheral; configuration is unchanged, but interrupts and message handling are disabled.
 *
 *  - Send: Prepares a message to be sent when the CAN peripheral is ready (puts it into a queue).
 *
 *  - Recv: Reads a message from the queue depending on ID.
 *
 * How to Use This Driver:
 *
 *  1. Init to initialize the CAN peripheral.
 *
 *  2. Start to activate the CAN peripheral.
 *
 *  3. Use Send, Recv, Stop, and Start as needed.
 *
 *  4. DeInit to deinitialize the CAN peripheral.
 *
 * Usage Notes:
 *
 *  - CAN2 is usually a slave CAN, sharing transistors with CAN1. Ensure CAN1 is initialized before CAN2.
 *
 *  - CAN2 often shares filter banks with CAN1; carefully consider filter configurations passed to can_init.
 *
 *  - CAN3 is usually a master CAN; no sharing considerations are needed.
 *
 *  - This driver uses a macro to define which messages a CAN interface should receive. 
 *    See can1_recv_entries.h for examples. Ensure entries align with filter configurations.
 */

#ifndef CAN_H
#define CAN_H

#include "stm32xx_hal.h"

#if !defined(CAN1)
  #error "[CONFIG] CAN not enabled on this chip."
#endif /* CAN1 */

// return code for can driver
typedef enum {
    CAN_ERR,   // unsuccessful operation
    CAN_OK,
    CAN_SENT,  // successful send
    CAN_RECV,  // successful recieve
    CAN_EMPTY, // recieved nothing with no errors
} can_status_t;

// can handlers
#ifdef CAN1
extern CAN_HandleTypeDef* hcan1;
#endif /* CAN1 */

#ifdef CAN2
extern CAN_HandleTypeDef* hcan2;
#endif /* CAN2 */

#ifdef CAN3
extern CAN_HandleTypeDef* hcan3;
#endif /* CAN3 */

/**
 * @brief Initializes the CAN peripheral.
 *
 * This function initializes the CAN peripheral, sets up send/receive queues,
 * configures the HAL CAN driver, applies the filter configuration, and 
 * enables CAN interrupts.
 *
 * @param handle Pointer to the CAN handle structure.
 * @param filter Pointer to the CAN filter configuration structure.
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERR on failure.
 */
can_status_t can_init(CAN_HandleTypeDef* handle, CAN_FilterTypeDef* filter);

/**
 * @brief Deinitializes the CAN peripheral.
 *
 * This function deinitializes the HAL CAN driver and disables CAN interrupts.
 *
 * @param handle Pointer to the CAN handle structure.
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERR on failure.
 */
can_status_t can_deinit(CAN_HandleTypeDef* handle);

/**
 * @brief Starts the CAN peripheral.
 *
 * Activates the CAN peripheral so it can transmit and receive messages.
 *
 * @param handle Pointer to the CAN handle structure.
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERR on failure.
 */
can_status_t can_start(CAN_HandleTypeDef* handle);

/**
 * @brief Stops the CAN peripheral.
 *
 * Stops the CAN peripheral; the configuration is preserved, but the
 * peripheral will not process messages or trigger interrupts.
 *
 * @param handle Pointer to the CAN handle structure.
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERR on failure.
 */
can_status_t can_stop(CAN_HandleTypeDef* handle);

/**
 * @brief Sends a CAN message.
 *
 * Places a CAN message into the transmit mailbox if available, otherwise
 * queues it in the send queue for later transmission.
 *
 * @param handle       Pointer to the CAN handle structure.
 * @param header       Pointer to the CAN transmit header structure.
 * @param data         Array containing the data to send.
 * @param delay_ticks  Maximum delay to wait if queue is full (FreeRTOS ticks).
 *
 * @return can_status_t Returns CAN_SENT if message was successfully sent or queued,
 *                      CAN_ERR on failure.
 */
can_status_t can_send(CAN_HandleTypeDef* handle,
                      const CAN_TxHeaderTypeDef* header, const uint8_t data[],
                      TickType_t delay_ticks);

/**
 * @brief Receives a CAN message.
 *
 * Reads a message from the receive queue corresponding to the specified ID.
 * Supports blocking or non-blocking behavior depending on delay_ticks.
 *
 * @param handle       Pointer to the CAN handle structure.
 * @param id           CAN identifier of the message to receive.
 * @param header       Pointer to a CAN_RxHeaderTypeDef structure to store the received header.
 * @param data         Array to store the received data.
 * @param delay_ticks  Maximum delay to wait if queue is empty (FreeRTOS ticks).
 *
 * @return can_status_t Returns CAN_RECV if a message was received,
 *                      CAN_EMPTY if the queue was empty,
 *                      CAN_ERR on failure or invalid ID.
 */
can_status_t can_recv(CAN_HandleTypeDef* handle, uint16_t id,
                      CAN_RxHeaderTypeDef* header, uint8_t data[],
                      TickType_t delay_ticks);

#endif /* CAN_H */
