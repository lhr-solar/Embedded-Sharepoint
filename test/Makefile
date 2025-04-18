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

# define VS_CODE_DIR
VS_CODE_DIR := $(MAKEFILE_DIR)/../.vscode

COMPILE_COMMANDS_ENABLED := 1
ifeq ($(COMPILE_COMMANDS_ENABLED), 1)
	COMPILE_COMMANDS_TXT := $(VS_CODE_DIR)/compile_commands.txt
	COMPILE_COMMANDS_JSON := $(VS_CODE_DIR)/compile_commands.json
	VERBOSE := 1
else
	VERBOSE := 0
endif
export VERBOSE

default: all
all:
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) all | tee -a $(COMPILE_COMMANDS_TXT)
	$(BUILD_MAKEFILE_DIR)/bear.py $(COMPILE_COMMANDS_TXT) $(COMPILE_COMMANDS_JSON) > /dev/null 2>&1

.PHONY: clean
clean:
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) clean
	-rm $(COMPILE_COMMANDS_JSON) $(COMPILE_COMMANDS_TXT)

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
