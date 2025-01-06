# default project configuration
TEST ?= main
PROJECT_TARGET ?= stm32f446ret

# source and include directories
PROJECT_C_SOURCES = $(wildcard Src/*.c)
PROJECT_C_INCLUDES = Inc

# build and driver directories
PROJECT_BUILD_DIR = ../build
BUILD_MAKEFILE_DIR = ../

# ensure all paths are absolute
MAKEFILE_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_C_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_SOURCES) tests/$(TEST).c)
PROJECT_C_INCLUDES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_INCLUDES))
PROJECT_BUILD_DIR := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_BUILD_DIR))

# export variables for sub-makes
export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_BUILD_DIR

# colors
RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

# check if bear is installed
BEAR_INSTALLED := $(shell command -v bear >/dev/null 2>&1 && echo yes || echo no)

ifeq ($(MAKECMDGOALS),)
default: conditional_bear_make
else ifeq ($(MAKECMDGOALS),all)
all: conditional_bear_make
else
%:
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)
endif

.PHONY: conditional_bear_make
conditional_bear_make:
ifeq ($(BEAR_INSTALLED),yes)
	@echo "Bear is installed, generating compile_commands.json..."; \
	bear --output $(VS_CODE_DIR)/compile_commands.json --append -- $(MAKE) -C $(BUILD_MAKEFILE_DIR) all;
else
	@echo -e "$(RED)Bear is not installed, running normal make...$(NC)"
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) all
endif

# generate documentation
.PHONY: docs
docs:
	cd .. && mkdocs serve

# help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  docs     - Generate documentation using mkdocs."
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) help
