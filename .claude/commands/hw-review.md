---
description: Review changed firmware C for embedded and RTOS hazards
allowed-tools: Bash(git diff:*), Bash(git status:*), Bash(grep:*), Bash(cat:*), Bash(rg:*)
---

Review the current changes ($ARGUMENTS, or `git diff` against `main` if no
target given) for hazards specific to this codebase. This is a firmware review,
not a style review — no build system exists, so careful reading is the only
verification available.

Check for:

**Concurrency / RTOS**
- Shared state touched from both an ISR and a task without a critical section,
  or without `volatile` where required
- Non-ISR-safe FreeRTOS calls in an ISR (must be the `...FromISR` variants, with
  `portYIELD_FROM_ISR` where needed)
- `portMAX_DELAY` blocking in a fault, timeout, or safety path
- Dynamic allocation — this codebase is statically allocated (`StaticTask_t`,
  `StaticQueue_t`, `StaticSemaphore_t`); flag `malloc`/`pvPortMalloc`
- Stack sizes that ignore `configMINIMAL_STACK_SIZE` conventions
- Lock ordering that could deadlock; locks held across slow I/O (SD writes)

**Peripheral correctness**
- Classic CAN vs. CAN-FD confusion — separate drivers, not interchangeable
- CAN filter configuration inconsistent with the `CAN_RECV_ENTRY` tables
- CAN2 initialized before CAN1 (they share filter banks — see `psp/Inc/CAN.h`)
- Renamed ISR or HAL callback symbols — these must match the vector table
  exactly or the handler silently never fires
- Unchecked HAL return values

**Safety (`psys/`, `controls/`)**
- Any weakened, bypassed, or removed fault check, watchdog, timeout, or bounds
  check
- Error paths that fall through to "continue" instead of a safe state
- Acting on CAN data with no staleness/freshness check
- Hardcoded physical thresholds with no cited source

**General embedded**
- Integer overflow in fixed-point or timing math; unsigned wraparound in
  tick comparisons
- Buffer sizing against actual DLC / message length
- Blocking or `printf` in time-critical paths
- Edits inside vendored read-only trees (`stm/CMSIS`, `STM32G4xx_HAL_Driver`,
  `middleware/*`) — flag these regardless of merit

Report findings most-severe first, each with file:line, the concrete failure
scenario, and a suggested fix. Say explicitly that nothing was compiled.
