# Battery Protection System Leader

The leader board of the Battery Protection System (BPS), which monitors the high-voltage battery pack and protects it.

**MCU:** STM32G473VET6 (LQFP100)

**KiCad:** [lhr-solar/PS-BPSLeader](https://github.com/lhr-solar/PS-BPSLeader)

## Folders

| Folder | Contents |
|---|---|
| `config/` | Pin and peripheral definitions for this board (`pinDefs.h`) |
| `core/` | Board production code, `core/Src/app.c` |
| `core/Src/common.c` | Board-wide clock setup and software error handling |
| `drivers/` | Board-specific drivers, always linked in |
| `drivers/Src/StatusLEDs.c` | Driver for the RGB heartbeat LED |
| `drivers/Src/Blinky.c` | FreeRTOS task that cycles the RGB LED through all three colors, used by the blinky test |
| `tests/` | One `main()` per file; each builds with `make TEST=<name>`. Omit `_test` from the file name |
| `tests/Src/blinky_test.c` | Tests that each color of the RGB LED works (`make TEST=blinky`) |
