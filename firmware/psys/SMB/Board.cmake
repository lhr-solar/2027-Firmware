### Segment Monitoring Board Config
# paths below are relative to this board's own CMakeLists.txt (firmware/psys/SMB/)
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
)

### INCLUDES (*.h)
# Populate with all the includes ...
set(BOARD_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/drivers/Inc
    ${CMAKE_CURRENT_LIST_DIR}/config/Inc
    ${CMAKE_CURRENT_LIST_DIR}/tests/Inc
)
