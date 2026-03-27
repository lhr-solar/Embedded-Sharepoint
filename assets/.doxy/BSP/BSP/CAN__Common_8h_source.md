

# File CAN\_Common.h

[**File List**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN\_Common.h**](CAN__Common_8h.md)

[Go to the documentation of this file](CAN__Common_8h.md)


```C++
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
  uint32_t id;
  uint16_t size;
  QueueHandle_t queue;
  uint8_t* storage;
  bool circular;
  StaticQueue_t buffer;
} can_recv_entry_t;

#if defined(CAN1) || defined(FDCAN1)
extern can_recv_entry_t can1_recv_entries[];
extern const uint32_t can1_recv_entry_count;
#endif /* CAN1 || FDCAN1 */

#if defined(CAN2) || defined(FDCAN2)
extern can_recv_entry_t can2_recv_entries[];
extern const uint32_t can2_recv_entry_count;
#endif /* CAN2 || FDCAN2 */

#if defined(CAN3) || defined(FDCAN3)
extern can_recv_entry_t can3_recv_entries[];
extern const uint32_t can3_recv_entry_count;
#endif /* CAN3 || FDCAN3 */
```


