TEST = none

test:
ifneq ($(TEST), none)
	$(MAKE) -C BSP -C STM32F413 -j TARGET=bsp PROJECT_DIR=../.. TEST=../Tests/$(TEST).c
else
	@echo "Testfile not found${NC}"
endif

clean:
	$(MAKE) -C BSP -C STM32F413 -j clean
