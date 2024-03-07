MCU?=F4

# Used to select the MCU (default is STM32F413)
ifeq ($(MCU), F4)
    $(info MCU is set to F4)
    MCU_TARGET = STM32F413
else ifeq ($(MCU), L4)
    $(info MCU is set to L4)
    MCU_TARGET = STM32L431
else
    $(error Invalid MCU specified (L4 and F4 are the only options))
endif

test:
ifdef test
	$(info MCU_TARGET is set to $(MCU_TARGET))
	$(MAKE) -C BSP -C $(MCU_TARGET) -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../Tests/$(test).c MCU_TARGET=$(MCU_TARGET) MCU=$(MCU)
else
	$(error test is not set (e.g. make test test=HelloWorld))
endif

flash:
	-st-flash write Objects/bsp.bin 0x8000000

clean:
	-rm -fR Objects