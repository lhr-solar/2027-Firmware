# LVC board build
# only `make` configures and builds; every other target works on the
# existing build and fails if there isn't one

BUILD_DIR := build
TOOLCHAIN := ../../platform/cmake/toolchain.cmake

# only pass TEST when given -- otherwise cmake keeps the cached one (default prod)
# `make TEST=` (empty) switches back to the production app
TEST_ARG := $(if $(filter undefined,$(origin TEST)),,-DTEST=$(TEST))

.PHONY: all prod-all configure flash flash-usb erase dump-symbols dump_size clean test-all

all: configure
	cmake --build $(BUILD_DIR) --target LVC

configure:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) $(TEST_ARG)

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

# For CI - builds production firmware for all instances of this board
prod-all:
	cmake -B $(BUILD_DIR)/build1 -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=
	cmake -B $(BUILD_DIR)/build2 -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=
	cmake --build $(BUILD_DIR)/build1 --target LVC	
	cmake --build $(BUILD_DIR)/build2 --target LVC

# For CI - builds all tests for all instances of this board
test-all:
	./build_all_tests.sh
