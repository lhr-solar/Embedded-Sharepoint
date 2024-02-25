test:
ifdef test
	$(MAKE) -C BSP -C STM32F413 -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../Tests/$(test).c
else
	$(error test is not set (e.g. make test test=HelloWorld))
endif

psom_test:
ifdef psom_test
	$(MAKE) -C BSP -C STM32F413 -j TARGET=Tests PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../PeripheralSOM/$(psom_test).c
else
	$(error PeripheralSOM test is not set (e.g. make psom_test psom_test=Heartbeat))
endif

# left commented as an example
# project:
# 	$(MAKE) -C BSP -C STM32F413 -j TARGET=bsp PROJECT_DIR=../.. BUILD_DIR=../../Objects TEST=../Tests/HelloWorld.c PROJECT_C_SOURCES="../../src/*.c ../../dogs/*.c"  PROJECT_C_INCLUDES=../../inc/

flash:
	-st-flash write Objects/bsp.bin 0x8000000

clean:
	-rm -fR Objects
