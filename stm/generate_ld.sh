#!/bin/bash

# Input the path to the CFG file as the first argument

# Generate linker script for STM32 based on LINKER_TEMPLATE.ld
CFG_FILE=$1
if [ -z "$CFG_FILE" ]
then
    echo -e "${RED}Please provide the path to the CFG file.${NC}"
    exit 1
fi

if [ ! -f "$CFG_FILE" ]
then
    echo -e "${RED}The file $CFG_FILE does not exist.${NC}"
    exit 1
fi

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Source the CFG file to extract values
source "$CFG_FILE"

MCU_NAME_CAP=$(echo "$MCU_NAME" | tr '[:lower:]' '[:upper:]')
APP_SIZE=$(($TOTAL_FLASH_SIZE - $BOOT_SIZE))
BOOT_OFFSET=$(printf "0x0%X" $((0x08000000 + ($APP_SIZE * 1024))))

LINKER_TEMPLATE="LINKER_TEMPLATE.ld"

OUTPUT_FLASH_SCRIPT="${MCU_NAME_CAP}x_APP.ld"
OUTPUT_BOOT_SCRIPT="${MCU_NAME_CAP}x_BOOT.ld"

echo -e "${GREEN}Generating linker script for $MCU_NAME${NC}"
echo -e "${YELLOW}RAM size: $RAM_SIZE Kb${NC}"
echo -e "${YELLOW}Application flash offset: 0x08000200${NC}"
echo -e "${YELLOW}Application flash size: $APP_SIZE Kb${NC}"
echo -e "${YELLOW}Boot flash offset: $BOOT_OFFSET${NC}"
echo -e "${YELLOW}Boot flash size: $BOOT_SIZE Kb${NC}"

FLASH_NAME=FLASH

sed -e "s/\${RAM_SIZE}/${RAM_SIZE}K/g" \
    -e "s/\${APP_SIZE}/${APP_SIZE}K/g" \
    -e "s/\${BOOT_OFFSET}/${BOOT_OFFSET}/g" \
    -e "s/\${BOOT_SIZE}/${BOOT_SIZE}K/g" \
    -e "s/\${FLASH_NAME}/${FLASH_NAME}/g" \
    "$LINKER_TEMPLATE" > "$OUTPUT_FLASH_SCRIPT"

FLASH_NAME=BOOT

sed -e "s/\${RAM_SIZE}/${RAM_SIZE}K/g" \
    -e "s/\${APP_SIZE}/${APP_SIZE}K/g" \
    -e "s/\${BOOT_OFFSET}/${BOOT_OFFSET}/g" \
    -e "s/\${BOOT_SIZE}/${BOOT_SIZE}K/g" \
    -e "s/\${FLASH_NAME}/${FLASH_NAME}/g" \
    -e '/\.isr_vector\s*:/,/}/d' \
    "$LINKER_TEMPLATE" > "$OUTPUT_BOOT_SCRIPT"

echo -e "${GREEN}Generated flash script at $OUTPUT_FLASH_SCRIPT${NC}"
echo -e "${GREEN}Generated boot script at $OUTPUT_BOOT_SCRIPT${NC}"

echo -e "${BLUE}Jolly good! Move the generated files to the appropriate directory!${NC}"
