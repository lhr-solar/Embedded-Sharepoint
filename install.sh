#!/bin/bash

RED='\033[0;31m'
NC='\033[0m'
distro=$(cat /etc/issue)
packages=(arm_toolchain build_essential gdb_multiarch openocd stlink_tools renode_simulator sphinx)

function arm_toolchain(){
    # ARM toolchain for compiling/debugging code
    echo -e "${RED}\nInstalling Arm Toolchain...\n==================================\n${NC}"
    wget -c --progress=bar "https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz?rev=7bd049b7a3034e64885fa1a71c12f91d&hash=732D909FA8F68C0E1D0D17D08E057619" -O /tmp/arm-none-eabi.tar.xz &&
    echo "Extracting tar, this may take a few minutes"
    sudo tar -xf /tmp/arm-none-eabi.tar.xz --totals -C /usr/share/ &&
    rm /tmp/arm-none-eabi.tar.xz
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
    sudo ln -s /usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb

    echo -e "${RED}\nInstalling arm-none-eabi-gdb dependencies...\n==================================\n${NC}"

    # Install dependencies for arm-none-eabi-gdb
    sudo apt install libncurses-dev
    sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
    sudo ln -s /usr/lib/x86_64-linux-gnu/libncursesw.so.6 /usr/lib/x86_64-linux-gnu/libncursesw.so.5
    sudo ln -s /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
    
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
    # For flashing code to the microcotnroller
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
    # For real-time simulation of the microcontroller
    wget -q -nv -O - https://api.github.com/repos/renode/renode/releases/latest \
  | awk -F': ' '/browser_download_url/ && /\.linux-portable.tar.gz/ \
  {gsub(/"/, "", $(NF)); system("wget -c --progress=bar -i -L " $(NF) " -O /tmp/renode-portable.tar.gz")}' &&
    mkdir /usr/share/renode_portable &&
    tar xf renode-portable.tar.gz -C /usr/share/renode_portable --strip-components=1 &&
    sudo ln -s /usr/share/renode_portable/renode /usr/bin/renode
}

function sphinx(){
    # For documentation
    echo -e "${RED}\nInstalling sphinx...\n==================================\n${NC}"
    sudo apt-get -y install python3-sphinx
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