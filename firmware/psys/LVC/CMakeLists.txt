### LV Carrier board build
## Config hardcoded for G473 series MCUS
cmake_minimum_required(VERSION 3.25.3)

project(
    LVC
    VERSION 1.0.0
    DESCRIPTION "LV Carrier"
    LANGUAGES C ASM # asm for startup file
)

include(${CMAKE_CURRENT_LIST_DIR}/../../platform/cmake/board_build.cmake)
