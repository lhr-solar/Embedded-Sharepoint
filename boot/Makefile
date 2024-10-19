SHELL := /bin/bash

# default project configuration
PROJECT_TARGET ?= stm32f401re

# source and include directories
PROJECT_C_SOURCES = boot.c
PROJECT_C_INCLUDES = ./

# build and driver directories
PROJECT_BUILD_DIR = ../build
BUILD_MAKEFILE_DIR = ../

# ensure all paths are absolute
MAKEFILE_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_C_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_SOURCES))
PROJECT_C_INCLUDES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_INCLUDES))
PROJECT_BUILD_DIR := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_BUILD_DIR))

LD_SUFFIX = BOOT

# grab the series just so we can get the cfg path
SERIES = $(shell echo $(PROJECT_TARGET) | cut -c6-7)
LINE = $(shell echo $(PROJECT_TARGET) | cut -c8-9)
CFG_FILE = $(MAKEFILE_DIR)/../stm/stm32$(SERIES)xx/stm32$(SERIES)$(LINE)/stm32$(SERIES)$(LINE).cfg

# set the flash address to 0x08000000 + the app size
TOTAL_FLASH_SIZE = $(shell source $(CFG_FILE) && echo $$TOTAL_FLASH_SIZE)
BOOT_SIZE = $(shell source $(CFG_FILE) && echo $$BOOT_SIZE)
FLASH_SIZE = $(shell echo $$(( $(TOTAL_FLASH_SIZE) - $(BOOT_SIZE) )))
FLASH_ADDRESS = $(shell printf "0x%08x" $$(( 0x08000000 + ($(FLASH_SIZE) * 1024))))

$(info TOTAL_FLASH_SIZE: $(TOTAL_FLASH_SIZE))
$(info BOOT_SIZE: $(BOOT_SIZE))
$(info FLASH_SIZE: $(FLASH_SIZE))
$(info FLASH_ADDRESS: $(FLASH_ADDRESS))

# export variables for sub-makes
export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_BUILD_DIR
export LD_SUFFIX
export FLASH_ADDRESS

# default target
default: all

# generate documentation
.PHONY: docs
docs:
	cd .. && mkdocs serve

# recursively build in the drivers directory
%:
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)

# help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  docs     - Generate documentation using mkdocs."
	@$(MAKE) -C $(DRIVERS_DIR) help
