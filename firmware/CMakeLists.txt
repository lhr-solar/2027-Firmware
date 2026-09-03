### Top level CMake directives
## Config hardcoded for G473 series MCUS
cmake_minimum_required(VERSION 3.28.0) # somewhat arbitrary (wsl install)

project(
    App
    VERSION 1.0.0
    DESCRIPTION "APP"
    LANGUAGES C ASM # asm for startup file
)

include(${CMAKE_CURRENT_LIST_DIR}/platform/Platform.cmake)

### Board selection
# default folder to look for entry points is platform tests
set(ENTRY_POINTS "platform/tests/Src")

set(BOARD_SOURCES "")
set(BOARD_INCLUDE_DIRS "")

set(EXEC_NAME "App")

set(BOARD "" CACHE STRING "Board to build (e.g. LVC), empty for a platform test")
if(BOARD)
    set(BOARD_CMAKE "${CMAKE_CURRENT_SOURCE_DIR}/psys/${BOARD}/Board.cmake")
    if(NOT EXISTS "${BOARD_CMAKE}")
        message(FATAL_ERROR "Unknown BOARD '${BOARD}' -- no ${BOARD_CMAKE}.")
    endif()
    include(${BOARD_CMAKE})
endif()

### Compile Tests
# unsupported tests on the g473
set(DISABLED_TESTS
    can
    can_isr
    can_mt
    can_queueSet
)

# cache blinky at first -> if recompiled with TEST variable then overwrites the last cached var
set(TEST "blinky" CACHE STRING "Name of test/app to build")

# check if test is disabled for g473
list(FIND DISABLED_TESTS "${TEST}" TEST_DISABLED_INDEX)
if(NOT TEST_DISABLED_INDEX EQUAL -1)
    message(FATAL_ERROR "TEST '${TEST}' is disabled on this board -- see DISABLED_TESTS in CMakeLists.txt.")
endif()

# resolve entry points -> file paths
set(TEST_SOURCE "")
foreach(DIR ${ENTRY_POINTS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/${TEST}.c")
        set(TEST_SOURCE "${DIR}/${TEST}.c")
        break()
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/${TEST}_test.c")
        set(TEST_SOURCE "${DIR}/${TEST}_test.c")
        break()
    endif()
endforeach()

if(TEST_SOURCE STREQUAL "")
    message(FATAL_ERROR "Unknown TEST '${TEST}' -- not found in ${ENTRY_POINTS}.")
endif()

### Compile and link source exec
add_executable(${EXEC_NAME}
    ${TEST_SOURCE}
    ${STARTUP_SOURCE}
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${FATFS_SOURCES}
    ${PSP_SOURCES}
    ${UTILS_SOURCES}
    ${DRIVERS_SOURCES}
    ${BOARD_SOURCES}
)
target_include_directories(${EXEC_NAME} PRIVATE platform/tests/Inc ${APP_INCLUDE_DIRS} ${BOARD_INCLUDE_DIRS})

build_firmware(${EXEC_NAME})

# must have jolly good message
string(ASCII 27 Esc)
set(ColorReset "${Esc}[0m")
set(ColorGreen "${Esc}[32m")
set(ColorBoldGreen "${Esc}[1;32m")

message(STATUS "${ColorBoldGreen}=== Jolly good! ===${ColorReset}")
