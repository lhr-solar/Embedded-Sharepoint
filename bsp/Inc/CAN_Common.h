#pragma once

#include "stm32xx_hal.h"

#if !defined(CAN1) && !defined(FDCAN1)
  #error "[CONFIG] CAN not enabled on this chip."
#endif

// Default statically allocated size per can payload
// If you want longer messages, redefine this macro
#ifndef MAX_CAN_DATA_SIZE
#define MAX_CAN_DATA_SIZE (8)
#endif

// return code for can driver
typedef enum {
    CAN_ERR,   // unsuccessful operation
    CAN_OK,
    CAN_EMPTY, // recieved nothing with no errors
} can_status_t;


#if ( configUSE_QUEUE_SETS == 1 )
// set to block on multiple IDs
typedef struct
{
    const uint32_t *ids;
    uint32_t id_count;
    QueueSetHandle_t queueSet;
} can_id_set_t;
#endif /* ( configUSE_QUEUE_SETS == 1 ) */

// entries in queues
typedef struct {
    // If we're using FDCAN (stm32g4xx), use FDCAN_TxHeaderTypeDef
#if defined(STM32G4xx)
    FDCAN_TxHeaderTypeDef header;
#else 
    CAN_TxHeaderTypeDef header;
#endif
    uint8_t data[MAX_CAN_DATA_SIZE];
} can_tx_payload_t;

typedef struct {
    // If we're using FDCAN (stm32g4xx), use FDCAN_RxHeaderTypeDef
#if defined(STM32G4xx)
    FDCAN_RxHeaderTypeDef header;
#else
    CAN_RxHeaderTypeDef header;
#endif
  uint8_t data[MAX_CAN_DATA_SIZE];
} can_rx_payload_t;

// metadata for recieve queues
typedef struct {
  uint16_t id;
  uint16_t size;
  QueueHandle_t queue;
  uint8_t* storage;
  bool circular;
  StaticQueue_t buffer;
} can_recv_entry_t;


#define CAN_ID_CONCAT_HELPER(prefix, id) prefix ## id
#define CAN_ID_CONCAT(prefix, id) CAN_ID_CONCAT_HELPER(prefix, id)

#if defined(CAN1) || defined(FDCAN1)

#if __has_include("can1_recv_entries.h")
// create can1 recv queue storage
#define CAN_RECV_ENTRY(ID_, SIZE_, CIRCULAR_) \
    static uint8_t CAN_ID_CONCAT(can1_recv_queue_storage_, ID_)[SIZE_ * sizeof(can_rx_payload_t)];
#include "can1_recv_entries.h"

#undef CAN_RECV_ENTRY

// create can1 recv queue array
#define CAN_RECV_ENTRY(ID_, SIZE_, CIRCULAR_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = CAN_ID_CONCAT(can1_recv_queue_storage_, ID_), \
   .circular = (CIRCULAR_), \
   .buffer = {{0}}},

static can_recv_entry_t can1_recv_entries[] = {
#include "can1_recv_entries.h"
};
#undef CAN_RECV_ENTRY

// calculate amount of can1 recv entries
static const uint32_t can1_recv_entry_count =
    sizeof(can1_recv_entries) / sizeof(can1_recv_entries[0]);

#else /* can1_recv_entries.h */
// create can1 recv queue array
__attribute__((unused)) static can_recv_entry_t can1_recv_entries[] = {};
// calculate amount of can1 recv entries
__attribute__((unused)) static const uint32_t can1_recv_entry_count = 0;
#endif /* can1_recv_entries.h */
#endif /* FDCAN1 || CAN1 */


#if defined(CAN2) || defined(FDCAN2)
#if __has_include("can2_recv_entries.h")
// create can2 recv queue storage
#define CAN_RECV_ENTRY(ID_, SIZE_, CIRCULAR_) \
    static uint8_t CAN_ID_CONCAT(can2_recv_queue_storage_, ID_)[SIZE_ * sizeof(can_rx_payload_t)];
#include "can2_recv_entries.h"

#undef CAN_RECV_ENTRY

// create can2 recv queue array
#define CAN_RECV_ENTRY(ID_, SIZE_, CIRCULAR_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = CAN_ID_CONCAT(can2_recv_queue_storage_, ID_), \
   .circular = (CIRCULAR_), \
   .buffer = {{0}}},

static can_recv_entry_t can2_recv_entries[] = {
#include "can2_recv_entries.h"
};
#undef CAN_RECV_ENTRY

// calculate amount of can2 recv entries
static const uint32_t can2_recv_entry_count =
    sizeof(can2_recv_entries) / sizeof(can2_recv_entries[0]);

#else /* can2_recv_entries.h */
// create can2 recv queue array
__attribute__((unused)) static can_recv_entry_t can2_recv_entries[] = {};
// calculate amount of can2 recv entries
__attribute__((unused)) static const uint32_t can2_recv_entry_count = 0;
#endif /* can2_recv_entries.h */
#endif /* FDCAN2 || CAN2 */


#if defined(CAN3) || defined(FDCAN3)

#if __has_include("can3_recv_entries.h")
// create recv queue storage
#define CAN_RECV_ENTRY(ID_, SIZE_, CIRCULAR_) \
    static uint8_t CAN_ID_CONCAT(can3_recv_queue_storage_, ID_)[SIZE_ * sizeof(can_rx_payload_t)];

#include "can3_recv_entries.h"

#undef CAN_RECV_ENTRY

// create can3 recv queue array
#define CAN_RECV_ENTRY(ID_, SIZE_, CIRCULAR_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = CAN_ID_CONCAT(can3_recv_queue_storage_, ID_), \
   .circular = (CIRCULAR_), \
   .buffer = {{0}}},

static can_recv_entry_t can3_recv_entries[] = {
#include "can3_recv_entries.h"
};
#undef CAN_RECV_ENTRY

// calculate amount of can3 recv entries
static const uint32_t can3_recv_entry_count =
    sizeof(can3_recv_entries) / sizeof(can3_recv_entries[0]);

#else /* can3_recv_entries.h */
// create can3 recv queue array
__attribute__((unused)) static can_recv_entry_t can3_recv_entries[] = {};
// calculate amount of can3 recv entries
__attribute__((unused)) static const uint32_t can3_recv_entry_count = 0;
#endif /* can3_recv_entries.h */
#endif /* FDCAN3 || CAN3 */
