### LV Carrier Config
# Folders to look for entry points in --- only one source gets linked
include_guard()

set(ENTRY_POINTS
    "psys/LVC/core/Src"
    "psys/LVC/tests/Src"
)

# Drivers/modules that are ALWAYS linked
file(GLOB BOARD_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/psys/LVC/drivers/Src/*.c")

# Populate with all the includes ...
set(BOARD_INCLUDE_DIRS
    psys/LVC/core/Inc
    psys/LVC/drivers/Inc
    psys/LVC/config/Inc
    psys/LVC/tests/Inc
)
