# Shared board build rules -- the whole of every board's Makefile is an include of this file.
# Boards don't add or override targets; a board's settings live in its Board.cmake.
# only `make` configures and builds; flash, erase, dump-* work on the existing build and fail
# if there isn't one; clean, format and format-check need no build at all

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

CLANG_FORMAT ?= clang-format

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

# first line of every recipe that works on the existing build: if there isn't one, say what to
# run instead of CMake's "not a build directory". A board with variants builds into
# $(BUILD_ROOT)/<variant>/ (from `make VARIANT=<name>` or `make prod-all`), never $(BUILD_ROOT)/
# itself, so without VARIANT list the variants that are built rather than claim nothing is.
# It never picks one: an image built for no particular instance must never be flashed.
define require_build
@if [ ! -f "$(BUILD_DIR)/build.ninja" ]; then \
    if [ -n "$(VARIANT)" ]; then \
        echo "error: nothing built in $(BUILD_DIR)/ -- run \`make VARIANT=$(VARIANT)\` first" >&2; \
        exit 1; \
    fi; \
    variants=$$(cmake -DBOARD_DIR="$(CURDIR)" -DQUERY=BOARD_VARIANTS -P "$(PLATFORM_DIR)/cmake/board_query.cmake") || exit 1; \
    if [ -z "$$variants" ]; then \
        echo "error: nothing built in $(BUILD_DIR)/ -- run \`make\` first" >&2; \
        exit 1; \
    fi; \
    built=""; \
    for v in $$variants; do \
        if [ -f "$(BUILD_ROOT)/$$v/build.ninja" ]; then built="$$built $$v"; fi; \
    done; \
    if [ -n "$$built" ]; then \
        echo "error: this board has variants -- pick a build with \`make $@ VARIANT=<name>\`; built:$$built" >&2; \
    else \
        echo "error: nothing built -- this board has variants, run \`make VARIANT=<name>\` (or \`make prod-all\`) first; variants: $$variants" >&2; \
    fi; \
    exit 1; \
fi
endef

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
	$(if $(strip $(BUILD_DIR)),,$(error BUILD_DIR is empty -- refusing to clean))
	rm -rf -- "$(BUILD_DIR)"

# clang-format, see .clang-format at repo root
# runs clang-format directly on this board's .c/.h files: no CMake, no build folder, no VARIANT.
# Paths are absolute, as CI's lint job reads them back out of the output. Build folders are
# skipped -- CMake writes its own .c files into them.
FORMAT_SOURCES = find "$(CURDIR)" \( -name build -o -path "$(abspath $(BUILD_ROOT))" \) -prune \
	-o -type f \( -name '*.c' -o -name '*.h' \) -exec $(CLANG_FORMAT)

define require_clang_format
@command -v $(CLANG_FORMAT) >/dev/null || { \
    echo "error: $(CLANG_FORMAT) not found -- use the nix dev shell (see README)" >&2; exit 1; }
endef

format:
	$(require_clang_format)
	@echo "clang-format: rewriting $(notdir $(CURDIR)) sources in place"
	@$(FORMAT_SOURCES) -i {} +

format-check:
	$(require_clang_format)
	@echo "clang-format: checking $(notdir $(CURDIR)) sources"
	@$(FORMAT_SOURCES) --dry-run --Werror {} +
