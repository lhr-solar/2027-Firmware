# Power Distribution Unit

Switches and protects the car's low-voltage loads using six TPS2HCS08-Q1 dual-channel
high-side switches (12 outputs) controlled over SPI. Reports load current and faults per channel.

**MCU:** STM32G473CETx (LQFP48)

**KiCad:** <!-- link to the schematic/PCB project -->

## Folders

| Folder | Contents |
|---|---|
| `config/` | Pin and peripheral definitions for this board (`pinDefs.h`) |
| `core/` | Board production code, `core/Src/app.c` |
| `drivers/` | Board-specific drivers, always linked in |
| `tests/` | One `main()` per file; each builds with `make TEST=<name>`. Omit `_test` from the file name |
