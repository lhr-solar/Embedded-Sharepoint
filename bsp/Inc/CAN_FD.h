#pragma once

#include "stm32xx_hal.h"
#include "CAN_Common.h"

#if !defined(FDCAN1)
  #error "[CONFIG] CAN FD not enabled on this chip."
#endif /* FDCAN1 */

