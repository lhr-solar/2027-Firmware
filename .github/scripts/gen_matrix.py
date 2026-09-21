#!/usr/bin/env python3
"""
Generate GitHub Actions build matrices from discovered firmware targets.

Two kinds of target, discovered differently:

  boards    firmware/<subteam>/<BOARD>/, marked by a Board.cmake. Each board
            gets exactly two jobs, both owner-defined entry points:
              prod   -> make prod-all
              tests  -> make test-all
            A rule mentioned only in .PHONY does not count as defined.
            The board owner controls what those do, so boards with variants
            (board number etc.) need no change here.
  platform  firmware/platform/ -- the shared HAL/RTOS layer. One job per
            tests/Src/*_test.c, honouring DISABLED_TESTS in its CMakeLists.txt
            (building a disabled test is a configure-time FATAL_ERROR).

Output is a JSON array suitable for strategy.matrix.include. Every entry has
the same keys: name, id, dir, cmd, blocked, os.
"""

import argparse
import json
import re
import sys
from pathlib import Path

TEST_SUFFIX = "_test.c"
# owner-defined make targets CI drives each board through
PROD_TARGET = "prod-all"
TEST_TARGET = "test-all"
ENTRY_POINTS = ((PROD_TARGET, "prod", "production firmware"),
                (TEST_TARGET, "tests", "all tests"))


def fail(msg: str) -> None:
    print(f"error: {msg}", file=sys.stderr)
    sys.exit(1)


def read(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except OSError as exc:
        fail(f"cannot read {path}: {exc}")


def cmake_set_values(text: str, var: str) -> list[str]:
    """Tokens of a `set(<var> ...)` block, comments stripped and quotes removed.

    Anchored on `set(` so it does not also match the `list(FIND DISABLED_TESTS
    ...)` lookup a few lines below the declaration.
    """
    match = re.search(rf"\bset\s*\(\s*{re.escape(var)}\s(?P<body>[^)]*)\)", text)
    if not match:
        return []
    body = re.sub(r"#[^\n]*", "", match.group("body"))
    return [tok.strip('"') for tok in body.split()]


def entry(name: str, ident: str, directory: str, cmd: str,
          blocked: str = "") -> dict:
    return {
        "name": name,
        "id": ident,
        "dir": directory,
        # exactly what CI runs, from the repo root
        "cmd": cmd,
        # non-empty means this target cannot be built, and why; the key is
        # always present so the workflow's `!= ''` guard is unambiguous
        "blocked": blocked,
    }


def boards_matrix(root: Path) -> list[dict]:
    firmware = root / "firmware"
    if not firmware.is_dir():
        fail(f"no firmware/ directory at {firmware}")

    board_dirs = sorted(p.parent for p in firmware.glob("*/*/Board.cmake")
                        if "build" not in p.parts)
    if not board_dirs:
        fail(f"no boards found under {firmware} (looked for */*/Board.cmake)")

    matrix: list[dict] = []
    for board_dir in board_dirs:
        board, subteam = board_dir.name, board_dir.parent.name
        rel = board_dir.relative_to(root).as_posix()

        makefile = board_dir / "Makefile"
        text = read(makefile) if makefile.is_file() else None

        for target, label, desc in ENTRY_POINTS:
            if text is None:
                blocked = (f"{board} has no Makefile -- CI builds {desc} with "
                           f"'make {target}' (see firmware/psys/LVC/Makefile)")
            elif not re.search(rf"^{re.escape(target)}\s*:(?!=)", text, re.MULTILINE):
                blocked = (f"{board}/Makefile defines no '{target}' target -- CI "
                           f"builds {desc} with it (see firmware/psys/LVC/Makefile)")
            else:
                blocked = ""
            matrix.append(entry(f"{board} / {label}",
                                f"{subteam}-{board}-{label}", rel,
                                f"make -C {rel} {target}", blocked))
    return matrix


def platform_matrix(root: Path) -> list[dict]:
    platform = root / "firmware" / "platform"
    cmakelists = platform / "CMakeLists.txt"
    if not cmakelists.is_file():
        fail(f"no platform CMakeLists.txt at {cmakelists}")

    disabled = set(cmake_set_values(read(cmakelists), "DISABLED_TESTS"))
    test_dir = platform / "tests" / "Src"
    if not test_dir.is_dir():
        fail(f"no platform test directory at {test_dir}")
    tests = sorted({p.name[: -len(TEST_SUFFIX)]
                    for p in test_dir.glob(f"*{TEST_SUFFIX}")} - disabled)
    if not tests:
        fail(f"no buildable tests in {test_dir}")
    if disabled:
        print(f"note: skipping DISABLED_TESTS: {', '.join(sorted(disabled))}",
              file=sys.stderr)

    rel = platform.relative_to(root).as_posix()
    return [entry(f"platform / {t}", f"platform-{t}", rel,
                  f"make -C {rel} TEST={t}") for t in tests]


def os_label(runner: str) -> str:
    """ubuntu-latest -> ubuntu; macos-14 -> macos-14."""
    return runner.removesuffix("-latest")


def with_runners(matrix: list[dict], runners: list[str]) -> list[dict]:
    """Cross every target with every runner.

    This has to happen here rather than as a second `os:` key in the workflow:
    GitHub merges `include` objects into the base matrix instead of taking a
    product with it, so `{os: [a, b], include: <targets>}` silently collapses
    to two jobs rather than 2 x len(targets).

    Names and ids only gain an OS suffix when there is more than one runner,
    so single-runner output is unchanged.
    """
    multi = len(runners) > 1
    out: list[dict] = []
    for item in matrix:
        for runner in runners:
            new = dict(item, os=runner)
            if multi:
                label = os_label(runner)
                new["name"] = f"{item['name']} ({label})"
                new["id"] = f"{item['id']}-{label}"
            out.append(new)
    return out


def as_markdown(matrix: list[dict]) -> str:
    lines = [f"### {len(matrix)} target(s)", "",
             "| target | command | runner | status |",
             "| --- | --- | --- | --- |"]
    lines += [f"| {e['name']} | `{e['cmd']}` | `{e['os']}` | "
              f"{'**BLOCKED** -- ' + e['blocked'] if e['blocked'] else 'ok'} |"
              for e in matrix]
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--kind", required=True, choices=("boards", "platform"),
                        help="which set of targets to enumerate")
    parser.add_argument("--format", default="json",
                        choices=("json", "pretty", "markdown"),
                        help="json (default, single line for GITHUB_OUTPUT), "
                             "pretty (indented), markdown (step summary table)")
    parser.add_argument("--os", default="ubuntu-latest",
                        help="comma-separated runner labels; every target is "
                             "built on every runner (default: ubuntu-latest)")
    parser.add_argument("--repo-root", type=Path, default=None,
                        help="defaults to the repo this script lives in")
    args = parser.parse_args()

    # Script lives in .github/scripts/, so the repo root is two levels up.
    root = (args.repo_root
            or Path(__file__).resolve().parent.parent.parent).resolve()

    runners = [o.strip() for o in args.os.split(",") if o.strip()]
    if not runners:
        fail("--os must name at least one runner label")

    matrix = boards_matrix(root) if args.kind == "boards" else platform_matrix(root)
    matrix = with_runners(matrix, runners)

    if args.format == "markdown":
        print(as_markdown(matrix))
    elif args.format == "pretty":
        print(json.dumps(matrix, indent=2))
    else:
        print(json.dumps(matrix, separators=(",", ":")))


if __name__ == "__main__":
    main()
