#!/usr/bin/env sh
# Build every platform test
set -eu

cd "$(dirname "$0")"

DISABLED=$(awk '/set\(DISABLED_TESTS/{f=1} f{print} f&&/\)/{exit}' CMakeLists.txt \
    | sed -e 's/.*set(DISABLED_TESTS//' -e 's/).*//' -e 's/#.*//' \
    | tr -d '"' | tr '\n' ' ')

built=""
failed=""
skipped=""

for src in tests/Src/*_test.c; do
    [ -e "$src" ] || continue
    name=$(basename "$src" _test.c)

    case " $DISABLED " in
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
echo "================ platform test summary ================"
echo " built   ($(count "$built")):$built"
echo " failed  ($(count "$failed")):$failed"
echo " skipped ($(count "$skipped")):$skipped"
echo "======================================================="

[ -z "$failed" ] || exit 1
