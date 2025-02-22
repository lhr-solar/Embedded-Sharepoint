#include "CAN.h"

// 8 for now unless extended payload is supported
#define DATA_SIZE (8)

// entries in queues
typedef struct {
  CAN_TxHeaderTypeDef header;
  uint8_t data[DATA_SIZE];
} tx_payload_t;

typedef struct {
  CAN_RxHeaderTypeDef header;
  uint8_t data[DATA_SIZE];
} rx_payload_t;

// metadata for recieve queues
typedef struct {
  uint16_t id;
  uint16_t size;
  QueueHandle_t queue;
  uint8_t* storage;
  StaticQueue_t buffer;
} recv_entry_t;

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
static uint8_t
    can1_send_queue_storage[CAN1_SEND_QUEUE_SIZE * sizeof(tx_payload_t)];

#if __has_include("can1_recv_entries.h")
// create can1 recv queue storage
#define CAN_RECV_ENTRY(ID_, SIZE_) \
  static uint8_t can1_recv_queue_storage_##ID_[SIZE_ * sizeof(rx_payload_t)];

#include "can1_recv_entries.h"

#undef CAN_RECV_ENTRY

// create can1 recv queue array
#define CAN_RECV_ENTRY(ID_, SIZE_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = can1_recv_queue_storage_##ID_, \
   .buffer = {{0}}},

static recv_entry_t can1_recv_entries[] = {
#include "can1_recv_entries.h"
};
#undef CAN_RECV_ENTRY

// calculate amount of can1 recv entries
static const uint32_t can1_recv_entry_count =
    sizeof(can1_recv_entries) / sizeof(can1_recv_entries[0]);

#else /* can1_recv_entries.h */
// create can1 recv queue array
static recv_entry_t can1_recv_entries[] = {};
// calculate amount of can1 recv entries
static const uint32_t can1_recv_entry_count = 0;
#endif /* can1_recv_entries.h */
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
static uint8_t
    can2_send_queue_storage[CAN2_SEND_QUEUE_SIZE * sizeof(tx_payload_t)];

#if __has_include("can2_recv_entries.h")
// create can2 recv queue storage
#define CAN_RECV_ENTRY(ID_, SIZE_) \
  static uint8_t can2_recv_queue_storage_##ID_[SIZE_ * sizeof(rx_payload_t)];

#include "can2_recv_entries.h"

#undef CAN_RECV_ENTRY

// create can2 recv queue array
#define CAN_RECV_ENTRY(ID_, SIZE_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = can2_recv_queue_storage_##ID_, \
   .buffer = {{0}}},

static recv_entry_t can2_recv_entries[] = {
#include "can2_recv_entries.h"
};
#undef CAN_RECV_ENTRY

// calculate amount of can2 recv entries
static const uint32_t can2_recv_entry_count =
    sizeof(can2_recv_entries) / sizeof(can2_recv_entries[0]);

#else /* can2_recv_entries.h */
// create can2 recv queue array
static recv_entry_t can2_recv_entries[] = {};
// calculate amount of can2 recv entries
static const uint32_t can2_recv_entry_count = 0;
#endif /* can2_recv_entries.h */
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
static uint8_t
    can3_send_queue_storage[CAN3_SEND_QUEUE_SIZE * sizeof(tx_payload_t)];

#if __has_include("can3_recv_entries.h")
// create recv queue storage
#define CAN_RECV_ENTRY(ID_, SIZE_) \
  static uint8_t recv_queue_storage_##ID_[SIZE_ * sizeof(rx_payload_t)];

#include "can3_recv_entries.h"

#undef CAN_RECV_ENTRY

// create can3 recv queue array
#define CAN_RECV_ENTRY(ID_, SIZE_)      \
  {.id = (ID_),                         \
   .size = (SIZE_),                     \
   .queue = NULL,                       \
   .storage = recv_queue_storage_##ID_, \
   .buffer = {{0}}},

static recv_entry_t can3_recv_entries[] = {
#include "can3_recv_entries.h"
};
#undef CAN_RECV_ENTRY

// calculate amount of can3 recv entries
static const uint32_t can3_recv_entry_count =
    sizeof(can3_recv_entries) / sizeof(can3_recv_entries[0]);

#else /* can3_recv_entries.h */
// create can3 recv queue array
static recv_entry_t can3_recv_entries[] = {};
// calculate amount of can3 recv entries
static const uint32_t can3_recv_entry_count = 0;
#endif /* can3_recv_entries.h */
#endif /* CAN3 */
    
// CAN MSP init
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {
  GPIO_InitTypeDef init = {0};

  // CAN1
  if (hcan->Instance == CAN1) {
    // enable clocks
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* enable gpio
    PA11 -> CAN1_RX
    PA12 -> CAN1_TX
    */
    init.Pin = GPIO_PIN_11;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &init);

    init.Pin = GPIO_PIN_12;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &init);

    // enable interrupts
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  }

  // CAN2
  #ifdef CAN2
  else if (hcan->Instance == CAN2) {
    // enable clocks
    __HAL_RCC_CAN2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* enable gpio
    PB12 -> CAN2_RX
    PB13 -> CAN2_TX
    */
    init.Pin = GPIO_PIN_12;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &init);

    init.Pin = GPIO_PIN_13;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &init);

    // enable interrupts
    HAL_NVIC_SetPriority(CAN2_TX_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
  }
  #endif /* CAN2 */

  // CAN3
  #ifdef CAN3
  else if (hcan->Instance == CAN3) {
    // enable clocks
    __HAL_RCC_CAN3_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* enable gpio
    PA8  -> CAN3_RX
    PA15 -> CAN3_TX
    */
    init.Pin = GPIO_PIN_8;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF11_CAN3;
    HAL_GPIO_Init(GPIOA, &init);

    init.Pin = GPIO_PIN_15;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF11_CAN3;
    HAL_GPIO_Init(GPIOA, &init);

    // enable interrupts
    HAL_NVIC_SetPriority(CAN3_TX_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN3_TX_IRQn);
    HAL_NVIC_SetPriority(CAN3_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN3_RX0_IRQn);
  }
  #endif /* CAN3 */
}

// CAN MSP deinit
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan) {
  // CAN1
  if (hcan->Instance == CAN1) {
    // disable clocks
    __HAL_RCC_CAN1_CLK_DISABLE();

    /* disable gpio
    PA11 -> CAN1_RX
    PA12 -> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);

    // disable interrupts
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  }

  // CAN2
  #ifdef CAN2
  else if (hcan->Instance == CAN2) {
    // disable clocks
    __HAL_RCC_CAN2_CLK_DISABLE();

    /* disable gpio
    PB12 -> CAN2_RX
    PB13 -> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

    // disable interrupts
    HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
  }
  #endif /* CAN2 */

  // CAN3
  #ifdef CAN3
  else if (hcan->Instance == CAN3) {
    // disable clocks
    __HAL_RCC_CAN3_CLK_DISABLE();

    /* disable gpio
    PA8  -> CAN3_RX
    PB15 -> CAN3_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    // disable interrupts
    HAL_NVIC_DisableIRQ(CAN3_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN3_RX0_IRQn);
  }
  #endif /* CAN3 */
}

can_status_t can_init(CAN_HandleTypeDef* handle, CAN_FilterTypeDef* filter) {
  // CAN1
  if (handle->Instance == CAN1) {
    // init queues
    can1_send_queue =
        xQueueCreateStatic(CAN1_SEND_QUEUE_SIZE, sizeof(tx_payload_t),
                           can1_send_queue_storage, &can1_send_queue_buffer);
    for (int i = 0; i < can1_recv_entry_count; i++) {
      can1_recv_entries[i].queue = xQueueCreateStatic(
          can1_recv_entries[i].size, sizeof(rx_payload_t),
          can1_recv_entries[i].storage, &can1_recv_entries[i].buffer);
    }
  }

  // CAN2
  #ifdef CAN2
  else if (handle->Instance == CAN2) {
    // init queues
    can2_send_queue =
        xQueueCreateStatic(CAN2_SEND_QUEUE_SIZE, sizeof(tx_payload_t),
                           can2_send_queue_storage, &can2_send_queue_buffer);
    for (int i = 0; i < can2_recv_entry_count; i++) {
      can2_recv_entries[i].queue = xQueueCreateStatic(
          can2_recv_entries[i].size, sizeof(rx_payload_t),
          can2_recv_entries[i].storage, &can2_recv_entries[i].buffer);
    }
  }
  #endif /* CAN2 */

  // CAN3
  #ifdef CAN3
  else if (handle->Instance == CAN3) {
    // init queues
    can3_send_queue =
        xQueueCreateStatic(CAN3_SEND_QUEUE_SIZE, sizeof(tx_payload_t),
                           can3_send_queue_storage, &can3_send_queue_buffer);
    for (int i = 0; i < can3_recv_entry_count; i++) {
      can3_recv_entries[i].queue = xQueueCreateStatic(
          can3_recv_entries[i].size, sizeof(rx_payload_t),
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
  if (HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) !=
      HAL_OK) {
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
  if (HAL_CAN_DeactivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) !=
      HAL_OK) {
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

can_status_t can_recv(CAN_HandleTypeDef* handle, uint16_t id,
                      CAN_RxHeaderTypeDef* header, uint8_t data[],
                      TickType_t delay_ticks) {
  // recieve from queue matching id
  rx_payload_t payload = {0};
  bool valid_id = false;
  // CAN1
  if (handle->Instance == CAN1) {
    for (int i = 0; i < can1_recv_entry_count; i++) {
      if (can1_recv_entries[i].id == id) {
        valid_id = true;

        // if delay_ticks == portMAX_DELAY thread blocks, 
        // other values of delay_ticks are delays
        if (xQueueReceive(can1_recv_entries[i].queue, &payload, delay_ticks) ==
            errQUEUE_EMPTY) {
          return CAN_EMPTY;
        }
  
        break;
      }
    }
  }

  // CAN2
  #ifdef CAN2
  else if (handle->Instance == CAN2) {
    for (int i = 0; i < can2_recv_entry_count; i++) {
      if (can2_recv_entries[i].id == id) {
        valid_id = true;

        // if delay_ticks == portMAX_DELAY thread blocks, 
        // other values of delay_ticks are delays
        if (xQueueReceive(can2_recv_entries[i].queue, &payload, delay_ticks) ==
            errQUEUE_EMPTY) {
          return CAN_EMPTY;
        }
  
        break;
      }
    }
  }
  #endif /* CAN2 */

  // CAN3
  #ifdef CAN3
  else if (handle->Instance == CAN3) {
    for (int i = 0; i < can3_recv_entry_count; i++) {
      if (can3_recv_entries[i].id == id) {
        valid_id = true;

        // if delay_ticks == portMAX_DELAY thread blocks, 
        // other values of delay_ticks are delays
        if (xQueueReceive(can3_recv_entries[i].queue, &payload, delay_ticks) ==
            errQUEUE_EMPTY) {
          return CAN_EMPTY;
        }
  
        break;
      }
    }
  }
  #endif /* CAN3 */

  else {
    return CAN_ERR;
  }

  // decode payload if it is valid and message recieved
  if (valid_id) {
    *header = payload.header;
    for (int i = 0; i < DATA_SIZE; i++) {
      data[i] = payload.data[i];
    }

    return CAN_RECV;

  } else {
    return CAN_ERR;
  }
}

can_status_t can_send(CAN_HandleTypeDef* handle,
                      const CAN_TxHeaderTypeDef* header, const uint8_t data[],
                      TickType_t delay_ticks) {

  // disable interrupts (do not want race conditions
  // on shared resource (mailbox) between threads and
  // interrupt routines (TxComplete))
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
    
    tx_payload_t payload = {0};
    payload.header = *header;
    for (int i = 0; i < DATA_SIZE; i++) {
      payload.data[i] = data[i];
    }

    // CAN1
    if (handle->Instance == CAN1) {
      if (xQueueSend(can1_send_queue, &payload, delay_ticks) != pdTRUE) {
        return CAN_ERR;
      }
    }

    // CAN2
    #ifdef CAN2
    else if (handle->Instance == CAN2) {
      if (xQueueSend(can2_send_queue, &payload, delay_ticks) != pdTRUE) {
        return CAN_ERR;
      }
    }
    #endif /* CAN2 */

    // CAN3
    #ifdef CAN3
    else if (handle->Instance == CAN3) {
      if (xQueueSend(can3_send_queue, &payload, delay_ticks) != pdTRUE) {
        return CAN_ERR;
      }
    }
    #endif /* CAN3 */
  }

  return CAN_SENT;
}

static void transmit(CAN_HandleTypeDef* handle) {
  tx_payload_t payload = {0};
  BaseType_t higherPriorityTaskWoken = pdFALSE;

  // receive data from send queue
  bool success = false;
  // CAN1
  if (handle->Instance == CAN1) {
    if (xQueueReceiveFromISR(can1_send_queue, &payload,
                             &higherPriorityTaskWoken) == pdTRUE) {
      success = true;
    }
  }

  // CAN2
  #ifdef CAN2
  else if (handle->Instance == CAN2) {
    if (xQueueReceiveFromISR(can2_send_queue, &payload,
                             &higherPriorityTaskWoken) == pdTRUE) {
      success = true;
    }
  }
  #endif /* CAN2 */

  // CAN3
  #ifdef CAN3
  else if (handle->Instance == CAN3) {
    if (xQueueReceiveFromISR(can3_send_queue, &payload,
                             &higherPriorityTaskWoken) == pdTRUE) {
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

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan) {
  transmit(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan) {
  transmit(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan) {
  transmit(hcan);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
  rx_payload_t payload = {0};
  BaseType_t higherPriorityTaskWoken = pdFALSE;

  // recieve messages from queue till empty and put into recieve queues
  while (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &payload.header,
                              payload.data) == HAL_OK) {
    // CAN1
    if (hcan->Instance == CAN1) {
      for (int i = 0; i < can1_recv_entry_count; i++) {
        if (can1_recv_entries[i].id == payload.header.StdId) {
          xQueueSendFromISR(can1_recv_entries[i].queue, &payload,
                            &higherPriorityTaskWoken);
	  break;
        }
      }
    }

    // CAN2
    #ifdef CAN2
    else if (hcan->Instance == CAN2) {
      for (int i = 0; i < can2_recv_entry_count; i++) {
        if (can2_recv_entries[i].id == payload.header.StdId) {
          xQueueSendFromISR(can2_recv_entries[i].queue, &payload,
                            &higherPriorityTaskWoken);
	  break;
        }
      }
    }
    #endif /* CAN2 */

    // CAN3
    #ifdef CAN3
    if (hcan->Instance == CAN3) {
      for (int i = 0; i < can3_recv_entry_count; i++) {
        if (can3_recv_entries[i].id == payload.header.StdId) {
          xQueueSendFromISR(can3_recv_entries[i].queue, &payload,
                            &higherPriorityTaskWoken);
	  break;
        }
      }
    }
    #endif /* CAN3 */
  }

  portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

// CAN1
void CAN1_TX_IRQHandler(void) { HAL_CAN_IRQHandler(hcan1); }
void CAN1_RX0_IRQHandler(void) { HAL_CAN_IRQHandler(hcan1); }

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
