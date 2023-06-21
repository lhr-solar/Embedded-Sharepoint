/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
// TODO: Reinclude and reimplement _write when BSP_UART is implemented in HAL.
// #include "BSP_UART.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

int _write(int fd, char *buffer, unsigned int len) {
    return 0;
    // if(buffer != NULL) {
    //     BSP_UART_Write(UART_2, buffer, len);
    // }
    // return len;
}

int _read(int const fd, char *buffer, unsigned const len) {
    if(buffer != NULL) {
        
    }
    return 1;
}

int _close(int file)
{
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}