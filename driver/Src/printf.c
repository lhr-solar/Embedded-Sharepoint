#include "printf.h"
#include "portmacro.h"

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
int __io_getchar() {
    if (printf_huart != NULL) {
        uint8_t data;
        uart_status_t ret = uart_recv(printf_huart, &data, 1, portMAX_DELAY);
        if(ret != UART_OK) return -1;

        if(data == '\n'){
            const uint8_t buf[2] = "\\n";
            ret = uart_send(printf_huart, buf, 2, portMAX_DELAY);
            if(ret != UART_OK) return -1;
        } else if(data == '\r'){
            const uint8_t buf[2] = "\\r";
            ret = uart_send(printf_huart, buf, 2, portMAX_DELAY);
            if(ret != UART_OK) return -1;
        }

        ret = uart_send(printf_huart, &data, 1, portMAX_DELAY);
        if(ret != UART_OK) return -1;

        return data;
    }
    return -1;
}

// Called by _write in syscalls.c
int __io_putchar(int data) {
    if (printf_huart != NULL) {
        uart_status_t ret = uart_send(printf_huart, (const uint8_t *)&data, 1, portMAX_DELAY);
        if(ret != UART_OK) return -1;
        return data;
    }
    return -1;
}

