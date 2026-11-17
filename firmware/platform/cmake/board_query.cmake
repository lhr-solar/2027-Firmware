### Board query
# Prints one variable from a board's Board.cmake, space-separated, so shell scripts can read it
# without parsing CMake themselves.
#   usage: cmake -DBOARD_DIR=<board_dir> -DQUERY=<VAR> -P board_query.cmake
# e.g. QUERY=BOARD_VARIANTS prints "build1 build2", or nothing for a board without variants

if(NOT BOARD_DIR OR NOT QUERY)
    message(FATAL_ERROR "usage: cmake -DBOARD_DIR=<board_dir> -DQUERY=<VAR> -P board_query.cmake")
endif()
if(NOT EXISTS "${BOARD_DIR}/Board.cmake")
    message(FATAL_ERROR "no Board.cmake in ${BOARD_DIR}")
endif()

# Board.cmake globs its sources with file(GLOB ... CONFIGURE_DEPENDS), which CMake rejects in
# script mode. A query never needs the source list, so file() does nothing while it's included.
function(file)
endfunction()

include("${BOARD_DIR}/Board.cmake")

string(REPLACE ";" " " QUERY_VALUE "${${QUERY}}")
execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "${QUERY_VALUE}")
