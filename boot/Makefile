SHELL := /bin/bash

# default project configuration
PROJECT_TARGET ?= stm32f429zit

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
USER_DEFINES = USER_VECT_TAB_ADDRESS

# export variables for sub-makes
export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_ASM_SOURCES
export PROJECT_BUILD_DIR
export LD_SUFFIX
export FLASH_ADDRESS
export USER_DEFINES

# check if bear is installed
BEAR_INSTALLED := $(shell command -v bear >/dev/null 2>&1 && echo yes || echo no)

BEAR_PREFIX := 
ifeq ($(BEAR_INSTALLED),yes)
$(info A wild bear has appeared!)
BEAR_PREFIX := bear --output $(BOOT_BUILD_DIR)/compile_commands.json --append --
endif

# default target
default: all

# generate documentation
.PHONY: docs
docs:
	cd .. && mkdocs serve

all: $(BOOT_BUILD_DIR)
	$(BEAR_PREFIX) $(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)
	mkdir -p $(APP_BUILD_DIR) && cp $(BOOT_BUILD_DIR)/$(PROJECT_TARGET).elf $(APP_BUILD_DIR)/$(PROJECT_TARGET)_boot.elf

clean:	
	-rm -fR $(BOOT_BUILD_DIR)/* $(APP_BUILD_DIR)/$(PROJECT_TARGET)_boot.elf
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)

# recursively build in the drivers directory
%:
	$(BEAR_PREFIX) $(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)

# help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  docs     - Generate documentation using mkdocs."
	@$(MAKE) -C $(DRIVERS_DIR) help
