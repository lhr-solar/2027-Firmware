# Sourced by the board build scripts -- not run on its own
#   board_cmake_list <VAR>: the values of set(<VAR> ...) in ./Board.cmake, space-separated
# the set(...) block is cut out with awk, then the command, closing ")", comments and quotes
# are stripped -- a ")" inside a comment within the list ends it early
board_cmake_list() {
    awk -v var="$1" '$0 ~ "^ *set\\(" var "([ \t)]|$)" {f=1} f{print} f&&/\)/{exit}' Board.cmake \
        | sed -e "s/.*set($1//" -e 's/).*//' -e 's/#.*//' \
        | tr -d '"' | tr '\n' ' '
}

#   board_variants_to_build <requested>: <requested> if given, otherwise all of BOARD_VARIANTS
# prints nothing for a board without variants; CMake rejects a <requested> it doesn't know
board_variants_to_build() {
    if [ -n "$1" ]; then
        echo "$1"
    else
        board_cmake_list BOARD_VARIANTS
    fi
}
