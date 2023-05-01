#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
distro=$(cat /etc/issue)
packages=(arm_toolchain renode_simulator)

function arm_toolchain(){
    local version=12.2

    # ARM toolchain for compiling/debugging code
    echo -e "${RED}\nRemoving Arm Toolchain...\n==================================\n${NC}"
    
    # Removing symlinks
    sudo rm /usr/bin/arm-none-eabi-g++
    sudo rm /usr/bin/arm-none-eabi-objcopy
    sudo rm /usr/bin/arm-none-eabi-size
    sudo rm /usr/bin/arm-none-eabi-gdb
    
    # Removing toolchain dir
    sudo rm -r /usr/share/arm-gnu-toolchain*/
}

function renode_simulator(){
    # For real-time simulation of the microcontroller
    echo -e "${RED}\nRemoving Renode Simulator...\n==================================\n${NC}"

    # Remove symlink
    sudo rm /usr/bin/renode

    # Remove directory
    sudo rm -r /usr/share/renode_portable/
}

function print_usage() {
    echo -e "Usage: ./uninstall.sh [-Yy] [-Hh]"
    exit 0
}

# Check for options (-y -h)
yes_flag=0
while getopts 'YyHh' OPTION; do
    case "$OPTION" in
        Y|y) 
            yes_flag=1 ;;
        H|h) 
            print_usage ;;
        ?) 
            print_usage ;;
    esac
done

# Loop through and uninstall each package
for pack in ${packages[@]}; do
    if [[ "$yes_flag" -eq 1 ]]; then
        $pack
        continue
    fi

    read -p "Remove ${pack}? (y/n/q)> " -r
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        $pack
    elif [[ $REPLY =~ ^[Qq]$ ]]; then
        exit 0
    fi
done

echo -e "${RED}\nFinished! Jolly good!\n${NC}"