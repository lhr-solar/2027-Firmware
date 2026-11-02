#!/usr/bin/env sh
# Build the production firmware of one board for every variant in BOARD_VARIANTS (its Board.cmake),
# each into <build_root>/<variant>/; a board without variants gets one build in <build_root>
#   usage: build_board_variants.sh <board_dir> <build_root> [variant]
# [variant] limits it to that one; `make prod-all` calls this (as `make variants`)
# Exits 1 if any variant failed to build
set -eu

. "$(dirname "$0")/board_cmake_list.sh"

usage="usage: build_board_variants.sh <board_dir> <build_root> [variant]"
cd "${1:?$usage}"
build_root=${2:?$usage}
only=${3:-}

if [ ! -f Board.cmake ]; then
    echo "error: no Board.cmake in $(pwd)" >&2
    exit 1
fi

VARIANTS=$(board_variants_to_build "$only")

# no variants: a single, plain production build
set -- $VARIANTS
if [ $# -eq 0 ]; then
    exec make BUILD_DIR="$build_root" all
fi

board=$(basename "$(pwd)")
built=""
failed=""

for variant in "$@"; do
    echo "==> building variant $variant"
    if make BUILD_DIR="$build_root/$variant" VARIANT="$variant" all; then
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
