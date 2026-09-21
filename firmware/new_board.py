# fully clauded just ask claude to edit this lowk if it needs changes

#!/usr/bin/env python3
"""Scaffold a new board folder, laid out like firmware/psys/LVC.

    python3 firmware/new_board.py                  # asks for everything
    python3 firmware/new_board.py psys LVC         # no questions
    python3 firmware/new_board.py psys LVC --description "LV Carrier"

Creates firmware/<system>/<board>/ with config/, core/, drivers/, tests/, a
Board.cmake, CMakeLists.txt and Makefile. The source files are created empty.
A "system" is any folder directly under firmware/ that is not in NOT_SYSTEMS.
"""
import argparse
import os
import re
import sys
from functools import partial
from pathlib import Path

FIRMWARE = Path(__file__).resolve().parent

# firmware/ folders that are not systems: shared code, the bootloader, cmake output
NOT_SYSTEMS = {"platform", "bootloader", "build"}

# ---------------------------------------------------------------- colors
# ANSI, off when piped or when NO_COLOR is set; symbols fall back to ASCII
# on terminals that are not UTF-8 (printing them would crash there).
COLOR = sys.stdout.isatty() and "NO_COLOR" not in os.environ
UNICODE = (sys.stdout.encoding or "").lower().replace("-", "") == "utf8"
CHECK, ARROW, BROOM = ("✔", "›", "🧹 ") if UNICODE else ("+", ">", "")


def paint(code, text):
    return "\033[%sm%s\033[0m" % (code, text) if COLOR else text


bold = partial(paint, "1")
dim = partial(paint, "2")
red = partial(paint, "1;31")
green = partial(paint, "1;32")
yellow = partial(paint, "1;33")
cyan = partial(paint, "1;36")
magenta = partial(paint, "1;35")

# ------------------------------------------------------------- templates
# @KEY@ placeholders (not $KEY): the CMake text is full of ${...}.
FILES = {}

FILES["CMakeLists.txt"] = """\
### @DESC@ board build
## Config hardcoded for G473 series MCUS
cmake_minimum_required(VERSION 3.25.3)

project(
    @BOARD@
    VERSION 1.0.0
    DESCRIPTION "@DESC@"
    LANGUAGES C ASM # asm for startup file
)

include(${CMAKE_CURRENT_LIST_DIR}/@PLATFORM@/cmake/platform.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/@PLATFORM@/cmake/stm32.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/@PLATFORM@/cmake/post_build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/@PLATFORM@/cmake/custom_commands.cmake)
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
add_executable(@BOARD@
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
target_include_directories(@BOARD@ PUBLIC ${PLATFORM_INCLUDE_DIRS} ${BOARD_INCLUDE_DIRS})

build_firmware(@BOARD@)
post_build(@BOARD@)

# external commands
add_flash_target(@BOARD@)
add_dump_symbols_target(@BOARD@)
add_dump_size_target(@BOARD@)
add_erase_target()

# must have jolly good message
string(ASCII 27 Esc)
set(ColorReset "${Esc}[0m")
set(ColorBoldGreen "${Esc}[1;32m")

message(STATUS "${ColorBoldGreen}=== Jolly good! ===${ColorReset}")
"""

FILES["Board.cmake"] = """\
### @DESC@ Config
# paths below are relative to this board's own CMakeLists.txt (firmware/@SYSTEM@/@BOARD@/)
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
"""

# make recipes need a real tab, so @TAB@ is swapped in (editors love to eat them)
FILES["Makefile"] = """\
# @BOARD@ board build
# thin wrapper -- every target just calls cmake

BUILD_DIR := build
TOOLCHAIN := @PLATFORM@/cmake/toolchain.cmake

TEST ?=

.PHONY: all configure flash erase dump-symbols dump_size clean

all: configure
@TAB@cmake --build $(BUILD_DIR) --target @BOARD@

configure:
@TAB@cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DTEST=$(TEST)

flash: configure
@TAB@cmake --build $(BUILD_DIR) --target flash

erase: configure
@TAB@cmake --build $(BUILD_DIR) --target erase

dump-symbols: configure
@TAB@cmake --build $(BUILD_DIR) --target dump-symbols

dump_size: configure
@TAB@cmake --build $(BUILD_DIR) --target dump_size

clean:
@TAB@cmake --build $(BUILD_DIR) --target clean
"""

# Source files are left empty. core/Src/app.c must exist: Board.cmake points at
# it and CMakeLists.txt errors at configure time without it.
for rel in ("config/Inc/pinDefs.h", "core/Inc/app.h", "core/Src/app.c"):
    FILES[rel] = ""

# folders with nothing to put in them yet (git does not track empty folders)
for rel in ("drivers/Inc", "drivers/Src", "tests/Inc", "tests/Src"):
    FILES[rel + "/.gitkeep"] = ""


# ------------------------------------------------------------- questions
def systems():
    return sorted(
        d.name for d in FIRMWARE.iterdir()
        if d.is_dir() and d.name not in NOT_SYSTEMS and not d.name.startswith((".", "_"))
    )


def find_system(value, options, allow_number=False):
    value = value.strip()
    if allow_number and value.isdigit() and 1 <= int(value) <= len(options):
        return options[int(value) - 1]
    return next((o for o in options if o.lower() == value.lower()), None)


def board_error(name, system):
    if not re.fullmatch(r"[A-Za-z][A-Za-z0-9_]*", name):
        return "use letters, digits and underscores, starting with a letter"
    for d in (FIRMWARE / system).iterdir():
        if d.name.lower() == name.lower():  # macOS checkouts would merge LVC and lvc
            return "%s/%s already exists" % (system, d.name)


def description_error(desc):
    if re.search(r'["\\$\r\n]', desc):
        return 'can not contain ", \\, $ or newlines (it is written into CMake)'


def ask(label, hint="", default="", check=None):
    while True:
        prompt = bold(label)
        if hint:
            prompt += " " + dim(hint)
        if default:
            prompt += " " + dim("[%s]" % default)
        print(prompt + " " + cyan(ARROW) + " ", end="", flush=True)
        value = input().strip() or default
        error = check(value) if check else None
        if not error:
            return value
        print("  " + red(error))


def choose_system(options):
    print(bold("Which system is this board for?"))
    for i, name in enumerate(options, 1):
        print("  %s %s" % (yellow("%d)" % i), name))
    unknown = "pick one of: " + ", ".join(options)
    answer = ask("System", hint="(number or name)",
                 check=lambda v: None if find_system(v, options, True) else unknown)
    return find_system(answer, options, True)


def render(text, values):
    unknown = set(re.findall(r"@([A-Z_]+)@", text)) - set(values)
    if unknown:
        sys.exit("internal error: template uses unknown placeholder(s): %s" % ", ".join(sorted(unknown)))
    for key, val in values.items():
        text = text.replace("@%s@" % key, val)
    return text


def banner():
    title = "LHR firmware  %s  new board" % ARROW
    bar = ("─" if UNICODE else "-") * (len(title) + 4)
    corners = "╭╮╰╯│" if UNICODE else "++++|"
    print(cyan("  %s%s%s\n  %s  %s  %s\n  %s%s%s\n" % (
        corners[0], bar, corners[1], corners[4], title, corners[4], corners[2], bar, corners[3])))


def main():
    options = systems()
    if not options:
        sys.exit("no system folders found under firmware/")

    parser = argparse.ArgumentParser(
        description="Scaffold firmware/<system>/<board>/ like firmware/psys/LVC.",
        epilog="Run with no arguments to be asked for everything.")
    parser.add_argument("system", nargs="?", help="one of: " + ", ".join(options))
    parser.add_argument("board", nargs="?", help="board name, used for the folder and CMake target (e.g. LVC)")
    parser.add_argument("--description", help='human-readable name, e.g. "LV Carrier" (default: board name)')
    args = parser.parse_args()

    prompting = not (args.system and args.board)
    if prompting:
        banner()

    if args.system:  # names only here: numbers would shift when a system folder is added
        system = find_system(args.system, options)
        if not system:
            parser.error("unknown system %r (choose from: %s)" % (args.system, ", ".join(options)))
    else:
        system = choose_system(options)

    if args.board:
        board = args.board
        error = board_error(board, system)
        if error:
            parser.error(error)
    else:
        board = ask("Board name", hint="(e.g. LVC)", check=lambda v: board_error(v, system))

    if args.description:
        desc = args.description
        error = description_error(desc)
        if error:
            parser.error("description " + error)
    elif prompting:
        desc = ask("Description", hint="(e.g. LV Carrier)", default=board, check=description_error)
    else:
        desc = board

    dest = FIRMWARE / system / board
    repo_rel = dest.relative_to(FIRMWARE.parent).as_posix()

    if prompting:
        print("\n  %s  %s" % (bold(repo_rel + "/"), dim('target %s, "%s"' % (board, desc))))
        print(bold("Create it?") + " " + dim("[Y/n]") + " " + cyan(ARROW) + " ", end="", flush=True)
        if input().strip().lower() not in ("", "y", "yes"):
            sys.exit(yellow("Cancelled, nothing was created."))
        print()

    values = {
        "BOARD": board,
        "DESC": desc,
        "SYSTEM": system,
        "PLATFORM": os.path.relpath(FIRMWARE / "platform", dest).replace(os.sep, "/"),
        "TAB": "\t",
    }
    for rel, template in FILES.items():
        path = dest / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        with open(path, "w", newline="\n") as f:  # LF on every OS
            f.write(render(template, values))
        folder, name = os.path.split(path.relative_to(FIRMWARE.parent).as_posix())
        print("  %s %s%s" % (green(CHECK), dim(folder + "/"), name))

    print()
    print(yellow("Note:") + " the .gitkeep files only keep empty folders in git -- delete each one once its folder has real files in it.")
    print()
    print(magenta("%sMaid Ravi has cleaned up a space for you in this repo, navigate to %s" % (BROOM, repo_rel)))


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit("\n" + yellow("Cancelled."))
    except EOFError:
        sys.exit("\n" + yellow("No input available -- pass the system and board name as arguments."))
