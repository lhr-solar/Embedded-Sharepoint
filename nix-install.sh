#!/usr/bin/env bash
set -e

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

OS="$(uname -s)"

# Check for root/sudo (Linux only)
if [[ "$OS" == "Linux" && $EUID -ne 0 ]]; then
    echo "This script must be run as root or with sudo privileges."
    echo "please run with sudo. For example:"
    echo "sudo ./nix_install.sh"
    exit 1
fi

echo -e "${GREEN}Checking Nix installation...${NC}"

# Handle pre-existing Nix backup issue (Linux only)
if [[ "$OS" == "Linux" && -f /etc/bash.bashrc.backup-before-nix ]]; then
    echo -e "${YELLOW}Existing /etc/bash.bashrc.backup-before-nix found. Renaming it...${NC}"
    sudo mv /etc/bash.bashrc.backup-before-nix /etc/bash.bashrc.backup-before-nix."$(date +%s)"
fi

# Install nix if missing
if ! command -v nix &>/dev/null; then
    echo -e "${YELLOW}Nix is not installed. Installing...${NC}"
    if [[ "$OS" == "Darwin" ]]; then
        sh <(curl -L https://nixos.org/nix/install)
    else
        sh <(curl -L https://nixos.org/nix/install) --daemon
    fi
else
    echo -e "${GREEN}Nix is already installed.${NC}"
fi

echo -e "${GREEN}Detecting Nix installation type...${NC}"

CONFIG_FILE="$HOME/.config/nix/nix.conf"
NEED_RESTART=false

if [[ "$OS" == "Linux" ]]; then
    if systemctl list-units --type=service 2>/dev/null | grep -q nix-daemon.service; then
        if [ "$(id -u)" -eq 0 ]; then
            CONFIG_FILE="/etc/nix/nix.conf"
        else
            CONFIG_FILE="$HOME/.config/nix/nix.conf"
        fi
        NEED_RESTART=true
        echo -e "${YELLOW}Multi-user Nix detected.${NC}"
    else
        echo -e "${YELLOW}Single-user Nix detected.${NC}"
    fi
else
    # On macOS, Nix always installs multi-user via launchctl
    CONFIG_FILE="/etc/nix/nix.conf"
    NEED_RESTART=true
    echo -e "${YELLOW}Multi-user Nix (macOS launchctl) detected.${NC}"
fi

echo -e "Using config file: ${GREEN}$CONFIG_FILE${NC}"

# Create config dir if missing
sudo mkdir -p "$(dirname "$CONFIG_FILE")"

# Add flakes support
if grep -q "experimental-features" "$CONFIG_FILE" 2>/dev/null; then
    if grep -q "nix-command" "$CONFIG_FILE" && grep -q "flakes" "$CONFIG_FILE"; then
        echo -e "${GREEN}Flakes already enabled.${NC}"
    else
        echo -e "${YELLOW}Updating experimental-features line...${NC}"
        if [[ "$OS" == "Darwin" ]]; then
            sudo sed -i '' '/experimental-features/ s/$/ nix-command flakes/' "$CONFIG_FILE"
        else
            sudo sed -i '/experimental-features/ s/$/ nix-command flakes/' "$CONFIG_FILE"
        fi
    fi
else
    echo -e "${YELLOW}Adding flakes support...${NC}"
    echo "experimental-features = nix-command flakes" | sudo tee -a "$CONFIG_FILE" >/dev/null
fi

# Restart daemon if needed
if $NEED_RESTART; then
    if [[ "$OS" == "Linux" ]]; then
        echo -e "${YELLOW}Restarting nix-daemon (systemd)...${NC}"
        sudo systemctl restart nix-daemon
    elif [[ "$OS" == "Darwin" ]]; then
        echo -e "${YELLOW}Restarting nix-daemon (launchctl)...${NC}"
        sudo launchctl kickstart -k system/org.nixos.nix-daemon
    fi
fi

# Load nix profile
if [ -e "$HOME/.nix-profile/etc/profile.d/nix.sh" ]; then
    source "$HOME/.nix-profile/etc/profile.d/nix.sh"
elif [ -e "/nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh" ]; then
    source "/nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh"
fi

# Install latest Nix
echo -e "${GREEN}Installing latest Nix version for full flakes support...${NC}"
nix-env -iA nixpkgs.nixVersions.latest -j auto

# Verify flake commands work
echo -e "${GREEN}Verifying flake support...${NC}"
if nix flake --help &>/dev/null; then
    echo -e "${GREEN}Flakes are now fully enabled!${NC}"
else
    echo -e "${RED}Something went wrong: flake commands not working.${NC}"
fi

# Linux-only nix-bin (skip on macOS)
if [[ "$OS" == "Linux" ]]; then
    sudo apt-get update -o Acquire::Retries=3
    sudo apt-get install -y --fix-missing nix-bin || true
fi

# --- Add shell prompt hook for flakes ---
USER_SHELL="$(basename "$SHELL")"

NIX_SNIPPET='
# Nix
if [ -e "/nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh" ]; then
  . "/nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh"
fi
# End Nix
'

add_snippet_if_missing() {
    local file="$1"
    if [ -f "$file" ]; then
        if ! grep -q "nix-daemon.sh" "$file" 2>/dev/null; then
            echo -e "${YELLOW}Adding Nix init to $file...${NC}"
            echo "$NIX_SNIPPET" >> "$file"
        else
            echo -e "${GREEN}Nix already configured in $file.${NC}"
        fi
    fi
}

echo -e "${GREEN}Configuring shell integration for: $USER_SHELL${NC}"

case "$USER_SHELL" in
    zsh)
        add_snippet_if_missing "$HOME/.zshrc"
        ;;
    bash)
        add_snippet_if_missing "$HOME/.bashrc"
        ;;
    *)
        echo -e "${YELLOW}Unknown shell ($USER_SHELL). Falling back to ~/.profile${NC}"
        add_snippet_if_missing "$HOME/.profile"
        ;;
esac

# Optional: also add to ~/.profile for broader compatibility (login shells, SSH, etc.)
add_snippet_if_missing "$HOME/.profile"

# Reload current shell config
echo -e "${YELLOW}Reloading shell configuration...${NC}"
if [[ -n "$ZSH_VERSION" ]]; then
    source "$HOME/.zshrc"
elif [[ -n "$BASH_VERSION" ]]; then
    source "$HOME/.bashrc"
else
    source "$HOME/.profile"
fi

echo -e "${GREEN}✅ Nix installation finished. Restart your shell to see ❄ when inside a flake.${NC}"
