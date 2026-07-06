#ifndef BOOTLOADER_HAL_H_
#define BOOTLOADER_HAL_H_

#if defined(STM32F4xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32L4xx)
#include "stm32l4xx_hal.h"
#elif defined(STM32G4xx)
#include "stm32g4xx_hal.h"
#else
#error "Unsupported STM32 series for bootloader."
#endif

#endif
