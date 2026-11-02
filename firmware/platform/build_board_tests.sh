#!/usr/bin/env sh
# Build every test of one board for every variant in BOARD_VARIANTS, skipping BOARD_SKIP_TESTS
# (both from its Board.cmake), each variant into <build_root>/<variant>/
# A board without variants builds its tests once, in <build_root>
#   usage: build_board_tests.sh <board_dir> <build_root> [variant]
# [variant] limits it to that one; `make test-all` calls this (as `make tests`)
# Exits 1 if any test failed to build
set -eu

. "$(dirname "$0")/board_cmake_list.sh"

usage="usage: build_board_tests.sh <board_dir> <build_root> [variant]"
cd "${1:?$usage}"
build_root=${2:?$usage}
only=${3:-}

if [ ! -f Board.cmake ]; then
    echo "error: no Board.cmake in $(pwd)" >&2
    exit 1
fi

SKIP=$(board_cmake_list BOARD_SKIP_TESTS)
VARIANTS=$(board_variants_to_build "$only")

board=$(basename "$(pwd)")
built=""
failed=""
skipped=""

# every test for one variant, or with no variant when it's empty
build_tests() {
    variant=$1
    for src in tests/Src/*_test.c; do
        [ -e "$src" ] || continue
        name=$(basename "$src" _test.c)
        label="${variant:+$variant/}$name"

        case " $SKIP " in
            *" $name "*) skipped="$skipped $label"; continue ;;
        esac

        echo "==> building $label"
        if make BUILD_DIR="$build_root${variant:+/$variant}" VARIANT="$variant" TEST="$name" all; then
            built="$built $label"
        else
            failed="$failed $label"
            echo "!!! FAILED: $label"
        fi
    done
}

set -- $VARIANTS
if [ $# -eq 0 ]; then
    build_tests ""
else
    for variant_name in "$@"; do
        build_tests "$variant_name"
    done
fi

count() { set -- $1; echo $#; }

echo
echo "================ $board test summary ================"
echo " built   ($(count "$built")):$built"
echo " failed  ($(count "$failed")):$failed"
echo " skipped ($(count "$skipped")):$skipped"
echo "======================================================="

[ -z "$failed" ] || exit 1
