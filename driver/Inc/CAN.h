#ifndef CAN_H
#define CAN_H

typedef enum CAN_Status {
    OK = 0,
    INIT,
    CONFIG,
    START,
    SEMAPHORE_CREATE,
    QUEUE_CREATE,
    INVALID_INSTANCE,
    NOT_INITIALIZED,
    SEMAPHORE_TAKE,
    ADD_MESSAGE,
    QUEUE_SEND,
    ACTIVATE_NOTIFICATION,
    YESSS,
    BAD,
    ENTRY_NOT_FOUND
} CAN_Status_e;

#define CAN_DATA_SIZE (8)

typedef struct {
    CAN_TxHeaderTypeDef header;
    uint8_t data[CAN_DATA_SIZE];
} payload_tx_t;

typedef struct {
    CAN_RxHeaderTypeDef header;
    uint8_t data[CAN_DATA_SIZE];
} payload_rx_t;

typedef struct {
    uint32_t ID;
    uint32_t size;
    QueueHandle_t queue;
    uint8_t* queueStorage;
    StaticQueue_t queueControlBlock;
} can_recv_entry_t;


extern CAN_HandleTypeDef* can1_handle;

CAN_Status_e can_init(CAN_HandleTypeDef *handle, CAN_FilterTypeDef *filter);
CAN_Status_e can_send(CAN_HandleTypeDef *handle, const CAN_TxHeaderTypeDef *header, const uint8_t data[], bool blocking);
CAN_Status_e can_recv(CAN_HandleTypeDef *handle, uint32_t ID, payload_rx_t* payload, bool blocking);

#endif /* CAN_H */
