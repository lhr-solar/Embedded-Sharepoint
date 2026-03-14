#include "printf.h"

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

UART_HandleTypeDef *printf_huart = NULL;

#define MAX_PRINTF_SIZE 512

/**
 * @brief Initializes printf
 * @param huart pointer to the UART handle
 * @return bool true if success
 */
bool printf_init(UART_HandleTypeDef *huart) { 
    printf_huart = huart; 
    return uart_init(huart) == UART_OK;
}

// Print to a buffer
int snprintf(char *buffer, size_t bufsz, char const *fmt, ...){
    va_list val;
    va_start(val, fmt);
    int const rv = npf_vsnprintf(buffer, bufsz, fmt, val);
    va_end(val);
    return rv;
}

static char printf_buf[MAX_PRINTF_SIZE];

// Regular old printf
int printf(const char *fmt, ...) { 
    va_list val;
    va_start(val, fmt);
    int const rv = npf_vsnprintf(printf_buf, MAX_PRINTF_SIZE, fmt, val);
    va_end(val);

    volatile uart_status_t ret = uart_send(printf_huart, (const uint8_t*)printf_buf, rv+1, portMAX_DELAY);
    UNUSED(ret);

    return rv;
}
