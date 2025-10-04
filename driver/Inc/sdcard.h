#ifndef SDCARD_H
#define SDCARD_H

#include "stm32xx_hal.h"
#include <stdint.h>

extern SPI_HandleTypeDef hspi1;  // declaration only

void init(void);
void SD_SPI_Init(void);

#endif
