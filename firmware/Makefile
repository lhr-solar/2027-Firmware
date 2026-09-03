# Thin pass-through wrapper around the CMake build.
#
#   make TEST=blinky        # configure + build build/App.elf/.bin/.hex from blinky_test.c
#   make TEST=can flash     # build then flash App.bin over ST-Link
#   make TEST=can debug     # build, start openocd, and attach gdb
#   make clean              # wipe the build dir
#
# Only one App.elf/.bin/.hex ever exists in the build dir -- switching TEST
# always reconfigures so the previous test's binary is replaced, not kept
# alongside it.

BUILD_DIR      := build
TOOLCHAIN      := ../toolchain.cmake
OPENOCD_CFG    := platform/openocd-stm32g4x.cfg
FLASH_ADDRESS  := 0x8000000

TEST ?= blinky

.PHONY: all configure flash debug clean

all: configure
	cmake --build $(BUILD_DIR) --target App

configure:
	cmake -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST)

flash: all
	st-flash write $(BUILD_DIR)/App.bin $(FLASH_ADDRESS)

debug: all
	openocd -f $(OPENOCD_CFG) &
	gdb-multiarch $(BUILD_DIR)/App.elf -ex "target extended-remote localhost:3333"

clean:
	rm -rf $(BUILD_DIR)
	