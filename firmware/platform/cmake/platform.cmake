### Platform configuration file
# doesn't get built independently - included in top level cmake
include_guard()

# this file lives in platform/cmake/ -- anchor paths on platform/ itself
get_filename_component(PLATFORM_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

# c11 for now :P
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

### Grab platform-wide sources (shared by every entry point)
## middleware/
file(GLOB FREERTOS_SOURCES
    "${PLATFORM_DIR}/middleware/FreeRTOS-Kernel/*.c"
    "${PLATFORM_DIR}/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/*.c"
)
file(GLOB FATFS_SOURCES "${PLATFORM_DIR}/middleware/FatFs/Src/*.c")
# TinyUSB
# TODO: in TinyUSB integration PR

## psp/
file(GLOB PSP_SOURCES "${PLATFORM_DIR}/psp/Src/*.c")
## utils/
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS "${PLATFORM_DIR}/utils/Src/*.c")
## drivers/
file(GLOB DRIVERS_SOURCES CONFIGURE_DEPENDS "${PLATFORM_DIR}/drivers/Src/*.c")

# Include ALL dirs
set(PLATFORM_INCLUDE_DIRS
    "${PLATFORM_DIR}/stm/CMSIS/Device/ST/STM32G4xx/Include" # includes all port headers (might change ltr)
    "${PLATFORM_DIR}/stm/CMSIS/Include"
    "${PLATFORM_DIR}/stm/STM32G4xx_HAL_Driver/Inc"
    "${PLATFORM_DIR}/common/Inc" # hal_conf, FreeRTOSConfig.h
    "${PLATFORM_DIR}/middleware/FreeRTOS-Kernel/include"
    "${PLATFORM_DIR}/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F"
    "${PLATFORM_DIR}/middleware/FatFs/Inc"
    "${PLATFORM_DIR}/middleware" # header-only libs (nanoprintf)
    "${PLATFORM_DIR}/psp/Inc"
    "${PLATFORM_DIR}/utils/Inc"
    "${PLATFORM_DIR}/drivers/Inc"
)

### build_firmware(<target>)
### Applies the compile/link recipe 
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
endfunction()
