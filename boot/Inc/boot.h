#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>
#include <stdbool.h>

#include "boot_config.h"

error_code_t boot_init();
void boot_deinit();
void startapp();

#endif