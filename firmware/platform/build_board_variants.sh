#!/usr/bin/env sh
# Build the production firmware of one board for every variant in BOARD_VARIANTS (its Board.cmake),
# each into <build_dir>/<variant>/; a board without variants gets one build in <build_dir>
#   usage: build_board_variants.sh <board_dir> <build_dir>
# A board's `make prod-all` calls this (as `make variants`); exits 1 if any variant failed to build
set -eu

. "$(dirname "$0")/board_cmake_list.sh"

cd "${1:?usage: build_board_variants.sh <board_dir> <build_dir>}"
build_dir=${2:?usage: build_board_variants.sh <board_dir> <build_dir>}

if [ ! -f Board.cmake ]; then
    echo "error: no Board.cmake in $(pwd)" >&2
    exit 1
fi

VARIANTS=$(board_cmake_list BOARD_VARIANTS)

# no variants: a single, plain production build
set -- $VARIANTS
if [ $# -eq 0 ]; then
    exec make BUILD_DIR="$build_dir" all
fi

board=$(basename "$(pwd)")
built=""
failed=""

for variant in $VARIANTS; do
    echo "==> building variant $variant"
    if make BUILD_DIR="$build_dir/$variant" VARIANT="$variant" all; then
        built="$built $variant"
    else
        failed="$failed $variant"
        echo "!!! FAILED: $variant"
    fi
done

count() { set -- $1; echo $#; }

echo
echo "================ $board variant summary ================"
echo " built   ($(count "$built")):$built"
echo " failed  ($(count "$failed")):$failed"
echo "========================================================="

[ -z "$failed" ] || exit 1
