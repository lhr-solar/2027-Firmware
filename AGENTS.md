# AGENTS.md — lhr-solar/2027-Firmware

Instructions for AI coding agents working in this repository. Human contributors
should read this too; it is the shortest accurate description of the repo.

`CLAUDE.md` at the repo root defers to this file. Nested `AGENTS.md` files exist
in `can/`, `firmware/platform/`, and each `firmware/<subteam>/` directory — when
working in one of those trees, read the nested file as well. **The nearest
`AGENTS.md` wins on conflict.**

---

## 1. What this repository is

Firmware monorepo for the **Longhorn Racing Solar (LHRS)** 2027 solar vehicle —
UT Austin's solar car team. It holds every embedded target that ships on the
car, the shared hardware-abstraction platform they build on, and the CAN network
specification that defines how they talk to each other.

Prior-generation code lives in the team's `2027-Chud` repository; `can/` was
copied from it (commit `1d2c939`). When you need historical context for a file
in `can/`, that is where it came from.

### Read this before you write anything: the repo is a scaffold

Most of this monorepo is **intentionally empty placeholder**. Do not mistake a
stub for working code, and do not describe stubs as implemented.

| Path | Real state |
|---|---|
| `firmware/platform/` | **Real.** Ported, substantial C. The only working code here. |
| `firmware/psys/`, `firmware/controls/`, `firmware/telemetry/` | README-only. No sources, no boards started. |
| `can/dbc/*.dbc` | All 8 files are **0 bytes**. No messages defined yet. |
| `can/codegen/*.py` | Every entry point raises `NotImplementedError`. |
| `can/canspec-viewer/` | Real React/Vite source; depends on DBCs that do not exist yet. |
| `.github/workflows/*.yml` | Jobs exist but bodies are `echo "TODO: ..."`. |
| `flake.nix` | 0 bytes. |
| CMake build | **Does not exist anywhere in the tree.** See §5. |

If a task depends on one of these, say so plainly rather than inventing the
missing piece. Building the missing piece is fine when that *is* the task.

---

## 2. Hardware and domain context

- **MCU:** STM32G473 (Arm Cortex-M4F, FPU, 170 MHz). Linker script and startup
  under `firmware/platform/stm/stm32g473/`. It is currently the only supported
  part — do not assume another STM32 family works without checking.
- **RTOS:** FreeRTOS kernel, statically allocated. Existing drivers use static
  tasks/queues/semaphores (`StaticTask_t`, `StaticQueue_t`, `StaticSemaphore_t`)
  with `configMINIMAL_STACK_SIZE` stacks. Match that; avoid heap allocation in
  firmware.
- **Debug/flash:** OpenOCD. Configs at `firmware/platform/openocd-stm32g4x.cfg`
  and `firmware/platform/stm/stm32g473/stm32g473xx.cfg`.
- **Buses:** classic CAN (`CAN.h`) and CAN-FD (`CAN_FD.h`) are separate
  peripherals with separate drivers on this part. Confirm which a board uses
  before writing message code — they are not interchangeable.
- **Other middleware in-tree:** TinyUSB (CDC), FatFs (SD card logging),
  `nanoprintf` (the `printf.h` shim in `psp/`).

### CAN networks

Each `.dbc` in `can/dbc/` is one physical bus, not one board:

| DBC | Bus |
|---|---|
| `CarCAN.dbc` | Primary vehicle bus |
| `MotorCAN.dbc` | Motor controller bus |
| `SteeringCAN.dbc` | Steering wheel / driver interface |
| `LightingCAN.dbc` | Lighting |
| `DAqCAN.dbc` | Data acquisition |
| `ElconCAN.dbc` | Elcon charger (vendor-defined IDs — follow the charger datasheet, not our conventions) |
| `telemetry_bebug.dbc` | Telemetry debug (filename typo is in the repo as-is; do not silently rename — see §8) |
| `bootloading.dbc` | Bootloader protocol |

### Boards (2027 targets)

None of these have firmware yet. Board directory names are not fixed; when you
create the first one, follow §5 and flag the naming choice for human review.

| Subteam | Boards | Meaning |
|---|---|---|
| `firmware/psys/` | LV, HV, BPS | Low-voltage distribution, high-voltage, battery protection system |
| `firmware/controls/` | VCU, MIKA, Pedals | Vehicle control unit, MIKA, pedal box |
| `firmware/telemetry/` | Sensor board | Telemetry / sensor acquisition |

**Safety-critical:** BPS and HV govern a high-voltage pack; VCU and Pedals
govern vehicle motion. Changes in `psys/` and `controls/` are not routine
refactors. Never weaken a fault path, watchdog, timeout, or bounds check in
those trees as a side effect of another change, and never disable a safety check
to make a build or test pass.

---

## 3. Repository map

```
2027-Firmware/
├── AGENTS.md               ← you are here (repo-wide agent instructions)
├── CLAUDE.md               ← pointer to this file
├── flake.nix               ← EMPTY. Intended Nix dev shell for the toolchain.
├── .github/
│   ├── CODEOWNERS          ← source of truth for ownership (see §4)
│   ├── PULL_REQUEST_TEMPLATE.md
│   └── workflows/          ← build-firmware.yml, test-can-matrix.yml (stubs)
├── .claude/                ← Claude Code settings + slash commands
├── can/                    ← CAN spec & tooling.  See can/AGENTS.md
│   ├── dbc/                ← one .dbc per physical bus (all empty today)
│   ├── codegen/            ← DBC → C header generation + validators (stubs)
│   └── canspec-viewer/     ← React/Vite single-file CAN spec browser
└── firmware/
    ├── platform/           ← shared HAL/RTOS/driver platform.  See firmware/platform/AGENTS.md
    ├── psys/               ← power systems boards (stub)
    ├── controls/           ← controls boards (stub)
    └── telemetry/          ← telemetry boards (stub)
```

---

## 4. Ownership and review routing

**`.github/CODEOWNERS` is the single source of truth for who owns what.** Read
it before proposing reviewers, and re-read it rather than trusting any list
copied elsewhere — including this file.

Right now the per-folder entries are **deliberately commented out** while the
repo is still being set up (commit `6f3336c`, "Surpress CODEOWNERS till after
MVP"), so GitHub currently routes every review to the single global owner. The
commented lines are not dead text — they record the intended routing and will be
enabled after MVP. Treat them as advisory today and authoritative once
uncommented.

Practical consequence for agents: when your change touches a subteam directory,
name the owner from `CODEOWNERS` (commented or not) in your PR description so
the right person sees it, even though GitHub will not request them
automatically yet.

Do not edit `CODEOWNERS` — including uncommenting entries — unless a human asks
for that specific change.

---

## 5. Building, flashing, testing

**There is no build system in this repository yet.** No Makefile, no
`CMakeLists.txt`, no toolchain file. Nothing in this repo currently compiles.
Do not fabricate build or flash commands, and do not claim a change builds.

The decided direction is **CMake + `arm-none-eabi-gcc`**, cross-compiling to
STM32G473. `compile_commands.json` is already gitignored, anticipating CMake's
`CMAKE_EXPORT_COMPILE_COMMANDS`. When the build lands it should provide, at
minimum:

- a toolchain file pinning `arm-none-eabi-gcc` with the correct
  `-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard` flags,
- one CMake target per board, each linking the shared platform,
- the linker script at `firmware/platform/stm/stm32g473/STM32G473XXx_FLASH.ld`,
- an OpenOCD flash target using the configs in §2.

If you are asked to add the build, propose the layout and get human sign-off
before generating dozens of files. Until it exists, verify C changes by careful
reading and by keeping them consistent with surrounding code — and say
explicitly in your summary that the change is **unbuilt and unverified**.

### What *can* be run today

```bash
# CAN spec viewer (needs Node)
cd can/canspec-viewer && npm ci && npm run dev

# CAN spec JSON export (needs Python + cantools) — will produce nothing
# useful until the DBCs have content
pip install -r can/canspec-viewer/requirements.txt
python3 can/canspec-viewer/export_canspec_json.py -o ./canspec-data.json
```

`firmware/platform/tests/` holds **on-target hardware test programs**, one
`main()` per file, flashed to a dev board and observed over UART/USB. They are
not host unit tests and there is no test runner. See
`firmware/platform/AGENTS.md`.

---

## 6. Coding standards

### Formatting

A `.clang-format` is planned but **not yet present**. Before formatting any C
code, check for it:

- **If `/.clang-format` exists**, it is authoritative — parse it and follow it
  over anything written below.
- **If it does not exist**, match the style of the surrounding file. Do not
  reformat existing code, do not run a formatter with guessed settings, and do
  not introduce a `.clang-format` unless a human asks for one.

Observed conventions in LHR-authored C (`psp/`, `drivers/`, `utils/`,
`common/`), for matching surrounding code — descriptive, not binding:

- 4-space indent, spaces not tabs
- opening brace on the same line, for functions and blocks alike
- pointer star binds to the type: `EMC2305_HandleTypeDef* chip`
- `#pragma once` in newer headers; older ones use `#ifndef` guards — follow the
  file you are in
- `//` line comments inside function bodies; Doxygen `/** ... */` blocks for
  file and public-API documentation
- lines run long in places (200+ chars exist); keep new code tighter, around
  100 columns

### Naming

- Public driver API: `Module_PascalCaseVerb` — `EMC2305_SetFanPWM`,
  `EMC2305_ReadReg`
- File-local statics follow FreeRTOS's `prv` convention:
  `prvAcquireFreeSemaphore`
- ISRs and HAL callbacks keep their vendor-mandated names exactly
  (`FDCAN1_IT0_IRQHandler`, `HAL_CAN_RxFifo0MsgPendingCallback`) — renaming them
  silently breaks the vector table
- Types: `TitleCase` with a `TypeDef` suffix where the file already does so
- Macros and constants: `SCREAMING_SNAKE_CASE`

### Documentation

Public headers carry a Doxygen file block explaining what the driver does, the
call order it expects, and its gotchas — `firmware/platform/psp/Inc/CAN.h` is
the reference example. Match that depth when adding a public driver; a
one-line comment is not sufficient for a peripheral API.

---

## 7. Git and PR workflow

- **Never commit directly to `main`.** Branch first.
- Recent history uses PRs with a `(#N)` suffix in the squashed subject
  (`Initial Ports (#12)`). Write imperative, specific subjects.
- Keep PRs scoped to one subteam directory where possible — it keeps review
  routing clean once `CODEOWNERS` is live.
- Fill in `.github/PULL_REQUEST_TEMPLATE.md`, including the hardware-testing
  section. "Not tested on hardware" is an acceptable answer; silence is not.
- Commit or push **only when the user explicitly asks.**

### Never commit

Build outputs (`**/build/`, `compile_commands.json`), `dist/` or
`canspec-data.json` from the viewer, `flake.lock`, Vector `.ldb` lock files,
editor droppings, or anything else already in `.gitignore`. The CAN spec site is
published by CI to `gh-pages`; generated site artifacts must never land on
`main`.

---

## 8. Rules for agents

1. **Do not invent state.** If the DBCs are empty, the codegen is a stub, and
   there is no build, say that. Never present a stub as working, and never
   report a change as building or tested when nothing compiled it.
2. **Vendored code is read-only.** `firmware/platform/stm/`,
   `middleware/FreeRTOS-Kernel/`, `middleware/TinyUSB/`, `middleware/FatFs/`,
   and `stm/CMSIS/` are upstream. Do not edit them without explicit human
   approval; if a change there is genuinely required, isolate it and call it out
   as a patch that must survive the next upstream bump.
3. **Safety code is not refactor fodder.** See §2 for what counts.
4. **Scope discipline.** Do the task asked. Do not opportunistically reformat,
   rename, add changelogs, or "fix" adjacent files — including obvious-looking
   typos such as `telemetry_bebug.dbc` or the `Surpress` in a commit message. A
   filename is referenced by tooling and CI; flag it, let a human decide.
5. **CAN IDs are a shared, contended resource.** Never assign or change one
   without checking every DBC in `can/dbc/` for collisions. See `can/AGENTS.md`.
6. **Prefer the platform over reimplementation.** Before writing a peripheral
   routine in board code, check `firmware/platform/psp/` and `drivers/` for an
   existing one.
7. **Ask when the answer changes the work.** Board naming, CAN ID allocation,
   build layout, and anything touching BPS/HV fault behavior are human calls.
8. When you finish, state plainly what you verified and what you did not.
