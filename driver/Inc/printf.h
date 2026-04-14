#pragma once

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#include <stdbool.h>
#include "UART.h"

int printf(const char *fmt, ...);
int snprintf(char *buffer, size_t bufsz, char const *fmt, ...);

bool printf_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_uart_tx);
