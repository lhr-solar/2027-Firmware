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
