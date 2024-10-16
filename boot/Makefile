# default project configuration
TEST ?= main
PROJECT_TARGET ?= stm32f446ret

# source and include directories
PROJECT_C_SOURCES = boot.c
PROJECT_C_INCLUDES = .

# build and driver directories
PROJECT_BUILD_DIR = ../build
BUILD_MAKEFILE_DIR = ../

# ensure all paths are absolute
MAKEFILE_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_C_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_SOURCES))
PROJECT_C_INCLUDES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_INCLUDES))
PROJECT_BUILD_DIR := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_BUILD_DIR))

FREERTOS_ENABLE = 0

# export variables for sub-makes
export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_BUILD_DIR
export FREERTOS_ENABLE

# default target
default: all

# generate documentation
.PHONY: docs
docs:
	cd .. && mkdocs serve

# recursively build
%:
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)

# help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  docs     - Generate documentation using mkdocs."
	@$(MAKE) -C $(DRIVERS_DIR) help