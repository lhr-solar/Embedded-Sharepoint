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

SERIES_CAP = $(shell echo $(SERIES) | tr '[:lower:]' '[:upper:]')
EXTRA_CAP = $(shell echo $(EXTRA) | tr '[:lower:]' '[:upper:]')
SERIES_GENERIC = stm32$(SERIES)xx
SERIES_GENERIC_CAP = STM32$(SERIES_CAP)xx
SERIES_LINE = stm32$(SERIES)$(LINE)
SERIES_LINE_CAP = STM32$(SERIES_CAP)$(LINE)
SERIES_LINE_GENERIC = $(SERIES_LINE)xx
SERIES_LINE_GENERIC_CAP = $(SERIES_LINE_CAP)xx

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
$(filter-out %template.c, $(wildcard $(SERIES_GENERIC)/$(SERIES_GENERIC_CAP)_HAL_Driver/Src/*.c)) \
$(SERIES_GENERIC)/system_$(SERIES_GENERIC).c \
$(SERIES_GENERIC)/$(SERIES_GENERIC)_hal_init.c \
$(SERIES_GENERIC)/$(SERIES_GENERIC)_hal_timebase_tim.c \
$(wildcard FreeRTOS-Kernel/*.c) \
FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c \
$(wildcard common/Src/*.c)

# ASM sources
ASM_SOURCES =  \
$(SERIES_GENERIC)/$(SERIES_LINE)/startup_$(SERIES_LINE_GENERIC).s

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
$(SERIES_GENERIC)/$(SERIES_GENERIC_CAP)_HAL_Driver/Inc \
$(SERIES_GENERIC)/$(SERIES_GENERIC_CAP)_HAL_Driver/Inc/Legacy \
$(SERIES_GENERIC)/CMSIS/Device/ST/$(SERIES_GENERIC_CAP)/Include \
$(SERIES_GENERIC)/CMSIS/Include \
FreeRTOS-Kernel/include \
FreeRTOS-Kernel/portable/GCC/ARM_CM4F \
common/Inc

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
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(SERIES_GENERIC)/$(SERIES_LINE)/$(SERIES_LINE_CAP)$(EXTRA_CAP)x_FLASH.ld

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

flash:
	-st-flash write $(BUILD_DIR)/$(TARGET).bin $(FLASH_ADDRESS)


#######################################
# tidy
#######################################
TIDY_CONFIG ?= --config-file=../.clang-tidy
TIDY_C_FLAGS = $(C_INCLUDES) $(C_DEFS)
# TIDY_CFLAGS += -m32

tidy:
	-clang-tidy $(TIDY_CONFIG) $(CLANG_INPUTS) -- $(TIDY_C_FLAGS)

tidy-fix:
	-clang-tidy $(TIDY_CONFIG) $(CLANG_INPUTS) --fix -- $(TIDY_C_FLAGS)


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