# Shared board build -- included first thing by every board's Makefile
# thin wrapper -- every target just calls cmake
# The board's own Makefile defines prod-all and test-all (the targets CI runs)

# absolute path to firmware/platform/, so it works from any board and under make -C
# must be first: MAKEFILE_LIST ends with this file only until something else is included
PLATFORM_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
TOOLCHAIN    := $(PLATFORM_DIR)/cmake/toolchain.cmake

BUILD_DIR ?= build

TEST ?=

TIDY ?= OFF

# plain `make` builds all, even though the board's Makefile defines rules after including this
.DEFAULT_GOAL := all

.PHONY: all tests prod-all test-all configure flash flash-usb erase dump-symbols dump_size clean format format-check

# a board's CMake project has one executable, so building everything builds the board
all: configure
	cmake --build $(BUILD_DIR)

# paths are quoted: the absolute ones break on checkouts with spaces in the path otherwise
configure:
	cmake -B $(BUILD_DIR) -G Ninja "-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN)" -DTEST=$(TEST) -DCLANG_TIDY=$(TIDY)

# every test in tests/Src, minus BOARD_SKIP_TESTS in Board.cmake
tests:
	sh "$(PLATFORM_DIR)/build_board_tests.sh" "$(CURDIR)"

flash: configure
	cmake --build $(BUILD_DIR) --target flash

flash-usb: configure
	cmake --build $(BUILD_DIR) --target flash-usb

erase: configure
	cmake --build $(BUILD_DIR) --target erase

dump-symbols: configure
	cmake --build $(BUILD_DIR) --target dump-symbols

dump_size: configure
	cmake --build $(BUILD_DIR) --target dump_size

clean:
	cmake --build $(BUILD_DIR) --target clean

# clang-format, see .clang-format at repo root
format: configure
	cmake --build $(BUILD_DIR) --target format

format-check: configure
	cmake --build $(BUILD_DIR) --target format-check
