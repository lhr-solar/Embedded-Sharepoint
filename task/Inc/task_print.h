#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "UART.h"

extern int printf ( const char * format, ... );
void print_task(void *huart);
