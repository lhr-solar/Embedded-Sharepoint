#!/bin/bash
set -e
echo "Running Container setup..."

# USB permissions
sudo tee /etc/udev/rules.d/99-stm32.rules > /dev/null <<EOF
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="1366", MODE="0666"
EOF
sudo udevadm control --reload-rules || true
sudo udevadm trigger || true

# Git CRLF
git config --global core.autocrlf input
git config --global core.eol lf

# Network test
ping -c1 google.com >/dev/null && echo "    ✅ Internet OK" || echo "    ⚠️ Internet unreachable"

# Tool checks
arm-none-eabi-gcc --version | head -1
openocd --version       | head -1
st-flash --version      | head -1

echo "Setup complete! Jolly good!"
