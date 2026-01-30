#pragma once

#include <stdbool.h>
#include "UART.h"

extern int printf ( const char * format, ... );
extern int scanf  ( const char * format, ... );

bool printf_init(UART_HandleTypeDef *huart);
