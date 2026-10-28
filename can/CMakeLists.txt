# Shared generated CAN codecs. Link only the physical buses a board needs.
cmake_minimum_required(VERSION 3.20)

foreach(entry IN ITEMS car:car_can charger:charger_can daq:d_aq_can elcon:elcon_can lighting:lighting_can motor:motor_can mppt:mpptcan steering:steering_can)
    string(REPLACE ":" ";" parts "${entry}")
    list(GET parts 0 bus)
    list(GET parts 1 source)
    add_library(can_${bus} STATIC EXCLUDE_FROM_ALL
        "${CMAKE_CURRENT_LIST_DIR}/generated/${source}.c"
        "${CMAKE_CURRENT_LIST_DIR}/generated/detail/${source}_raw.c"
    )
    target_include_directories(can_${bus} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/generated")
    target_link_libraries(can_${bus} PUBLIC m)
endforeach()
