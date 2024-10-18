#!/bin/bash

# Input the MCU name as the first argument

# Generate linker script for STM32 based on LINKER_TEMPLATE.ld
MCU_NAME=$1
MCU_NAME_CAP=$(echo "$MCU_NAME" | tr '[:lower:]' '[:upper:]')

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

read -p "$(echo -e ${YELLOW}Enter the continuous RAM1 size for your MCU in Kb: ${NC})" RAM_SIZE
read -p "$(echo -e ${YELLOW}Enter the total flash size for your MCU in Kb: ${NC})" TOTAL_FLASH_SIZE
read -p "$(echo -e ${YELLOW}Enter the percentage of flash to be used for the application: ${NC})" FLASH_APP_PERCENTAGE

FLASH_SIZE=$(($TOTAL_FLASH_SIZE * $FLASH_APP_PERCENTAGE / 100))
FLASH_NAME=FLASH

BOOT_SIZE=$(($TOTAL_FLASH_SIZE - $FLASH_SIZE))

BOOT_OFFSET=$(printf "0x0%X" $((0x08000000 + 0x0200 + $FLASH_SIZE)))

LINKER_TEMPLATE="LINKER_TEMPLATE.ld"

OUTPUT_FLASH_SCRIPT="${MCU_NAME_CAP}x_FLASH.ld"
OUTPUT_BOOT_SCRIPT="${MCU_NAME_CAP}x_BOOT.ld"

echo -e "${GREEN}Generating linker script for $MCU_NAME${NC}"
echo -e "${YELLOW}RAM size: $RAM_SIZE Kb${NC}"
echo -e "${YELLOW}Application flash size: $TOTAL_FLASH_SIZE Kb${NC}"
echo -e "${YELLOW}Boot size: $BOOT_SIZE Kb${NC}"
echo -e "${YELLOW}Boot offset: $BOOT_OFFSET${NC}"

sed -e "s/\${RAM_SIZE}/${RAM_SIZE}K/g" \
    -e "s/\${FLASH_SIZE}/${FLASH_SIZE}K/g" \
    -e "s/\${BOOT_OFFSET}/${BOOT_OFFSET}/g" \
    -e "s/\${BOOT_SIZE}/${BOOT_SIZE}K/g" \
    -e "s/\${FLASH_NAME}/${FLASH_NAME}/g" \
    "$LINKER_TEMPLATE" > "$OUTPUT_FLASH_SCRIPT"

FLASH_NAME=BOOT

sed -e "s/\${RAM_SIZE}/${RAM_SIZE}K/g" \
    -e "s/\${FLASH_SIZE}/${FLASH_SIZE}K/g" \
    -e "s/\${BOOT_OFFSET}/${BOOT_OFFSET}/g" \
    -e "s/\${BOOT_SIZE}/${BOOT_SIZE}K/g" \
    -e "s/\${FLASH_NAME}/${FLASH_NAME}/g" \
    "$LINKER_TEMPLATE" > "$OUTPUT_BOOT_SCRIPT"

echo -e "${GREEN}Generated flash script at $OUTPUT_FLASH_SCRIPT${NC}"
echo -e "${GREEN}Generated boot script at $OUTPUT_BOOT_SCRIPT${NC}"

echo -e "${BLUE}Jolly good! Move the generated files to the appropriate directory!${NC}"