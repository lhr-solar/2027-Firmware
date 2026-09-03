# AGENTS.md — `firmware/psys/` (Power Systems)

Area rules for power systems firmware. The root [`AGENTS.md`](../../AGENTS.md)
applies; this file wins on conflict.

**State: empty.** This directory contains only `README.md`. No boards, no
sources, no build. Everything below describes intent.

## Boards

| Board | Role |
|---|---|
| **LV** | Low-voltage distribution and control |
| **HV** | High-voltage system control |
| **BPS** | Battery Protection System — pack monitoring and protection |

## This tree is safety-critical

BPS and HV govern a high-voltage battery pack. A firmware fault here is a fire,
a shock hazard, or a disqualification — not a bug report.

Rules that override ordinary convenience:

1. **Never weaken a protection path.** Fault detection, contactor control,
   watchdogs, timeouts, and bounds checks are not refactor targets. Do not
   relax, bypass, comment out, or "temporarily" disable one — including to make
   something build or a test pass.
2. **Fail safe by default.** On any unexpected state, error return, or timeout,
   the safe action is to open contactors / de-energize, not to continue. New
   code must have a defined behavior for every error path; no silent
   `return` on failure.
3. **No unbounded blocking in a fault path.** `portMAX_DELAY` on a semaphore in
   a protection loop is a hang, and a hung BPS is an unprotected pack.
4. **Human review is mandatory.** An agent must not be the last reviewer of BPS
   or HV logic. Flag every such change explicitly in the PR description.
5. **Cite your source for any physical threshold.** Cell voltage limits,
   temperature limits, current limits, timing requirements — these come from the
   cell datasheet, the pack design, or the competition regulations. Never invent
   a plausible-looking number. If you do not have the value, leave a `TODO`
   naming what is needed and who must supply it.

## Getting started here

Building on `firmware/platform/` — see
[`firmware/platform/AGENTS.md`](../platform/AGENTS.md). There is no CMake build
yet (root `AGENTS.md` §5), so creating the first board means proposing a
directory layout and build integration. Get human sign-off on both before
generating files.


## Hardware vetting

Every board in this tree must declare its **package** — STM32G473CET6 (LQFP48)
or STM32G473VET6 (LQFP100) — in its README and in a comment at the top of its
pin configuration. The two packages expose different pins: LQFP48 has no port D
or E and only part of port C.

Before writing or reviewing any pin configuration or peripheral init, check it
against the matching file in [`references/`](../../references/). Verify the pin
exists in that package, that the signal is on that pin, and that the AF number
is the right one *for that pin* — a peripheral sits at different AF numbers on
different pins, and picking the wrong one compiles fine and silently produces a
dead peripheral. Root [`AGENTS.md`](../../AGENTS.md) §2 has the full procedure;
it is mandatory, not advisory.

For this tree specifically: a mis-muxed pin on BPS or HV is not a dead
peripheral, it is a protection path that never fires. Pin vetting is part of the
safety review, not a separate formality.

Ownership: see `.github/CODEOWNERS` (per-folder routing is currently suppressed
but records the intended owner).
