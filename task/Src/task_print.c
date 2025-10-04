#include "UART.h"
#include "stm32xx_hal.h"
#include <string.h>

#define MAX_PRINTF_SIZE  (512)
#define MAX_PRINTF_ITEMS (4)

#ifndef TASK_PRINT_DELAY
#define TASK_PRINT_DELAY (pdMS_TO_TICKS(1))
#endif

static QueueHandle_t print_tx_queue = NULL;
static StaticQueue_t print_tx_queue_buffer;
static uint8_t print_tx_queue_storage[MAX_PRINTF_SIZE * MAX_PRINTF_ITEMS];

static UART_HandleTypeDef *print_huart = NULL;

void task_print_init(UART_HandleTypeDef *huart){
  print_huart = huart;
  uart_init(print_huart);
  print_tx_queue = xQueueCreateStatic(MAX_PRINTF_ITEMS, MAX_PRINTF_SIZE, print_tx_queue_storage, &print_tx_queue_buffer);
}

void task_print(void *huart){
  static char print_item_buffer[MAX_PRINTF_SIZE];

  while(1) {
    if(xQueueReceive(print_tx_queue, (uint8_t *)&print_item_buffer[0], portMAX_DELAY) == pdPASS){
        uart_send(print_huart, (uint8_t *)print_item_buffer, strlen(print_item_buffer) + 1, portMAX_DELAY);
    }
    vTaskDelay(TASK_PRINT_DELAY);
  }
}

int _write(int file, char *ptr, int len) {
    (void)file;

    static char write_item_buffer[MAX_PRINTF_SIZE] = {0};
    strncpy(write_item_buffer, ptr, len);

    if(xQueueSend(print_tx_queue, (const uint8_t *)write_item_buffer, portMAX_DELAY) != pdPASS) return -1;

    return len;
}
