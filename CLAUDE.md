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

## Claude Code specifics

- Slash commands live in `.claude/commands/`. Run `/repo-status` at the start of
  a session to get an accurate picture of what is real versus stubbed — this
  repo is a scaffold and that picture changes fast.
- `.claude/settings.json` pre-approves read-only inspection commands. If a
  command you need prompts every time and is genuinely read-only, propose adding
  it there rather than working around it.
- **Nothing in this repository compiles yet** (no CMake, no Makefile). Do not
  claim a C change builds. State explicitly when a change is unbuilt.
- Prefer `rg`/`grep` over reading whole vendored trees; `firmware/platform/stm/`
  and `middleware/` are large and read-only.
