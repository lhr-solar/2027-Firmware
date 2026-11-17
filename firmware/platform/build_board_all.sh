#!/usr/bin/env sh
# Build one board for every variant in BOARD_VARIANTS (its Board.cmake), each into
# <build_root>/<variant>/; a board without variants is built once, in <build_root>
#   usage: build_board_all.sh <prod|tests> <board_dir> <build_root> [variant]
#   prod:  the production firmware      -- what `make prod-all` runs
#   tests: every tests/Src/*_test.c     -- what `make test-all` runs
# [variant] limits it to that one (CMake rejects a name it doesn't know)
# Exits 1 if anything failed to build
set -eu

usage="usage: build_board_all.sh <prod|tests> <board_dir> <build_root> [variant]"
mode=${1:?$usage}
board_dir=${2:?$usage}
build_root=${3:?$usage}
only=${4:-}

case "$mode" in
    prod | tests) ;;
    *) echo "$usage" >&2; exit 2 ;;
esac

query="$(cd "$(dirname "$0")" && pwd)/cmake/board_query.cmake"
cd "$board_dir"

if [ ! -f Board.cmake ]; then
    echo "error: no Board.cmake in $(pwd)" >&2
    exit 1
fi

if [ -n "$only" ]; then
    variants=$only
else
    variants=$(cmake -DBOARD_DIR="$(pwd)" -DQUERY=BOARD_VARIANTS -P "$query")
fi

board=$(basename "$(pwd)")
built=""
failed=""

# build_one <label> <make args...>: one make invocation, recorded under <label>
build_one() {
    label=$1
    shift
    echo "==> building $label"
    if make "$@" all; then
        built="$built $label"
    else
        failed="$failed $label"
        echo "!!! FAILED: $label"
    fi
}

# build_variant <variant>: prod or every test for one variant, or with no variant when it's empty
build_variant() {
    variant=$1
    dir="$build_root${variant:+/$variant}"
    if [ "$mode" = prod ]; then
        build_one "${variant:-$board}" BUILD_DIR="$dir" VARIANT="$variant"
        return
    fi
    for src in tests/Src/*_test.c; do
        [ -e "$src" ] || continue
        name=$(basename "$src" _test.c)
        build_one "${variant:+$variant/}$name" BUILD_DIR="$dir" VARIANT="$variant" TEST="$name"
    done
}

set -- $variants
if [ $# -eq 0 ]; then
    build_variant ""
else
    for variant_name in "$@"; do
        build_variant "$variant_name"
    done
fi

count() { set -- $1; echo $#; }

echo
echo "================ $board $mode summary ================"
echo " built   ($(count "$built")):$built"
echo " failed  ($(count "$failed")):$failed"
echo "======================================================="

[ -z "$failed" ] || exit 1
