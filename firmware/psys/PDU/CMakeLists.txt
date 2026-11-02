### Power Distribution Unit board build
## Config hardcoded for G473 series MCUS
cmake_minimum_required(VERSION 3.25.3)

project(
    PDU
    VERSION 1.0.0
    DESCRIPTION "Power Distribution Unit"
    LANGUAGES C ASM # asm for startup file
)

include(${CMAKE_CURRENT_LIST_DIR}/../../platform/cmake/platform.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../platform/cmake/stm32.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../platform/cmake/post_build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../platform/cmake/custom_commands.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Board.cmake)

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
add_executable(PDU
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
target_include_directories(PDU PUBLIC ${PLATFORM_INCLUDE_DIRS} ${BOARD_INCLUDE_DIRS})

build_firmware(PDU)
post_build(PDU)

# external commands
add_flash_target(PDU)
add_dump_symbols_target(PDU)
add_dump_size_target(PDU)
add_erase_target()

# must have jolly good message
string(ASCII 27 Esc)
set(ColorReset "${Esc}[0m")
set(ColorBoldGreen "${Esc}[1;32m")

message(STATUS "${ColorBoldGreen}=== Jolly good! ===${ColorReset}")
