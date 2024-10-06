#include "BSP_UART.h"
#include "FreeRTOS.h"
#include "queue.h"

// Define static queues for TX and RX
static QueueHandle_t txQueue;
static QueueHandle_t rxQueue;


// Define a structure to hold message with ID
typedef struct {
    uint32_t  bus;  // UART bus
    uint8_t *data;
    uint16_t length;
} UARTMessage;

static UART_HandleTypeDef huart;

HAL_StatusTypeDef BSP_UART_Init(UART_HandleTypeDef *device)
{
// TODO
}

HAL_StatusTypeDef BSP_UART_Write(char* data, uint32_t length, uint32_t id)
{
    // TODO
}

HAL_StatusTypeDef BSP_UART_Read(char* data, uint32_t len, uint32_t *id, uint32_t timeout)
{
    // TODO
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // TODO
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // TODO
}
