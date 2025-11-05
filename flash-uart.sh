#!/usr/bin/env bash
set -e

# Usage: ./flash-uart.sh <file> <address>
# Example: ./flash-uart.sh build/stm32l431cbt.bin 0x08000000

FLASH_FILE="$1"
FLASH_ADDRESS="$2"

# ----------------------------
# Input validation
# ----------------------------
if [[ -z "$FLASH_FILE" || -z "$FLASH_ADDRESS" ]]; then
    echo "Usage: $0 <path_to_flash_file> <flash_address>"
    exit 1
fi

if [[ ! -f "$FLASH_FILE" ]]; then
    echo "❌ ERROR: Flash file does not exist: $FLASH_FILE"
    exit 1
fi

# ----------------------------
# Detect OS
# ----------------------------
OS="$(uname -s)"
echo "🖥  OS detected: $OS"

case "$OS" in
    Linux*)
        STM32PROG="$HOME/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"
        ;;
    Darwin*)
        STM32PROG="/Applications/STM32CubeProgrammer.app/Contents/MacOs/STM32_Programmer_CLI"
        ;;
    *)
        echo "❌ Unsupported OS: $OS"
        exit 1
        ;;
esac

# ----------------------------
# Check STM32CubeProgrammer
# ----------------------------
if [[ ! -x "$STM32PROG" ]]; then
    echo "❌ ERROR: STM32_Programmer_CLI not found at:"
    echo "   $STM32PROG"
    echo "   Please install STM32CubeProgrammer or fix the path."
    exit 1
fi

# ----------------------------
# Detect CP210x UART
# ----------------------------
echo "🌉 Detecting CP210x UART bridge..."

detect_linux() {
    for t in /dev/ttyUSB*; do
        [[ -e "$t" ]] || continue
        u=$(udevadm info -q property -n "$t" | grep ID_VENDOR_ID)
        p=$(udevadm info -q property -n "$t" | grep ID_MODEL_ID)
        if [[ "$u" == "ID_VENDOR_ID=10c4" && "$p" == "ID_MODEL_ID=ea60" ]]; then
            echo "$t"
            return
        fi
    done
}

detect_macos() {
    # Shortlist: /dev/tty.usbserial-* or /dev/tty.SLAB_USBtoUART
    for t in /dev/tty.usbserial* /dev/tty.SLAB_USBtoUART*; do
        [[ -e "$t" ]] && echo "$t" && return
    done

    # Fallback: detect CP210x by system_profiler
    MAYBE_CP210=$(system_profiler SPUSBDataType 2>/dev/null | \
        awk '/CP210/ {found=1} found && /Serial Number/ {print; exit}')

    if [[ -n "$MAYBE_CP210" ]]; then
        # Still need a device path → assume tty.SLAB_USBtoUART
        echo "/dev/tty.SLAB_USBtoUART"
    fi
}

if [[ "$OS" == "Linux" ]]; then
    PORT=$(detect_linux)
else
    PORT=$(detect_macos)
fi

if [[ -z "$PORT" ]]; then
    echo "❌ ERROR: CP210x device not found!"
    echo "   Make sure your CP2102/CP210x USB-UART is connected."
    exit 1
fi

echo "⚓ Using $PORT"

# ----------------------------
# Flash
# ----------------------------
echo "🔦 Flashing $FLASH_FILE to $FLASH_ADDRESS"
$STM32PROG -c port="$PORT" -w "$FLASH_FILE" "$FLASH_ADDRESS" -v
RET=$?

if [[ $RET -eq 0 ]]; then
    echo "✅ Flash complete"
else
    echo "❌ Flash failed (exit $RET)"
fi

exit $RET
