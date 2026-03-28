#include "CAN_Common.h"
#include "CAN_FD.h"
#include <string.h>

// Define CAN FD handles
#ifdef FDCAN1
static FDCAN_HandleTypeDef hfdcan1_ = {.Instance = FDCAN1};
FDCAN_HandleTypeDef* hfdcan1 = &hfdcan1_;
#endif

#ifdef FDCAN2
static FDCAN_HandleTypeDef hfdcan2_ = {.Instance = FDCAN2};
FDCAN_HandleTypeDef* hfdcan2 = &hfdcan2_;
#endif

#ifdef FDCAN3
static FDCAN_HandleTypeDef hfdcan3_ = {.Instance = FDCAN3};
FDCAN_HandleTypeDef* hfdcan3 = &hfdcan3_;
#endif

#ifdef FDCAN1
#ifndef FDCAN1_SEND_QUEUE_SIZE
#define FDCAN1_SEND_QUEUE_SIZE (10)
#endif /* FDCAN1_SEND_QUEUE_SIZE */

static QueueHandle_t fdcan1_send_queue = NULL;
static StaticQueue_t fdcan1_send_queue_buffer;
static uint8_t fdcan1_send_queue_storage[FDCAN1_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif /* FDCAN1 */

#ifdef FDCAN2
#ifndef FDCAN2_SEND_QUEUE_SIZE
#define FDCAN2_SEND_QUEUE_SIZE (10)
#endif /* FDCAN2_SEND_QUEUE_SIZE */

static QueueHandle_t fdcan2_send_queue = NULL;
static StaticQueue_t fdcan2_send_queue_buffer;
static uint8_t fdcan2_send_queue_storage[FDCAN2_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif /* FDCAN2 */

#ifdef FDCAN3
#ifndef FDCAN3_SEND_QUEUE_SIZE
#define FDCAN3_SEND_QUEUE_SIZE (10)
#endif /* FDCAN3_SEND_QUEUE_SIZE */

static QueueHandle_t fdcan3_send_queue = NULL;
static StaticQueue_t fdcan3_send_queue_buffer;
static uint8_t fdcan3_send_queue_storage[FDCAN3_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif /* FDCAN3 */

can_status_t can_fd_init(FDCAN_HandleTypeDef* handle, FDCAN_FilterTypeDef* filter) {
    if (HAL_FDCAN_Init(handle) != HAL_OK) {
        return CAN_ERR;
    }

    if (HAL_FDCAN_ConfigFilter(handle, filter) != HAL_OK) {
        return CAN_ERR;
    }

#ifdef FDCAN1
    if (handle->Instance == FDCAN1) {
        fdcan1_send_queue =
            xQueueCreateStatic(FDCAN1_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                               fdcan1_send_queue_storage, &fdcan1_send_queue_buffer);
        for (uint32_t i = 0; i < can1_recv_entry_count; i++) {
            can1_recv_entries[i].queue =
                xQueueCreateStatic(can1_recv_entries[i].size, sizeof(can_rx_payload_t),
                                   can1_recv_entries[i].storage, &can1_recv_entries[i].buffer);
        }
    }
#endif
#ifdef FDCAN2
    if (handle->Instance == FDCAN2) {
        fdcan2_send_queue =
            xQueueCreateStatic(FDCAN2_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                               fdcan2_send_queue_storage, &fdcan2_send_queue_buffer);

        for (uint32_t i = 0; i < can2_recv_entry_count; i++) {
            can2_recv_entries[i].queue =
                xQueueCreateStatic(can2_recv_entries[i].size, sizeof(can_rx_payload_t),
                                   can2_recv_entries[i].storage, &can2_recv_entries[i].buffer);
        }
    }
#endif
#ifdef FDCAN3
    if (handle->Instance == FDCAN3) {
        fdcan3_send_queue =
            xQueueCreateStatic(FDCAN3_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                               fdcan3_send_queue_storage, &fdcan3_send_queue_buffer);
        for (uint32_t i = 0; i < can3_recv_entry_count; i++) {
            can3_recv_entries[i].queue =
                xQueueCreateStatic(can3_recv_entries[i].size, sizeof(can_rx_payload_t),
                                   can3_recv_entries[i].storage, &can3_recv_entries[i].buffer);
        }
    }

#endif
    return CAN_OK;
}

can_status_t can_fd_deinit(FDCAN_HandleTypeDef* handle) {
    // deinit HAL
    if (HAL_FDCAN_DeInit(handle) != HAL_OK) {
        return CAN_ERR;
    }

    // disable interrupts
    if (HAL_FDCAN_DeactivateNotification(
            handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_TX_COMPLETE) != HAL_OK) {
        return CAN_ERR;
    }

    return CAN_OK;
}

can_status_t can_fd_start(FDCAN_HandleTypeDef* handle) {
    // activate interrupts for rx and tx interrupts
    if (HAL_FDCAN_ConfigInterruptLines(handle, FDCAN_ILS_SMSG, FDCAN_INTERRUPT_LINE0) != HAL_OK) {
        return CAN_ERR;
    }

    // Interrupt triggers when a new message in the rx fifo, and when a tranmission is complete
    if (HAL_FDCAN_ActivateNotification(handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_TX_COMPLETE,
                                       0) != HAL_OK) {
        return CAN_ERR;
    }

    // start the can peripheral
    if (HAL_FDCAN_Start(handle) != HAL_OK) {
        return CAN_ERR;
    }
    return CAN_OK;
}

can_status_t can_fd_send(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, uint8_t data[],
                         TickType_t delay_ticks) {
    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }

    // create CAN payload so we can forward it
    can_tx_payload_t payload = {0};
    payload.header = *header;
    memcpy(payload.data, data, FDCAN_BYTES_FROM_DLC(header->DataLength));

    // optional callback the user can implement (by default does nothing)
    can_fd_tx_callback_hook(handle, &payload);

    // disable interrupts while we check the status of the can mailboxes, so other interrupts don't
    // change it
    portENTER_CRITICAL();
    // Check if there is a free can mailbox to send a message
    if (HAL_FDCAN_GetTxFifoFreeLevel(handle) >= 1) {
        // if the mailbox is free, add the message to the hardware fifo
        if (HAL_FDCAN_AddMessageToTxFifoQ(handle, header, data) != HAL_OK) {
            // If adding to the can fd mailbox was not succesful

            // enable interrupts
            portEXIT_CRITICAL();
            return CAN_ERR;
        }
        // enable interrupts
        portEXIT_CRITICAL();
        return CAN_OK;
    }
    // hardware mailbox is full, so must add to the queue
    else {
        // enable interrupts
        portEXIT_CRITICAL();

#ifdef FDCAN1
        if (handle->Instance == FDCAN1) {
            if (xQueueSend(fdcan1_send_queue, &payload, delay_ticks) != pdTRUE) {
                return CAN_ERR;
            }
        }
#endif /* FDCAN1 */
#ifdef FDCAN2
        if (handle->Instance == FDCAN2) {
            if (xQueueSend(fdcan2_send_queue, &payload, delay_ticks) != pdTRUE) {
                return CAN_ERR;
            }
        }
#endif /* FDCAN2 */
#ifdef FDCAN3
        if (handle->Instance == FDCAN3) {
            if (xQueueSend(fdcan3_send_queue, &payload, delay_ticks) != pdTRUE) {
                return CAN_ERR;
            }
        }
#endif /* FDCAN3 */
    }
    return CAN_OK;
}


can_status_t can_fd_send_isr(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header,
                             uint8_t data[], BaseType_t* higherPriorityTaskWoken) {
    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }

    can_tx_payload_t payload = {0};
    payload.header = *header;
    memcpy(payload.data, data, FDCAN_BYTES_FROM_DLC(header->DataLength));

    if (0) {
    }
#ifdef FDCAN1
    else if (handle->Instance == FDCAN1) {
        if (xQueueSendFromISR(fdcan1_send_queue, &payload, higherPriorityTaskWoken) != pdTRUE) {
            return CAN_ERR;
        }
    }
#endif
#ifdef FDCAN2
    else if (handle->Instance == FDCAN2) {
        if (xQueueSendFromISR(fdcan2_send_queue, &payload, higherPriorityTaskWoken) != pdTRUE) {
            return CAN_ERR;
        }
    }
#endif
#ifdef FDCAN3
    else if (handle->Instance == FDCAN3) {
        if (xQueueSendFromISR(fdcan3_send_queue, &payload, higherPriorityTaskWoken) != pdTRUE) {
            return CAN_ERR;
        }
    }
#endif

    return CAN_OK;
}

can_status_t can_fd_recv(FDCAN_HandleTypeDef* handle, uint32_t id, FDCAN_RxHeaderTypeDef* header,
                         uint8_t data[], TickType_t delay_ticks) {
    can_rx_payload_t payload = {0};
    can_recv_entry_t* can_recv_entries = NULL;
    uint32_t can_recv_entry_count = 0;
    if (0) {  // placeholder if no FDCAN peripherals are enabled
    }
#ifdef FDCAN1
    else if (handle->Instance == FDCAN1) {
        can_recv_entry_count = can1_recv_entry_count;
        can_recv_entries = can1_recv_entries;
    }
#endif /* FDCAN1 */
#ifdef FDCAN2
    else if (handle->Instance == FDCAN2) {
        can_recv_entry_count = can2_recv_entry_count;
        can_recv_entries = can2_recv_entries;
    }
#endif /* FDCAN2 */
#ifdef FDCAN3
    else if (handle->Instance == FDCAN3) {
        can_recv_entry_count = can3_recv_entry_count;
        can_recv_entries = can3_recv_entries;
    }
#endif /* FDCAN3 */

    // no CAN FD peripherals enabled
    if (can_recv_entries == NULL) {
        return CAN_ERR;
    }

    for (uint32_t i = 0; i < can_recv_entry_count; i++) {
        if (can_recv_entries[i].id == id) {
            // if delay_ticks == portMAX_DELAY thread blocks,
            // other values of delay_ticks are delays
            if (xQueueReceive(can_recv_entries[i].queue, &payload, delay_ticks) ==
                errQUEUE_EMPTY) {
                return CAN_EMPTY;
            }

            // decode payload
            *header = payload.header;
            memcpy(data, payload.data, FDCAN_BYTES_FROM_DLC(header->DataLength));
            return CAN_OK;
        }
    }

    return CAN_ERR;
}

can_status_t can_fd_recv_isr(FDCAN_HandleTypeDef* handle, uint32_t id,
                             FDCAN_RxHeaderTypeDef* header, uint8_t data[],
                             BaseType_t* higherPriorityTaskWoken) {
    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }

    can_rx_payload_t payload = {0};
    can_recv_entry_t* entries = NULL;
    uint32_t entry_count = 0;

    if (0) {
    }
#ifdef FDCAN1
    else if (handle->Instance == FDCAN1) {
        entries = can1_recv_entries;
        entry_count = can1_recv_entry_count;
    }
#endif
#ifdef FDCAN2
    else if (handle->Instance == FDCAN2) {
        entries = can2_recv_entries;
        entry_count = can2_recv_entry_count;
    }
#endif
#ifdef FDCAN3
    else if (handle->Instance == FDCAN3) {
        entries = can3_recv_entries;
        entry_count = can3_recv_entry_count;
    }
#endif

    if (entries == NULL) {
        return CAN_ERR;
    }

    for (uint32_t i = 0; i < entry_count; i++) {
        if (entries[i].id == id) {
            if (xQueueReceiveFromISR(entries[i].queue, &payload, higherPriorityTaskWoken) !=
                pdTRUE) {
                return CAN_EMPTY;
            }

            *header = payload.header;
            memcpy(data, payload.data, FDCAN_BYTES_FROM_DLC(header->DataLength));
            return CAN_OK;
        }
    }

    return CAN_ERR;
}

#if (configUSE_QUEUE_SETS == 1)
can_status_t can_fd_register_id_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set) {
    if (handle == NULL || set == NULL) {
        return CAN_ERR;
    }

    can_recv_entry_t* entries = NULL;
    uint32_t entry_count = 0;

    if (0) {
    }
#ifdef FDCAN1
    else if (handle->Instance == FDCAN1) {
        entries = can1_recv_entries;
        entry_count = can1_recv_entry_count;
    }
#endif /* FDCAN1 */
#ifdef FDCAN2
    else if (handle->Instance == FDCAN2) {
        entries = can2_recv_entries;
        entry_count = can2_recv_entry_count;
    }
#endif /* FDCAN2 */
#ifdef FDCAN3
    else if (handle->Instance == FDCAN3) {
        entries = can3_recv_entries;
        entry_count = can3_recv_entry_count;
    }
#endif /* FDCAN3 */

    if (entries == NULL) {
        return CAN_ERR;
    }

    // go through all IDs in the given ID array
    for (uint32_t i = 0; i < set->id_count; i++) {
        bool found = false;
        // iterate through the list of internal entries to ensure the ID is registered with the
        // driver it MUST be an ID declared in the can_recv_entires header file
        for (uint32_t j = 0; j < entry_count; j++) {
            if (entries[j].id == set->ids[i]) {
                if (xQueueAddToSet(entries[j].queue, set->queueSet) != pdPASS) {
                    return CAN_ERR;
                }

                found = true;
                break;
            }
        }

        if (!found) {
            return CAN_ERR;
        }
    }

    return CAN_OK;
}

can_status_t can_fd_recv_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set, uint32_t* id,
                             TickType_t delay_ticks) {
    if (handle == NULL || set == NULL || id == NULL || set->queueSet == NULL) {
        return CAN_ERR;
    }

    QueueSetMemberHandle_t ready_can_queue = xQueueSelectFromSet(set->queueSet, delay_ticks);
    if (ready_can_queue == NULL) {
        return CAN_ERR;
    }
    can_recv_entry_t* entries = NULL;
    uint32_t entry_count = 0;

    // placeholder if statement so we can do do an else-if chain
    if (0) {
    }
#ifdef FDCAN1
    else if (handle->Instance == FDCAN1) {
        entries = can1_recv_entries;
        entry_count = can1_recv_entry_count;
    }
#endif /* FDCAN1 */
#ifdef FDCAN2
    else if (handle->Instance == FDCAN2) {
        entries = can2_recv_entries;
        entry_count = can2_recv_entry_count;
    }
#endif /* FDCAN2 */
#ifdef FDCAN3
    else if (handle->Instance == FDCAN3) {
        entries = can3_recv_entries;
        entry_count = can3_recv_entry_count;
    }
#endif /* FDCAN3 */

    if (entries == NULL) {
        return CAN_ERR;
    }

    // iterate through all of the can recieve entries for that FDCAN
    for (uint32_t i = 0; i < entry_count; i++) {
        if (entries[i].queue == ready_can_queue) {
            *id = entries[i].id;
            return CAN_OK;
        }
    }

    return CAN_ERR;
}
#endif /* ( configUSE_QUEUE_SETS == 1 ) */

__weak void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
                                    can_rx_payload_t recv_payload) {
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hfdcan);
    UNUSED(RxFifo0ITs);
    UNUSED(recv_payload);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs) {
    can_rx_payload_t payload = {0};
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
        while (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &payload.header, payload.data) ==
               HAL_OK) {
            can_fd_rx_callback_hook(hfdcan, RxFifo0ITs, payload);

            // placeholder if no FDCAN peripherals are enabled
            if (0) {
            }
#ifdef FDCAN1
            else if (hfdcan->Instance == FDCAN1) {
                for (uint32_t i = 0; i < can1_recv_entry_count; i++) {
                    if (can1_recv_entries[i].id == payload.header.Identifier) {
                        if (can1_recv_entries[i].circular) {
                            xQueueSendCircularBufferFromISR(can1_recv_entries[i].queue, &payload,
                                                            &higherPriorityTaskWoken,
                                                            sizeof(can_rx_payload_t));
                        } else {
                            xQueueSendFromISR(can1_recv_entries[i].queue, &payload,
                                              &higherPriorityTaskWoken);
                        }
                        break;
                    }
                }
            }
#endif /* FDCAN1 */
#ifdef FDCAN2
            else if (hfdcan->Instance == FDCAN2) {
                for (uint32_t i = 0; i < can2_recv_entry_count; i++) {
                    if (can2_recv_entries[i].id == payload.header.Identifier) {
                        // if we need to wrap around
                        if (can2_recv_entries[i].circular) {
                            xQueueSendCircularBufferFromISR(can2_recv_entries[i].queue, &payload,
                                                            &higherPriorityTaskWoken,
                                                            sizeof(can_rx_payload_t));
                        } else {
                            xQueueSendFromISR(can2_recv_entries[i].queue, &payload,
                                              &higherPriorityTaskWoken);
                        }
                        break;
                    }
                }
            }
#endif /* FDCAN2 */
#ifdef FDCAN3
            else if (hfdcan->Instance == FDCAN3) {
                for (uint32_t i = 0; i < can3_recv_entry_count; i++) {
                    if (can3_recv_entries[i].id == payload.header.Identifier) {
                        if (can3_recv_entries[i].circular) {
                            xQueueSendCircularBufferFromISR(can3_recv_entries[i].queue, &payload,
                                                            &higherPriorityTaskWoken,
                                                            sizeof(can_rx_payload_t));
                        } else {
                            xQueueSendFromISR(can3_recv_entries[i].queue, &payload,
                                              &higherPriorityTaskWoken);
                        }
                        break;
                    }
                }
            }
#endif /* FDCAN3 */
        }
    }
}

__weak void can_fd_tx_callback_hook(FDCAN_HandleTypeDef* hfdcan, const can_tx_payload_t* payload) {
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hfdcan);
    UNUSED(payload);
}

void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef* hfdcan, uint32_t BufferIndexes) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    can_tx_payload_t payload = {0};

    // placeholder if no FDCAN peripheral is enabled
    if (0) {
    }
#ifdef FDCAN1
    else if (hfdcan->Instance == FDCAN1) {
        // check if data in the queue to send
        if (xQueueReceiveFromISR(fdcan1_send_queue, &payload, NULL) == pdTRUE) {
            HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &payload.header, payload.data);
        }
    }
#endif
#ifdef FDCAN2
    else if (hfdcan->Instance == FDCAN2) {
        // check if data in the queue to send
        if (xQueueReceiveFromISR(fdcan2_send_queue, &payload, NULL) == pdTRUE) {
            HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &payload.header, payload.data);
        }
    }
#endif
#ifdef FDCAN3
    else if (hfdcan->Instance == FDCAN3) {
        // check if data in the queue to send
        if (xQueueReceiveFromISR(fdcan3_send_queue, &payload, NULL) == pdTRUE) {
            HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &payload.header, payload.data);
        }
    }
#endif

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

// defintions of FDCAN interrupt handlers
#ifdef FDCAN1
void FDCAN1_IT0_IRQHandler(void) { HAL_FDCAN_IRQHandler(hfdcan1); }
void FDCAN1_IT1_IRQHandler(void) { HAL_FDCAN_IRQHandler(hfdcan1); }
#endif

#ifdef FDCAN2
void FDCAN2_IT0_IRQHandler(void) { HAL_FDCAN_IRQHandler(hfdcan2); }
void FDCAN2_IT1_IRQHandler(void) { HAL_FDCAN_IRQHandler(hfdcan2); }
#endif

#ifdef FDCAN3
void FDCAN3_IT0_IRQHandler(void) { HAL_FDCAN_IRQHandler(hfdcan3); }
void FDCAN3_IT1_IRQHandler(void) { HAL_FDCAN_IRQHandler(hfdcan3); }
#endif