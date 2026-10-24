# Platform test build
# only `make` configures and builds; every other target works on the
# existing build and fails if there isn't one

BUILD_DIR := build
TOOLCHAIN := cmake/toolchain.cmake

TEST ?= blinky

TIDY ?= OFF

.PHONY: all test-all configure flash flash-usb erase dump-symbols dump_size clean format format-check

all: configure
	cmake --build $(BUILD_DIR) --target App

configure:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST) -DCLANG_TIDY=$(TIDY)

flash:
	cmake --build $(BUILD_DIR) --target flash

flash-usb:
	cmake --build $(BUILD_DIR) --target flash-usb

erase:
	cmake --build $(BUILD_DIR) --target erase

dump-symbols:
	cmake --build $(BUILD_DIR) --target dump-symbols

dump_size:
	cmake --build $(BUILD_DIR) --target dump_size

clean:
	cmake --build $(BUILD_DIR) --target clean

# clang-format, see .clang-format at repo root
format: 
	cmake --build $(BUILD_DIR) --target format

format-check: 
	cmake --build $(BUILD_DIR) --target format-check

# For CI - builds all tests, fails if any failed
test-all:
	./build_all_tests.sh
