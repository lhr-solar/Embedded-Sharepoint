

# File printf.h

[**File List**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**printf.h**](printf_8h.md)

[Go to the documentation of this file](printf_8h.md)


```C++
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
char *uart_fgets(char *buffer, size_t maxsz);

bool printf_init(UART_HandleTypeDef *huart);
```


