# default project configuration
TEST ?= main
PROJECT_TARGET ?= stm32f446ret

# source and include directories
PROJECT_C_SOURCES = $(wildcard Src/*.c)
PROJECT_C_INCLUDES = Inc

# build and driver directories
PROJECT_BUILD_DIR = ../build
DRIVERS_DIR = ../Drivers

# ensure all paths are absolute
MAKEFILE_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_C_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_SOURCES) Test/$(TEST).c)
PROJECT_C_INCLUDES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_INCLUDES))
PROJECT_BUILD_DIR := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_BUILD_DIR))

# export variables for sub-makes
export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_BUILD_DIR

# default target
default: all

# generate documentation
.PHONY: docs
docs:
	cd .. && mkdocs serve

# recursively build in the drivers directory
%:
	$(MAKE) -C $(DRIVERS_DIR) $(MAKECMDGOALS)

# help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  docs     - Generate documentation using mkdocs."
	@$(MAKE) -C $(DRIVERS_DIR) help
