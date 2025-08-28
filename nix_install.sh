#!/usr/bin/env bash
set -e

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Check for root/sudo
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root or with sudo privileges."
    echo "please run with sudo. For example:"
    echo "sudo ./nix_install.sh"
    exit 1
fi

echo -e "${GREEN}Checking Nix installation...${NC}"

# Handle pre-existing Nix backup issue
if [ -f /etc/bash.bashrc.backup-before-nix ]; then
    echo -e "${YELLOW}Existing /etc/bash.bashrc.backup-before-nix found. Renaming it to preserve old backup...${NC}"
    sudo mv /etc/bash.bashrc.backup-before-nix /etc/bash.bashrc.backup-before-nix."$(date +%s)"
fi

if ! command -v nix &>/dev/null; then
    echo -e "${YELLOW}Nix is not installed. Installing...${NC}"
    # Install multi-user Nix by default
    sh <(curl -L https://nixos.org/nix/install) --daemon
else
    echo -e "${GREEN}Nix is already installed.${NC}"
fi

echo -e "${GREEN}Detecting Nix installation type...${NC}"

CONFIG_FILE="$HOME/.config/nix/nix.conf"
NEED_RESTART=false

# Check if multi-user daemon exists
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

echo -e "Using config file: ${GREEN}$CONFIG_FILE${NC}"

# Create the directory if needed
mkdir -p "$(dirname "$CONFIG_FILE")"

# Add flakes feature if missing
if grep -q "experimental-features" "$CONFIG_FILE" 2>/dev/null; then
    if grep -q "nix-command" "$CONFIG_FILE" && grep -q "flakes" "$CONFIG_FILE"; then
        echo -e "${GREEN}Flakes already enabled.${NC}"
    else
        echo -e "${YELLOW}Updating experimental-features line...${NC}"
        sed -i '/experimental-features/ s/$/ nix-command flakes/' "$CONFIG_FILE"
    fi
else
    echo -e "${YELLOW}Adding flakes support...${NC}"
    echo "experimental-features = nix-command flakes" | sudo tee -a "$CONFIG_FILE" >/dev/null
fi

# Restart daemon if needed
if $NEED_RESTART; then
    echo -e "${YELLOW}Restarting nix-daemon...${NC}"
    sudo systemctl restart nix-daemon
fi

# Load nix profile so 'nix' command works in this script
if [ -e "$HOME/.nix-profile/etc/profile.d/nix.sh" ]; then
    source "$HOME/.nix-profile/etc/profile.d/nix.sh"
elif [ -e "/nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh" ]; then
    source "/nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh"
fi

# Install latest Nix version (replaces nixUnstable)
echo -e "${GREEN}Installing latest Nix version for full flakes support...${NC}"
nix-env -iA nixpkgs.nixVersions.latest

# Verify flake commands work
echo -e "${GREEN}Verifying flake support...${NC}"
if nix flake --help &>/dev/null; then
    echo -e "${GREEN}Flakes are now fully enabled!${NC}"
else
    echo -e "${RED}Something went wrong: flake commands not working.${NC}"
fi

# Install NixOS bin
sudo apt install -y nix-bin

# --- Add shell prompt hook for flakes ---
BASHRC="$HOME/.bashrc"
ZSHRC="$HOME/.zshrc"

FLAKE_PROMPT_SNIPPET='
# Nix flake prompt
if [ -n "$IN_NIX_SHELL" ]; then
  FLAKE_NAME=$(basename "$(git rev-parse --show-toplevel 2>/dev/null || pwd)")
  export PS1="[\u@\h ❄ $FLAKE_NAME \W]$ "
fi
'

if ! grep -q "Nix flake prompt" "$BASHRC" 2>/dev/null; then
    echo -e "${YELLOW}Adding nix flake prompt to $BASHRC...${NC}"
    echo "$FLAKE_PROMPT_SNIPPET" >> "$BASHRC"
fi

if [ -f "$ZSHRC" ] && ! grep -q "Nix flake prompt" "$ZSHRC" 2>/dev/null; then
    echo -e "${YELLOW}Adding nix flake prompt to $ZSHRC...${NC}"
    echo "$FLAKE_PROMPT_SNIPPET" >> "$ZSHRC"
fi

echo -e "${GREEN}✅ Nix installation finished. Restart your shell to see ❄ when inside a flake.${NC}"