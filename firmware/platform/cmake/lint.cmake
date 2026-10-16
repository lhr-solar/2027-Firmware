### Lint integration: clang-tidy and include-what-you-use (per-file, during the build) and clang-format targets
# Config lives at the repo root: .clang-tidy, .iwyu.imp, .clang-format, .clang-format-ignore
include_guard()

option(CLANG_TIDY "Run clang-tidy on LHRS-authored sources while building" OFF)
option(IWYU "Run include-what-you-use on LHRS-authored sources while building" OFF)

# vendored upstream code (stm/, middleware/) plus common/, which holds ST/FreeRTOS/newlib
# templates rather than LHR-authored code -- never linted or formatted
# keep in sync with .clang-format-ignore
set(VENDORED_REGEX "/firmware/platform/(stm|middleware|common)/")

# repo root, where the shared lint configs live
get_filename_component(LINT_REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)

### lint(<target>)
function(lint TARGET_NAME)
    if(NOT CLANG_TIDY AND NOT IWYU)
        return()
    endif()

    # SKIP_LINTING is how vendored sources in the same target are left alone
    if(CMAKE_VERSION VERSION_LESS 3.27)
        message(FATAL_ERROR "CLANG_TIDY/IWYU need CMake >= 3.27 (have ${CMAKE_VERSION}) -- use the nix dev shell.")
    endif()

    # clang parses the arm-none-eabi-gcc command line but cannot find newlib on its
    # own -- hand it the target and the gcc toolchain's sysroot explicitly
    execute_process(
        COMMAND ${CMAKE_C_COMPILER} -print-sysroot
        OUTPUT_VARIABLE ARM_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        COMMAND_ERROR_IS_FATAL ANY
    )
    get_filename_component(ARM_SYSROOT "${ARM_SYSROOT}" ABSOLUTE)

    if(CLANG_TIDY)
        find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
        set_target_properties(${TARGET_NAME} PROPERTIES
            C_CLANG_TIDY "${CLANG_TIDY_EXE};--quiet;--extra-arg=--target=arm-none-eabi;--extra-arg=--sysroot=${ARM_SYSROOT}"
        )
        message(STATUS "clang-tidy enabled for ${TARGET_NAME}")
    endif()

    if(IWYU)
        find_program(IWYU_EXE NAMES include-what-you-use iwyu REQUIRED)
        # nixpkgs wraps IWYU in a script that appends the *host* glibc and every dev-shell
        # include dir to CPATH, ahead of newlib -- <stdint.h> would silently resolve to x86
        # glibc. The real binary sits next to the wrapper; use it so IWYU sees only clang's
        # builtin headers and the arm-none-eabi sysroot, exactly as clang-tidy does.
        file(REAL_PATH "${IWYU_EXE}" IWYU_REAL)
        get_filename_component(IWYU_DIR "${IWYU_REAL}" DIRECTORY)
        if(EXISTS "${IWYU_DIR}/.include-what-you-use-unwrapped")
            set(IWYU_REAL "${IWYU_DIR}/.include-what-you-use-unwrapped")
        endif()

        set(IWYU_CMD
            "${IWYU_REAL}" --target=arm-none-eabi "--sysroot=${ARM_SYSROOT}"
            # IWYU only judges includes -- compiler warnings are gcc's and clang-tidy's job,
            # and clang's -Wall differs from gcc's (e.g. -Wunused-but-set-variable)
            -w
            # any suggestion fails the compile step, like clang-tidy WarningsAsErrors
            -Xiwyu --error
            # vendor umbrella headers -- see the file for why
            -Xiwyu "--mapping_file=${LINT_REPO_ROOT}/.iwyu.imp"
            # task.h/queue.h/semphr.h/timers.h #error unless FreeRTOS.h came first, but
            # IWYU only sees which header declares a symbol and would drop it
            -Xiwyu "--keep=*/FreeRTOS-Kernel/include/FreeRTOS.h"
        )
        set_target_properties(${TARGET_NAME} PROPERTIES C_INCLUDE_WHAT_YOU_USE "${IWYU_CMD}")
        message(STATUS "include-what-you-use enabled for ${TARGET_NAME}")
    endif()

    get_target_property(TARGET_SOURCES ${TARGET_NAME} SOURCES)
    get_target_property(TARGET_SOURCE_DIR ${TARGET_NAME} SOURCE_DIR)
    foreach(SRC IN LISTS TARGET_SOURCES)
        get_filename_component(SRC_ABS "${SRC}" ABSOLUTE BASE_DIR "${TARGET_SOURCE_DIR}")
        if(SRC_ABS MATCHES "${VENDORED_REGEX}")
            set_source_files_properties("${SRC}" PROPERTIES SKIP_LINTING ON)
        endif()
    endforeach()
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

    add_custom_target(formatf
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
