#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

pushd $SCRIPT_DIR

# Input the path to the CFG file as the first argument
# Generate linker script for STM32 based on LINKER_TEMPLATE.ld
CFG_FILE=$1
if [ -z "$CFG_FILE" ]
then
  echo -e "${RED}Please provide the path to the CFG file (relative to Embedded-Sharepoint/stm).${NC}"
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
BOOT_SIZE=128 #Kb
APP_OFFSET=$(printf "0x0%X" $((0x08000000 + ($BOOT_SIZE * 1024))))
APP_SIZE=$(($TOTAL_FLASH_SIZE - $BOOT_SIZE))

LINKER_TEMPLATE="LINKER_TEMPLATE.ld"

LINKER_VARS="_sidata _sdata _edata _sbss _ebss _estack _Min_Heap_Size _Min_Stack_Size"

OUTPUT_FLASH_SCRIPT="$SCRIPT_DIR/${MCU_NAME_CAP}x_APP.ld"
OUTPUT_BOOT_SCRIPT="$SCRIPT_DIR/${MCU_NAME_CAP}x_BOOT.ld"

RAM_SIZE=$(($RAM_SIZE - 1))

echo -e "${GREEN}Generating linker script for $MCU_NAME${NC}"
echo -e "${YELLOW}RAM size: $RAM_SIZE Kb${NC}"
echo -e "${YELLOW}Boot flash offset: 0x08000000${NC}"
echo -e "${YELLOW}Boot flash size: $BOOT_SIZE Kb${NC}"
echo -e "${YELLOW}Application flash offset: $APP_OFFSET${NC}"
echo -e "${YELLOW}Application flash size: $APP_SIZE Kb${NC}"

FLASH_NAME=FLASH
ENTRY_PT=Reset_Handler # defined as main so that it gets defined immediately after the vector table
WRITE_PERM=
sed -e "s/\${RAM_SIZE}/${RAM_SIZE}K/g" \
    -e "s/\${APP_SIZE}/${APP_SIZE}K/g" \
    -e "s/\${APP_OFFSET}/${APP_OFFSET}/g" \
    -e "s/\${BOOT_SIZE}/${BOOT_SIZE}K/g" \
    -e "s/\${FLASH_NAME}/${FLASH_NAME}/g" \
    -e "s/\${ENTRY_PT}/${ENTRY_PT}/g" \
    -e "s/\${WRITE_PERM}/${WRITE_PERM}/g" \
    "$LINKER_TEMPLATE" > "$OUTPUT_FLASH_SCRIPT"

# for var in $LINKER_VARS
# do
#     sed -i "s/${var}/${var}_${FLASH_NAME,,}/g" "$OUTPUT_FLASH_SCRIPT"
# done

FLASH_NAME=BOOT
ENTRY_PT=Reset_Handler
WRITE_PERM=w
sed -e "s/\${RAM_SIZE}/${RAM_SIZE}K/g" \
    -e "s/\${APP_SIZE}/${APP_SIZE}K/g" \
    -e "s/\${APP_OFFSET}/${APP_OFFSET}/g" \
    -e "s/\${BOOT_SIZE}/${BOOT_SIZE}K/g" \
    -e "s/\${FLASH_NAME}/${FLASH_NAME}/g" \
    -e "s/\${ENTRY_PT}/${ENTRY_PT}/g" \
    -e "s/\${WRITE_PERM}/${WRITE_PERM}/g" \
    "$LINKER_TEMPLATE" > "$OUTPUT_BOOT_SCRIPT"
# -e "/\.reset/,/}/d" \

# for var in $LINKER_VARS
# do
#     sed -i "s/${var}/${var}_${FLASH_NAME,,}/g" "$OUTPUT_BOOT_SCRIPT"
# done

echo -e "${GREEN}Generated flash script at $OUTPUT_FLASH_SCRIPT${NC}"
echo -e "${GREEN}Generated boot script at $OUTPUT_BOOT_SCRIPT${NC}"

echo -e "${BLUE}Jolly good! Move the generated files to the appropriate directory!${NC}"

popd 
