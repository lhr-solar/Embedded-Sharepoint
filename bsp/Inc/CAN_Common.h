#pragma once
#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"

#if !defined(CAN1) && !defined(FDCAN1)
  #error "[CONFIG] CAN not enabled on this chip."
#endif

// 8 for now unless extended payload is supported
#define CAN_DATA_SIZE (8)

// return code for can driver
typedef enum {
    CAN_ERR,   // unsuccessful operation
    CAN_OK,
    CAN_SENT,  // successful send
    CAN_RECV,  // successful recieve
    CAN_EMPTY, // recieved nothing with no errors
} can_status_t;

// entries in queues
typedef struct {
    // If we're using FDCAN (stm32g4xx), use FDCAN_TxHeaderTypeDef
#ifdef stm32g4xx
    FDCAN_TxHeaderTypeDef header;
#else 
    CAN_TxHeaderTypeDef header;
#endif
    uint8_t data[CAN_DATA_SIZE];
} can_tx_payload_t;

typedef struct {
    // If we're using FDCAN (stm32g4xx), use FDCAN_RxHeaderTypeDef
#ifdef stm32g4xx
    FDCAN_RxHeaderTypeDef header;
#else
    CAN_RxHeaderTypeDef header;
#endif
  uint8_t data[CAN_DATA_SIZE];
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

void can_recv_queue_init();