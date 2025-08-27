#!/usr/bin/env sh
set -e

echo "Running container setup..."

# Convert line endings
echo "[INFO] Converting file formats..."
sudo find /workdir -type f \( -name "*.sh" -o -name "*.py" -o -name "*.conf" -o -name "*.yml" -o -name "Makefile" \) -exec dos2unix {} + 2>/dev/null || true

# SSH Setup
setup_ssh() {
    echo "[INFO] Setting up SSH..."
    sudo mkdir -p "$HOME/.ssh/sockets"
    sudo chmod 700 "$HOME/.ssh" "$HOME/.ssh/sockets"

    # Copy SSH keys from project if they exist
    if [ -d "/workdir/.devcontainer/.ssh" ]; then
        echo "  Copying SSH keys from project..."
        cp -r /workdir/.devcontainer/.ssh/* "$HOME/.ssh/" 2>/dev/null || true
        sudo find "$HOME/.ssh" -type f ! -name "*.pub" ! -name "config" ! -name "known_hosts*" -exec chmod 600 {} \;
        sudo find "$HOME/.ssh" -type f -name "*.pub" -exec chmod 644 {} \;
        sudo find "$HOME/.ssh" -type f -name "config" -exec chmod 600 {} \;
    fi

    # Start SSH agent if needed
    if ! pgrep -x ssh-agent >/dev/null 2>&1; then
        echo "  Starting ssh-agent..."
        eval "$(ssh-agent -s)" >/dev/null 2>&1
    fi

    # Collect keys (no arrays in sh, use string with space-separated keys)
    FOUND_KEYS=""
    for key in "$HOME"/.ssh/*; do
    if [ -f "$key" ] && [ -f "${key}.pub" ]; then
        case "$key" in
        *.pub|*known_hosts*|*config) ;;
        *)
            echo "  Found SSH key: $key"
            FOUND_KEYS="$FOUND_KEYS $key"
            ;;
        esac
    fi
    done


    # Add keys to agent
    for key in $FOUND_KEYS; do
        echo "  Adding key: $key..."
        ssh-add "$key" 2>/dev/null || true
    done
}

# Setup Python environment
setup_python() {
    echo "[INFO] Setting up Python environment..."

    if [ -f "$HOME/.venv/bin/activate" ]; then
        # Add venv activation to bashrc if not already there
        if ! grep -q "source.*venv.*activate" "$HOME/.bashrc" 2>/dev/null; then
            echo 'source "$HOME/.venv/bin/activate"' >> "$HOME/.bashrc"
        fi
        echo "  Python venv: OK"
    else
        echo "  Python venv not found at $HOME/.venv"
    fi

    if [ -f "/workdir/requirements.txt" ]; then
        echo "  Requirements: OK"
        pip list --format=columns
    else
        echo "  No requirements.txt found"
    fi
}

# Configure Git
setup_git() {
    echo "[INFO] Configuring Git..."
    git config --global core.autocrlf input
    git config --global core.eol lf
    git config --global init.defaultBranch main
    git config --global pull.rebase false
    git config --global push.default simple

    echo "  Configuring Git user..."
    username="$(git config --global user.name 2>/dev/null || echo "")"
    useremail="$(git config --global user.email 2>/dev/null || echo "")"

    if [ -z "$username" ]; then
        echo "   Git user not configured"
        echo "   Configure Git with:"
        echo "   git config --global user.name 'Your Name'"
        echo "   git config --global user.email 'your@email.com'"
    else
        echo "  Git user configured: $username <$useremail>"
    fi
}

# Setup USB permissions for STM32 devices
# TODO: confirm this works as expected
setup_usb() {
    echo "[INFO] Setting up USB permissions..."
    sudo tee /etc/udev/rules.d/99-stm32.rules > /dev/null << 'EOF'
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="1366", MODE="0666"
EOF
    sudo udevadm control --reload-rules 2>/dev/null || true
    sudo udevadm trigger 2>/dev/null || true

    echo "[INFO] Created udev rule at /etc/udev/rules.d/99-stm32.rules:"
    cat /etc/udev/rules.d/99-stm32.rules
}

run_tests() {
    echo "[INFO] Running tests..."

    if ping -c1 google.com >/dev/null 2>&1; then
        echo "  Network: OK"
    else
        echo "  Network: COOKED"
    fi

    if arm-none-eabi-gcc --version >/dev/null 2>&1; then
        echo "  ARM GCC: OK"
    else
        echo "  ARM GCC: COOKED"
        exit 1
    fi

    if openocd --version >/dev/null 2>&1; then
        echo "  OpenOCD: OK"
    else
        echo "  OpenOCD: COOKED"
        exit 1
    fi

    if lsusb | grep -iq "STMicroelectronics"; then
        echo "  STM32 device: OK"
    else
        echo "  STM32 device: NOT CONNECTED"
    fi

    if which python3 >/dev/null 2>&1; then
        echo "  Python3: OK"
    else
        echo "  Python3: COOKED"
        exit 1
    fi

    if which pip >/dev/null 2>&1; then
        echo "  pip: OK"
    else
        echo "  pip: COOKED"
        exit 1
    fi

    if which git >/dev/null 2>&1; then
        echo "  Git: OK"
    else
        echo "  Git: COOKED"
        exit 1
    fi

    if which ssh >/dev/null 2>&1; then
        echo "  SSH: OK"
    else
        echo "  SSH: COOKED"
        exit 1
    fi

    # SSH test
    FOUND_KEYS=""
    for key in "$HOME"/.ssh/*; do
        if [ -f "$key" ] && [ -f "${key}.pub" ]; then
            case "$key" in
                *.pub|*known_hosts*|*config) ;;
                *) FOUND_KEYS="$FOUND_KEYS $key" ;;
            esac
        fi
    done

    if [ -n "$FOUND_KEYS" ]; then
        echo "  SSH keys found $(echo $FOUND_KEYS | wc -w)"
        echo "    - Agent keys:"
        ssh-add -l || echo "      (ssh-agent not running or no keys loaded)"
        echo "    - GitHub SSH check:"
        SSH_GITHUB_OUTPUT=$(ssh -T git@github.com 2>&1) || STATUS=$?
        STATUS=${STATUS:-0}
        echo "      GitHub: $SSH_GITHUB_OUTPUT"

    else
        echo "  SSH keys: COOKED (add key pairs to .devcontainer/.ssh/)"
    fi
}

setup_ssh_agent() {
    # Probably want to append this to bashrc because setup.sh is only run once
    if ! grep -q "SSH Agent setup" "$HOME/.bashrc" 2>/dev/null; then
        cat >> "$HOME/.bashrc" << 'EOF'


# SSH Agent setup
if [ -z "$SSH_AUTH_SOCK" ] && ! pgrep -x ssh-agent >/dev/null 2>&1; then
    eval "$(ssh-agent -s)" >/dev/null 2>&1
fi

# Auto-add SSH keys
for key in ~/.ssh/*; do
    if [ -f "$key" ] && [ -f "${key}.pub" ]; then
        ssh-add -l 2>/dev/null | grep -q "$(basename "$key")" || ssh-add "$key" 2>/dev/null || true
    fi
done
EOF
    fi
}

setup_ssh
setup_python
setup_git
setup_usb
setup_ssh_agent
run_tests


echo "[DONE] Setup complete. Jolly good!"