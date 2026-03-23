#include "CAN.h"
#include "queue_ex.h"
#include <string.h>

#ifdef CAN1
// fallback can1 send queue size
#ifndef CAN1_SEND_QUEUE_SIZE
#define CAN1_SEND_QUEUE_SIZE (10)
#endif

// can1 handle
static CAN_HandleTypeDef hcan1_ = {.Instance = CAN1};
CAN_HandleTypeDef* hcan1 = &hcan1_;

// can1 send queue
static QueueHandle_t can1_send_queue = NULL;
static StaticQueue_t can1_send_queue_buffer;
static uint8_t can1_send_queue_storage[CAN1_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif /* CAN1 */

#ifdef CAN2
// fallback can2 send queue size
#ifndef CAN2_SEND_QUEUE_SIZE
#define CAN2_SEND_QUEUE_SIZE (10)
#endif

// can2 handle
static CAN_HandleTypeDef hcan2_ = {.Instance = CAN2};
CAN_HandleTypeDef* hcan2 = &hcan2_;

// can2 send queue
static QueueHandle_t can2_send_queue = NULL;
static StaticQueue_t can2_send_queue_buffer;
static uint8_t can2_send_queue_storage[CAN2_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif /* CAN2 */

#ifdef CAN3
// fallback can3 send queue size
#ifndef CAN3_SEND_QUEUE_SIZE
#define CAN3_SEND_QUEUE_SIZE (10)
#endif

// can3 handle
static CAN_HandleTypeDef hcan3_ = {.Instance = CAN3};
CAN_HandleTypeDef* hcan3 = &hcan3_;

// can3 send queue
static QueueHandle_t can3_send_queue = NULL;
static StaticQueue_t can3_send_queue_buffer;
static uint8_t can3_send_queue_storage[CAN3_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif /* CAN3 */

can_status_t can_init(CAN_HandleTypeDef* handle, CAN_FilterTypeDef* filter) {
    if (0) {
    }
#ifdef CAN1
    else if (handle->Instance == CAN1) {
        // init queues
        can1_send_queue = xQueueCreateStatic(CAN1_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                                             can1_send_queue_storage, &can1_send_queue_buffer);
        for (uint32_t i = 0; i < can1_recv_entry_count; i++) {
            can1_recv_entries[i].queue =
                xQueueCreateStatic(can1_recv_entries[i].size, sizeof(can_rx_payload_t),
                                   can1_recv_entries[i].storage, &can1_recv_entries[i].buffer);
        }
    }
#endif /* CAN1 */
#ifdef CAN2
    else if (handle->Instance == CAN2) {
        // init queues
        can2_send_queue = xQueueCreateStatic(CAN2_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                                             can2_send_queue_storage, &can2_send_queue_buffer);
        for (uint32_t i = 0; i < can2_recv_entry_count; i++) {
            can2_recv_entries[i].queue =
                xQueueCreateStatic(can2_recv_entries[i].size, sizeof(can_rx_payload_t),
                                   can2_recv_entries[i].storage, &can2_recv_entries[i].buffer);
        }
    }
#endif /* CAN2 */
#ifdef CAN3
    else if (handle->Instance == CAN3) {
        // init queues
        can3_send_queue = xQueueCreateStatic(CAN3_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                                             can3_send_queue_storage, &can3_send_queue_buffer);
        for (uint32_t i = 0; i < can3_recv_entry_count; i++) {
            can3_recv_entries[i].queue =
                xQueueCreateStatic(can3_recv_entries[i].size, sizeof(can_rx_payload_t),
                                   can3_recv_entries[i].storage, &can3_recv_entries[i].buffer);
        }
    }
#endif /* CAN3 */
    else {
        return CAN_ERR;
    }

    // init HAL
    if (HAL_CAN_Init(handle) != HAL_OK) {
        return CAN_ERR;
    }

    // init filter
    if (HAL_CAN_ConfigFilter(handle, filter) != HAL_OK) {
        return CAN_ERR;
    }

    // enable interrupts
    if (HAL_CAN_ActivateNotification(handle, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        return CAN_ERR;
    }
    if (HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        return CAN_ERR;
    }

    return CAN_OK;
}

can_status_t can_deinit(CAN_HandleTypeDef* handle) {
    // deinit HAL
    if (HAL_CAN_DeInit(handle) != HAL_OK) {
        return CAN_ERR;
    }

    // disable interrupts
    if (HAL_CAN_DeactivateNotification(handle, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        return CAN_ERR;
    }
    if (HAL_CAN_DeactivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        return CAN_ERR;
    }

    return CAN_OK;
}

can_status_t can_start(CAN_HandleTypeDef* handle) {
    if (HAL_CAN_Start(handle) != HAL_OK) {
        return CAN_ERR;
    }

    return CAN_OK;
}

can_status_t can_stop(CAN_HandleTypeDef* handle) {
    if (HAL_CAN_Stop(handle) != HAL_OK) {
        return CAN_ERR;
    }

    return CAN_OK;
}

can_status_t can_recv(CAN_HandleTypeDef* handle, uint16_t id, CAN_RxHeaderTypeDef* header,
                      uint8_t data[], TickType_t delay_ticks) {
    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }
    // recieve from queue matching id
    can_rx_payload_t payload = {0};
    bool valid_id = false;

    can_recv_entry_t* can_recv_entries = NULL;
    uint32_t can_recv_entry_count = 0;

    if (0) {
    }
#ifdef CAN1
    else if (handle->Instance == CAN1) {
        can_recv_entry_count = can1_recv_entry_count;
        can_recv_entries = can1_recv_entries;
    }
#endif
#ifdef CAN2
    else if (handle->Instance == CAN2) {
        can_recv_entry_count = can2_recv_entry_count;
        can_recv_entries = can2_recv_entries;
    }
#endif
#ifdef CAN3
    else if (handle->Instance == CAN3) {
        can_recv_entry_count = can3_recv_entry_count;
        can_recv_entries = can3_recv_entries;
    }
#endif

    if (can_recv_entries == NULL) {
        return CAN_ERR;
    }

    for (uint32_t i = 0; i < can_recv_entry_count; i++) {
        if (can_recv_entries[i].id == id) {
            // if delay_ticks == portMAX_DELAY thread blocks,
            // other values of delay_ticks are delays
            if (xQueueReceive(can_recv_entries[i].queue, &payload, delay_ticks) == errQUEUE_EMPTY) {
                return CAN_EMPTY;
            }

            // decode payload
            *header = payload.header;
            memcpy(data, payload.data, header->DLC);
            return CAN_OK;
        }
    }

    // id not found
    return CAN_ERR;
}

can_status_t can_send(CAN_HandleTypeDef* handle, const CAN_TxHeaderTypeDef* header,
                      const uint8_t data[], TickType_t delay_ticks) {
    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }

    // Define payload
    can_tx_payload_t payload = {0};
    payload.header = *header;
    memcpy(payload.data, data, header->DLC);

    // Optional callback for user to implement
    can_tx_callback_hook(handle, &payload);

    // disable interrupts (do not want race conditions) on shared resource 
    // (mailbox) between threads and interrupt routines (TxComplete))
    portENTER_CRITICAL();

    // if transmit is inactive, put payload into mailbox
    if (HAL_CAN_GetTxMailboxesFreeLevel(handle) >= 1) {
        uint32_t mailbox;
        if (HAL_CAN_AddTxMessage(handle, header, data, &mailbox) != HAL_OK) {
            // enable interrupts
            portEXIT_CRITICAL();

            return CAN_ERR;
        }

        // enable interrupts
        portEXIT_CRITICAL();
    }
    // otherwise, put into send queue
    else {
        // enable interrupts
        portEXIT_CRITICAL();

        // CAN1
        if (handle->Instance == CAN1) {
            if (xQueueSend(can1_send_queue, &payload, delay_ticks) != pdTRUE) {
                return CAN_ERR;
            }
        }
#ifdef CAN2
        else if (handle->Instance == CAN2) {
            if (xQueueSend(can2_send_queue, &payload, delay_ticks) != pdTRUE) {
                return CAN_ERR;
            }
        }
#endif /* CAN2 */
#ifdef CAN3
        else if (handle->Instance == CAN3) {
            if (xQueueSend(can3_send_queue, &payload, delay_ticks) != pdTRUE) {
                return CAN_ERR;
            }
        }
#endif /* CAN3 */
    }

    return CAN_OK;
}

can_status_t can_send_isr(CAN_HandleTypeDef* handle, const CAN_TxHeaderTypeDef* header,
                          const uint8_t data[], BaseType_t* higherPriorityTaskWoken) {
    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }

    can_tx_payload_t payload = {0};
    payload.header = *header;
    memcpy(payload.data, data, header->DLC);

    can_tx_callback_hook(handle, &payload);

    if (0) {
    }
#ifdef CAN1
    else if (handle->Instance == CAN1) {
        if (xQueueSendFromISR(can1_send_queue, &payload, higherPriorityTaskWoken) != pdTRUE) {
            return CAN_ERR;
        }
    }
#endif /* CAN1 */
#ifdef CAN2
    else if (handle->Instance == CAN2) {
        if (xQueueSendFromISR(can2_send_queue, &payload, higherPriorityTaskWoken) != pdTRUE) {
            return CAN_ERR;
        }
    }
#endif /* CAN2 */
#ifdef CAN3
    else if (handle->Instance == CAN3) {
        if (xQueueSendFromISR(can3_send_queue, &payload, higherPriorityTaskWoken) != pdTRUE) {
            return CAN_ERR;
        }
    }
#endif /* CAN3 */

    return CAN_OK;
}

can_status_t can_recv_isr(CAN_HandleTypeDef* handle, uint16_t id, CAN_RxHeaderTypeDef* header,
                          uint8_t data[], BaseType_t* higherPriorityTaskWoken) {
    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }

    can_rx_payload_t payload = {0};
    can_recv_entry_t* entries = NULL;
    uint32_t entry_count = 0;

    if (0) {
    }
#ifdef CAN1
    else if (handle->Instance == CAN1) {
        entries = can1_recv_entries;
        entry_count = can1_recv_entry_count;
    }
#endif /* CAN1 */
#ifdef CAN2
    else if (handle->Instance == CAN2) {
        entries = can2_recv_entries;
        entry_count = can2_recv_entry_count;
    }
#endif /* CAN2 */
#ifdef CAN3
    else if (handle->Instance == CAN3) {
        entries = can3_recv_entries;
        entry_count = can3_recv_entry_count;
    }
#endif /* CAN3 */

    if (entries == NULL) {
        return CAN_ERR;
    }

    for (uint32_t i = 0; i < entry_count; i++) {
        if (entries[i].id == id) {
            if (xQueueReceiveFromISR(entries[i].queue, &payload, higherPriorityTaskWoken) != pdTRUE) {
                return CAN_EMPTY;
            }

            *header = payload.header;
            memcpy(data, payload.data, header->DLC);
            return CAN_OK;
        }
    }

    return CAN_ERR;
}


__weak void can_tx_callback_hook(CAN_HandleTypeDef* hcan, const can_tx_payload_t* payload) {
    UNUSED(hcan);
    UNUSED(payload);
}

static void transmit(CAN_HandleTypeDef* handle) {
    can_tx_payload_t payload = {0};
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    // receive data from send queue
    bool success = false;
    // CAN1
    if (handle->Instance == CAN1) {
        if (xQueueReceiveFromISR(can1_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
            success = true;
        }
    }
#ifdef CAN2
    else if (handle->Instance == CAN2) {
        if (xQueueReceiveFromISR(can2_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
            success = true;
        }
    }
#endif /* CAN2 */
#ifdef CAN3
    else if (handle->Instance == CAN3) {
        if (xQueueReceiveFromISR(can3_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
            success = true;
        }
    }
#endif /* CAN3 */

    // add payload to mailbox
    if (success) {
        uint32_t mailbox;
        if (HAL_CAN_AddTxMessage(handle, &payload.header, payload.data, &mailbox) != HAL_OK) {
            // Handle transmission error (optional: log or retry mechanism)
            // treated as lost packet for now
        }
    }

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan) { transmit(hcan); }

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan) { transmit(hcan); }

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan) { transmit(hcan); }

__weak void can_rx_callback_hook(CAN_HandleTypeDef* hcan, const can_rx_payload_t* payload) {
    UNUSED(hcan);
    UNUSED(payload);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    can_rx_payload_t payload = {0};
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    // recieve messages from queue till empty and put into recieve queues
    while (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &payload.header, payload.data) == HAL_OK) {
        // Optional callback for user to implement
        can_rx_callback_hook(hcan, &payload);

        if (0) {
        }
#ifdef CAN1
        else if (hcan->Instance == CAN1) {
            for (uint32_t i = 0; i < can1_recv_entry_count; i++) {
                if (can1_recv_entries[i].id == payload.header.StdId) {
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
#endif
#ifdef CAN2
        else if (hcan->Instance == CAN2) {
            for (uint32_t i = 0; i < can2_recv_entry_count; i++) {
                if (can2_recv_entries[i].id == payload.header.StdId) {
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
#endif /* CAN2 */
#ifdef CAN3
        else if (hcan->Instance == CAN3) {
            for (uint32_t i = 0; i < can3_recv_entry_count; i++) {
                if (can3_recv_entries[i].id == payload.header.StdId) {
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
#endif /* CAN3 */
    }

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

#if (configUSE_QUEUE_SETS == 1)
can_status_t can_register_id_set(CAN_HandleTypeDef* handle, can_id_set_t* set) {
    if (handle == NULL || set == NULL) {
        return CAN_ERR;
    }

    can_recv_entry_t* entries = NULL;
    uint32_t entry_count = 0;

    if (0) {
    }
#ifdef CAN1
    else if (handle->Instance == CAN1) {
        entries = can1_recv_entries;
        entry_count = can1_recv_entry_count;
    }
#endif /* CAN1 */
#ifdef CAN2
    else if (handle->Instance == CAN2) {
        entries = can2_recv_entries;
        entry_count = can2_recv_entry_count;
    }
#endif /* CAN2 */
#ifdef CAN3
    else if (handle->Instance == CAN3) {
        entries = can3_recv_entries;
        entry_count = can3_recv_entry_count;
    }
#endif /* CAN3 */

    if (entries == NULL) {
        return CAN_ERR;
    }
    // go through all IDs in the given ID array
    for (uint32_t i = 0; i < set->id_count; i++) {
        bool found = false;
        // iterate through the list of internal entries to ensure the ID is
        // registered with the driver it MUST be an ID declared in the
        // can_recv_entires header file
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

can_status_t can_recv_set(CAN_HandleTypeDef* handle, can_id_set_t* set, uint16_t* id,
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
#ifdef CAN1
    else if (handle->Instance == CAN1) {
        entries = can1_recv_entries;
        entry_count = can1_recv_entry_count;
    }
#endif /* CAN1 */
#ifdef CAN2
    else if (handle->Instance == CAN2) {
        entries = can2_recv_entries;
        entry_count = can2_recv_entry_count;
    }
#endif /* CAN2 */
#ifdef CAN3
    else if (handle->Instance == CAN3) {
        entries = can3_recv_entries;
        entry_count = can3_recv_entry_count;
    }
#endif /* CAN3 */

    if (entries == NULL) {
        return CAN_ERR;
    }

    // iterate through all of the can recieve entries for that FDCAN
    for (uint32_t i = 0; i < entry_count; i++) {
        // find the
        if (entries[i].queue == ready_can_queue) {
            *id = entries[i].id;
            return CAN_OK;
        }
    }

    return CAN_ERR;
}

#endif /* ( configUSE_QUEUE_SETS == 1 ) */

// CAN1
#ifdef CAN1
void CAN1_TX_IRQHandler(void) { HAL_CAN_IRQHandler(hcan1); }
void CAN1_RX0_IRQHandler(void) { HAL_CAN_IRQHandler(hcan1); }
#endif /* CAN1 */

// CAN2
#ifdef CAN2
void CAN2_TX_IRQHandler(void) { HAL_CAN_IRQHandler(hcan2); }
void CAN2_RX0_IRQHandler(void) { HAL_CAN_IRQHandler(hcan2); }
#endif /* CAN2 */

// CAN3
#ifdef CAN3
void CAN3_TX_IRQHandler(void) { HAL_CAN_IRQHandler(hcan3); }
void CAN3_RX0_IRQHandler(void) { HAL_CAN_IRQHandler(hcan3); }
#endif /* CAN3 */
