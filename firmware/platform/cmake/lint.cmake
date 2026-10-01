### Lint integration: clang-tidy (per-file, during the build) and clang-format targets
# Config lives at the repo root: .clang-tidy, .clang-format, .clang-format-ignore
include_guard()

option(CLANG_TIDY "Run clang-tidy on LHRS-authored sources while building" OFF)

# vendored, read-only upstream code -- never linted or formatted
# keep in sync with .clang-format-ignore
set(VENDORED_REGEX "/firmware/platform/(stm|middleware)/")

### lint(<target>)
function(lint TARGET_NAME)
    if(NOT CLANG_TIDY)
        return()
    endif()

    # SKIP_LINTING is how vendored sources in the same target are left alone
    if(CMAKE_VERSION VERSION_LESS 3.27)
        message(FATAL_ERROR "CLANG_TIDY needs CMake >= 3.27 (have ${CMAKE_VERSION}) -- use the nix dev shell.")
    endif()

    find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)

    # clang parses the arm-none-eabi-gcc command line but cannot find newlib on its
    # own -- hand it the target and the gcc toolchain's sysroot explicitly
    execute_process(
        COMMAND ${CMAKE_C_COMPILER} -print-sysroot
        OUTPUT_VARIABLE ARM_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        COMMAND_ERROR_IS_FATAL ANY
    )
    get_filename_component(ARM_SYSROOT "${ARM_SYSROOT}" ABSOLUTE)

    set_target_properties(${TARGET_NAME} PROPERTIES
        C_CLANG_TIDY "${CLANG_TIDY_EXE};--quiet;--extra-arg=--target=arm-none-eabi;--extra-arg=--sysroot=${ARM_SYSROOT}"
    )

    get_target_property(TARGET_SOURCES ${TARGET_NAME} SOURCES)
    get_target_property(TARGET_SOURCE_DIR ${TARGET_NAME} SOURCE_DIR)
    foreach(SRC IN LISTS TARGET_SOURCES)
        get_filename_component(SRC_ABS "${SRC}" ABSOLUTE BASE_DIR "${TARGET_SOURCE_DIR}")
        if(SRC_ABS MATCHES "${VENDORED_REGEX}")
            set_source_files_properties("${SRC}" PROPERTIES SKIP_LINTING ON)
        endif()
    endforeach()

    message(STATUS "clang-tidy enabled for ${TARGET_NAME}")
endfunction()

### add_format_targets(<dir>...)
function(add_format_targets)
    find_program(CLANG_FORMAT_EXE NAMES clang-format)
    if(NOT CLANG_FORMAT_EXE)
        message(STATUS "clang-format not found -- format targets unavailable")
        return()
    endif()

    set(FORMAT_GLOBS "")
    foreach(DIR IN LISTS ARGN)
        list(APPEND FORMAT_GLOBS "${DIR}/*.c" "${DIR}/*.h")
    endforeach()
    file(GLOB_RECURSE FORMAT_SOURCES CONFIGURE_DEPENDS ${FORMAT_GLOBS})
    list(FILTER FORMAT_SOURCES EXCLUDE REGEX "/build/")
    list(FILTER FORMAT_SOURCES EXCLUDE REGEX "${VENDORED_REGEX}")

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${FORMAT_SOURCES}
        COMMENT "clang-format: rewriting sources in place"
        VERBATIM
    )
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${FORMAT_SOURCES}
        COMMENT "clang-format: checking sources"
        VERBATIM
    )
endfunction()
