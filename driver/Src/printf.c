#include "printf.h"

UART_HandleTypeDef *printf_huart = NULL;

/**
 * @brief Initializes printf
 * @param huart pointer to the UART handle
 * @return bool true if success
 */
bool printf_init(UART_HandleTypeDef *huart) { 
    printf_huart = huart; 
    return uart_init(huart) == UART_OK;
}

// Called by _read in syscalls.c
char __io_getchar() {
  if (printf_huart != NULL) {
    char data;
    uart_recv(printf_huart, (uint8_t *)&data, 1, portMAX_DELAY);
    return data;
  }
  return 0;
}

// Called by _write in syscalls.c
void __io_putchar(char data) {
  if (printf_huart != NULL) {
    uart_send(printf_huart, (const uint8_t *)&data, 1, portMAX_DELAY);
  }
}

