### Platform test build
## Config hardcoded for G473 series MCUS
cmake_minimum_required(VERSION 3.25.3)

project(
    App
    VERSION 1.0.0
    DESCRIPTION "Platform Tests"
    LANGUAGES C ASM # asm for startup file
)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/platform.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/stm32.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/post_build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/custom_commands.cmake)

### Compile Tests
# unsupported tests on the g473
set(DISABLED_TESTS
    can
    can_isr
    can_mt
    can_queueSet
)

# check if test is disabled for g473
list(FIND DISABLED_TESTS "${TEST}" TEST_DISABLED_INDEX)
if(NOT TEST_DISABLED_INDEX EQUAL -1)
    message(FATAL_ERROR "TEST '${TEST}' is disabled on this board -- see DISABLED_TESTS in CMakeLists.txt.")
endif()

set(TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tests/Src/${TEST}_test.c")
if(NOT EXISTS "${TEST_SOURCE}")
    message(FATAL_ERROR "Unknown TEST '${TEST}' -- no ${TEST_SOURCE}.")
endif()

### Compile and link source exec
add_executable(App
    ${TEST_SOURCE}
    ${STARTUP_SOURCE}
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${FATFS_SOURCES}
    ${PSP_SOURCES}
    ${UTILS_SOURCES}
    ${DRIVERS_SOURCES}
)
target_include_directories(App PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests/Inc ${PLATFORM_INCLUDE_DIRS})

build_firmware(App)
post_build(App)

# external commands
add_flash_target(App)
add_dump_symbols_target(App)
add_dump_size_target(App)
add_erase_target()

# must have jolly good message
string(ASCII 27 Esc)
set(ColorReset "${Esc}[0m")
set(ColorBoldGreen "${Esc}[1;32m")
message(STATUS "${ColorBoldGreen}=== Jolly good! ===${ColorReset}")
