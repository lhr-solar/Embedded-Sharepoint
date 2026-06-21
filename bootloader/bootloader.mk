# Standalone resident bootloader build.
# Usage: make -f bootloader/bootloader.mk TARGET=stm32g491vet

SHELL := /bin/bash

MK_DIR   := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
ROOT     := $(abspath $(MK_DIR)/..)
TARGET   ?= stm32g491vet
BUILD_DIR := $(MK_DIR)build/$(TARGET)

ifeq ($(TARGET),stm32g491vet)
  DEVICE_DEF   := STM32G491xx
  STARTUP      := $(ROOT)/stm/stm32g4xx/stm32g491/startup_stm32g491xx.s
  LDSCRIPT     := $(MK_DIR)ld/stm32g491vet_BL.ld
  BL_BUDGET    := 16384
else ifeq ($(TARGET),stm32g431cbt)
  DEVICE_DEF   := STM32G431xx
  STARTUP      := $(ROOT)/stm/stm32g4xx/stm32g431/startup_stm32g431xx.s
  LDSCRIPT     := $(MK_DIR)ld/stm32g431cbt_BL.ld
  BL_BUDGET    := 8192
else
  $(error Unsupported TARGET=$(TARGET); use stm32g491vet or stm32g431cbt)
endif

PREFIX  := arm-none-eabi-
CC      := $(PREFIX)gcc
AS      := $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY := $(PREFIX)objcopy
SIZE    := $(PREFIX)size
NM      := $(PREFIX)nm

CPU_FLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

INC := \
  -I$(MK_DIR)Inc \
  -I$(ROOT)/stm/stm32g4xx/CMSIS/Device/ST/STM32G4xx/Include \
  -I$(ROOT)/stm/stm32g4xx/CMSIS/Include

DEFS := -D$(DEVICE_DEF) -DSTM32G4xx

CFLAGS := $(CPU_FLAGS) -Os -ffreestanding -ffunction-sections -fdata-sections \
          -Wall -Werror $(DEFS) $(INC)

LDFLAGS := $(CPU_FLAGS) -nostdlib -T$(LDSCRIPT) \
           -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET)_BL.map -Wl,--cref

C_SRCS := $(wildcard $(MK_DIR)Src/*.c) \
          $(ROOT)/stm/stm32g4xx/system_stm32g4xx.c

ASM_SRCS := $(STARTUP)

OBJS := $(addprefix $(BUILD_DIR)/, \
          $(notdir $(C_SRCS:.c=.o)) \
          $(notdir $(ASM_SRCS:.s=.o)))

ELF := $(BUILD_DIR)/$(TARGET)_BL.elf
BIN := $(BUILD_DIR)/$(TARGET)_BL.bin

.PHONY: all clean

all: $(BIN)
	@$(SIZE) -B $(ELF)
	@TEXT=$$($(SIZE) -B $(ELF) | tail -1 | awk '{print $$1}'); \
	DATA=$$($(SIZE) -B $(ELF) | tail -1 | awk '{print $$2}'); \
	TOTAL=$$((TEXT + DATA)); \
	if [ $$TOTAL -gt $(BL_BUDGET) ]; then \
	  echo "BL size FAIL: $$TOTAL bytes exceeds budget $(BL_BUDGET)"; \
	  exit 1; \
	fi; \
	echo "BL size OK: $$TOTAL/$(BL_BUDGET)"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(MK_DIR)Src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/system_stm32g4xx.o: $(ROOT)/stm/stm32g4xx/system_stm32g4xx.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(ROOT)/stm/stm32g4xx/stm32g491/%.s | $(BUILD_DIR)
	$(AS) $(CPU_FLAGS) $(DEFS) -c $< -o $@

$(BUILD_DIR)/%.o: $(ROOT)/stm/stm32g4xx/stm32g431/%.s | $(BUILD_DIR)
	$(AS) $(CPU_FLAGS) $(DEFS) -c $< -o $@

$(ELF): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(MK_DIR)build
