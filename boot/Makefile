# default project configuration
PROJECT_TARGET ?= stm32f446ret
BEAR_ENABLE ?= 1

BOOT_CONFIG ?= Src/boot_config.c

# source and include directories
PROJECT_C_SOURCES = $(filter-out Src/boot_config.c, $(wildcard Src/*.c)) $(BOOT_CONFIG)
PROJECT_C_INCLUDES = Inc
PROJECT_ASM_SOURCES = $(wildcard Src/*.s)

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

# bear
BEAR_PREFIX := 
MV_COMPILE_COMMANDS_FILE :=

ifeq ($(BEAR_ENABLE), 1)
  BEAR_INSTALLED := $(shell command -v bear >/dev/null 2>&1 && echo yes || echo no)
  ifeq ($(BEAR_INSTALLED),yes)
    BEAR_PREFIX := bear --output compile_commands.json --append --
    MV_COMPILE_COMMANDS_FILE := -mv compile_commands.json $(PROJECT_BUILD_DIR)/compile_commands.json 
  else
    $(warning BEAR_ENABLE=1, but bear is out searching for honey and is nowhere to be found!)
  endif
endif

$(info $(PROJECT_BUILD_DIR))

default: all

.PHONY: all
all:
	@if [ -n "$(BEAR_PREFIX)" ]; then \
	    echo "A wild bear has appeared!"; \
	fi

	$(BEAR_PREFIX) $(MAKE) -C $(BUILD_MAKEFILE_DIR) all
	$(MAKE) move-commands

%:
	@if [ "$@" != "default" ]; then \
		$(MAKE) -C $(BUILD_MAKEFILE_DIR) $@; \
	fi

.PHONY: move-commands
move-commands: # only runs if compile_commands.json exists
	$(MV_COMPILE_COMMANDS_FILE)

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
