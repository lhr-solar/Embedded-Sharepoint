#!/bin/bash

TARGET=$1

# STM32 scrpts provided by OpenOCD:
# stm32f0x.cfg
# stm32f1x.cfg
# stm32f2x.cfg
# stm32f3x.cfg
# stm32f4x.cfg
# stm32f7x.cfg
# stm32g0x.cfg
# stm32g4x.cfg
# stm32h7x.cfg
# stm32h7x_dual_bank.cfg
# stm32l0.cfg
# stm32l0_dual_bank.cfg
# stm32l1.cfg
# stm32l1x_dual_bank.cfg
# stm32l4x.cfg
# stm32l5x.cfg
# stm32mp13x.cfg
# stm32mp15x.cfg
# stm32u5x.cfg
# stm32w108xx.cfg
# stm32wbx.cfg
# stm32wlx.cfg
# stm32x5x_common.cfg
# stm32xl.cfg
# The target MCU. This should match your board
# Check which target was passed and set the appropriate configuration file
case $TARGET in
    L4)
        MCU_CONFIG="stm32l4x.cfg"
        ;;
    F4)
        MCU_CONFIG="stm32f4x.cfg"
        ;;
    *)
        echo "Unsupported target: $TARGET"
        echo "Usage: $0 [L4|F4]"
        exit 1
        ;;
esac

# Run the OpenOCD command
echo "Running OpenOCD for STM32$TARGET..."
openocd -f interface/stlink.cfg -f target/$MCU_CONFIG
