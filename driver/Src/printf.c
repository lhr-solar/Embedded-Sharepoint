#include "printf.h"
#include "UART.h"
#include "stm32xx_hal.h"
#include <string.h>

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

#ifndef MAX_PRINTF_SIZE
#define MAX_PRINTF_SIZE (256)
#endif

#ifndef NUM_PRINTF_BUFFERS
#define NUM_PRINTF_BUFFERS (5)
#endif

// How long to wait for a printf buffer to open up
#ifndef PRINTF_MTX_DELAY
#define PRINTF_MTX_DELAY (pdMS_TO_TICKS(100))
#endif

typedef struct {
    char buffer[MAX_PRINTF_SIZE];
    SemaphoreHandle_t buffer_mtx;
    StaticSemaphore_t buffer_mtx_buffer;
} printf_buffer_t;

printf_buffer_t printf_pool[NUM_PRINTF_BUFFERS];
uint8_t printf_pool_next = 0;

SemaphoreHandle_t printf_pool_mtx;
StaticSemaphore_t printf_pool_mtx_buf;

UART_HandleTypeDef *printf_huart = NULL;

/**
 * @brief Initializes printf
 * @param huart pointer to the UART handle
 * @return bool true if success
 */
bool printf_init(UART_HandleTypeDef *huart) { 
    printf_huart = huart;

    printf_pool_mtx = xSemaphoreCreateMutexStatic(&printf_pool_mtx_buf);

    for(int i=0; i<NUM_PRINTF_BUFFERS; i++){
        printf_pool[i].buffer_mtx = xSemaphoreCreateBinaryStatic(&printf_pool[i].buffer_mtx_buffer); // has to be a binary semaphore rather than a mutex, since released by interrupt (not owning thread)
        xSemaphoreGive(printf_pool[i].buffer_mtx); // start at 1
    }
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

// Regular old printf
int printf(const char *fmt, ...) {
    printf_buffer_t *pbuf = pvTaskGetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 0);
    if(pbuf == NULL){
        // only if this task is not assigned a buffer, assign it a buffer
        // we must use a separate lock here for the "next pool entry" variable
        if(xSemaphoreTake(printf_pool_mtx, portMAX_DELAY) != pdTRUE) return -1;
        vTaskSetThreadLocalStoragePointer(xTaskGetCurrentTaskHandle(), 
                                        0, 
                                        (void *)(&printf_pool[printf_pool_next]));

        pbuf = &printf_pool[printf_pool_next];
        printf_pool_next = (printf_pool_next + 1) % NUM_PRINTF_BUFFERS;
        xSemaphoreGive(printf_pool_mtx);
    }

    va_list val;
    va_start(val, fmt);

    // Lock on the specific buffer's mutex (if it's shared between threads)
    if(xSemaphoreTake(pbuf->buffer_mtx, PRINTF_MTX_DELAY) != pdTRUE){
        va_end(val);
        return -1;
    }

    int const rv = npf_vsnprintf(pbuf->buffer, MAX_PRINTF_SIZE, fmt, val);
    va_end(val);
    
    if(rv <= 0){
        xSemaphoreGive(pbuf->buffer_mtx);
        return rv;
    }

    // Should release the buffer mtx once transmission is complete, preventing any changes
    uart_status_t status = uart_send_buf(printf_huart, (const uint8_t *)pbuf->buffer, (rv > MAX_PRINTF_SIZE)?MAX_PRINTF_SIZE:rv, pbuf->buffer_mtx, portMAX_DELAY);
    return (status == UART_OK)?rv:-1;
}
