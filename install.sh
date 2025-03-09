#!/bin/bash
set -e

RED='\033[0;31m'
NC='\033[0m' # No Color

# Check for root/sudo
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root or with sudo privileges."
    exit 1
fi

# Detect distribution
source /etc/os-release
DISTRO=""
case $ID in
    ubuntu|debian) DISTRO="ubuntu" ;;
    fedora) DISTRO="fedora" ;;
    arch) DISTRO="arch" ;;
    *) echo "Unsupported distribution"; exit 1 ;;
esac

function install_gh() {
    echo -e "${RED}\nInstalling GitHub CLI...\n==================================\n${NC}"
    case $DISTRO in
        ubuntu)
            # Official GH CLI installation for Debian/Ubuntu
            apt-get install -y wget
            mkdir -p -m 755 /etc/apt/keyrings
            wget -qO- https://cli.github.com/packages/githubcli-archive-keyring.gpg | \
                tee /etc/apt/keyrings/githubcli-archive-keyring.gpg >/dev/null
            chmod go+r /etc/apt/keyrings/githubcli-archive-keyring.gpg
            echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/githubcli-archive-keyring.gpg] \
                https://cli.github.com/packages stable main" | \
                tee /etc/apt/sources.list.d/github-cli.list >/dev/null
            apt-get update
            apt-get install -y gh
            ;;

        fedora)
            # Official GH CLI installation for Fedora
            if dnf --version | grep -q 'dnf5'; then
                dnf install -y dnf5-plugins
                dnf config-manager --add-repo https://cli.github.com/packages/rpm/gh-cli.repo
            else
                dnf install -y 'dnf-command(config-manager)'
                dnf config-manager --add-repo https://cli.github.com/packages/rpm/gh-cli.repo
            fi
            dnf install -y gh --repo gh-cli
            ;;

        arch)
            # Official GH CLI package for Arch
            pacman -Sy --noconfirm github-cli
            ;;
    esac
}

function install_packages() {
    echo -e "${RED}\nInstalling System Packages...\n==================================\n${NC}"
    case $DISTRO in
        ubuntu)
            apt-get update
            apt-get install -y software-properties-common
            add-apt-repository ppa:deadsnakes/ppa -y
            apt-get update
            apt-get install -y vim wget build-essential gdb-multiarch openocd stlink-tools \
                libncurses-dev python3-pip git gnupg dirmngr bear picocom ca-certificates \
                openssh-client python3.10 python3.10-dev usbutils
            update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 1
            pip3 install mkdocs pyserial
            wget https://apt.llvm.org/llvm.sh -O /tmp/llvm.sh
            chmod +x /tmp/llvm.sh
            /tmp/llvm.sh 17
            apt-get install -y gcc-multilib
            rm /tmp/llvm.sh
            ;;

        fedora)
            dnf install -y python3.10 python3.10-devel
            dnf groupinstall -y "Development Tools" "Development Libraries"
            dnf install -y vim wget gdb openocd stlink ncurses-devel python3-pip git \
                gnupg bear picocom ca-certificates openssh-clients usbutils
            alternatives --set python3 /usr/bin/python3.10
            pip3 install mkdocs pyserial
            dnf install -y clang clang-tools-extra lld
            dnf install -y gcc-multilib
            ;;

        arch)
            pacman -Sy --noconfirm vim wget base-devel gdb openocd stlink ncurses \
                python-pip git gnupg bear picocom ca-certificates openssh usbutils
            pip3 install mkdocs pyserial
            pacman -Sy --noconfirm clang lldb lld
            sed -i '/^#\[multilib\]/{s/^#//;n;s/^#//}' /etc/pacman.conf
            pacman -Sy --noconfirm gcc-multilib
            ;;
    esac
}

function arm_toolchain() {
    local action="$1"
    case $DISTRO in
        ubuntu|fedora)
            local version=14.2
            local src="https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz"
            
            if [[ "$action" == "install" ]]; then
                echo -e "${RED}\nInstalling ARM Toolchain...\n==================================\n${NC}"
                wget -O /tmp/arm-none-eabi.tar.xz "$src"
                echo "Extracting tar, this may take a few minutes"
                tar -xf /tmp/arm-none-eabi.tar.xz -C /usr/share/
                mv "/usr/share/arm-gnu-toolchain-${version}.rel1-x86_64-arm-none-eabi" /usr/share/arm-gnu-toolchain
                rm /tmp/arm-none-eabi.tar.xz

                ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-{gcc,g++,objcopy,size,gdb} /usr/bin/

                echo -e "${RED}\nInstalling dependencies...\n==================================\n${NC}"
                if [[ "$DISTRO" == "ubuntu" ]]; then
                    apt-get install -y libncurses-dev python3.8
                    ln -sf /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
                    ln -sf /usr/lib/x86_64-linux-gnu/libncursesw.so.6 /usr/lib/x86_64-linux-gnu/libncursesw.so.5
                    ln -sf /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
                elif [[ "$DISTRO" == "fedora" ]]; then
                    dnf install -y ncurses-devel python3.8
                    ln -sf /usr/lib64/libncurses.so.6 /usr/lib64/libncurses.so.5
                    ln -sf /usr/lib64/libncursesw.so.6 /usr/lib64/libncursesw.so.5
                    ln -sf /usr/lib64/libtinfo.so.6 /usr/lib64/libtinfo.so.5
                fi

            elif [[ "$action" == "remove" ]]; then
                echo -e "${RED}\nRemoving ARM Toolchain...\n==================================\n${NC}"
                rm -f /usr/bin/arm-none-eabi-{gcc,g++,objcopy,size,gdb}
                rm -rf /usr/share/arm-gnu-toolchain
            fi
            ;;

        arch)
            if [[ "$action" == "install" ]]; then
                echo -e "${RED}\nInstalling ARM Toolchain for Arch...\n==================================\n${NC}"
                pacman -Sy --noconfirm gcc-arm-none-eabi-bin
                pacman -Sy --noconfirm gcc-multilib
            elif [[ "$action" == "remove" ]]; then
                pacman -Rns --noconfirm gcc-arm-none-eabi-bin gcc-multilib
            fi
            ;;
    esac
}

# Main execution
install_packages
install_gh
arm_toolchain install

echo -e "${RED}Installation completed successfully!${NC}"