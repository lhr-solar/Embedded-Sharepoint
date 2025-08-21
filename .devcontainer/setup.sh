#!/usr/bin/env bash
set -euo pipefail

echo "Running container setup script..."

# USB permissions
sudo tee /etc/udev/rules.d/99-stm32.rules > /dev/null << 'EOF'
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="1366", MODE="0666"
EOF

sudo udevadm control --reload-rules || true
sudo udevadm trigger || true

# Git CRLF settings
git config --global core.autocrlf input
git config --global core.eol lf

# Network test
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

echo "[INFO] Checking MkDocs documentation..."
if mkdocs --version; then
    echo "[INFO] MkDocs is installed."
    mkdocs build --strict --quiet || echo "[WARN] MkDocs build failed -- check mkdocs.yml"
else
    echo "[ERROR] MkDocs not installed!" >&2
    exit 1
fi

echo "[DONE] Setup complete! Jolly good!"
