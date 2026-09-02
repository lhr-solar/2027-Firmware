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

Ownership: see `.github/CODEOWNERS`.
