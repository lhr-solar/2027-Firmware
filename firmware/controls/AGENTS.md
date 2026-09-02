# AGENTS.md — `firmware/controls/`

Area rules for controls firmware. The root [`AGENTS.md`](../../AGENTS.md)
applies; this file wins on conflict.

**State: empty.** This directory contains only `README.md`. No boards, no
sources, no build. Everything below describes intent.

## Boards

| Board | Role |
|---|---|
| **VCU** | Vehicle Control Unit — top-level vehicle state and coordination |
| **MIKA** | Driver interface / instrumentation |
| **Pedals** | Pedal box — accelerator and brake sensing |

## This tree governs vehicle motion

VCU and Pedals decide whether and how hard the car moves. Treat them with the
same discipline as `psys/`:

1. **Plausibility and redundancy checks stay.** Dual-sensor agreement on the
   accelerator, brake-plus-throttle detection, and range/short/open detection on
   pedal inputs are safety requirements, not defensive extras. Never remove or
   loosen one.
2. **Fail safe means torque off.** Any implausible input, stale CAN message, or
   error return must command zero torque, not the last known value. Design for
   the timeout case explicitly.
3. **Stale CAN data is dangerous.** A message that stopped arriving must be
   treated as a fault after a defined timeout. Never act on a cached value with
   no freshness check — this is the most common way a `Recv`-based control loop
   goes wrong.
4. **Vehicle state transitions are explicit.** Model them as a state machine
   with defined entry conditions, not as scattered boolean flags.
5. **Human review is mandatory** for VCU and Pedals logic. Flag it in the PR.

## Getting started here

Building on `firmware/platform/` — see
[`firmware/platform/AGENTS.md`](../platform/AGENTS.md). Motor commands go out
over `MotorCAN`, driver interface over `SteeringCAN`, vehicle-wide state over
`CarCAN`; the DBCs are empty today, so message definitions must be added in
`can/dbc/` first — see [`can/AGENTS.md`](../../can/AGENTS.md) for the CAN ID
collision rule.

There is no CMake build yet (root `AGENTS.md` §5). Get human sign-off on board
directory layout and build integration before generating files.

Ownership: see `.github/CODEOWNERS`.
