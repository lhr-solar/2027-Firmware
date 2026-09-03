### Platform configuration file
# doesn't get built independently - included in top level cmake
include_guard()

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

### Grab platform-wide sources (shared by every entry point)
## stm/
file(GLOB HAL_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/stm/STM32G4xx_HAL_Driver/Src/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/stm/system_stm32g4xx.c"
    "${CMAKE_CURRENT_LIST_DIR}/stm/stm32g4xx_hal_timebase_tim.c"
    "${CMAKE_CURRENT_LIST_DIR}/common/Src/*.c" # hal_init, sysmem, syscalls
)
list(FILTER HAL_SOURCES EXCLUDE REGEX "_template\\.c$") # ignore template files

## middleware/
file(GLOB FREERTOS_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/middleware/FreeRTOS-Kernel/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/*.c"
)
file(GLOB FATFS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/middleware/FatFs/Src/*.c")
# TinyUSB
# TODO: in TinyUSB integration PR

## psp/
file(GLOB PSP_SOURCES "${CMAKE_CURRENT_LIST_DIR}/psp/Src/*.c")

## utils/
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/utils/Src/*.c")

## drivers/
file(GLOB DRIVERS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/drivers/Src/*.c")

## bootloader/
# TODO: in bootloader PR

# Include ALL dirs
set(APP_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/stm/CMSIS/Device/ST/STM32G4xx/Include" # includes all port headers (might change ltr)
    "${CMAKE_CURRENT_LIST_DIR}/stm/CMSIS/Include"
    "${CMAKE_CURRENT_LIST_DIR}/stm/STM32G4xx_HAL_Driver/Inc"
    "${CMAKE_CURRENT_LIST_DIR}/common/Inc" # hal_conf, FreeRTOSConfig.h
    "${CMAKE_CURRENT_LIST_DIR}/middleware/FreeRTOS-Kernel/include"
    "${CMAKE_CURRENT_LIST_DIR}/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F"
    "${CMAKE_CURRENT_LIST_DIR}/middleware/FatFs/Inc"
    "${CMAKE_CURRENT_LIST_DIR}/middleware" # header-only libs (nanoprintf)
    "${CMAKE_CURRENT_LIST_DIR}/psp/Inc"
    "${CMAKE_CURRENT_LIST_DIR}/utils/Inc"
    "${CMAKE_CURRENT_LIST_DIR}/drivers/Inc"
)

set(STARTUP_SOURCE "${CMAKE_CURRENT_LIST_DIR}/stm/stm32g473/startup_stm32g473xx.s")
set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/stm/stm32g473/STM32G473XXx_FLASH.ld")

### build_firmware(<target>)
### Applies the compile/link recipe and post-build objcopy steps that are
### identical for every entry point. 
function(build_firmware TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE ${ARM_MCU_FLAGS})
    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".elf")

    target_link_options(${TARGET_NAME} PRIVATE
        ${ARM_MCU_FLAGS}
        -T${LINKER_SCRIPT}
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map,--cref
        -Wl,--gc-sections
        -nodefaultlibs
    )

    # Generate .hex and .bin post build
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin
        COMMENT "Generating binary image: ${TARGET_NAME}.bin"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex
        COMMENT "Generating Intel HEX image: ${TARGET_NAME}.hex"
    )
endfunction()
