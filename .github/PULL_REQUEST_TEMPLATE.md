NOTE TO HUMAN AND AGENT REVIEWERS - DO NOT REVIEW UNLESS THE FOLLOWING HAS BEEN FILLED OUT

## What & why

<!-- What changes, and what problem it solves. Link the issue if there is one. -->

## Area

<!-- Check every tree this PR touches. -->

- [ ] `firmware/platform/` — shared platform (**affects every board**)
- [ ] `firmware/psys/` — LV / HV / BPS
- [ ] `firmware/controls/` — VCU / MIKA / Pedals
- [ ] `firmware/telemetry/` — sensor board
- [ ] `can/` — DBCs, codegen, or spec viewer
- [ ] CI / tooling / docs

## Hardware testing

<!-- Required. "Not tested on hardware" is an acceptable answer; silence is not. -->

- [ ] Flashed and verified on hardware — board(s) and what was observed:
- [ ] Builds only, not run on hardware
- [ ] Not built (no build system exists yet for this target)

**Details:**

## Hardware / pin validity

<!-- Required for any change that configures a pin or initializes a peripheral. -->

- [ ] No pin or peripheral configuration changed
- [ ] Target package stated below, and every pin/AF checked against the matching
      file in `references/` — pin exists in that package, signal is on that pin,
      AF number is correct *for that pin*

**Package (LQFP48 / LQFP100):**
**Pins & AFs touched, and reference rows checked:**

## CAN impact

- [ ] No CAN changes
- [ ] Adds or changes message IDs — **all DBCs in `can/dbc/` checked for
      collisions**, IDs checked listed below
- [ ] Changes a receive filter or `CAN_RECV_ENTRY` table — verified consistent
      with the peripheral filter configuration

**IDs touched:**

## Safety

<!-- Delete this section only if the PR touches neither psys/ nor controls/. -->

- [ ] No fault path, watchdog, timeout, or bounds check was weakened, bypassed,
      or removed
- [ ] All new error paths reach a defined safe state
- [ ] Any physical threshold (voltage, current, temperature, timing) cites its
      source — datasheet, pack design, or regulation
- [ ] A human has reviewed the safety-relevant logic

## Reviewers

<!-- Per-folder CODEOWNERS routing is currently suppressed, so GitHub will not
     auto-request the right person. Name the owner from .github/CODEOWNERS for
     each directory you touched. -->

## Checklist

- [ ] No build artifacts, `dist/`, `canspec-data.json`, or `.ldb` files committed
- [ ] No edits to vendored trees (`stm/CMSIS`, `STM32G4xx_HAL_Driver`,
      `middleware/*`) — or the patch is called out above and justified
- [ ] New public driver APIs have a Doxygen block describing call order and gotchas
- [ ] Relevant `AGENTS.md` updated if this changes repo structure, conventions,
      or what is stubbed vs. implemented

<!-- If an AI agent wrote part of this PR, say which parts and what was
     human-verified. -->
