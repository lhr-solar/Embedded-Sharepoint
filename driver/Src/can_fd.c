#include "can_fd.h"
#include <string.h>

/* ponytail: default FDCAN pin map lifted from legacy examples; verify against board schematic (FDCAN1 PA11/PA12, FDCAN2 PB12/PB13, FDCAN3 PA8/PA15). */

/*
configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY is the maximum FreeRTOS priority
for an interrupt
*/
#define FDCAN_NVIC_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)

/* Standard-ID acceptance filter elements available in FDCAN message RAM
 * (SRAMCAN_FLS_NBR on STM32G4). One element per registered receive ID. */
#define CAN_MAX_STD_FILTERS (28)

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

/*
 * Low-level FDCAN init (pins, peripheral clock, NVIC) is provided by the project's
 * board file as a strong HAL_FDCAN_MspInit / HAL_FDCAN_MspDeInit (see test/bsp/
 * for an example). These empty __weak fallbacks let the driver link on boards
 * with no CAN bus.
 */
__weak void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle) {
    (void)fdcanHandle;
}

__weak void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle) {
    (void)fdcanHandle;
}

can_status_t can_fd_init(FDCAN_HandleTypeDef* handle) {
    /*
     * The hardware acceptance filter is derived from this instance's
     * CAN_RECV_ENTRY list: one standard-ID element per registered ID, with the
     * peripheral rejecting everything else. An ID therefore only wakes the
     * driver if it is a CAN_RECV_ENTRY, so anything that must be received has to
     * be listed there. The bootloader host-command IDs (CMD/DATA) are added to
     * FDCAN1 automatically by default — see the can1_recv_entries block below.
     *
     * Define CAN_FILTER_ACCEPT_ALL to bypass this and accept every standard ID
     * (the RX ISR still demuxes by ID, so unlisted IDs are simply dropped).
     */
#ifndef CAN_FILTER_ACCEPT_ALL
    can_recv_entry_t* entries = NULL;
    uint32_t count = 0;
    if (0) {  // placeholder if no FDCAN peripherals are enabled
    }
#ifdef FDCAN1
    else if (handle->Instance == FDCAN1) {
        entries = can1_recv_entries;
        count = can1_recv_entry_count;
    }
#endif /* FDCAN1 */
#ifdef FDCAN2
    else if (handle->Instance == FDCAN2) {
        entries = can2_recv_entries;
        count = can2_recv_entry_count;
    }
#endif /* FDCAN2 */
#ifdef FDCAN3
    else if (handle->Instance == FDCAN3) {
        entries = can3_recv_entries;
        count = can3_recv_entry_count;
    }
#endif /* FDCAN3 */
    if (entries == NULL) {
        return CAN_ERR;
    }
    handle->Init.StdFiltersNbr = count;  // entry count fits CAN_MAX_STD_FILTERS (static_assert below)
#else
    handle->Init.StdFiltersNbr = 1;
#endif /* CAN_FILTER_ACCEPT_ALL */

    if (HAL_FDCAN_Init(handle) != HAL_OK) {
        return CAN_ERR;
    }

#ifndef CAN_FILTER_ACCEPT_ALL
    for (uint32_t i = 0; i < count; i++) {
        FDCAN_FilterTypeDef f = {
            .IdType = FDCAN_STANDARD_ID,
            .FilterIndex = i,
            .FilterType = FDCAN_FILTER_MASK,
            .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
            .FilterID1 = entries[i].id,
            .FilterID2 = 0x7FF,  // mask: every ID bit must match -> exact ID
        };
        if (HAL_FDCAN_ConfigFilter(handle, &f) != HAL_OK) {
            return CAN_ERR;
        }
    }
    if (HAL_FDCAN_ConfigGlobalFilter(handle, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE,
                                     FDCAN_REJECT_REMOTE) != HAL_OK) {
        return CAN_ERR;
    }
#else
    FDCAN_FilterTypeDef accept_all = {
        .IdType = FDCAN_STANDARD_ID,
        .FilterIndex = 0,
        .FilterType = FDCAN_FILTER_MASK,
        .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
        .FilterID1 = 0x000,
        .FilterID2 = 0x000,  // mask 0 -> match every standard ID
    };
    if (HAL_FDCAN_ConfigFilter(handle, &accept_all) != HAL_OK) {
        return CAN_ERR;
    }
#endif /* CAN_FILTER_ACCEPT_ALL */

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
    
    // disable interrupts
    if (HAL_FDCAN_DeactivateNotification(
            handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_TX_COMPLETE) != HAL_OK) {
        return CAN_ERR;
    }
    
    // deinit HAL
    if (HAL_FDCAN_DeInit(handle) != HAL_OK) {
        return CAN_ERR;
    }

    return CAN_OK;
}

can_status_t can_fd_start(FDCAN_HandleTypeDef* handle) {
    // activate interrupts for rx and tx interrupts
    if (HAL_FDCAN_ConfigInterruptLines(handle, FDCAN_ILS_SMSG, FDCAN_INTERRUPT_LINE0) != HAL_OK) {
        return CAN_ERR;
    }

    // Interrupt triggers when a new message arrives in the rx fifo, a tranmission is complete, or the FDCAN peripheral experiences a Bus-Off event
    if (HAL_FDCAN_ActivateNotification(handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_TX_COMPLETE | FDCAN_IT_BUS_OFF,
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
        portEXIT_CRITICAL();
        return CAN_ERR;
    }

    portEXIT_CRITICAL();
    return CAN_OK;
}
// hardware mailbox is full, so must add to the queue
else {
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
    
    // If the HW FIFO has a free slot, write directly into it and return.
    // This mirrors can_fd_send and prevents the SW queue from never draining.
    
UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

// Check if there is a free can mailbox to send a message
if (HAL_FDCAN_GetTxFifoFreeLevel(handle) >= 1) {
    // if the mailbox is free, add the message to the hardware fifo
    if (HAL_FDCAN_AddMessageToTxFifoQ(handle, header, data) != HAL_OK) {
        // If adding to the can fd mailbox was not succesful
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
        return CAN_ERR;
    }

    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
    return CAN_OK;
}
// hardware mailbox is full, so must add to the queue
else {
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
 
        // HW FIFO full, fall back to the SW queue
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
}
    return CAN_OK;
}

can_status_t can_fd_recv(FDCAN_HandleTypeDef* handle, uint32_t id, FDCAN_RxHeaderTypeDef* header,
                         uint8_t data[], TickType_t delay_ticks) {

    if (handle == NULL || header == NULL || data == NULL) {
        return CAN_ERR;
    }

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
 
    // Yield if any queue send unblocked a higher-priority task
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
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
        if (xQueueReceiveFromISR(fdcan1_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {    // actually passing this in now??
            HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &payload.header, payload.data);
        }
    }
#endif
#ifdef FDCAN2
    else if (hfdcan->Instance == FDCAN2) {
        if (xQueueReceiveFromISR(fdcan2_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
            HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &payload.header, payload.data);
        }
    }
#endif
#ifdef FDCAN3
    else if (hfdcan->Instance == FDCAN3) {
        if (xQueueReceiveFromISR(fdcan3_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
            HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &payload.header, payload.data);
        }
    }
#endif
 
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

__weak void can_fd_error_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t ErrorStatusITs) {
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hfdcan);
    UNUSED(ErrorStatusITs);
}

// Automatically recover from Bus-Off event
void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
    can_fd_error_callback_hook(hfdcan, ErrorStatusITs);
    if ((ErrorStatusITs & FDCAN_IT_BUS_OFF) != 0) { // If Bus-Off error occurred
        hfdcan->Instance->CCCR &= ~FDCAN_CCCR_INIT; // Clear INIT bit to recover from Bus-Off
    }
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

// macros to help expand macros defined as IDs into numbers 
#define CAN_ID_CONCAT_HELPER(prefix, id) prefix ## id
#define CAN_ID_CONCAT(prefix, id) CAN_ID_CONCAT_HELPER(prefix, id)

/*
 * Each receive entry's queue storage is named by token-pasting its CAN ID, so a
 * plain CAN_RECV_ENTRY(ID, ...) requires ID to be a single token (a literal or a
 * macro that expands to one). Bridge/bootloader buses need arithmetic IDs such
 * as BL_CAN_ID_RESP_BASE + dev (see bl_can_filter.h), which cannot be pasted into
 * an identifier. CAN_RECV_ENTRY_KEYED takes a single-token KEY_ for the storage
 * name while ID_ stays an arbitrary integer expression; CAN_RECV_ENTRY keeps the
 * legacy behaviour (KEY_ == ID_). The per-bus blocks below (re)define
 * CAN_RECV_ENTRY_KEYED for each pass; this alias is defined once.
 */
#define CAN_RECV_ENTRY(ID_, SIZE_, CIRCULAR_) CAN_RECV_ENTRY_KEYED(ID_, ID_, SIZE_, CIRCULAR_)

/*
 * Bootloader host-command IDs are registered on FDCAN1 by default. The resident
 * CAN bootloader (see bootloader/) uses two shared host->device IDs — CMD
 * (0x010) and DATA (0x011) — so the application's hardware filter must accept
 * them, or a DFU/ENTER request gets dropped before the app can hand off to the
 * bootloader. They are appended to whatever the project lists in
 * can1_recv_entries.h, and an app reads them via can_fd_recv(hfdcan1,
 * BL_CAN_ID_CMD, ...). Do not also list 0x010/0x011 yourself.
 *
 * Define CAN_NO_BOOTLOADER_ENTRIES to opt out (board with no resident BL); the
 * defaults also vanish automatically if bl_protocol.h is not on the include path.
 */
#if defined(FDCAN1) && !defined(CAN_NO_BOOTLOADER_ENTRIES) && __has_include("bl_protocol.h")
#include "bl_protocol.h"
#ifndef BL_CAN_RECV_QUEUE_DEPTH
#define BL_CAN_RECV_QUEUE_DEPTH 4
#endif
#define CAN1_BL_RECV_ENTRIES                                      \
    CAN_RECV_ENTRY(BL_CAN_ID_CMD, BL_CAN_RECV_QUEUE_DEPTH, false) \
    CAN_RECV_ENTRY(BL_CAN_ID_DATA, BL_CAN_RECV_QUEUE_DEPTH, false)
#else
#define CAN1_BL_RECV_ENTRIES
#endif

#ifdef FDCAN1

// create can1 recv queue storage (bootloader defaults + project entries)
#define CAN_RECV_ENTRY_KEYED(KEY_, ID_, SIZE_, CIRCULAR_) \
    uint8_t CAN_ID_CONCAT(can1_recv_queue_storage_, KEY_)[SIZE_ * sizeof(can_rx_payload_t)];
CAN1_BL_RECV_ENTRIES
#if __has_include("can1_recv_entries.h")
#include "can1_recv_entries.h"
#endif
#undef CAN_RECV_ENTRY_KEYED

// create can1 recv queue array (bootloader defaults + project entries)
#define CAN_RECV_ENTRY_KEYED(KEY_, ID_, SIZE_, CIRCULAR_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = CAN_ID_CONCAT(can1_recv_queue_storage_, KEY_), \
   .circular = (CIRCULAR_), \
   .buffer = {{0}}},

__attribute__((unused)) can_recv_entry_t can1_recv_entries[] = {
CAN1_BL_RECV_ENTRIES
#if __has_include("can1_recv_entries.h")
#include "can1_recv_entries.h"
#endif
};
#undef CAN_RECV_ENTRY_KEYED

// calculate amount of can1 recv entries
__attribute__((unused)) const uint32_t can1_recv_entry_count =
    sizeof(can1_recv_entries) / sizeof(can1_recv_entries[0]);
_Static_assert(sizeof(can1_recv_entries) / sizeof(can1_recv_entries[0]) <= CAN_MAX_STD_FILTERS,
               "FDCAN1: more CAN_RECV_ENTRY IDs than standard hardware filters (see can_fd_init)");

#endif /* FDCAN1 */


#ifdef FDCAN2
#if __has_include("can2_recv_entries.h")
// create can2 recv queue storage
#define CAN_RECV_ENTRY_KEYED(KEY_, ID_, SIZE_, CIRCULAR_) \
    uint8_t CAN_ID_CONCAT(can2_recv_queue_storage_, KEY_)[SIZE_ * sizeof(can_rx_payload_t)];
#include "can2_recv_entries.h"

#undef CAN_RECV_ENTRY_KEYED

// create can2 recv queue array
#define CAN_RECV_ENTRY_KEYED(KEY_, ID_, SIZE_, CIRCULAR_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = CAN_ID_CONCAT(can2_recv_queue_storage_, KEY_), \
   .circular = (CIRCULAR_), \
   .buffer = {{0}}},

can_recv_entry_t can2_recv_entries[] = {
#include "can2_recv_entries.h"
};
#undef CAN_RECV_ENTRY_KEYED

// calculate amount of can2 recv entries
const uint32_t can2_recv_entry_count =
    sizeof(can2_recv_entries) / sizeof(can2_recv_entries[0]);
_Static_assert(sizeof(can2_recv_entries) / sizeof(can2_recv_entries[0]) <= CAN_MAX_STD_FILTERS,
               "FDCAN2: more CAN_RECV_ENTRY IDs than standard hardware filters (see can_fd_init)");

#else /* can2_recv_entries.h */
// create can2 recv queue array
__attribute__((unused)) can_recv_entry_t can2_recv_entries[] = {};
// calculate amount of can2 recv entries
__attribute__((unused)) const uint32_t can2_recv_entry_count = 0;
#endif /* can2_recv_entries.h */
#endif /* FDCAN2 */


#ifdef FDCAN3

#if __has_include("can3_recv_entries.h")
// create recv queue storage
#define CAN_RECV_ENTRY_KEYED(KEY_, ID_, SIZE_, CIRCULAR_) \
    uint8_t CAN_ID_CONCAT(can3_recv_queue_storage_, KEY_)[SIZE_ * sizeof(can_rx_payload_t)];

#include "can3_recv_entries.h"

#undef CAN_RECV_ENTRY_KEYED

// create can3 recv queue array
#define CAN_RECV_ENTRY_KEYED(KEY_, ID_, SIZE_, CIRCULAR_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = CAN_ID_CONCAT(can3_recv_queue_storage_, KEY_), \
   .circular = (CIRCULAR_), \
   .buffer = {{0}}},

can_recv_entry_t can3_recv_entries[] = {
#include "can3_recv_entries.h"
};
#undef CAN_RECV_ENTRY_KEYED

// calculate amount of can3 recv entries
const uint32_t can3_recv_entry_count =
    sizeof(can3_recv_entries) / sizeof(can3_recv_entries[0]);
_Static_assert(sizeof(can3_recv_entries) / sizeof(can3_recv_entries[0]) <= CAN_MAX_STD_FILTERS,
               "FDCAN3: more CAN_RECV_ENTRY IDs than standard hardware filters (see can_fd_init)");

#else /* can3_recv_entries.h */
// create can3 recv queue array
__attribute__((unused)) can_recv_entry_t can3_recv_entries[] = {};
// calculate amount of can3 recv entries
__attribute__((unused)) const uint32_t can3_recv_entry_count = 0;
#endif /* can3_recv_entries.h */
#endif /* FDCAN3 */
