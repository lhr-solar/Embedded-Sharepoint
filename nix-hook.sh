#!/usr/bin/env bash

# Exit on error
set -e

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# --- Python Setup ---
export PIP_DISABLE_PIP_VERSION_CHECK=1
VENV_PATH="$SCRIPT_DIR/.venv"
REQ_PATH="$SCRIPT_DIR/requirements.txt"
SENTINEL="$VENV_PATH/.pip_installed"

if [ ! -d "$VENV_PATH" ]; then
    echo "Creating virtual environment..."
    python3 -m venv "$VENV_PATH"
fi

source "$VENV_PATH/bin/activate"

if [ -f "$REQ_PATH" ]; then
    # Only install if the sentinel doesn't exist OR requirements.txt is newer than the sentinel
    if [ ! -f "$SENTINEL" ] || [ "$REQ_PATH" -nt "$SENTINEL" ]; then
        echo "Updating python requirements..."
        if pip install -q -r "$REQ_PATH"; then
            touch "$SENTINEL"
        else
            echo "Error: pip install failed."
            exit 1
        fi
    fi
fi


# Define potential global paths including the macOS Library path
GLOBAL_LINUX_DOT="$HOME/.arduino15"
GLOBAL_MAC_LIBRARY="$HOME/Library/Arduino15"
GLOBAL_MAC_LOCAL="$HOME/Arduino15"

if [ -d "$GLOBAL_LINUX_DOT" ]; then
    echo "Using existing Arduino data at $GLOBAL_LINUX_DOT"
    export ARDUINO_DIRECTORIES_DATA="$GLOBAL_LINUX_DOT"
elif [ -d "$GLOBAL_MAC_LIBRARY" ]; then
    echo "Using existing Arduino data at $GLOBAL_MAC_LIBRARY"
    export ARDUINO_DIRECTORIES_DATA="$GLOBAL_MAC_LIBRARY"
elif [ -d "$GLOBAL_MAC_LOCAL" ]; then
    echo "Using existing Arduino data at $GLOBAL_MAC_LOCAL"
    export ARDUINO_DIRECTORIES_DATA="$GLOBAL_MAC_LOCAL"
else
    echo "No global Arduino folder found. Using local project storage."
    export ARDUINO_DIRECTORIES_DATA="$SCRIPT_DIR/.arduino15"
fi

# Ensure the directory exists
mkdir -p "$ARDUINO_DIRECTORIES_DATA"

# Now proceed with the core check
if ! arduino-cli core list | grep -q "esp32" > /dev/null; then
    echo "Installing ESP32 core..."
    arduino-cli core update-index > /dev/null
    arduino-cli core install esp32:esp32 > /dev/null
fi

# --- Helpers ---
if [[ "$OSTYPE" == "darwin"* ]]; then
    lsusb_mac() { system_profiler SPUSBDataType; }
    export -f lsusb_mac
    
    ls_stm32_dev_port() { ls /dev/cu.*; }
    export -f ls_stm32_dev_port
fi
