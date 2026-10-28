"""Verify committed CAN codecs match fresh generation from all current DBCs."""

import argparse
import difflib
from pathlib import Path
import sys
import tempfile

from dbc import load_buses
from emit import generate

ROOT = Path(__file__).resolve().parents[1]


def files(root):
    """Directory -> relative regular-file bytes, including the generator manifest."""
    return {p.relative_to(root): p.read_bytes() for p in root.rglob("*") if p.is_file()}


def validate(dbc_dir, committed):
    """Regenerate in isolation and report missing, stale and unexpected files."""
    paths = sorted(dbc_dir.glob("*.dbc"))
    if not paths:
        raise ValueError(f"No DBC files found in {dbc_dir}")
    with tempfile.TemporaryDirectory(prefix="can-codegen-check-") as directory:
        generate(load_buses(paths), Path(directory))
        expected, actual = files(Path(directory)), files(committed)
    differences = False
    for path in sorted(expected.keys() | actual.keys()):
        if path not in actual:
            print(f"Missing generated file: {path}")
        elif path not in expected:
            print(f"Unexpected generated file: {path}")
        elif expected[path] != actual[path]:
            print(f"Stale generated file: {path}")
            before = actual[path].decode("utf-8", errors="replace").splitlines()
            after = expected[path].decode("utf-8", errors="replace").splitlines()
            for line in list(difflib.unified_diff(before, after, fromfile="committed", tofile="regenerated", lineterm=""))[:40]:
                print(line)
        else:
            continue
        differences = True
    if differences:
        print("Regenerate with: uv run --locked python can/codegen/generate_can_headers.py -o can/generated")
    return not differences


def board_copies(repo, generated):
    """Reject generated CAN codec copies under board directories."""
    names = {p.name for p in generated.rglob("*") if p.suffix in (".c", ".h") and p.name != "can.h"}
    duplicates = []
    for marker in (repo / "firmware").glob("*/*/Board.cmake"):
        for path in marker.parent.rglob("*"):
            if not path.is_file() or "build" in path.relative_to(marker.parent).parts:
                continue
            is_umbrella_copy = (path.name == "can.h" and (generated / "can.h").is_file()
                                and path.read_bytes() == (generated / "can.h").read_bytes())
            if path.name in names or is_umbrella_copy:
                duplicates.append(path.relative_to(repo))
    return duplicates


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dbc-dir", type=Path, default=ROOT / "dbc")
    parser.add_argument("--generated-dir", type=Path, default=ROOT / "generated")
    args = parser.parse_args()
    try:
        fresh = validate(args.dbc_dir, args.generated_dir)
        duplicates = board_copies(ROOT.parent, args.generated_dir)
        for path in duplicates:
            print(f"Board-local generated CAN file: {path}; use can/generated instead")
        return 0 if fresh and not duplicates else 1
    except (ValueError, OSError) as error:
        print(f"CAN generation check failed: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
