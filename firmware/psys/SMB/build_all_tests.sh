#!/usr/bin/env sh
# CI runs this to build all SMB tests
# ravi is a chud
set -eu

Esc=$'\e'
ColorReset="${Esc}[0m"
ColorBoldPurple="${Esc}[1;35m"
ColorBoldYellow="${Esc}[1;33m"

cd "$(dirname "$0")"

# Tests that should not be built (space separated), e.g. SKIP="can can_isr"
SKIP=""

for src in tests/Src/*_test.c; do
    [ -e "$src" ] || continue
    name=$(basename "$src" _test.c)

    case " $SKIP " in
        *" $name "*) echo "${ColorBoldYellow}==> skipping $name${ColorReset}"; continue ;;
    esac

    echo "${ColorBoldPurple}==> building $name${ColorReset}"
    make TEST="$name"
done
