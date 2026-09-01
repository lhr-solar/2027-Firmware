# Top level CMake directives
cmake_minimum_required(VERSION 3.2.0) 

project(
    App
    VERSION 1.0.0
    DESCRIPTION "APP"
    LANGUAGES C
)

### Set parameters
set(ARM_MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
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
)
# FatFS
file(GLOB FATFS_SOURCES "platform/middleware/FatFs/Src/*.c")
add_library(FatFS STATIC ${FATFS_SOURCES})
target_include_directories(FatFS PUBLIC platform/middleware/FatFs/Inc)
# TinyUSB
# TODO: in TinyUSB integration PR

add_library(middleware INTERFACE)
target_link_libraries(middleware INTERFACE
    FreeRTOS
    FatFS
)

## psp/
file(GLOB PSP_SOURCES "platform/psp/Src/*.c")
add_library(psp STATIC ${PSP_SOURCES})
target_include_directories(psp PUBLIC platform/psp/Inc)
target_link_libraries(psp
    PUBLIC
        common
        hal
        middleware
)

## utils/
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS "platform/utils/Src/*.c")
add_library(utils STATIC ${UTILS_SOURCES})
target_include_directories(utils PUBLIC platform/utils/Inc)
target_link_libraries(utils
    PUBLIC
        common
        hal
        middleware
)

## drivers/
file(GLOB DRIVERS_SOURCES CONFIGURE_DEPENDS "platform/drivers/Src/*.c")
add_library(drivers STATIC ${DRIVERS_SOURCES})
target_include_directories(drivers PUBLIC platform/drivers/Inc)
target_link_libraries(drivers
    PUBLIC
        common
        hal
        middleware
)

## bootloader/
# TODO: in bootloader PR


### Exec / link libs
set(TEST "blinky" CACHE STRING "Name of test")
set(TEST_SOURCE "platform/tests/Src/${TEST}_test.c")
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SOURCE}")
    message(FATAL_ERROR "Test source file not found: ${TEST_SOURCE}")
endif()

add_executable(App ${TEST_SOURCE})
target_include_directories(App PRIVATE platform/tests/Inc )

target_compile_options(App PRIVATE ${ARM_MCU_FLAGS})

target_link_libraries(App
  common # will remove some based on deps tracking
  hal
  middleware
  drivers
  psp
  utils
)