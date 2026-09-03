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

.PHONY: all configure flash clean dump-symbols

# requires generating build files every build
all: configure
	cmake --build $(BUILD_DIR) --target App

configure:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST) -DBOARD=$(BOARD)

flash: 
	st-flash write $(BUILD_DIR)/App.bin $(FLASH_ADDRESS)

# dump elf debug symbols to see what got linked
dump-symbols:
	objdump -x build/App.elf

clean:
	rm -rf $(BUILD_DIR)

