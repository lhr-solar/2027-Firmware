# Platform test build
# thin wrapper -- every target just calls cmake

BUILD_DIR := build
TOOLCHAIN := cmake/toolchain.cmake

TEST ?= blinky

.PHONY: all test-all configure flash erase dump-symbols dump_size clean

all: configure
	cmake --build $(BUILD_DIR) --target App

configure:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST)

flash: configure
	cmake --build $(BUILD_DIR) --target flash

erase: configure
	cmake --build $(BUILD_DIR) --target erase

dump-symbols: configure
	cmake --build $(BUILD_DIR) --target dump-symbols

dump_size: configure
	cmake --build $(BUILD_DIR) --target dump_size

clean:
	cmake --build $(BUILD_DIR) --target clean

# For CI - builds all tests, fails if any failed
test-all:
	./build-all-tests.sh
