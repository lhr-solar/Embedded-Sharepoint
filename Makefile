test:
ifdef test
	$(MAKE) -C BSP -C STM32F413 -j TARGET=bsp PROJECT_DIR=../.. TEST=../Tests/$(test).c BUILD_DIR=../../Objects
else
	$(error test is not set (e.g. make test helloworld))
endif

flash:
	-st-flash write Objects/bsp.bin 0x8000000

clean:
	-rm -fR Objects
