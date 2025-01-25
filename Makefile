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
# clang
#######################################
IGNORED_CLANG_INPUTS = %/stm32f4xx_hal_conf.h %/stm32l4xx_hal_conf.h %/FreeRTOSConfig.h

CLANG_INPUTS = $(PROJECT_C_SOURCES) $(foreach DIR, $(PROJECT_C_INCLUDES), $(wildcard $(DIR)/*))
CLANG_INPUTS := $(filter-out $(IGNORED_CLANG_INPUTS), $(CLANG_INPUTS))


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

# Generate a list of STM32 variant patterns:
# 1. Remove the '.h' extension from filenames.
# 2. Replace 'x' with '.' for regex matching.
MCU_MATCHES = $(shell ls stm/$(SERIES_GENERIC)/CMSIS/Device/ST/$(SERIES_GENERIC_CAP)/Include \
                | sed 's/\.h//g; s/x/./g')

# Find the generic STM32 series variant:
# 1. Iterate over MCU_MATCHES.
# 2. Check if the entry is exactly 11 characters long.
# 3. Match it against the pattern 'stm32$(SERIES_LINE)$(EXTRA_CUT)'.
# 4. Replace '.' with 'x' in the matched entry.
# 5. Break on the first valid match.
SERIES_LINE_GENERIC = $(shell \
    for match in $(MCU_MATCHES); do \
        if [ $${#match} -eq 11 ] && echo "stm32$(SERIES_LINE)$(EXTRA_CUT)" | grep -qE "$$match"; then \
            echo "$${match//./x}"; \
            break; \
        fi; \
    done)

SERIES_LINE_GENERIC_CAP = $(shell echo $(SERIES_LINE_GENERIC) | sed 's/[^x]/\U&/g')

ifeq ($(strip $(SERIES_LINE_GENERIC)),)
$(error SERIES_LINE_GENERIC is not found in stm/$(SERIES_GENERIC)/CMSIS/Device/ST/$(SERIES_GENERIC_CAP)/Include. Please check the target configuration.)
endif

TARGET = $(PROJECT_TARGET)

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = $(PROJECT_BUILD_DIR)

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(PROJECT_C_SOURCES) \
$(filter-out %template.c, $(wildcard stm/$(SERIES_GENERIC)/$(SERIES_GENERIC_CAP)_HAL_Driver/Src/*.c)) \
stm/$(SERIES_GENERIC)/system_$(SERIES_GENERIC).c \
stm/$(SERIES_GENERIC)/$(SERIES_GENERIC)_hal_init.c \
stm/$(SERIES_GENERIC)/$(SERIES_GENERIC)_hal_timebase_tim.c \
$(wildcard FreeRTOS-Kernel/*.c) \
FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c \
$(wildcard common/Src/*.c) \
$(wildcard bsp/Src/*.c)

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
USE_HAL_DRIVER \
$(SERIES_LINE_GENERIC_CAP) \
$(SERIES_GENERIC_CAP)

C_DEFS := $(addprefix -D,$(C_DEFS))

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
$(PROJECT_C_INCLUDES) \
stm/$(SERIES_GENERIC)/$(SERIES_GENERIC_CAP)_HAL_Driver/Inc \
stm/$(SERIES_GENERIC)/CMSIS/Device/ST/$(SERIES_GENERIC_CAP)/Include \
stm/$(SERIES_GENERIC)/CMSIS/Include \
FreeRTOS-Kernel/include \
FreeRTOS-Kernel/portable/GCC/ARM_CM4F \
common/Inc \
bsp/Inc

C_INCLUDES := $(addprefix -I,$(C_INCLUDES))

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -Werror -Wfatal-errors -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -Wfatal-errors -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# edge case
#######################################
# exclude CAN
ifeq ($(filter $(SERIES_LINE_GENERIC), stm32f401xe stm32f401xc), $(SERIES_LINE_GENERIC))
    C_SOURCES := $(filter-out bsp/Src/CAN.c, $(C_SOURCES))
    C_DEFS += -DCAN_UNDEFINED
endif

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = stm/$(SERIES_GENERIC)/$(SERIES_LINE)/$(SERIES_LINE_CAP)$(EXTRA_CAP)x_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash
#######################################
FLASH_ADDRESS ?= 0x8000000
FLASH_FILE = $(shell find $(BUILD_DIR) -name 'stm*.bin' -exec basename {} \;)

flash:
	@echo "Flashing $(FLASH_FILE) to $(FLASH_ADDRESS)"
	-st-flash write $(BUILD_DIR)/$(FLASH_FILE) $(FLASH_ADDRESS)

#######################################
# format
#######################################
FORMAT_CONFIG ?= --style=file:../.clang-format

format:
	-clang-format $(FORMAT_CONFIG) $(CLANG_INPUTS)

format-fix:
	-clang-format -i $(FORMAT_CONFIG) $(CLANG_INPUTS)


#######################################
# help
#######################################
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
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
