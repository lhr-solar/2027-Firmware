# AGENTS.md — `can/`

Area rules for the CAN specification and its tooling. The root
[`AGENTS.md`](../AGENTS.md) still applies; this file wins on conflict.

`README.md` in this directory documents the human workflow (publishing, Pages,
releases) and is worth reading before changing anything in `canspec-viewer/`.

## What lives here

| Path | Purpose |
|---|---|
| `dbc/` | One `.dbc` per **physical bus** — flat, no subdirectories. The network contract. |
| `codegen/` | DBC → C header generation and validators, consumed by the `test-can-matrix` workflow. |
| `canspec-viewer/` | React + Vite single-file spec browser, published to `gh-pages` under `/canspec/`. |

Copied from the team's `2027-Chud` repository in commit `1d2c939`.

## Current state

- **All 8 DBC files are 0 bytes.** There are no messages, signals, or nodes
  defined anywhere yet. Any statement about "the existing CAN matrix" is a
  statement about an empty set.
- All three scripts in `codegen/` are entry-point stubs that raise
  `NotImplementedError`:
  - `generate_can_headers.py` — DBC → C headers
  - `check_duplicate_can_ids.py` — cross-bus ID collision check
  - `validate_dbc_generation.py` — codegen drift check (generated output vs. committed)
- `canspec-viewer/` source is real, but `export_canspec_json.py` will produce an
  empty spec until the DBCs have content.

## Rules

**CAN IDs are a contended, safety-relevant resource.** Two boards sharing an ID
is a silent, intermittent, on-track failure. Before adding or changing any ID,
grep every file in `dbc/` — not just the one you are editing — and report the
IDs you checked. `check_duplicate_can_ids.py` is meant to automate this and
does not work yet; until it does, the check is manual and non-optional.

**Bus, not board.** A `.dbc` describes one physical bus. A board that sits on
two buses appears in two DBCs. Do not create a per-board DBC.

**`ElconCAN.dbc` is vendor-defined.** IDs and layouts come from the Elcon
charger's datasheet. Do not renumber them to fit our conventions.

**Do not rename DBC files.** `telemetry_bebug.dbc` is misspelled. It is
referenced by tooling, workflow path filters, and the published site; renaming
it is a coordinated change a human must approve. Flag it, do not fix it.

**Never commit generated artifacts.** `dist/`, `canspec-data.json`, `.ldb`
Vector lock files. CI builds and publishes the site; generated output on `main`
is always a mistake.

**Generated headers are outputs, not sources.** Once `generate_can_headers.py`
works, never hand-edit a generated header — change the DBC and regenerate. If
you find yourself editing generated C, you are fixing the wrong file.

## Tooling

```bash
# Python side
pip install -r canspec-viewer/requirements.txt      # cantools
python3 canspec-viewer/export_canspec_json.py -o ./canspec-data.json

# Viewer (Node)
cd canspec-viewer
npm ci
npm run dev          # local dev server
npm run build-site   # tsc -b && vite build → dist/index.html (do not commit)
```

The viewer is TypeScript/React 19 with `react-router-dom`, bundled by
`vite-plugin-singlefile` into one HTML file. It loads `canspec-data.json` from
the same directory as `index.html` at runtime; if the JSON is missing the UI
says so explicitly rather than failing silently.
