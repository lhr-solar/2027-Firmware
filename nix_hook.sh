#!/usr/bin/env bash

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# --- Python Setup ---
VENV_PATH="$SCRIPT_DIR/.venv"

if [ -f "$SCRIPT_DIR/pyproject.toml" ]; then
    export UV_PROJECT_ENVIRONMENT="$VENV_PATH"
    uv sync --quiet --project "$SCRIPT_DIR"
    source "$VENV_PATH/bin/activate"
fi


# --- Helpers ---
if [[ "$OSTYPE" == "darwin"* ]]; then
    lsusb_mac() { system_profiler SPUSBDataType; }
    export -f lsusb_mac
    
    ls_stm32_dev_port() { ls /dev/cu.*; }
    export -f ls_stm32_dev_port
fi