#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>

typedef enum {
    BLDR_OK,
    BLDR_TMOUT,
    BLDR_FAIL_INIT,
    BLDR_FAIL
} error_code_t;

void boot();
void startapp();

#endif