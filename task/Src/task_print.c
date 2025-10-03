#include "UART.h"
#include "stm32xx_hal.h"
#include <string.h>

#define MAX_PRINTF_SIZE  (512)
#define MAX_PRINTF_ITEMS (4)

static QueueHandle_t print_tx_queue = NULL;
static StaticQueue_t print_tx_queue_buffer;
static uint8_t print_tx_queue_storage[MAX_PRINTF_SIZE * MAX_PRINTF_ITEMS];

void print_task(void *huart){
  uart_init(huart);
  xQueueCreateStatic(MAX_PRINTF_ITEMS, MAX_PRINTF_SIZE, print_tx_queue_storage, &print_tx_queue_buffer);

  char print_item[MAX_PRINTF_SIZE];

  while(1) {
    if(xQueueReceive(print_tx_queue, (uint8_t *)print_item, portMAX_DELAY) == pdPASS){
        uart_send(huart, (uint8_t *)print_item, strlen(print_item), portMAX_DELAY);
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

int _write(int file, char *ptr, int len) {
    (void)file;

    char print_item[MAX_PRINTF_SIZE] = {0};
    strncpy(print_item, ptr, MAX_PRINTF_SIZE);

    uart_status_t ret = xQueueSend(print_tx_queue, (const uint8_t *)print_item, portMAX_DELAY);
    if(ret != UART_OK) return -1;

    return len;
}
