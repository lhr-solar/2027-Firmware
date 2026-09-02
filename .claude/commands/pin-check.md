---
description: Vet pin assignments and peripheral inits against the MCU reference tables
allowed-tools: Bash(grep:*), Bash(awk:*), Bash(sed:*), Bash(cat:*), Bash(rg:*), Bash(git diff:*), Bash(find:*)
---

Vet hardware pin usage against `references/`. Target: **$ARGUMENTS** — a file, a
directory, a `PIN AF SIGNAL` triple to look up, or nothing (then check the
working-tree diff).

## References

| Package | File |
|---|---|
| LQFP48 (STM32G473CET6), 38 GPIO, ports A/B/C-partial/F/G | `references/stm32g473cet_lqfp48_alternate_functions.md` |
| LQFP100 (STM32G473VET6), 86 GPIO, ports A/B/C/D/E/F/G | `references/stm32g473vet_lqfp100_alternate_functions.md` |

Rows are `| Port | Pin | AF0 | ... | AF15 |`; `-` means nothing is mapped. In
pipe-split terms field *n* is AF*(n-4)* — read by the header, don't count.

If the target's package is unknown, check **both** and report per package. Say
which you used; never guess one.

## Procedure

For every `init.Alternate = GPIO_AFn_PERIPH`, `init.Pin = GPIO_PIN_x` /
`HAL_GPIO_Init(GPIOy, ...)` pair, and peripheral instance in scope:

1. **Pin exists in the package?** LQFP48 has no port D or E and only part of
   port C. A pin valid on LQFP100 may not be bonded out.
2. **Signal on that pin at that AF?** Read the pin's row, the `AFn` column.
3. **AF number correct for this pin?** The same peripheral sits at different AF
   numbers on different pins. ST defines a HAL macro for each, so the wrong one
   compiles and the pin muxes elsewhere — silently.
4. **Direction and channel:** `_TX`/`_RX`, `_SCL`/`_SDA`, `_CH1`/`_CH1N`.
5. Skip `#ifdef` blocks guarded for other MCU families (`STM32F4xx`, `STM32L4xx`
   …) — but if a G4 block's mapping matches the F4 mapping, that is a red flag,
   not a coincidence.

```bash
# what can this pin do?
grep -E '^\| PA2 ' references/stm32g473vet_lqfp100_alternate_functions.md

# which pins carry a signal, and at which AF?
awk -F'|' '/^\| P/{for(i=4;i<=19;i++){gsub(/^ +| +$/,"",$i);
  if($i ~ /FDCAN1_RX/) printf "%s AF%d\n", $2, i-4}}' \
  references/stm32g473vet_lqfp100_alternate_functions.md
```

## Report

One line per problem: `file:line — PIN AFn: code says X, <package> says Y`, plus
the correct pin/AF if one exists on this part. Then a one-line verdict with the
count checked. If a mapping isn't in `references/`, say so and stop — do not
fall back on recalled datasheet knowledge.

`psp/Src/UART.c` has three known-wrong mappings (UART4, UART5, LPUART1) already
documented in `firmware/platform/AGENTS.md`. Report them only if in scope, and
mark them **known**, not new.
