# references/

**Hardware ground truth.** Transcribed MCU documentation that firmware must be
checked against. Reference data only — nothing here is compiled, generated, or
imported by code.

| File | Part | Package | GPIO | Ports |
|---|---|---|---|---|
| `stm32g473cet_lqfp48_alternate_functions.md` | STM32G473CET6 | LQFP48 | 38 | A, B, C (partial), F, G |
| `stm32g473vet_lqfp100_alternate_functions.md` | STM32G473VET6 | LQFP100 | 86 | A, B, C, D, E, F, G |

Both are the same die with the same peripherals — **only the pins bonded out
differ.** Source for both: *STM32G473xB/xC/xE datasheet DS12712 Rev 5*, Table 13
(alternate function mapping) filtered by the package column of Table 12.

## Table format

After an AF-number-to-peripheral-family legend, one row per GPIO pin:

```
| Port | Pin | AF0 | AF1 | ... | AF15 |
| PA2  | 22  |  -  | TIM2_CH3 | ... | EVENTOUT |
```

- **Port** — pin name; some carry a parenthesized default function, e.g.
  `PC14 (OSC32_IN)`, `PG10 (NRST)`
- **Pin** — physical package pin number, and it **differs between packages**
  (PA2 is pin 10 on LQFP48, pin 22 on LQFP100)
- **AF0–AF15** — the signal available at that alternate function number; `-`
  means nothing is mapped there
- Power, ground, and reference pins are omitted — they have no alternate
  functions

Note when parsing: in the raw markdown a leading empty field precedes `Port`, so
in pipe-split terms field *n* is AF*(n-4)*. Read by the header row rather than
by counting.

## How to use these

Every pin configuration and peripheral init in firmware must be checked against
the file for that board's package — see root [`AGENTS.md`](../AGENTS.md) §2,
"Pin and alternate-function vetting", which is the authoritative procedure. The
short version:

```bash
# What can PA2 do?
grep -E '^\| PA2 ' references/stm32g473vet_lqfp100_alternate_functions.md

# Which pins can carry FDCAN1_RX, and at which AF?
awk -F'|' '/^\| P/{for(i=4;i<=19;i++){gsub(/^ +| +$/,"",$i);
  if($i ~ /FDCAN1_RX/) printf "%s AF%d\n", $2, i-4}}' \
  references/stm32g473vet_lqfp100_alternate_functions.md
```

The failure this prevents: a peripheral appears at different AF numbers on
different pins, ST's HAL defines a macro for each (`GPIO_AF8_LPUART1` *and*
`GPIO_AF12_LPUART1`), and both compile. Pick the wrong one and the pin muxes to
something else — silently, with no build error and no runtime error, just a dead
peripheral. Nothing in this repo catches that automatically today.

## Maintaining

Record the document ID and revision in any file added here, and re-verify
against the current datasheet revision when ST publishes one. If a board uses a
part not listed above, its reference must be added here before firmware for it
is written.

Errata are **not** covered by these files. Silicon bugs are invisible in the
alternate-function tables and need their own reference.
