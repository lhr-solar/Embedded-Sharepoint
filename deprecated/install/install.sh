#!/bin/bash

RED='\033[0;31m'
ORANGE='\033[0;33m'
NC='\033[0m'
distro=$(cat /etc/issue)
packages=(arm_toolchain build-essential gdb-multiarch openocd stlink-tools renode_simulator documentation_toolset llvm)

function arm_toolchain(){
    # ARM toolchain for compiling/debugging code
    local version=12.2
    
    if [[ "$1" = "install" ]]; then
        # Install
        local src="https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz?rev=7bd049b7a3034e64885fa1a71c12f91d&hash=732D909FA8F68C0E1D0D17D08E057619"
        
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
    
    elif [[ "$1" = "remove" ]]; then
        # Remove
        echo -e "${RED}\nRemoving Arm Toolchain...\n==================================\n${NC}"
        
        # Removing symlinks
        sudo rm /usr/bin/arm-none-eabi-g++
        sudo rm /usr/bin/arm-none-eabi-objcopy
        sudo rm /usr/bin/arm-none-eabi-size
        sudo rm /usr/bin/arm-none-eabi-gdb
        
        # Removing toolchain dir
        sudo rm -r /usr/share/arm-gnu-toolchain*/
    fi
}

function renode_simulator(){
    # For real-time simulation of the microcontroller
    local version=1.14.0
    
    if [[ "$1" = "install" ]]; then
        # Install
        echo -e "${RED}\nInstalling Renode Simulator...\n==================================\n${NC}"
        
        wget -c --progress=bar "https://github.com/renode/renode/releases/download/v${version}/renode-${version}.linux-portable.tar.gz" -O /tmp/renode-portable.tar.gz &&
        sudo mkdir -p /usr/share/renode_portable &&
        sudo tar -xf /tmp/renode-portable.tar.gz --totals -C /usr/share/renode_portable --strip-components=1 &&
        sudo ln -sf /usr/share/renode_portable/renode /usr/bin/renode
        
        sudo apt install python3-pip
        pip install -r /usr/share/renode_portable/tests/requirements.txt
    
    elif [[ "$1" = "remove" ]]; then
        # Remove
        echo -e "${RED}\nRemoving Renode Simulator...\n==================================\n${NC}"

        # Remove symlink
        sudo rm /usr/bin/renode

        # Remove directory
        sudo rm -r /usr/share/renode_portable/
    fi
}

function documentation_toolset(){
    sphinx "$1"
    doxygen "$1"
    breathe "$1"
}

function sphinx(){
    # For documentation (ReadTheDocs)
    if [[ "$1" = "install" ]]; then
        # Install
        echo -e "${RED}\nInstalling sphinx...\n==================================\n${NC}"
        sudo apt-get -y install python3-sphinx
        pip install sphinx_rtd_theme
    
    elif [[ "$1" = "remove" ]]; then
        # Remove
        echo -e "${RED}\Removing sphinx...\n==================================\n${NC}"
        pip uninstall sphinx_rtd_theme
        sudo apt-get remove python3-sphinx
    fi
}

function doxygen(){
    # For docs generation from C source & header files

    local version=1.9.7
    
    if [[ "$1" = "install" ]]; then
        # Install
        echo -e "${RED}\nInstalling doxygen...\n==================================\n${NC}"

        wget -c --progress=bar "https://github.com/doxygen/doxygen/releases/download/Release_${version//./_}/doxygen-${version}.linux.bin.tar.gz" -O /tmp/doxygen.bin.tar.gz &&
        sudo mkdir -p /usr/share/doxygen &&
        sudo tar -xf /tmp/doxygen.bin.tar.gz --totals -C /usr/share/doxygen --strip-components=1 &&
        sudo ln -sf /usr/share/doxygen/bin/doxygen /usr/bin/doxygen
    
    elif [[ "$1" = "remove" ]]; then
        # Remove
        echo -e "${RED}\Removing doxygen...\n==================================\n${NC}"

        # Remove symlink
        sudo rm /usr/bin/doxygen

        # Remove directory
        sudo rm -r /usr/share/doxygen/
    fi
}

function breathe(){
    # For linking doxygen and sphinx

    if [[ "$1" = "install" ]]; then
        # Install
        echo -e "${RED}\nInstalling breathe...\n==================================\n${NC}"
        pip install breathe
    
    elif [[ "$1" = "remove" ]]; then
        # Remove
        echo -e "${RED}\nRemoving breathe...\n==================================\n${NC}"
        pip uninstall breathe
    fi
}

function llvm(){
    # For clang tidy and clang format

    if [[ "$1" = "install" ]]; then
        # Install
        wget https://apt.llvm.org/llvm.sh 
        chmod +x llvm.sh
        sudo ./llvm.sh 17 all
        sudo apt-get install gcc-multilib
        sudo rm llvm.sh
    
    elif [[ "$1" = "remove" ]]; then
        # Remove
        echo -e "Need to remove manually :("
    fi
}

function print_usage() {
    echo -e "Usage: ./install.sh [-Ii] [-Uu] [-Hh]"
    echo -e "   For each package, choose between (y/n/r/q) to:"
    echo -e "       y: yes, install"
    echo -e "       n: no, don't install"
    echo -e "       r: remove package"
    echo -e "       q: quit"
    echo -e "   [-Ii] install all available packages"
    echo -e "   [-Uu] remove all available packages"
    echo -e "   [-Hh] show this help page"
    exit 0
}

if [ "${distro:0:4}" = "Arch" ]; then
    #special case for arch linux
    sudo pacman -Sy arm-none-eabi-newlib stlink openocd --noconfirm
    exit 0
fi

# Check for options (-y -h -u)
install_all=0
remove_all=0
while getopts 'IiUuHh' OPTION; do
    case "$OPTION" in
        I|i) 
            install_all=1 ;;
        U|u)
            remove_all=1 ;;
        H|h) 
            print_usage ;;
        ?) 
            print_usage ;;
    esac
done

if [ $install_all -eq 1 ] && [ $remove_all -eq 1 ]; then
    echo -e "Invalid flags."
    exit 0;
fi

# Loop through each package
for pack in ${packages[@]}; do
    if [[ $install_all -eq 1 ]]; then
        mode="install"
    elif [[ $remove_all -eq 1 ]]; then
        mode="remove"
    else
        echo -e -n "\n${ORANGE}${pack}: (y/n/r/q)>${NC} "
        read -r
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            mode="install"
        elif [[ $REPLY =~ ^[Rr]$ ]]; then
            mode="remove"
        elif [[ $REPLY =~ ^[Qq]$ ]]; then
            exit 0
        else
            mode="none"
        fi
    fi

    if [[ "$mode" != "none" ]]; then
        
        if [ "$(type -t $pack)" = 'function' ]; then
            # Use special behavior
            $pack "$mode"
        else
            # Install with apt-get if no special behavior
            echo -e "${RED}"
            
            if [[ "$mode" == "install" ]]; then echo -e -n "Installing "; else echo -e -n "Removing "; fi

            echo -e "${pack}...\n==================================\n${NC}"
            sudo apt-get -y "$mode" $pack
        fi
    fi
done

# Make sure that necessary OS submodules are initialized
if [ -d ".git" ]; then
    echo -e "${RED}\nUpdating and initializing submodules...\n==================================\n${NC}"
    git submodule update --init --recursive
fi

echo -e "${RED}\nFinished! Jolly good!\n${NC}"
