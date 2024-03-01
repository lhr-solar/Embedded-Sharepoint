MCU?=F4

# Used to select the MCU (default is STM32F413)
ifeq ($(MCU), F4)
    MCU_TARGET = STM32F413
else ifeq ($(MCU), L4)
    MCU_TARGET = STM32L431
else
    $(error Invalid MCU specified (L4 and F4 are the only options))
endif

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