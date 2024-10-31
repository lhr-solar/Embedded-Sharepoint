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
    YESSS
} CAN_Status_e;

#define CAN_DATA_SIZE (8)

typedef struct payload {
    CAN_TxHeaderTypeDef header;
    uint8_t data[CAN_DATA_SIZE];
} payload_t;

extern CAN_HandleTypeDef* can1_handle;

CAN_Status_e can_init(CAN_HandleTypeDef *handle, CAN_FilterTypeDef *filter);
CAN_Status_e can_send(CAN_HandleTypeDef *handle, const CAN_TxHeaderTypeDef *header, const uint8_t data[], bool blocking);

#endif /* CAN_H */
