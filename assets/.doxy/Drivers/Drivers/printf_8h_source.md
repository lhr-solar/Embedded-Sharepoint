

# File printf.h

[**File List**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**printf.h**](printf_8h.md)

[Go to the documentation of this file](printf_8h.md)


```C++
#pragma once

#include <stdbool.h>
#include "UART.h"

extern int printf ( const char * format, ... );
extern int scanf  ( const char * format, ... );

bool printf_init(UART_HandleTypeDef *huart);
```


