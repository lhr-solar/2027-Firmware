# Top level CMake directives
cmake_minimum_required(VERSION 3.2.0) 

project(
    App
    VERSION 1.0.0
    DESCRIPTION "APP"
    LANGUAGES C ASM
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


### Add libraries
## stm/ + common/
file(GLOB HAL_SOURCES
    "platform/stm/STM32G4xx_HAL_Driver/Src/*.c"
    "platform/stm/stm32g4xx_hal_timebase_tim.c"
    "platform/stm/system_stm32g4xx.c"
    "platform/common/Src/*.c"
)
add_library(hal STATIC ${HAL_SOURCES})
target_include_directories(hal PUBLIC
    "platform/stm/CMSIS/Device/ST/STM32G4xx/Include" # includes all port headers (might change ltr)
    "platform/stm/CMSIS/Include"
    "platform/stm/STM32G4xx_HAL_Driver/Inc"
    "platform/common/Inc"
)

## middleware/ 
# FreeRTOS
file(GLOB FREERTOS_SOURCES 
    "platform/middleware/FreeRTOS-Kernel/*.c"
    "platform/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/*.c"
)
add_library(FreeRTOS STATIC ${FREERTOS_SOURCES})
target_include_directories(FreeRTOS PUBLIC
    platform/middleware/FreeRTOS-Kernel/include
    platform/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
    platform/common/Inc # FreeRTOSConfig.h
)
# FatFS
file(GLOB FATFS_SOURCES "platform/middleware/FatFs/Src/*.c")
add_library(FatFS STATIC ${FATFS_SOURCES})
target_include_directories(FatFS PUBLIC 
    platform/middleware/FatFs/Inc
    platform/common/Inc

    # just fucking kill me
    platform/drivers/Inc
    platform/stm/CMSIS/Device/ST/STM32G4xx/Include
    platform/stm/CMSIS/Include
    platform/stm/STM32G4xx_HAL_Driver/Inc
    platform/middleware/FreeRTOS-Kernel/include
    platform/middleware/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
    
)
# TinyUSB
# TODO: in TinyUSB integration PR

add_library(middleware INTERFACE)
target_link_libraries(middleware INTERFACE
    FreeRTOS
    FatFS
)
# Include header-only libs (nanoprintf)
target_include_directories(middleware INTERFACE
    platform/middleware 
)

## psp/
file(GLOB PSP_SOURCES "platform/psp/Src/*.c")
add_library(psp STATIC ${PSP_SOURCES})
target_include_directories(psp PUBLIC platform/psp/Inc)
target_link_libraries(psp
    PUBLIC
        hal
        middleware
)

## utils/
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS "platform/utils/Src/*.c")
add_library(utils STATIC ${UTILS_SOURCES})
target_include_directories(utils PUBLIC platform/utils/Inc)
target_link_libraries(utils
    PUBLIC
        hal
        middleware
)

## drivers/
file(GLOB DRIVERS_SOURCES CONFIGURE_DEPENDS "platform/drivers/Src/*.c")
add_library(drivers STATIC ${DRIVERS_SOURCES})
target_include_directories(drivers PUBLIC platform/drivers/Inc)
target_link_libraries(drivers
    PUBLIC
        hal
        middleware
)

## bootloader/
# TODO: in bootloader PR


### Exec / link libs
set(TEST "blinky" CACHE STRING "Name of test")
set(TEST_SOURCE "platform/tests/Src/${TEST}_test.c")
set(STARTUP_SOURCE "platform/stm/stm32g473/startup_stm32g473xx.s")

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SOURCE}")
    message(FATAL_ERROR "Test source file not found: ${TEST_SOURCE}")
endif()

add_executable(App ${TEST_SOURCE} ${STARTUP_SOURCE})
target_include_directories(App PRIVATE platform/tests/Inc )

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/platform/stm/stm32g473/STM32G473XXx_FLASH.ld")

target_compile_options(App PRIVATE ${ARM_MCU_FLAGS})

target_link_options(App PRIVATE
    ${ARM_MCU_FLAGS}
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/App.map,--cref
    -Wl,--gc-sections
    # -nostdlib
    -nodefaultlibs
)

target_link_libraries(App
  hal
  middleware
  drivers
  psp
  utils
)