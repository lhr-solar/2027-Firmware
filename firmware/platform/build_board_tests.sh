#!/usr/bin/env sh
# Build every test of one board, skipping BOARD_SKIP_TESTS from its Board.cmake
#   usage: build_board_tests.sh <board_dir>
# A board's `make test-all` calls this; exits 1 if any test failed to build
set -eu

cd "${1:?usage: build_board_tests.sh <board_dir>}"

if [ ! -f Board.cmake ]; then
    echo "error: no Board.cmake in $(pwd)" >&2
    exit 1
fi

# set(BOARD_SKIP_TESTS ...) block from Board.cmake, flattened to one space-separated line
# a ")" inside a comment within the list ends it early
SKIP=$(awk '/^ *set\(BOARD_SKIP_TESTS/{f=1} f{print} f&&/\)/{exit}' Board.cmake \
    | sed -e 's/.*set(BOARD_SKIP_TESTS//' -e 's/).*//' -e 's/#.*//' \
    | tr -d '"' | tr '\n' ' ')

board=$(basename "$(pwd)")
built=""
failed=""
skipped=""

for src in tests/Src/*_test.c; do
    [ -e "$src" ] || continue
    name=$(basename "$src" _test.c)

    case " $SKIP " in
        *" $name "*) skipped="$skipped $name"; continue ;;
    esac

    echo "==> building $name"
    if make TEST="$name"; then
        built="$built $name"
    else
        failed="$failed $name"
        echo "!!! FAILED: $name"
    fi
done

count() { set -- $1; echo $#; }

echo
echo "================ $board test summary ================"
echo " built   ($(count "$built")):$built"
echo " failed  ($(count "$failed")):$failed"
echo " skipped ($(count "$skipped")):$skipped"
echo "======================================================="

[ -z "$failed" ] || exit 1
