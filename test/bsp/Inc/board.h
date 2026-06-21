#pragma once

#include "stm32xx_hal.h"

/*
 * Board-specific bring-up. Exactly one bsp/Src/<mcu>_board.c compiles per target
 * (each is guarded by its STM32G4xx part macro). Each board file provides the
 * strong HAL_*_MspInit (pins + clock + NVIC) plus a per-instance init for every
 * bus it physically routes. Apps call board_init() once at startup; it brings up
 * only the instances this board routes.
 *
 * Per-instance entry points are declared only on parts that have the peripheral
 * (CMSIS defines FDCAN1/2/3 per device), so the same header fits every G4 part.
 */

#ifdef FDCAN1
void board_fdcan1_init(void);
#endif
#ifdef FDCAN2
void board_fdcan2_init(void);
#endif
#ifdef FDCAN3
void board_fdcan3_init(void);
#endif

void board_init(void);
