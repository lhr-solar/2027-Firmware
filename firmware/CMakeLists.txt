# Top level CMake directives
cmake_minimum_required(VERSION 3.2.0) 

project(
    App
    VERSION 1.0.0
    DESCRIPTION "APP"
    LANGUAGES C ASM # asm for startup file
)

### Set parameters
set(ARM_MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Compiler flags
add_compile_options(
    ${ARM_MCU_FLAGS}
    -Og # change to O2 if builds are slow
    -g
    -gdwarf-2
    -Wall
    -Werror
    -Wfatal-errors
    -fdata-sections
    -ffunction-sections
    -ffreestanding
)

add_link_options(${ARM_MCU_FLAGS})

# Preprocessor stuff for HAL 
add_compile_definitions(
    USE_HAL_DRIVER
    STM32G4xx
    STM32G473xx
)

### Set options
# GCC v5 - 14 default to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### Grab sources
## stm/
file(GLOB HAL_SOURCES
    "platform/stm/STM32G4xx_HAL_Driver/Src/*.c"
    "platform/stm/system_stm32g4xx.c"
    "platform/stm/stm32g4xx_hal_timebase_tim.c"
    "platform/common/Src/*.c" # hal_init, sysmem, syscalls
)
list(FILTER HAL_SOURCES EXCLUDE REGEX "_template\\.c$") # ignore template files

## middleware/
file(GLOB FREERTOS_SOURCES
    "platform/middleware/FreeRTOS-Kernel/*.c"
    "platform/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/*.c"
)
file(GLOB FATFS_SOURCES "platform/middleware/FatFs/Src/*.c")
# TinyUSB
# TODO: in TinyUSB integration PR

## psp/
file(GLOB PSP_SOURCES "platform/psp/Src/*.c")

## utils/
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS "platform/utils/Src/*.c")

## drivers/
file(GLOB DRIVERS_SOURCES CONFIGURE_DEPENDS "platform/drivers/Src/*.c")

## bootloader/
# TODO: in bootloader PR

set(APP_INCLUDE_DIRS
    "platform/stm/CMSIS/Device/ST/STM32G4xx/Include" # includes all port headers (might change ltr)
    "platform/stm/CMSIS/Include"
    "platform/stm/STM32G4xx_HAL_Driver/Inc"
    "platform/common/Inc" # hal_conf, FreeRTOSConfig.h
    "platform/middleware/FreeRTOS-Kernel/include"
    "platform/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F"
    "platform/middleware/FatFs/Inc"
    "platform/middleware" # header-only libs (nanoprintf)
    "platform/psp/Inc"
    "platform/utils/Inc"
    "platform/drivers/Inc"
)

# cache blinky at first -> if recompiled with TEST variable then overwrites the last cached var
set(TEST "blinky" CACHE STRING "Name of test (platform/tests/Src/<TEST>_test.c)")
set(TEST_SOURCE "platform/tests/Src/${TEST}_test.c")

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SOURCE}")
    message(FATAL_ERROR "Unknown TEST '${TEST}' -- no ${TEST_SOURCE}.")
endif()

set(STARTUP_SOURCE "platform/stm/stm32g473/startup_stm32g473xx.s") # only asm source
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/platform/stm/stm32g473/STM32G473XXx_FLASH.ld")

add_executable(App
    ${TEST_SOURCE}
    ${STARTUP_SOURCE}
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${FATFS_SOURCES}
    ${PSP_SOURCES}
    ${UTILS_SOURCES}
    ${DRIVERS_SOURCES}
)
target_include_directories(App PRIVATE platform/tests/Inc ${APP_INCLUDE_DIRS})
target_compile_options(App PRIVATE ${ARM_MCU_FLAGS})
set_target_properties(App PROPERTIES SUFFIX ".elf")

target_link_options(App PRIVATE
    ${ARM_MCU_FLAGS}
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/App.map,--cref
    -Wl,--gc-sections
    -nodefaultlibs
)

# Generate .hex and .bin post build
add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:App> ${CMAKE_CURRENT_BINARY_DIR}/App.bin
    COMMENT "Generating binary image: App.bin"
)
add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:App> ${CMAKE_CURRENT_BINARY_DIR}/App.hex
    COMMENT "Generating Intel HEX image: App.hex"
)

# must have jolly good message
string(ASCII 27 Esc)
set(ColorReset "${Esc}[0m")
set(ColorGreen "${Esc}[32m")
set(ColorBoldGreen "${Esc}[1;32m")

message(STATUS "${ColorBoldGreen}=== Jolly good! ===${ColorReset}")