---
description: Audit CAN IDs across every DBC for collisions and spec drift
allowed-tools: Bash(grep:*), Bash(ls:*), Bash(find:*), Bash(python3:*), Bash(sort:*), Bash(awk:*)
---

CAN IDs are a shared, safety-relevant resource across the whole vehicle. Two
boards on one ID is a silent, intermittent, on-track failure. Audit them.

Note first that `can/codegen/check_duplicate_can_ids.py` is intended to automate
this but currently raises `NotImplementedError` — so this check is manual. Run
it anyway to confirm it is still a stub; if it now works, run it and report its
output instead.

1. List every DBC and its message count:
   `for f in can/dbc/*.dbc; do echo "$f: $(grep -c '^BO_ ' $f)"; done`
2. Extract every message ID across **all** DBCs (`BO_ <id> <Name>: <dlc> <node>`)
   and look for the same ID defined in more than one place. DBC IDs are decimal
   in the file; extended IDs carry the high bit — normalize before comparing.
3. Cross-check the X-macro receive tables in
   `firmware/platform/tests/Inc/can*_recv_entries.h` against the DBCs: every
   `CAN_RECV_ENTRY` ID should correspond to a defined message, and symbolic
   names should resolve in `can_msgs.h`.
4. Flag IDs referenced in firmware C but absent from every DBC, and vice versa.

Report: total messages per bus, any duplicate IDs (with both locations), any
firmware/DBC mismatches. If everything is empty, say so — an empty matrix is the
expected state today, not a passing check.

$ARGUMENTS
