## STM32 CMake file
# paths for HAL, linker and startup script, compiler defs, ...
include_guard()

# this file lives in platform/cmake/ -- anchor paths on platform/ itself
get_filename_component(PLATFORM_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

# Preprocessor stuff for HAL
add_compile_definitions(
    USE_HAL_DRIVER
    STM32G4xx
    STM32G473xx
)

# Glob HAL
file(GLOB HAL_SOURCES
    "${PLATFORM_DIR}/stm/STM32G4xx_HAL_Driver/Src/*.c"
    "${PLATFORM_DIR}/stm/system_stm32g4xx.c"
    "${PLATFORM_DIR}/stm/stm32g4xx_hal_timebase_tim.c"
    "${PLATFORM_DIR}/common/Src/*.c" # hal_init, sysmem, syscalls
)
list(FILTER HAL_SOURCES EXCLUDE REGEX "_template\\.c$") # ignore template files

set(STARTUP_SOURCE "${PLATFORM_DIR}/stm/stm32g473/startup_stm32g473xx.s")
set(LINKER_SCRIPT "${PLATFORM_DIR}/stm/stm32g473/STM32G473XXx_FLASH.ld")