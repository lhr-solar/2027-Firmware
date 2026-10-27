### Board build
# Shared build for every board: included from a board's CMakeLists.txt, after project().
# The board's settings come from its Board.cmake; the target is named after the project.
include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/platform.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stm32.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/post_build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/custom_commands.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/lint.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/Board.cmake)

# check if test is skipped for this board
list(FIND BOARD_SKIP_TESTS "${TEST}" TEST_DISABLED_INDEX)
if(NOT TEST_DISABLED_INDEX EQUAL -1)
    message(FATAL_ERROR "TEST '${TEST}' is disabled on this board -- see BOARD_SKIP_TESTS in Board.cmake.")
endif()

# set test to default to build prod
set(TEST "" CACHE STRING "Name of test to build, empty for production app")
if("${TEST}" STREQUAL "")
    set(BOARD_ENTRY_POINT "${CMAKE_CURRENT_SOURCE_DIR}/${BOARD_PROD_SOURCE}")
else()
    set(BOARD_ENTRY_POINT "${CMAKE_CURRENT_SOURCE_DIR}/${BOARD_TEST_SOURCE_DIR}/${TEST}_test.c")
endif()

if(NOT EXISTS "${BOARD_ENTRY_POINT}")
    message(FATAL_ERROR "Unknown TEST '${TEST}' -- no ${BOARD_ENTRY_POINT}.")
endif()

### Compile and link source exec
add_executable(${PROJECT_NAME}
    # board specific
    ${BOARD_ENTRY_POINT}
    ${BOARD_OTHER_SOURCES}
    # platform
    ${STARTUP_SOURCE}
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${FATFS_SOURCES}
    ${PSP_SOURCES}
    ${UTILS_SOURCES}
    ${DRIVERS_SOURCES}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${PLATFORM_INCLUDE_DIRS} ${BOARD_INCLUDE_DIRS})

build_firmware(${PROJECT_NAME})
post_build(${PROJECT_NAME})
lint(${PROJECT_NAME})

# external commands
add_flash_target(${PROJECT_NAME})
add_flash_usb_target(${PROJECT_NAME})
add_dump_symbols_target(${PROJECT_NAME})
add_dump_size_target(${PROJECT_NAME})
add_erase_target()
add_format_targets(${CMAKE_CURRENT_SOURCE_DIR})

# must have jolly good message
string(ASCII 27 Esc)
set(ColorReset "${Esc}[0m")
set(ColorBoldGreen "${Esc}[1;32m")

message(STATUS "${ColorBoldGreen}=== Jolly good! ===${ColorReset}")
