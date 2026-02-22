#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "UART.h"

extern int printf ( const char * format, ... );

void task_print_init(UART_HandleTypeDef *huart);
void task_print(void *huart);
