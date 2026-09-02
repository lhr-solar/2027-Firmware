# CLAUDE.md

Read **[AGENTS.md](AGENTS.md)** first — it is the authoritative guide to this
repository (what it is, hardware context, directory meanings, ownership, coding
standards, and the rules agents must follow here). Everything in it applies to
Claude Code.

Nested `AGENTS.md` files add area-specific rules and take precedence over the
root file within their directory:

- [`can/AGENTS.md`](can/AGENTS.md) — CAN spec, DBCs, codegen, spec viewer
- [`firmware/platform/AGENTS.md`](firmware/platform/AGENTS.md) — shared HAL/RTOS platform, vendored code boundaries
- [`firmware/psys/AGENTS.md`](firmware/psys/AGENTS.md) — power systems (LV, HV, BPS)
- [`firmware/controls/AGENTS.md`](firmware/controls/AGENTS.md) — controls (VCU, MIKA, Pedals)
- [`firmware/telemetry/AGENTS.md`](firmware/telemetry/AGENTS.md) — telemetry sensor board

[`references/`](references/) holds the MCU pin/alternate-function tables for both
packages in use (STM32G473 in LQFP48 and LQFP100). Checking pin and peripheral
configuration against them is **mandatory** — see root `AGENTS.md` §2 and
[`references/README.md`](references/README.md).

## Claude Code specifics

- Slash commands live in `.claude/commands/`. Run `/repo-status` at the start of
  a session to get an accurate picture of what is real versus stubbed — this
  repo is a scaffold and that picture changes fast.
- `/pin-check` vets pin assignments and peripheral inits against `references/`;
  `/hw-review` includes that check and adds RTOS/concurrency/safety review. Run
  one of them on any change that configures hardware — the wrong AF number
  compiles cleanly and silently kills the peripheral, and no build or static
  check in this repo will catch it.
- `.claude/settings.json` pre-approves read-only inspection commands. If a
  command you need prompts every time and is genuinely read-only, propose adding
  it there rather than working around it.
- **Nothing in this repository compiles yet** (no CMake, no Makefile). Do not
  claim a C change builds. State explicitly when a change is unbuilt.
- Prefer `rg`/`grep` over reading whole vendored trees; `firmware/platform/stm/`
  and `middleware/` are large and read-only.
