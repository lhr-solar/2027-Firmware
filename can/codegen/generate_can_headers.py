#!/usr/bin/env python3
"""CLI: turn canonical bus DBCs into standalone C codecs in a build directory."""

import argparse
from pathlib import Path
import sys

from dbc import load_buses
from emit import generate


def main() -> int:
    """Read DBC paths and an output directory; emit C, or report an invalid contract."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("dbcs", nargs="*", type=Path, help="One DBC per physical bus")
    parser.add_argument("-o", "--output", required=True, type=Path)
    args = parser.parse_args()
    paths = args.dbcs or sorted(
        (Path(__file__).resolve().parents[1] / "dbc").glob("*.dbc")
    )
    try:
        generate(load_buses(paths), args.output)
    except (ValueError, OSError) as error:
        print(f"codegen: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
