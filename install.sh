#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
distro=$(cat /etc/issue)
packages=(arm_toolchain build_essential gdb_multiarch openocd stlink_tools renode_simulator sphinx)

function arm_toolchain(){
    local version=12.2
    local src="https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz?rev=7bd049b7a3034e64885fa1a71c12f91d&hash=732D909FA8F68C0E1D0D17D08E057619"

    # ARM toolchain for compiling/debugging code
    echo -e "${RED}\nInstalling Arm Toolchain...\n==================================\n${NC}"
    wget -c -N --progress=bar ${src} -O /tmp/arm-none-eabi.tar.xz &&
    echo "Extracting tar, this may take a few minutes"
    sudo tar -xf /tmp/arm-none-eabi.tar.xz --totals -C /usr/share/
    sudo mv /usr/share/arm-gnu-toolchain-${version}.rel1-x86_64-arm-none-eabi /usr/share/arm-gnu-toolchain &&
    sudo rm /tmp/arm-none-eabi.tar.xz

    sudo ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc
    sudo ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
    sudo ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy
    sudo ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
    sudo ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb

    echo -e "${RED}\nInstalling arm-none-eabi-gdb dependencies...\n==================================\n${NC}"

    # Install dependencies for arm-none-eabi-gdb
    sudo apt install libncurses-dev
    sudo ln -sf /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
    sudo ln -sf /usr/lib/x86_64-linux-gnu/libncursesw.so.6 /usr/lib/x86_64-linux-gnu/libncursesw.so.5
    sudo ln -sf /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
    
    # Get python 3.8 (required for arm-none-eabi-gdb)
    sudo add-apt-repository ppa:deadsnakes/ppa
    sudo apt-get install python3.8
}

function openocd(){
    # Starts a debugging service on the microcontroller
    echo -e "${RED}\nInstalling openocd...\n==================================\n${NC}"
    sudo apt-get -y install openocd
}

function stlink_tools(){
    # For flashing code to the microcontroller
    echo -e "${RED}\nInstalling stlink_tools...\n==================================\n${NC}"
    sudo apt-get -y install stlink-tools
}

function gdb_multiarch(){
    # For debugging code on the microcontroller
    echo -e "${RED}\nInstalling gdb-multiarch...\n==================================\n${NC}"
    sudo apt-get -y install gdb-multiarch
}

function build_essential(){
    # For making and compiling projects
    echo -e "${RED}\nInstalling build-essential...\n==================================\n${NC}"
    sudo apt-get -y install build-essential
}

function renode_simulator(){
    local version=1.13.3
    
    # For real-time simulation of the microcontroller
    echo -e "${RED}\nInstalling Renode Simulator...\n==================================\n${NC}"
    
    wget -c -N --progress=bar "https://github.com/renode/renode/releases/download/v${version}/renode-${version}.linux-portable.tar.gz" -O /tmp/renode-portable.tar.gz &&
    sudo mkdir -p /usr/share/renode_portable &&
    sudo tar -xf /tmp/renode-portable.tar.gz --totals -C /usr/share/renode_portable --strip-components=1 &&
    sudo ln -sf /usr/share/renode_portable/renode /usr/bin/renode
}

function sphinx(){
    # For documentation
    echo -e "${RED}\nInstalling sphinx...\n==================================\n${NC}"
    sudo apt-get -y install python3-sphinx
}

function print_usage() {
    echo -e "Usage: ./uninstall.sh [-Yy] [-Hh]"
    exit 0
}

if [ "${distro:0:4}" = "Arch" ]; then
    #special case for arch linux
    sudo pacman -Sy arm-none-eabi-newlib stlink openocd --noconfirm
    exit 0
fi

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

    read -p $"Install ${pack}? (y/n/q)> " -r
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        $pack
    elif [[ $REPLY =~ ^[Qq]$ ]]; then
        exit 0
    fi
done

# Make sure that necessary OS submodules are initialized
if [ -d ".git" ]; then
    echo -e "${RED}\nUpdating and initializing submodules...\n==================================\n${NC}"
    git submodule update --init --recursive
fi

echo -e "${RED}\nFinished! Jolly good!\n${NC}"
