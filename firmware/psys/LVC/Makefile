# LVC board build
# every target except the two below comes from platform/board.mk

include ../../platform/board.mk

# For CI - builds production firmware for all instances of this board
prod-all:
	$(MAKE) BUILD_DIR=$(BUILD_DIR)/build1 all
	$(MAKE) BUILD_DIR=$(BUILD_DIR)/build2 all

# For CI - builds all tests for this board
test-all: tests
