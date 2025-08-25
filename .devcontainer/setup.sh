#!/usr/bin/env bash

set -e

echo "Running container setup script..."

echo "[INFO] Making sure unix file format..."
sudo find /workdir -type f \( -name "*.sh" -o -name "*.py" -o -name "*.conf" -o -name "*.yml" -o -name "Makefile" \) -exec dos2unix {} +

# Activate Python virtual environment
echo "[INFO] Activating Python virtual environment..."
if [ -f "$HOME/.venv/bin/activate" ]; then
    # shellcheck source=/dev/null
    echo 'source "$HOME/.venv/bin/activate"' >> /home/dev/.bashrc
else
    echo "[WARN] Python virtual environment not found at $HOME/.venv"
fi

echo "[INFO] Installing Python dependencies with uv..."
if [ -f "/workdir/requirements.txt" ]; then
    uv pip sync /workdir/requirements.txt
else
    echo "[WARN] requirements.txt not found at /workdir"
fi

# USB permissions
echo "[INFO] Editing udev rules..."
sudo tee /etc/udev/rules.d/99-stm32.rules > /dev/null << 'EOF'
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="1366", MODE="0666"
EOF

echo "[INFO] Reloading udev rules..."
sudo udevadm control --reload-rules || true
sleep 1
sudo udevadm trigger || true

# Git settings
echo "[INFO] Configuring Git line endings..."
git config --global core.autocrlf input
git config --global core.eol lf
# echo "[INFO] Configuring SSH for Git..."
# chown -R dev:dev /home/dev/.ssh
# chmod 700 /home/dev/.ssh
# chmod 600 /home/dev/.ssh/config
# chmod 600 /home/dev/.ssh/id_*
# chmod 600 /home/dev/.ssh/config
# sudo chmod 644 /home/dev/.ssh/*.pub || true

# Network test
echo "[INFO] Testing network connectivity..."
if ping -c1 google.com >/dev/null 2>&1; then
  echo "    Internet OK"
else
  echo "    ⚠️ Internet unreachable"
fi

#Check tools
echo "[INFO] Checking ARM toolchain..."
arm-none-eabi-gcc --version || { echo "[ERROR] ARM GCC missing!" >&2; exit 1; }

echo "[INFO] Checking OpenOCD..."
openocd --version || { echo "[ERROR] OpenOCD missing!" >&2; exit 1; }

echo "[INFO] Checking STM32 programmer connection..."
if lsusb | grep -iq "STMicroelectronics"; then
    echo "[INFO] STM32 device found!"
else
    echo "[WARN] No STM32 device detected."
fi

echo "[INFO] Checking MkDocs installation..."
if ! python -c "import mkdocs" &> /dev/null; then
    echo "[ERROR] MkDocs not installed!"
    exit 1
fi
echo "[INFO] MkDocs is installed!"

echo "[DONE] Setup complete! Jolly good!"
