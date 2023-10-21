#include "stm32f4xx_hal.h"
#include <stdint.h> 

typedef struct Listener {
    uint32_t id;
    QueueHandle_t *queue;
} Listener_t;