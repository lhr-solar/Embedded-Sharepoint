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
        
        # Explicitly use the venv's python binary to avoid hitting the read-only /nix/store
        if "$VENV_PATH/bin/python" -m pip install -q -r "$REQ_PATH"; then
            touch "$SENTINEL"
        else
            echo "Error: pip install failed."
            exit 1
        fi
    fi
fi


# --- Helpers ---
if [[ "$OSTYPE" == "darwin"* ]]; then
    lsusb_mac() { system_profiler SPUSBDataType; }
    export -f lsusb_mac
    
    ls_stm32_dev_port() { ls /dev/cu.*; }
    export -f ls_stm32_dev_port
fi
