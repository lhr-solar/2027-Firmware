---
description: Review changed firmware C for embedded and RTOS hazards
allowed-tools: Bash(git diff:*), Bash(git status:*), Bash(grep:*), Bash(cat:*), Bash(rg:*), Bash(awk:*), Bash(sed:*)
---

Review the current changes ($ARGUMENTS, or `git diff` against `main` if no
target given) for hazards specific to this codebase. This is a firmware review,
not a style review — no build system exists, so careful reading is the only
verification available.

Check for:

**Pin and alternate-function validity — do this first, every time**

Non-negotiable, even when the diff looks unrelated to hardware. Read the
relevant file in `references/` (LQFP48 = `stm32g473cet_*`, LQFP100 =
`stm32g473vet_*`; check both if the target package is unknown, and say that you
did). For every pin or peripheral touched by the diff:

- Every `init.Alternate = GPIO_AFn_PERIPH` — is that signal on that pin at that
  AF number? Look up the pin's row and read the `AFn` column.
- Every pin in an `init.Pin = GPIO_PIN_x` / `HAL_GPIO_Init(GPIOy, ...)` pair —
  does it exist in the target package? LQFP48 has no port D or E and only part
  of port C.
- Direction: `_TX` vs `_RX`, `_SCL` vs `_SDA`, `_CH1` vs `_CH1N`.
- Every peripheral instance claimed — does it exist on this part?

Useful lookups:

```bash
grep -E '^\| PA2 ' references/stm32g473vet_lqfp100_alternate_functions.md
awk -F'|' '/^\| P/{for(i=4;i<=19;i++){gsub(/^ +| +$/,"",$i);
  if($i ~ /FDCAN1_RX/) printf "%s AF%d\n", $2, i-4}}' \
  references/stm32g473vet_lqfp100_alternate_functions.md
```

**Treat a wrong AF number as a high-severity finding.** ST's HAL defines a macro
for every AF a peripheral appears at (`GPIO_AF8_LPUART1` *and*
`GPIO_AF12_LPUART1`), so the wrong one compiles cleanly and the peripheral is
silently dead. There is no build and no static check in this repo to catch it.
Be especially suspicious of mappings that look like STM32F4 — that is the known
source of the existing defects in `psp/Src/UART.c` (listed in
`firmware/platform/AGENTS.md`); do not flag those three again unless the diff
touches them.

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
scenario, and a suggested fix. For pin/AF findings, quote the reference row you
checked against and name the file. Say explicitly that nothing was compiled, and
state which package(s) you vetted against.
