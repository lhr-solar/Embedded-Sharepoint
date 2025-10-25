#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H

typedef enum {
    BSP_OK       = 0x00U,
    BSP_ERROR    = 0x01U,
    BSP_BUSY     = 0x02U,
    BSP_TIMEOUT  = 0x03U,
    BSP_QUEUE_FULL = 0x04U,
    BSP_QUEUE_EMPTY = 0x05U,
    BSP_MAILBOX_FULL = 0x06U,
    BSP_MAILBOX_EMPTY = 0x07U,
    BSP_INIT_ERROR = 0x08U,
} bsp_status_t;

#endif
