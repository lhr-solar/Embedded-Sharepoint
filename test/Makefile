# default project configuration
TEST ?= main
PROJECT_TARGET ?= stm32f446ret
BEAR_ENABLE ?= 1

# source and include directories
_PROJECT_C_SOURCES = $(wildcard Src/*.c)
_PROJECT_C_INCLUDES = Inc

# build and driver directories
_PROJECT_BUILD_DIR ?= ../build
BUILD_MAKEFILE_DIR = ../

# ensure all paths are absolute
MAKEFILE_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_C_SOURCES = $(addprefix $(MAKEFILE_DIR)/, $(_PROJECT_C_SOURCES) tests/$(TEST).c)
PROJECT_C_INCLUDES = $(addprefix $(MAKEFILE_DIR)/, $(_PROJECT_C_INCLUDES))
PROJECT_BUILD_DIR = $(addprefix $(MAKEFILE_DIR)/, $(_PROJECT_BUILD_DIR))

export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_BUILD_DIR

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
