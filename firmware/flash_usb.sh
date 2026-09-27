#!/usr/bin/env bash
set -e

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

# strip ANSI color codes (STM32CubeProgrammer emits them even when piped)
# before parsing, so no escape sequence ends up stuck to the parsed port
PORT=$("$STM32PROG" -l usb 2>&1 | sed -E 's/\x1b\[[0-9;]*[a-zA-Z]//g' \
    | awk -F: '/Port/ {gsub(/^[ \t]+|[ \t]+$/, "", $2); print $2; exit}')

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
