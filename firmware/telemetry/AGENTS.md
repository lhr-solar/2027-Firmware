# AGENTS.md — `firmware/telemetry/`

Area rules for telemetry firmware. The root [`AGENTS.md`](../../AGENTS.md)
applies; this file wins on conflict.

**State: empty.** This directory contains only `README.md`. No boards, no
sources, no build. Everything below describes intent.

## Board

**Sensor board** — telemetry and data acquisition: sampling vehicle sensors,
logging, and getting data off the car.

## Rules

1. **Telemetry is an observer, never an actuator.** Code in this tree reads,
   logs, and transmits. It must not command motion, contactors, or any control
   output. If a task seems to require that, it belongs in `controls/` or
   `psys/` — stop and ask.
2. **Never let logging degrade the bus or the RTOS.** Telemetry shares CAN with
   safety-critical traffic. Do not flood a bus, block in an ISR, spin at high
   priority, or hold a lock across an SD-card write. Drop samples rather than
   stall — and count the drops.
3. **A failed log is not a failed car.** SD card absent, full, or erroring must
   degrade gracefully. Never fault the vehicle because logging failed.
4. **Timestamps and units are part of the data.** Log them explicitly. An
   unlabeled number is not telemetry.

## Relevant platform pieces

- `drivers/sdcard.c` + `drivers/user_diskio.c` and `middleware/FatFs/` — logging
- `psp/UART.c`, `psp/printf.c`, `middleware/TinyUSB` (CDC) — data egress
- `utils/Src/slcanFormat.c` — SLCAN framing for streaming CAN over a serial link
- `utils/Inc/movingAverage.h` — sensor smoothing (header-only)
- `psp/ADC.c` — analog sensor acquisition

See [`firmware/platform/AGENTS.md`](../platform/AGENTS.md). `DAqCAN.dbc` and
`telemetry_bebug.dbc` in `can/dbc/` are this subteam's buses — both empty today.

There is no CMake build yet (root `AGENTS.md` §5). Get human sign-off on board
layout and build integration before generating files.


## Hardware vetting

Every board in this tree must declare its **package** — STM32G473CET6 (LQFP48)
or STM32G473VET6 (LQFP100) — in its README and in a comment at the top of its
pin configuration. The two packages expose different pins: LQFP48 has no port D
or E and only part of port C.

Before writing or reviewing any pin configuration or peripheral init, check it
against the matching file in [`references/`](../../references/). Verify the pin
exists in that package, that the signal is on that pin, and that the AF number
is the right one *for that pin* — a peripheral sits at different AF numbers on
different pins, and picking the wrong one compiles fine and silently produces a
dead peripheral. Root [`AGENTS.md`](../../AGENTS.md) §2 has the full procedure;
it is mandatory, not advisory.

For this tree specifically: check the SD-card SPI, UART, and ADC pins against
the package file before wiring them up — `psp/` ships default mappings that may
not match this board's schematic.

Ownership: see `.github/CODEOWNERS`.
