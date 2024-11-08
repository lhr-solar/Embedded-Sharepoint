SHELL := /bin/bash

# default project configuration
PROJECT_TARGET ?= stm32f446ret

# source and include directories
PROJECT_C_SOURCES = $(wildcard Src/*.c)
PROJECT_C_INCLUDES = Inc
PROJECT_ASM_SOURCES = reset.s

# build and driver directories
APP_BUILD_DIR = ../build
BOOT_BUILD_DIR = build
BUILD_MAKEFILE_DIR = ../

# ensure all paths are absolute
MAKEFILE_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_C_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_SOURCES))
PROJECT_C_INCLUDES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_INCLUDES))
PROJECT_BUILD_DIR := $(addprefix $(MAKEFILE_DIR)/, $(BOOT_BUILD_DIR))
PROJECT_ASM_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_ASM_SOURCES))

BOOT_BUILD_DIR := $(addprefix $(MAKEFILE_DIR)/, $(BOOT_BUILD_DIR))
LD_SUFFIX = BOOT
FLASH_ADDRESS = 0x08000000

# export variables for sub-makes
export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_ASM_SOURCES
export PROJECT_BUILD_DIR
export LD_SUFFIX
export FLASH_ADDRESS

# default target
default: all

# generate documentation
.PHONY: docs
docs:
	cd .. && mkdocs serve

all: $(APP_BUILD_DIR)/$(PROJECT_TARGET).elf $(APP_BUILD_DIR)/$(PROJECT_TARGET).hex $(APP_BUILD_DIR)/$(PROJECT_TARGET).bin
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)
	cp $(BOOT_BUILD_DIR)/$(PROJECT_TARGET).elf $(APP_BUILD_DIR)/$(PROJECT_TARGET)_boot.elf

clean:
	rm -f $(APP_BUILD_DIR)/$(PROJECT_TARGET)_boot.elf
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)

# recursively build in the drivers directory
%:
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)

# help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  docs     - Generate documentation using mkdocs."
	@$(MAKE) -C $(DRIVERS_DIR) help
