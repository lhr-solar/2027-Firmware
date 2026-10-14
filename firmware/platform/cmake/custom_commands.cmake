### Custom build targets
include_guard()

# start of user flash
set(FLASH_ADDRESS 0x8000000)

# resolved here (include-time, when CMAKE_CURRENT_LIST_DIR is reliably this
# file's own directory) rather than inside add_flash_usb_target() -- inside a
# function, CMAKE_CURRENT_LIST_DIR follows the *caller's* listfile, not this one
set(FLASH_USB_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../../flash_usb.sh")

### add_flash_target(<target>)
# calls st link to flash binary
function(add_flash_target TARGET_NAME)
    add_custom_target(flash
        COMMAND st-flash write $<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.bin ${FLASH_ADDRESS}
        COMMENT "Flashing ${TARGET_NAME}.bin @ ${FLASH_ADDRESS}"
    )
    add_dependencies(flash ${TARGET_NAME})
endfunction()

### add_flash_usb_target(<target>)
# flashes over USB DFU via STM32CubeProgrammer -- board must already be in
# its USB DFU bootloader (see firmware/flash_usb.sh)
function(add_flash_usb_target TARGET_NAME)
    add_custom_target(flash-usb
        COMMAND ${FLASH_USB_SCRIPT} $<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.bin ${FLASH_ADDRESS}
        COMMENT "Flashing ${TARGET_NAME}.bin @ ${FLASH_ADDRESS} over USB"
    )
    add_dependencies(flash-usb ${TARGET_NAME})
endfunction()

### add_dump_symbols_target(<target>)
# dump symbols of generated ELF binary
function(add_dump_symbols_target TARGET_NAME)
    add_custom_target(dump-symbols
        COMMAND ${CMAKE_OBJDUMP} -x $<TARGET_FILE:${TARGET_NAME}>
        COMMENT "Dumping symbols for ${TARGET_NAME}"
    )
    add_dependencies(dump-symbols ${TARGET_NAME})
endfunction()

### add_dump_size_target(<target>)
# dump ELF size (.text, .data, .bss, ...)
function(add_dump_size_target TARGET_NAME)
    add_custom_target(dump_size
        COMMAND ${CMAKE_OBJDUMP} -h $<TARGET_FILE:${TARGET_NAME}>
        COMMENT "Section sizes for ${TARGET_NAME}"
    )
    add_dependencies(dump_size ${TARGET_NAME})
endfunction()

### add_erase_target()
# erases mcu programmable flash 
function(add_erase_target)
    add_custom_target(erase
        COMMAND st-flash erase
        COMMENT "Erasing MCU flash"
    )
endfunction()
