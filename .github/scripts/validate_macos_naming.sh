#!/usr/bin/env sh
# Fail if two C/H files that get built together differ only by case.
set -eu

cd "${1:-$(dirname "$0")/../..}"

status=0

check() {
    label=$1
    shift
    printf '==> %s\n' "$label"
    if find "$@" \( -name '*.c' -o -name '*.h' \) -not -path '*/build/*' \
        | awk '
            { n = split($0, a, "/"); base = a[n]; low = tolower(base) }
            !(low in seen) { seen[low] = base; path[low] = $0; next }
            seen[low] != base { print "    " path[low] "\n    " $0 "\n"; bad = 1 }
            END { exit bad ? 1 : 0 }
        '
    then
        printf '    ok\n'
    else
        printf '    FAIL: names above differ only by case\n'
        status=1
    fi
}

check "platform" firmware/platform

for board_cmake in firmware/*/*/Board.cmake; do
    [ -e "$board_cmake" ] || continue
    board_dir=$(dirname "$board_cmake")
    check "$(basename "$board_dir") + platform" "$board_dir" firmware/platform
done

exit $status
