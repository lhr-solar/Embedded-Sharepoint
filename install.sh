#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
distro=$(cat /etc/issue)
packages=(arm_toolchain openocd gdb_multiarch build_essential)

function arm_toolchain(){
    # Arm toolchain for compiling/debugging code
    echo -e "${RED}\nInstalling Arm Toolchain...\n==================================\n${NC}"
    wget -c --progress=bar "https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz?rev=7bd049b7a3034e64885fa1a71c12f91d&hash=732D909FA8F68C0E1D0D17D08E057619" -O /tmp/arm-none-eabi.tar.xz &&
    echo "Extracting tar, this may take a few minutes"
    sudo tar -xf /tmp/arm-none-eabi.tar.xz --totals -C /usr/share/ &&
    rm /tmp/arm-none-eabi.tar.xz &&
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb

    echo -e "${RED}\nInstalling arm-none-eabi-gdb dependencies...\n==================================\n${NC}"
    # Get python 3.8
    sudo add-apt-repository ppa:deadsnakes/ppa
    sudo apt-get install python3.8

    # Install dependencies for arm-none-eabi-gdb
    sudo apt install libncurses-dev
    sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
    sudo ln -s /usr/lib/x86_64-linux-gnu/libncursesw.so.6 /usr/lib/x86_64-linux-gnu/libncursesw.so.5
    sudo ln -s /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
}

function openocd(){
    # Debug code on the microcontroller
    echo -e "${RED}\nInstalling openocd...\n==================================\n${NC}"
    sudo apt-get -y install openocd
}

function gdb_multiarch(){
    # GDB multiarch
    echo -e "${RED}\nInstalling gdb-multiarch...\n==================================\n${NC}"
    sudo apt-get -y install gdb-multiarch
}

function build_essential(){
    # Run make command and compile projects
    echo -e "${RED}\nInstalling build-essential...\n==================================\n${NC}"
    sudo apt-get -y install build-essential
}

if [ "${distro:0:4}" = "Arch" ]; then
#special case for arch linux
sudo pacman -Sy arm-none-eabi-newlib stlink openocd --noconfirm
else

for pack in ${packages[@]}; do
    read -p "Install ${pack}? > " -r
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
fi