### Battery Protection System Leader Config
# paths below are relative to this board's own CMakeLists.txt (firmware/psys/BPSL/)
include_guard()

### SOURCES (*.c)
# path to file containing production code
set(BOARD_PROD_SOURCE
    "core/Src/app.c"
)
# path to directory containing tests
set(BOARD_TEST_SOURCE_DIR
    "tests/Src"
)
# Drivers/modules that are ALWAYS linked
file(GLOB BOARD_OTHER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_LIST_DIR}/drivers/Src/*.c" # drivers/
    "${CMAKE_CURRENT_LIST_DIR}/core/Src/common.c" # board-wide Error_Handler, SystemClock_Config
)

### INCLUDES (*.h)
# Populate with all the includes ...
set(BOARD_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/drivers/Inc
    ${CMAKE_CURRENT_LIST_DIR}/config/Inc
    ${CMAKE_CURRENT_LIST_DIR}/tests/Inc
)

# Preprocessor definitions for every build of this board, e.g. USE_IMU SD_LOGGING=1
set(BOARD_COMPILE_DEFINITIONS "")

# Variants: one build per instance of this board on the car, picked with `make VARIANT=<name>`,
# e.g. set(BOARD_VARIANTS front rear). Each build gets BOARD_VARIANT_<NAME> defined (here
# BOARD_VARIANT_FRONT or BOARD_VARIANT_REAR); put per-variant values in a config/Inc header
# behind #if defined(BOARD_VARIANT_FRONT). With variants, plain `make` must be given one.
# `make prod-all` and `make test-all` build every variant into build/<name>/ -- leave as "" for a single build
set(BOARD_VARIANTS "")
