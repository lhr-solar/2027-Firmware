# Top level makefile
# wraps cmake build calls
# flashing utils

BUILD_DIR      := build
TOOLCHAIN      := ../toolchain.cmake
OPENOCD_CFG    := openocd-stm32g4x.cfg
FLASH_ADDRESS  := 0x8000000

# compile blinky if no board & test specified
# compile board prod if board & no test specified
# recomputed on every invocation -- an unset TEST always defaults fresh
# based on THIS run's BOARD, it does not stick to a previous run's value
BOARD ?=
TEST  ?=
ifeq ($(strip $(BOARD)),)
TEST := $(if $(TEST),$(TEST),blinky)
else
TEST := $(if $(TEST),$(TEST),app)
endif

.PHONY: all configure flash clean

all: configure
	cmake --build $(BUILD_DIR) --target App

configure:
	cmake -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST) -DBOARD=$(BOARD)

flash: all
	st-flash write $(BUILD_DIR)/App.bin $(FLASH_ADDRESS)

clean:
	rm -rf $(BUILD_DIR)

