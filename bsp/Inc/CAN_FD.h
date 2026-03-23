#pragma once

#include "CAN_Common.h"
#include "queue_ex.h"
#include "stm32xx_hal.h"

#if !defined(FDCAN1)
#error "[CONFIG] CAN FD not enabled on this chip."
#endif /* FDCAN1 */

// fdcan handlers
#ifdef FDCAN1
extern FDCAN_HandleTypeDef* hfdcan1;
#endif /* FDCAN1 */

#ifdef FDCAN2
extern FDCAN_HandleTypeDef* hfdcan2;
#endif /* FDCAN2 */

#ifdef FDCAN3
extern FDCAN_HandleTypeDef* hfdcan3;
#endif /* FDCAN3 */

/**
 * @brief Initializes the FDCAN peripheral.
 *
 * This function initializes the FDCAN peripheral, sets up send/receive queues,
 * configures the HAL FDCAN driver, applies the filter configuration, and
 * enables FDCAN interrupts.
 *
 * @param handle Pointer to the FDCAN handle structure.
 * @param filter Pointer to the FDCAN filter configuration structure.
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERR on failure.
 */
can_status_t can_fd_init(FDCAN_HandleTypeDef* handle, FDCAN_FilterTypeDef* filter);

/**
 * @brief Deinitializes the FDCAN peripheral.
 *
 * This function deinitializes the HAL FDCAN driver and disables FDCAN interrupts.
 *
 * @param handle Pointer to the FDCAN handle structure.
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERR on failure.
 */
can_status_t can_fd_deinit(FDCAN_HandleTypeDef* handle);

/**
 * @brief Starts the FDCAN peripheral.
 *
 * Activates the FDCAN peripheral so it can transmit and receive messages.
 *
 * @param handle Pointer to the FDCAN handle structure.
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERR on failure.
 */
can_status_t can_fd_start(FDCAN_HandleTypeDef* handle);

/**
 * @brief Sends a FDCAN message.
 *
 * Places a FDCAN message into the transmit mailbox if available, otherwise
 * queues it in the send queue for later transmission.
 *
 * @param handle       Pointer to the FDCAN handle structure.
 * @param header       Pointer to the FDCAN transmit header structure.
 * @param data         Array containing the data to send.
 * @param delay_ticks  Maximum delay to wait if queue is full (FreeRTOS ticks).
 *
 * @return can_status_t Returns CAN_OK if message was successfully sent or queued,
 *                      CAN_ERR on failure.
 */
can_status_t can_fd_send(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, uint8_t data[],
                         TickType_t delay_ticks);

/**
 * @brief Sends a FDCAN message (but safe from isr context).
 *
 * Adds it to the send queue for later transmission.
 *
 * @param handle                    Pointer to the FDCAN handle structure.
 * @param header                    Pointer to the FDCAN transmit header structure.
 * @param data                      Array containing the data to send.
 * @param delay_ticks               Maximum delay to wait if queue is full (FreeRTOS ticks)
 * @param higherPriorityTaskWoken   Signals if ISR woke higher-priority task
 *
 * @return car_status_t Returns CAN_OK if message was successfully sent or queued,
 *                      CAN_ERR on failure.
 */
can_status_t can_fd_recv_isr(FDCAN_HandleTypeDef* handle, uint16_t id,
                             FDCAN_RxHeaderTypeDef* header, uint8_t data[],
                             BaseType_t* higherPriorityTaskWoken);

/**
 * @brief Receives a FDCAN message.
 *
 * Reads a message from the receive queue corresponding to the specified ID.
 * Supports blocking or non-blocking behavior depending on delay_ticks.
 *
 * @param handle       Pointer to the FDCAN handle structure.
 * @param id           CAN identifier of the message to receive.
 * @param header       Pointer to a FDCAN_RxHeaderTypeDef structure to store the received header.
 * @param data         Array to store the received data.
 * @param delay_ticks  Maximum delay to wait if queue is empty (FreeRTOS ticks).
 *
 * @return can_status_t Returns CAN_OK if a message was received,
 *                      CAN_EMPTY if the queue was empty,
 *                      CAN_ERR on failure or invalid ID.
 */
can_status_t can_fd_recv(FDCAN_HandleTypeDef* handle, uint16_t id, FDCAN_RxHeaderTypeDef* header,
                         uint8_t data[], TickType_t delay_ticks);

/**
 * @brief Receives a FDCAN message (but safe from isr context).
 *
 * Reads a message from the receive queue corresponding to the specified ID.
 *
 * @param handle                    Pointer to the FDCAN handle structure.
 * @param id                        CAN identifier of the message to receive.
 * @param header                    Pointer to FDCAN_RxHeaderTypeDef struct to store received header
 * @param data                      Array to store the received data.
 * @param higherPriorityTaskWoken   Signals if ISR woke higher-priority task
 */
can_status_t can_fd_recv_isr(FDCAN_HandleTypeDef* handle, uint16_t id,
                             FDCAN_RxHeaderTypeDef* header, uint8_t data[],
                             BaseType_t* higherPriorityTaskWoken)

#if (configUSE_QUEUE_SETS == 1)
/**
 * @brief Adds a set of IDs to a user-defined queue set
 *
 *
 * @param handle       Pointer to the FDCAN handle structure.
 * @param set          Pointer to an already allocated can_id_set_t struct
 *
 * @return can_status_t Returns CAN_OK if all IDs were added correctly,
 *                      CAN_ERR on failure.
 */
can_status_t can_fd_register_id_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set);

/**
 * @brief Blocks on a queue set until a message is available on one of the member queues.
 *
 * @param handle       Pointer to the FDCAN handle structure.
 * @param set          Pointer to an already allocated and registered can_id_set_t struct.
 * @param id           Pointer to a variable to store the CAN ID of the ready queue.
 * @param delay_ticks  Maximum delay to wait if no message is available (FreeRTOS ticks).
 *
 * @return can_status_t Returns CAN_OK if an ID was retrieved, CAN_EMPTY on timeout, or CAN_ERR on
 * failure.
 */
can_status_t can_fd_recv_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set, uint16_t* id,
                             TickType_t delay_ticks);

#endif /* ( configUSE_QUEUE_SETS == 1 ) */

void can_fd_tx_callback_hook(FDCAN_HandleTypeDef* hfdcan, const can_tx_payload_t* payload);

void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
                             can_rx_payload_t recv_payload);
