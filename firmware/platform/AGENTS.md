# AGENTS.md — `firmware/platform/`

Area rules for the shared embedded platform. The root
[`AGENTS.md`](../../AGENTS.md) still applies; this file wins on conflict.

**This is the only tree in the repository with substantial working code.** Every
board in `psys/`, `controls/`, and `telemetry/` will build on it, so a change
here is a change to all of them. Treat it as shared infrastructure: prefer
additive changes, and get a human's agreement before altering an existing public
API signature.

## Editable vs. vendored

| Path | Status | What it is |
|---|---|---|
| `psp/` | **Editable** | Platform support package — LHR's peripheral drivers |
| `drivers/` | **Editable** | Off-chip device drivers |
| `utils/` | **Editable** | Small reusable helpers |
| `common/` | **Editable** | Used to modify middleware/HAL configurations |
| `tests/` | **Editable** | On-target hardware test programs |
| `stm/` | **READ-ONLY** | Arm CMSIS headers, STM32 HAL |
| `middleware/FreeRTOS-Kernel/` | **READ-ONLY** | FreeRTOS |
| `middleware/TinyUSB/` | **READ-ONLY** | TinyUSB stack |
| `middleware/FatFs/` | **READ-ONLY** | FatFs |
| `middleware/nanoprintf.h` | **READ-ONLY** | Single-header printf |

Do not edit a read-only tree without explicit human approval. If a vendor fix is
genuinely required, keep it minimal, comment it as a local patch, and call it out
prominently — it must survive the next upstream bump. Configuration belongs in
`common/Inc/` (`FreeRTOSConfig.h`, `stm32g4xx_hal_conf.h`) or in a glue file
under `drivers/`, never in the vendor source itself.

`stm/` also holds LHR-adjacent files that *are* editable in practice —
`system_stm32g4xx.c`, `stm32g4xx_hal_timebase_tim.c`, and the `stm32g473/`
startup, linker script, and OpenOCD config. Touch them only with a specific
reason; they govern clock setup, the RTOS tick source, and memory layout.

## Contents

**`psp/`** — on-chip peripherals: `ADC`, `CAN` (classic bxCAN), `CAN_FD`
(FDCAN), `UART`, `printf` (nanoprintf shim). Classic CAN and CAN-FD are separate
peripherals with separate drivers; they are not interchangeable.

**`drivers/`** — `EMC2305` (Microchip PWM fan controller, I²C, FreeRTOS worker
task + semaphore pool), `WS2812B` (addressable LEDs), `SevenSegment`, `sdcard`,
`user_diskio` (FatFs glue).

**`utils/`** — `movingAverage` (header-only), `slcanFormat` (SLCAN serial-line CAN framing).

**`common/`** — `stm32g4xx_hal_init.c`, `FreeRTOSConfig.h`,
`stm32g4xx_hal_conf.h`, and newlib `syscalls.c` / `sysmem.c` / `stubs.c`.

## Conventions specific to this tree

**Static allocation.** Existing drivers allocate FreeRTOS objects statically —
`StaticTask_t`, `StaticQueue_t`, `StaticSemaphore_t`, explicit stack arrays,
explicit queue storage. `EMC2305.c` is the reference. Do not introduce
`malloc`/`pvPortMalloc` in new firmware code.

**ISR and callback names are fixed.** `FDCAN1_IT0_IRQHandler`,
`CAN1_RX0_IRQHandler`, `HAL_ADC_ConvCpltCallback`, and friends must match the
vector table and ST's weak symbols exactly. Renaming one produces a build that
links and a peripheral that silently never fires.

**Doxygen file blocks on public headers.** `psp/Inc/CAN.h` sets the bar: what
the driver does, the required call order (Init → Start → use → Stop → DeInit),
and the hardware gotchas (CAN2 shares filter banks with CAN1 and must be
initialized after it; CAN3 is independent). A new public driver needs the same
treatment.

**Hardware constraints belong in comments.** Peripheral sharing, filter-bank
conflicts, required init ordering — write them down. They are invisible in the
code and expensive to rediscover on a bench.

## Pin and alternate-function vetting

Root [`AGENTS.md`](../../AGENTS.md) §2 makes vetting every pin and peripheral
init against `references/` mandatory. It applies here with one wrinkle specific
to this tree.

**`psp/` and `drivers/` are package-agnostic but not pin-agnostic.** They ship
default pin mappings inside `HAL_*_MspInit`-style functions — `psp/Src/UART.c`
picks the pins for every UART instance. Those defaults must be valid on the
**part**, and any board whose schematic routes a peripheral elsewhere must
override them rather than silently inherit a wrong mapping. When you touch one:

- check the pin and AF number against **both** package files in `references/`
- if a default is only valid on LQFP100, say so in a comment beside it — a
  board on LQFP48 will not have that pin at all
- treat a change to a default mapping as a change to every board, per the
  shared-infrastructure rule at the top of this file

### Known defects in `psp/Src/UART.c`

Verified against `references/` (DS12712 Rev 5) on 2026-09-02 — **not yet
fixed.** All three are STM32F4 pin mappings left in the STM32G4 path; each
compiles cleanly against the G4 HAL and each leaves the peripheral dead.

| Site | Code says | STM32G473 actually has |
|---|---|---|
| `UART.c:251` | UART4 on PA0/PA1, `GPIO_AF8_UART4` | UART4_TX/RX exist **only** on PC10/PC11 at **AF5**; UART4 is not on PA0/PA1 at any AF |
| `UART.c:270` | UART5 on PC12/PD2, `GPIO_AF8_UART5` | correct pins, wrong AF — UART5_TX PC12 / UART5_RX PD2 are **AF5** |
| `UART.c:344` | LPUART1 on PA2/PA3, `GPIO_AF8_LPUART1` | correct pins, wrong AF — LPUART1_TX/RX on PA2/PA3 are **AF12**. `GPIO_AF8_LPUART1` is valid, but only on PB10/PB11, PB12/PB13, PC0/PC1 |

Do not copy these mappings into new code, and do not "fix" them as a drive-by:
`psp/` is shared by every board, the repo has no build to verify against, and
the owner in `.github/CODEOWNERS` should sign off. Flag them if you are working
nearby.

## `tests/` is not a unit-test suite

`tests/Src/*.c` are **on-target hardware test programs**: one `main()` per file,
flashed to a dev board and observed over UART/USB/LEDs. There is no test runner,
no assertions framework, and no way to run them on a host. `blinky_test.c`,
`can_isr_test.c`, `sdcard_mt_test.c`, `tusb_cdc_test.c` are typical; `_mt_`
means multi-task.

`tests/Inc/canN_recv_entries.h` declare which CAN IDs each interface accepts,
via an X-macro:

```c
// CAN_RECV_ENTRY(ID, SIZE, CIRCULAR)
CAN_RECV_ENTRY(0x001, 5, false)
CAN_RECV_ENTRY(BPS_FAULT_ID, 4, true)
```

These entries must stay consistent with the peripheral's hardware filter
configuration passed to `can_init` — an entry with no matching filter receives
nothing, and the mismatch is silent. Symbolic IDs come from `can_msgs.h`, which
will eventually be generated from `can/dbc/`.

Do not describe a change here as "tested" unless a human actually flashed it.
