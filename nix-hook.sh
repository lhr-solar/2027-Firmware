#!/usr/bin/env bash

# --- Helpers ---
if [[ "$OSTYPE" == "darwin"* ]]; then
    lsusb_mac() { system_profiler SPUSBDataType; }
    export -f lsusb_mac
    
    ls_stm32_dev_port() { ls /dev/cu.*; }
    export -f ls_stm32_dev_port
fi