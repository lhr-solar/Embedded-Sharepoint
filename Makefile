chip?=F4
customBSP?=off

# Used to select the MCU (default is STM32F413)
ifeq ($(chip), F4)
    MCU_TARGET = STM32F413
else ifeq ($(chip), L4)
    MCU_TARGET = STM32L431
else
    $(error Invalid MCU specified (L4 and F4 are the only options))
endif

# to do: this is hella jank and only here since BSP.C is built for the F4 MCU. 
# Keeping it optional to use BSP.C is nice, but there are cleaner ways we can do it.

# If customBSP = on, we compile BSP.C
ifeq ($(customBSP), on)
    customBSP = on
endif

# to do: add functionality to externally call this makefile and compile their relevant STM32

test:
ifdef test
	$(MAKE) -C BSP -C $(MCU_TARGET) -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../Tests/$(test).c
else
	$(error test is not set (e.g. make test test=HelloWorld))
endif


# left commented as an example
# project:
# 	$(MAKE) -C BSP -C STM32F413 -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../Tests/HelloWorld.c PROJECT_C_SOURCES="../../src/*.c ../../dogs/*.c"  PROJECT_C_INCLUDES=../../inc/

flash:
	-st-flash write Objects/bsp.bin 0x8000000

clean:
	-rm -fR Objects
