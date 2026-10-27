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

### Variants -- one build per instance of the board on the car, see BOARD_VARIANTS in Board.cmake
set(VARIANT "" CACHE STRING "Board variant to build, empty for none")

# variant names become C macros (BOARD_VARIANT_<NAME>), so they must be valid identifiers
foreach(NAME IN LISTS BOARD_VARIANTS)
    if(NOT NAME MATCHES "^[A-Za-z][A-Za-z0-9_]*$")
        message(FATAL_ERROR "Variant name '${NAME}' in BOARD_VARIANTS -- use letters, digits and underscores, starting with a letter.")
    endif()
endforeach()

# a typo in BOARD_VARIANT_<name>_DEFINITIONS would otherwise silently drop its definitions
get_cmake_property(ALL_VARIABLES VARIABLES)
foreach(VAR IN LISTS ALL_VARIABLES)
    if(VAR MATCHES "^BOARD_VARIANT_(.+)_DEFINITIONS$")
        if(NOT CMAKE_MATCH_1 IN_LIST BOARD_VARIANTS)
            message(FATAL_ERROR "${VAR} is set in Board.cmake, but '${CMAKE_MATCH_1}' is not in BOARD_VARIANTS.")
        endif()
    endif()
endforeach()

if(NOT "${VARIANT}" STREQUAL "" AND NOT VARIANT IN_LIST BOARD_VARIANTS)
    if("${BOARD_VARIANTS}" STREQUAL "")
        message(FATAL_ERROR "Unknown VARIANT '${VARIANT}' -- this board has no variants (see BOARD_VARIANTS in Board.cmake).")
    endif()
    string(REPLACE ";" ", " VARIANT_CHOICES "${BOARD_VARIANTS}")
    message(FATAL_ERROR "Unknown VARIANT '${VARIANT}' -- choose one of: ${VARIANT_CHOICES} (see BOARD_VARIANTS in Board.cmake).")
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

# conditional compilation: board-wide definitions, then the chosen variant's
target_compile_definitions(${PROJECT_NAME} PRIVATE ${BOARD_COMPILE_DEFINITIONS})
if(NOT "${VARIANT}" STREQUAL "")
    string(TOUPPER "${VARIANT}" VARIANT_UPPER)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        BOARD_VARIANT_${VARIANT_UPPER}
        ${BOARD_VARIANT_${VARIANT}_DEFINITIONS}
    )
    message(STATUS "Variant: ${VARIANT}")
endif()

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
