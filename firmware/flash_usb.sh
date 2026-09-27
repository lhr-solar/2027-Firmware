#!/usr/bin/env bash
set -e

# NOTE: The STM32 Cube Programmer CLI cannot detect new USB devices unless you
# add it as a rule to udev (monitors perms of devices you have plugged in).
#
# Run the following:
#
#   echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="df11", MODE="0666"' \
#     | sudo tee /etc/udev/rules.d/99-stm32-dfu.rules
#   sudo udevadm control --reload-rules && sudo udevadm trigger

# Usage: ./flash_usb.sh <file> <address>
# Example: ./flash_usb.sh build/App.bin 0x08000000

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
        STM32PROG="/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/Resources/bin/STM32_Programmer_CLI"
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
# Detect USB DFU port
# ----------------------------
echo "🔌 Detecting USB DFU device..."

# strip ANSI color codes first (STM32CubeProgrammer emits them even when
# piped), then just take the last word of the Device Index line -- e.g.
# "  Device Index           : USB1" -> "usb1"
PORT=$("$STM32PROG" -l usb 2>&1 | awk '
    { gsub(/\x1b\[[0-9;]*[a-zA-Z]/, "") }
    /Device Index/ { print tolower($NF); exit }
')

if [[ -z "$PORT" ]]; then
    echo "❌ ERROR: No USB DFU device found!"
    echo "   Make sure the board is connected over USB and in DFU mode"
    echo "   (BOOT0 held during reset)."
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
