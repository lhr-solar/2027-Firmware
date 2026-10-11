# Platform test build
# only `make` configures and builds; every other target works on the
# existing build and fails if there isn't one

BUILD_DIR := build
TOOLCHAIN := cmake/toolchain.cmake

TEST ?= blinky

.PHONY: all test-all configure flash flash-usb erase dump-symbols dump_size clean

all: configure
	cmake --build $(BUILD_DIR) --target App

configure:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST)

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

# For CI - builds all tests, fails if any failed
test-all:
	./build_all_tests.sh
