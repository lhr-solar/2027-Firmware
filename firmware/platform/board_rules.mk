# Shared board build rules -- the whole of every board's Makefile is an include of this file.
# Boards don't add or override targets; a board's settings live in its Board.cmake.
# only `make` configures and builds (format and format-check also configure); flash, erase,
# dump-* work on the existing build and fail if there isn't one; clean needs no build at all

# absolute path to firmware/platform/, so it works from any board and under make -C
# must be first: MAKEFILE_LIST ends with this file only until something else is included
PLATFORM_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
TOOLCHAIN    := $(PLATFORM_DIR)/cmake/toolchain.cmake

# board variant (BOARD_VARIANTS in Board.cmake), empty for none
# a board with variants has no default: plain `make` without VARIANT fails at configure
VARIANT ?=

# each variant gets its own build folder, so switching variants doesn't rebuild everything
BUILD_ROOT ?= build
BUILD_DIR ?= $(BUILD_ROOT)$(if $(VARIANT),/$(VARIANT))

TEST ?=

TIDY ?= OFF

# plain `make` builds all, whatever a board's Makefile might add after including this
.DEFAULT_GOAL := all

.PHONY: all prod-all test-all configure flash flash-usb erase dump-symbols dump-size clean format format-check

# a board's CMake project has one executable, so building everything builds the board
all: configure
	cmake --build $(BUILD_DIR)

# paths are quoted: the absolute ones break on checkouts with spaces in the path otherwise
configure:
	cmake -B $(BUILD_DIR) -G Ninja "-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN)" -DTEST=$(TEST) -DVARIANT=$(VARIANT) -DCLANG_TIDY=$(TIDY)

# production firmware for every variant in BOARD_VARIANTS (just VARIANT if set), each into
# $(BUILD_ROOT)/<variant>/, or one plain build when the board has none -- CI runs this
prod-all:
	sh "$(PLATFORM_DIR)/build_board_all.sh" prod "$(CURDIR)" "$(BUILD_ROOT)" "$(VARIANT)"

# every test in tests/Src, for every variant in BOARD_VARIANTS (just VARIANT if set) -- CI runs this
test-all:
	sh "$(PLATFORM_DIR)/build_board_all.sh" tests "$(CURDIR)" "$(BUILD_ROOT)" "$(VARIANT)"

# first line of every recipe that works on the existing build: if this build folder isn't
# configured, say what to run instead of CMake's "not a build directory". BUILD_DIR is
# $(BUILD_ROOT)/<variant>/ with VARIANT set, so one check covers both kinds of board
require_build = @test -f "$(BUILD_DIR)/build.ninja" || { \
	echo "error: nothing built in $(BUILD_DIR)/ -- run \`make$(if $(VARIANT), VARIANT=$(VARIANT))\` first (boards with variants need VARIANT=<name>)" >&2; exit 1; }

flash:
	$(require_build)
	cmake --build $(BUILD_DIR) --target flash

flash-usb:
	$(require_build)
	cmake --build $(BUILD_DIR) --target flash-usb

erase:
	$(require_build)
	cmake --build $(BUILD_DIR) --target erase

dump-symbols:
	$(require_build)
	cmake --build $(BUILD_DIR) --target dump-symbols

dump-size:
	$(require_build)
	cmake --build $(BUILD_DIR) --target dump-size

# deletes the build folder outright: just this variant's with VARIANT set, otherwise every build
# of this board; works whether or not anything was built
clean:
	rm -rf "$(BUILD_DIR)"

# clang-format, see .clang-format at repo root
format: configure
	cmake --build $(BUILD_DIR) --target format

format-check: configure
	cmake --build $(BUILD_DIR) --target format-check
