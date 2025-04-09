# default project configuration
TEST ?= main
PROJECT_TARGET ?= stm32f446ret
BEAR_ENABLE ?= 1

# source and include directories
PROJECT_C_SOURCES = $(wildcard Src/*.c)
PROJECT_C_INCLUDES = Inc

# build and driver directories
PROJECT_BUILD_DIR ?= ../build
BUILD_MAKEFILE_DIR = ../

# ensure all paths are absolute
MAKEFILE_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_C_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_SOURCES) tests/$(TEST).c)
PROJECT_C_INCLUDES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_INCLUDES))
PROJECT_BUILD_DIR := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_BUILD_DIR))

# bear
ifeq ($(BEAR_ENABLE), 1)
	BEAR_INSTALLED := $(shell command -v bear >/dev/null 2>&1 && echo yes || echo no)
endif

BEAR_PREFIX := 
ifeq ($(BEAR_INSTALLED),yes)
$(info A wild bear has appeared!)
VS_CODE_DIR ?= $(MAKEFILE_DIR)/../.vscode

# check if the directory exists
ifneq (,$(wildcard $(VS_CODE_DIR)))
$(info VS Code directory found: $(VS_CODE_DIR))
else
$(info VS Code directory not found: $(VS_CODE_DIR))
$(error Please create the directory $(VS_CODE_DIR) or set the VS_CODE_DIR variable to a valid path. \
		If you do not want to use bear, set BEAR_ENABLE to 0.)
endif

BEAR_PREFIX := bear --output $(VS_CODE_DIR)/compile_commands.json --append --
endif

TEST_FILE = $(filter %/tests/$(TEST).c, $(PROJECT_C_SOURCES))

default: all
%:
	$(BEAR_PREFIX) $(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS) \
		PROJECT_TARGET=$(PROJECT_TARGET) \
		PROJECT_C_SOURCES="$(PROJECT_C_SOURCES)" \
		PROJECT_C_INCLUDES="$(PROJECT_C_INCLUDES)" \
		PROJECT_BUILD_DIR=$(PROJECT_BUILD_DIR)

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
