---
description: Report what is actually implemented vs. stubbed in this scaffold repo
allowed-tools: Bash(find:*), Bash(ls:*), Bash(wc:*), Bash(grep:*), Bash(git status:*), Bash(git log:*)
---

This repository is a scaffold: much of it is empty placeholder. Before doing any
work, establish ground truth so you never describe a stub as working code.

Gather and report:

1. **Zero-byte files** — these are unimplemented placeholders:
   `find . -path ./.git -prune -o -type f -empty -print`
2. **Stubbed Python** in `can/codegen/` and `can/canspec-viewer/`:
   `grep -rln "NotImplementedError\|TODO" can/ --include=*.py`
3. **Stubbed CI** — workflow bodies that only echo:
   `grep -rn "TODO" .github/workflows/`
4. **Board directories with no sources**:
   `find firmware/psys firmware/controls firmware/telemetry -type f`
5. **Whether a build exists yet**:
   `find . -path ./.git -prune -o \( -name CMakeLists.txt -o -name Makefile -o -name "*.cmake" \) -print`
6. **Whether `/.clang-format` exists** — if it does, it governs C formatting
   (root `AGENTS.md` §6).
7. **DBC content**: for each file in `can/dbc/`, its size and message count
   (`grep -c '^BO_ '`).
8. `git status` and the last few commits.

Then give a short summary in this shape: what is real, what is a stub, what
changed since the state recorded in `AGENTS.md`. **If anything contradicts
`AGENTS.md` — a stub got implemented, a build appeared, DBCs gained content —
say so explicitly and offer to update the affected `AGENTS.md` file.**
