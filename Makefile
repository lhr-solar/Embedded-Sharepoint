######################################
# inputs
######################################
ifndef PROJECT_TARGET
$(error PROJECT_TARGET is not defined. Please provide a target to build for.)
endif

ifndef PROJECT_BUILD_DIR
$(error PROJECT_BUILD_DIR is not defined. Please provide a place to build.)
endif

ifndef PROJECT_C_SOURCES
$(error PROJECT_C_SOURCES is not defined. Please provide a set of C source files.)
endif

ifndef PROJECT_C_INCLUDES
$(error PROJECT_C_INCLUDES is not defined. Please provide a set of C header files.)
endif

# Make sure shell commands run in /bin/bash, not /bin/sh
SHELL := /bin/bash

#######################################
# misc
#######################################
IGNORED_CLANG_INPUTS = %/stm32f4xx_hal_conf.h %/stm32l4xx_hal_conf.h %/FreeRTOSConfig.h

CLANG_INPUTS = $(PROJECT_C_SOURCES) $(foreach DIR, $(PROJECT_C_INCLUDES), $(wildcard $(DIR)/*))
CLANG_INPUTS := $(filter-out $(IGNORED_CLANG_INPUTS), $(CLANG_INPUTS))

MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BEAR_ENABLE ?= 1
FIRMWARE_ROLE ?= app
BOOTLOADER_SIZE_KB ?= 64

######################################
# target
######################################
SERIES = $(shell echo $(PROJECT_TARGET) | cut -c6-7)
LINE = $(shell echo $(PROJECT_TARGET) | cut -c8-9)
EXTRA = $(shell echo $(PROJECT_TARGET) | cut -c10-)
EXTRA_CUT = $(shell echo $(PROJECT_TARGET) | cut -c10-11)

SERIES_CAP = $(shell echo $(SERIES) | tr '[:lower:]' '[:upper:]')
EXTRA_CAP = $(shell echo $(EXTRA) | tr '[:lower:]' '[:upper:]')
SERIES_GENERIC = stm32$(SERIES)xx
SERIES_GENERIC_CAP = STM32$(SERIES_CAP)xx

SERIES_LINE = stm32$(SERIES)$(LINE)
SERIES_LINE_CAP = STM32$(SERIES_CAP)$(LINE)

SERIES_LINE_GENERIC = $(SERIES_LINE)xx

SERIES_LINE_GENERIC_CAP = $(shell python3 -c 's="$(SERIES_LINE_GENERIC)"; print(s[:-2].upper() + s[-2:])')

ifeq ($(strip $(SERIES_LINE_GENERIC)),)
$(error SERIES_LINE_GENERIC is not found in stm/$(SERIES_GENERIC)/CMSIS/Device/ST/$(SERIES_GENERIC_CAP)/Include. Please check the target configuration.)
endif

TARGET = $(PROJECT_TARGET)

# We pull any embedded sharepoint feature enables from here
CFG_FILE = stm/$(SERIES_GENERIC)/$(SERIES_LINE)/$(PROJECT_TARGET).cfg
include $(CFG_FILE)

OPENOCD_CFG_FILE := openocd-$(patsubst %xx,%x,$(SERIES_GENERIC)).cfg

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og
# verbose
VERBOSE ?= 0

#######################################
# paths
#######################################
# Build path
BUILD_DIR = $(PROJECT_BUILD_DIR)
# FreeRTOS path
FREERTOS_PATH := middleware/FreeRTOS-Kernel
FATFS_PATH := middleware/FatFs

######################################
# source
######################################
# C sources
COMMON_STM_SOURCES = \
$(filter-out %template.c, $(wildcard stm/$(SERIES_GENERIC)/$(SERIES_GENERIC_CAP)_HAL_Driver/Src/*.c)) \
stm/$(SERIES_GENERIC)/system_$(SERIES_GENERIC).c \
stm/$(SERIES_GENERIC)/$(SERIES_GENERIC)_hal_init.c \
stm/$(SERIES_GENERIC)/$(SERIES_GENERIC)_hal_timebase_tim.c

APP_ROLE_SOURCES = \
$(PROJECT_C_SOURCES) \
$(wildcard $(FREERTOS_PATH)/*.c) \
$(FREERTOS_PATH)/portable/GCC/ARM_CM4F/port.c \
$(wildcard common/Src/*.c) \
$(wildcard driver/Src/*.c) \
$(wildcard $(FATFS_PATH)/Src/*.c) \
$(filter-out $(addprefix bsp/Src/,$(addsuffix .c,$(BSP_DISABLE))),$(wildcard bsp/Src/*.c))

BOOTLOADER_ROLE_SOURCES = \
$(wildcard bootloader/Src/*.c) \
common/Src/uart_bootloader.c \
common/Src/stubs.c

ifeq ($(FIRMWARE_ROLE),bootloader)
C_SOURCES = $(BOOTLOADER_ROLE_SOURCES) $(COMMON_STM_SOURCES)
else
C_SOURCES = $(APP_ROLE_SOURCES) $(COMMON_STM_SOURCES)
endif


# ASM sources
ASM_SOURCES =  \
stm/$(SERIES_GENERIC)/$(SERIES_LINE)/startup_$(SERIES_LINE_GENERIC).s

# ASM sources
ASMM_SOURCES = 

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
$(PROJECT_MACROS) \
USE_HAL_DRIVER \
$(SERIES_LINE_GENERIC_CAP) \
$(SERIES_GENERIC_CAP)

ifeq ($(FIRMWARE_ROLE),bootloader)
C_DEFS += FIRMWARE_ROLE_BOOTLOADER
endif

C_DEFS := $(addprefix -D,$(C_DEFS))

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
$(PROJECT_C_INCLUDES) \
stm/$(SERIES_GENERIC)/$(SERIES_GENERIC_CAP)_HAL_Driver/Inc \
stm/$(SERIES_GENERIC)/CMSIS/Device/ST/$(SERIES_GENERIC_CAP)/Include \
stm/$(SERIES_GENERIC)/CMSIS/Include \
$(FREERTOS_PATH)/include \
$(FREERTOS_PATH)/portable/GCC/ARM_CM4F \
$(FATFS_PATH)/Inc \
common/Inc \
driver/Inc \
bsp/Inc \
middleware

ifeq ($(FIRMWARE_ROLE),bootloader)
C_INCLUDES += bootloader/Inc
endif

C_INCLUDES := $(addprefix -I,$(C_INCLUDES))

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -Werror -Wfatal-errors -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -Wfatal-errors -fdata-sections -ffunction-sections -ffreestanding

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
ORIG_LDSCRIPT = stm/$(SERIES_GENERIC)/$(SERIES_LINE)/$(SERIES_LINE_CAP)$(EXTRA_CAP)x_FLASH.ld
GENERATED_LDSCRIPT = $(BUILD_DIR)/$(TARGET)_$(FIRMWARE_ROLE).ld
LDSCRIPT = $(ORIG_LDSCRIPT)

ifeq ($(FIRMWARE_ROLE),app)
LDSCRIPT = $(GENERATED_LDSCRIPT)
endif

# libraries
LIBS = 
LIBDIR = 
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -nostdlib -nodefaultlibs

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# default action: build all
.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
ifeq ($(BEAR_ENABLE), 1)
	@echo $(MAKEFILE_DIR) > $(BUILD_DIR)/cc_$(notdir $@).txt
	@echo $(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@ >> $(BUILD_DIR)/cc_$(notdir $@).txt
endif

ifeq ($(VERBOSE), 1)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
else
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	@echo "CC $< -> $@"
endif

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
ifeq ($(BEAR_ENABLE), 1)
	@echo $(MAKEFILE_DIR) > $(BUILD_DIR)/cc_$(notdir $@).txt
	@echo $(AS) -c $(CFLAGS) $< -o $@ >> $(BUILD_DIR)/cc_$(notdir $@).txt
endif

ifeq ($(VERBOSE), 1)
	$(AS) -c $(CFLAGS) $< -o $@
else
	@$(AS) -c $(CFLAGS) $< -o $@
	@echo "AS $< -> $@"
endif


$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
ifeq ($(BEAR_ENABLE), 1)
	@echo $(MAKEFILE_DIR) > $(BUILD_DIR)/cc_$(notdir $@).txt
	@echo $(AS) -c $(CFLAGS) $< -o $@ >> $(BUILD_DIR)/cc_$(notdir $@).txt
endif

ifeq ($(VERBOSE), 1)
	$(AS) -c $(CFLAGS) $< -o $@
else
	@$(AS) -c $(CFLAGS) $< -o $@
	@echo "AS $< -> $@"
endif

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile $(LDSCRIPT)
	@if ls $(BUILD_DIR)/*.elf 1> /dev/null 2>&1; then \
	rm -rf $(BUILD_DIR)/stm*.elf; \
	fi
	
ifeq ($(BEAR_ENABLE), 1)
	@echo $(MAKEFILE_DIR) > $(BUILD_DIR)/cc_$(notdir $@).txt
	@echo $(CC) $(OBJECTS) $(LDFLAGS) -o $@ >> $(BUILD_DIR)/cc_$(notdir $@).txt
endif

ifeq ($(VERBOSE), 1)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
else
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "LD $@"
endif
	@$(SZ) $@
	@echo "Finished compiling. Jolly good!"

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@if ls $(BUILD_DIR)/*.hex 1> /dev/null 2>&1; then \
	rm -rf $(BUILD_DIR)/stm*.hex; \
	fi

ifeq ($(VERBOSE), 1)
	$(HEX) $< $@
else
	@$(HEX) $< $@
	@echo "HEX $< -> $@"
endif
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@if ls $(BUILD_DIR)/*.bin 1> /dev/null 2>&1; then \
	rm -rf $(BUILD_DIR)/stm*.bin; \
	fi

ifeq ($(VERBOSE), 1)
	$(BIN) $< $@
else
	@$(BIN) $< $@
	@echo "BIN $< -> $@"
endif
	
$(BUILD_DIR):
	mkdir -p $@		

$(GENERATED_LDSCRIPT): $(ORIG_LDSCRIPT) Makefile | $(BUILD_DIR)
	@python3 -c 'import pathlib; ld = pathlib.Path("$(ORIG_LDSCRIPT)").read_text(); size_kb = int("$(BOOTLOADER_SIZE_KB)"); flash_origin = 0x08000000 + size_kb * 1024; flash_length_kb = 512 - size_kb; ld = ld.replace("FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 512K", f"FLASH (rx)      : ORIGIN = 0x{flash_origin:08x}, LENGTH = {flash_length_kb}K"); pathlib.Path("$(GENERATED_LDSCRIPT)").write_text(ld)'
	@echo "LD_SCRIPT $(ORIG_LDSCRIPT) -> $(GENERATED_LDSCRIPT) ($(FIRMWARE_ROLE), FLASH=0x$$(printf '%x' $$((0x08000000 + $(BOOTLOADER_SIZE_KB)*1024))), LENGTH=$$((512-$(BOOTLOADER_SIZE_KB)))K)"

#######################################
# clean up
#######################################
.PHONY: clean
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash
#######################################
FLASH_ADDRESS ?= 0x8000000
FLASH_FILE = $(shell find $(BUILD_DIR) -name 'stm*.bin' -exec basename {} \;)

.PHONY: flash
flash:
	@echo "🔦 Flashing $(FLASH_FILE) to $(FLASH_ADDRESS)"
	st-flash write $(BUILD_DIR)/$(FLASH_FILE) $(FLASH_ADDRESS)

.PHONY: flash-uart
flash-uart:
	./flash-uart.sh $(BUILD_DIR)/$(FLASH_FILE) $(FLASH_ADDRESS)

#######################################
# format
#######################################
FORMAT_CONFIG ?= --style=file:../.clang-format

.PHONY: format
format:
	-clang-format $(FORMAT_CONFIG) $(CLANG_INPUTS)

.PHONY: format-fix
format-fix:
	-clang-format -i $(FORMAT_CONFIG) $(CLANG_INPUTS)

#######################################
# help
#######################################
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build the project."
	@echo "  clean        - Remove build artifacts."
	@echo "  flash        - Flash the target device."
	@echo "  tidy         - Run clang-tidy."
	@echo "  tidy-fix     - Run clang-tidy with fixes."
	@echo "  format       - Run clang-format."
	@echo "  format-fix   - Run clang-format and apply fixes."


#######################################
# openocd
#######################################	
.PHONY: debug
debug:
	@echo "🔬 Using $(OPENOCD_CFG_FILE)"
	openocd -f $(OPENOCD_CFG_FILE)

#######################################
# dependencies
#######################################


-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
