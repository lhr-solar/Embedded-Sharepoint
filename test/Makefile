<<<<<<< HEAD:Test/Makefile
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
PROJECT_C_SOURCES := $(addprefix $(MAKEFILE_DIR)/, $(PROJECT_C_SOURCES) Tests/$(TEST).c)
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
	$(MAKE) -C $(BUILD_MAKEFILE_DIR) $(MAKECMDGOALS)

# help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  docs     - Generate documentation using mkdocs."
	@$(MAKE) -C $(DRIVERS_DIR) help
=======
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
	@echo "Bear is not installed, running normal make..."
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
>>>>>>> 8d064395c8b6bd25adc30862eef81b43163b4ba7:test/Makefile
