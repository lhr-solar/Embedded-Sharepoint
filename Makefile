# COLORS
RED=\033[0;31m
GREEN=\033[0;32m
ORANGE=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
LIGHTGRAY=\033[0;37m
DARKGRAY=\033[1;30m
YELLOW=\033[0;33m
NC=\033[0m # No Color

chip?=F4
BSPConfig?=off

# Used to select the MCU (default is STM32F413)
ifeq ($(chip), F4)
    MCU_TARGET = STM32F413
else ifeq ($(chip), L4)
    MCU_TARGET = STM32L431
else
    $(error $(shell echo "${RED}Invalid MCU specified ${NC}(Please choose between L4 and F4)"))
endif

$(info Compiling for the $(MCU_TARGET))

bsp_test:
ifdef test
	$(MAKE) -C BSP -C $(MCU_TARGET) -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../Tests/$(test).c
else
	$(error $(shell echo  "${RED}error${NC}test is not set (e.g. make bsp_test test=HelloWorld)"))
endif

hw_test:
ifdef test
	$(MAKE) -C BSP -C $(MCU_TARGET) -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../../HwTests/$(test).c
else
	$(error $(shell echo  "${RED}error${NC}test is not set (e.g. make hw_test test=Heartbeat)"))
endif

help:
	@echo  "MCU selection:"
	@echo  "	The ${BLUE}chip${NC}variable selects which MCU is compiled"
	@echo  "	${ORANGE}make${NC}chip=${PURPLE}<chip type>${NC}"
	@echo  "     Setting chip as ${PURPLE}F4${NC}selects the STM32F413 mcu and ${PURPLE}L4${NC}sets it as STM32L431"
	@echo  "BSP Tests:"
	@echo  "	${ORANGE}make ${BLUE}bsp_test ${NC}TEST=${PURPLE}<test name>${NC} exclude the .c file extension from the test name"
	@echo  "	exclude the .c file extension from the test name (e.g) to build a test with ${PURPLE}ADC.c${NC}"
	@echo  "	${ORANGE}make ${BLUE}bsp_test ${NC}test=${PURPLE}ADC${NC}"
	@echo  "Hardware Tests:"
	@echo  "	${ORANGE}make ${BLUE}hw_test ${NC}TEST=${PURPLE}<test name>${NC} exclude the .c file extension from the test name"
	@echo  "	exclude the .c file extension from the test name (e.g) to build a test with ${PURPLE}Heartbeat.c${NC}"
	@echo  "	${ORANGE}make ${BLUE}hw_test ${NC}test=${PURPLE}Heartbeat${NC}"


# left commented as an example
# project:
# 	$(MAKE) -C BSP -C STM32F413 -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../Tests/HelloWorld.c PROJECT_C_SOURCES="../../src/*.c ../../dogs/*.c"  PROJECT_C_INCLUDES=../../inc/

flash:
	-st-flash write Objects/bsp.bin 0x8000000

clean:
	-rm -fR Objects
