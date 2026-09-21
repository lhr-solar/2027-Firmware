# fully clauded just ask claude to edit this lowk if it needs changes

#!/usr/bin/env python3
"""Scaffold a new board folder, laid out like firmware/psys/LVC.

    python3 firmware/new_board.py                  # asks for everything
    python3 firmware/new_board.py psys LVC         # no questions
    python3 firmware/new_board.py psys LVC --description "LV Carrier"

Creates firmware/<system>/<board>/ with config/, core/, drivers/, tests/, a
Board.cmake, CMakeLists.txt and Makefile (rendered from firmware/templates/).
The source files are created empty, except core/Src/app.c, which gets a main()
that only loops.
A "system" is any folder directly under firmware/ that is not in NOT_SYSTEMS.
"""
import argparse
import os
import re
import sys
from functools import partial
from pathlib import Path

FIRMWARE = Path(__file__).resolve().parent
TEMPLATES = FIRMWARE / "templates"

# firmware/ folders that are not systems: shared code, the bootloader, cmake output, board templates
NOT_SYSTEMS = {"platform", "bootloader", "build", "templates"}

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
# CMakeLists.txt, Board.cmake and Makefile are rendered from firmware/templates/.
# Placeholders are @KEY@ (not $KEY: the CMake text is full of ${...}):
#   @BOARD@     board name, used for the folder and the CMake target
#   @DESC@      human-readable description
#   @SYSTEM@    system folder, e.g. psys
#   @PLATFORM@  relative path from the board folder to firmware/platform
FROM_TEMPLATE = {  # generated file -> template it is rendered from
    "CMakeLists.txt": "CMakeLists.txt.in",
    "Board.cmake": "Board.cmake.in",
    "Makefile": "Makefile.in",
}

# everything else is written as-is
FILES = {}

# Source files are left empty, except app.c: it gets a bare main so a new board
# links out of the box (Board.cmake points at it, and CMakeLists.txt errors at
# configure time without it).
for rel in ("config/Inc/pinDefs.h", "core/Inc/app.h"):
    FILES[rel] = ""

FILES["core/Src/app.c"] = """\
int main(void) {
    while (1) {
    }
}
"""

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


def load_templates():
    files = {}
    for rel, name in FROM_TEMPLATE.items():
        try:
            with open(TEMPLATES / name, encoding="utf-8") as f:  # text mode: CRLF checkouts read as LF
                files[rel] = f.read()
        except FileNotFoundError:
            sys.exit("missing template: %s" % (TEMPLATES / name).relative_to(FIRMWARE.parent).as_posix())
    if re.search(r"^ +\S", files["Makefile"], re.M):
        sys.exit("firmware/templates/%s has space-indented lines -- make recipes need tabs" % FROM_TEMPLATE["Makefile"])
    files.update(FILES)
    return files


def render(name, text, values):
    unknown = set(re.findall(r"@([A-Z_]+)@", text)) - set(values)
    if unknown:
        sys.exit("%s uses unknown placeholder(s): %s" % (name, ", ".join(sorted(unknown))))
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
    files = load_templates()  # before any question: a broken template should fail first

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
    }
    # render everything before writing anything: a bad template must not leave a half-made board
    rendered = {rel: render(rel, text, values) for rel, text in files.items()}
    for rel, text in rendered.items():
        path = dest / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        with open(path, "w", newline="\n") as f:  # LF on every OS
            f.write(text)
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
