---
description: Scaffold a new board target under a subteam (proposal first, then files)
---

Scaffold firmware for a new board: **$ARGUMENTS**

Read the relevant subteam `AGENTS.md` (`firmware/psys|controls|telemetry/`),
`firmware/platform/AGENTS.md`, and root `AGENTS.md` §5 before doing anything.

**Do not generate files first.** There is no CMake build in this repo yet and no
board has ever been created here, so the first board sets the convention for
every board after it. Present a short proposal and get explicit approval:

1. Which subteam directory it belongs in, and the exact directory name.
2. Directory layout (`Inc/`, `Src/`, board `CMakeLists.txt`?) — mirror
   `firmware/platform/`'s `Inc/`+`Src/` convention.
3. How it links the shared platform, and which platform pieces it needs
   (`psp/CAN` vs `psp/CAN_FD`, ADC, specific `drivers/`).
4. Which CAN buses it sits on, and whether any DBC work must land first.
5. **Which package** — STM32G473CET6 (LQFP48) or STM32G473VET6 (LQFP100).
   This is required before any pin can be assigned; the two expose different
   pins. Record it in the board README and in a comment at the top of the pin
   configuration.
6. Which peripherals and pins it uses — if this is not known, say so and stop;
   do not invent a pinout. Every pin you do assign must be vetted against the
   matching file in `references/`: pin present in that package, signal on that
   pin, correct AF number *for that pin*. Show the reference rows you checked.
7. For `psys/` or `controls/`: what the fail-safe state is and which fault paths
   the board owns.

After approval, generate the minimum that compiles-in-principle: a `main` with
RTOS init, no speculative feature code. Note that `psp/` ships default pin
mappings (e.g. `psp/Src/UART.c`) that may not match this board's schematic —
check them against the package rather than inheriting them blindly, and be aware
three of them are known-wrong (see `firmware/platform/AGENTS.md`). State clearly that it is unbuilt, since
no build system exists to verify it, and name the `CODEOWNERS` owner who should
review.
