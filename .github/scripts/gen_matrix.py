#!/usr/bin/env python3
"""
Generate GitHub Actions build matrices from discovered firmware targets.

This repo has two kinds of buildable thing, discovered differently:

  boards    firmware/<subteam>/<BOARD>/, marked by a Board.cmake. Each board
            builds its production app (TEST="", entry point from
            BOARD_PROD_SOURCE) plus one binary per test in
            BOARD_TEST_SOURCE_DIR.
  platform  firmware/platform/ -- the shared HAL/RTOS layer. No production
            app; it builds one binary per tests/Src/*_test.c.

Output is a JSON array suitable for strategy.matrix.include.
"""

import argparse
import json
import re
import sys
from pathlib import Path

TEST_SUFFIX = "_test.c"


def fail(msg: str) -> None:
    print(f"error: {msg}", file=sys.stderr)
    sys.exit(1)


def read(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except OSError as exc:
        fail(f"cannot read {path}: {exc}")


def strip_comments(text: str) -> str:
    return re.sub(r"#[^\n]*", "", text)


def cmake_set_values(text: str, var: str) -> list[str]:
    """Return the tokens of a `set(<var> ...)` block, quotes stripped.

    Anchored on `set(` so it does not also match the `list(FIND DISABLED_TESTS
    ...)` lookup that lives a few lines below the real declaration.
    """
    match = re.search(rf"\bset\s*\(\s*{re.escape(var)}\s(?P<body>[^)]*)\)", text)
    if not match:
        return []
    return [tok.strip('"') for tok in strip_comments(match.group("body")).split()]


def cmake_project_name(cmakelists: Path) -> str:
    """The CMake target name, which is what the build outputs are named."""
    match = re.search(r"\bproject\s*\(\s*([A-Za-z0-9_]+)", read(cmakelists))
    if not match:
        fail(f"no project() name found in {cmakelists}")
    return match.group(1)


def find_tests(test_dir: Path, disabled: set[str]) -> list[str]:
    if not test_dir.is_dir():
        return []
    found = {p.name[: -len(TEST_SUFFIX)] for p in test_dir.glob(f"*{TEST_SUFFIX}")}
    return sorted(found - disabled)


def entry(name: str, ident: str, directory: Path, root: Path, target: str,
          test: str, kind: str, subteam: str) -> dict:
    return {
        "name": name,
        "id": ident,
        "dir": directory.relative_to(root).as_posix(),
        "target": target,
        "test": test,
        "kind": kind,
        "subteam": subteam,
    }


def boards_matrix(root: Path) -> list[dict]:
    firmware = root / "firmware"
    if not firmware.is_dir():
        fail(f"no firmware/ directory at {firmware}")

    # <subteam>/<BOARD>/Board.cmake. Stale build trees never contain one, but
    # filter defensively so a local build dir can't inject a phantom board.
    board_dirs = sorted(
        p.parent for p in firmware.glob("*/*/Board.cmake")
        if "build" not in p.parts
    )
    if not board_dirs:
        fail(f"no boards found under {firmware} (looked for */*/Board.cmake)")

    matrix: list[dict] = []
    for board_dir in board_dirs:
        board = board_dir.name
        subteam = board_dir.parent.name
        cmakelists = board_dir / "CMakeLists.txt"
        if not cmakelists.is_file():
            fail(f"{board} has Board.cmake but no CMakeLists.txt")

        target = cmake_project_name(cmakelists)
        disabled = set(cmake_set_values(read(cmakelists), "DISABLED_TESTS"))
        board_cmake = read(board_dir / "Board.cmake")

        # Production app. The board's CMakeLists FATAL_ERRORs on a missing
        # entry point; catching it here fails one fast job instead of every
        # job in the matrix.
        prod = cmake_set_values(board_cmake, "BOARD_PROD_SOURCE")
        if not prod:
            fail(f"{board}/Board.cmake does not set BOARD_PROD_SOURCE")
        if not (board_dir / prod[0]).is_file():
            fail(f"{board}: BOARD_PROD_SOURCE points at missing {prod[0]}")
        matrix.append(entry(f"{board} / prod", f"{subteam}-{board}-prod",
                            board_dir, root, target, "", "board", subteam))

        # Board tests, from the directory the board itself declares.
        test_dirs = cmake_set_values(board_cmake, "BOARD_TEST_SOURCE_DIR")
        if not test_dirs:
            print(f"warning: {board} sets no BOARD_TEST_SOURCE_DIR", file=sys.stderr)
            continue
        tests = find_tests(board_dir / test_dirs[0], disabled)
        if not tests:
            print(f"warning: no tests found in {board}/{test_dirs[0]}", file=sys.stderr)
        for test in tests:
            matrix.append(entry(f"{board} / {test}", f"{subteam}-{board}-{test}",
                                board_dir, root, target, test, "board", subteam))
    return matrix


def platform_matrix(root: Path) -> list[dict]:
    platform = root / "firmware" / "platform"
    cmakelists = platform / "CMakeLists.txt"
    if not cmakelists.is_file():
        fail(f"no platform CMakeLists.txt at {cmakelists}")

    target = cmake_project_name(cmakelists)
    disabled = set(cmake_set_values(read(cmakelists), "DISABLED_TESTS"))
    tests = find_tests(platform / "tests" / "Src", disabled)
    if not tests:
        fail(f"no buildable tests in {platform / 'tests' / 'Src'}")
    if disabled:
        print(f"note: skipping DISABLED_TESTS: {', '.join(sorted(disabled))}",
              file=sys.stderr)

    return [entry(f"platform / {test}", f"platform-{test}", platform, root,
                  target, test, "platform", "platform") for test in tests]


def as_markdown(matrix: list[dict]) -> str:
    lines = [f"### {len(matrix)} target(s)", "",
             "| target | directory | TEST |", "| --- | --- | --- |"]
    lines += [f"| {e['name']} | `{e['dir']}` | `{e['test'] or '(prod)'}` |"
              for e in matrix]
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--kind", required=True, choices=("boards", "platform"),
                        help="which set of targets to enumerate")
    parser.add_argument("--format", default="json",
                        choices=("json", "pretty", "markdown"),
                        help="json (default, single line for GITHUB_OUTPUT), "
                             "pretty (indented), markdown (step summary table)")
    parser.add_argument("--repo-root", type=Path, default=None,
                        help="defaults to the repo this script lives in")
    args = parser.parse_args()

    # Script lives in .github/scripts/, so the repo root is two levels up.
    root = (args.repo_root or Path(__file__).resolve().parent.parent.parent).resolve()

    matrix = boards_matrix(root) if args.kind == "boards" else platform_matrix(root)

    if args.format == "markdown":
        print(as_markdown(matrix))
    elif args.format == "pretty":
        print(json.dumps(matrix, indent=2))
    else:
        print(json.dumps(matrix, separators=(",", ":")))


if __name__ == "__main__":
    main()
