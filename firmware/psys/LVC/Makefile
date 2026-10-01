# LVC board build
# thin wrapper -- every target just calls cmake

BUILD_DIR := build
TOOLCHAIN := ../../platform/cmake/toolchain.cmake

TEST ?=

TIDY ?= OFF

.PHONY: all prod-all configure flash erase dump-symbols dump_size clean test-all format format-check

all: configure
	cmake --build $(BUILD_DIR) --target LVC

configure:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST) -DCLANG_TIDY=$(TIDY)

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

# clang-format, see .clang-format at repo root
format: configure
	cmake --build $(BUILD_DIR) --target format

format-check: configure
	cmake --build $(BUILD_DIR) --target format-check

# For CI - builds production firmware for all instances of this board
prod-all:
	cmake -B $(BUILD_DIR)/build1 -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST) -DCLANG_TIDY=$(TIDY)
	cmake -B $(BUILD_DIR)/build2 -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST) -DCLANG_TIDY=$(TIDY)
	cmake --build $(BUILD_DIR)/build1 --target LVC	
	cmake --build $(BUILD_DIR)/build2 --target LVC

# For CI - builds all tests for all instances of this board
test-all:
	./build_all_tests.sh
