# CAN

CAN specification & tooling

- `dbc/` — vehicle DBC files (flat)
- `canspec-viewer/` — CANspec UI source (Vite + vite-plugin-singlefile)
- `codegen/` — CAN header generation & validators

## Who does what

| Role | Edit | Build site? | Commit local `dist/` / JSON? |
|------|------|-------------|------------------------------|
| DBC author | `can/dbc/*.dbc` | No — use a UI Release or live Pages | **No** (gitignored) |
| Site editor | `can/canspec-viewer/` | Local preview optional; CI publishes | **No** on source branches |

Docs (including CANspec) are published by **`build-docs`** → **`gh-pages`**. Never push `dist/` or `canspec-data.json` to `main` / `canspec`.

## DBC authors — edit, generate JSON, view locally (no site build)

1. Edit DBCs under `can/dbc/`.
2. Download the latest **CANspec UI** Release asset (built `index.html`), or use the live Pages site.
3. Generate JSON next to that HTML (exact filename the UI loads):

```bash
pip install -r can/canspec-viewer/requirements.txt
python3 can/canspec-viewer/export_canspec_json.py -o ./canspec-data.json
```

4. Put **`canspec-data.json` in the same folder as `index.html`**.
5. Serve that folder (browsers often block `file://` fetch):

```bash
python3 -m http.server 4173
# open http://127.0.0.1:4173/
```

If the JSON is missing, the UI shows this path explicitly.

## Docs publish (`build-docs`)

One workflow builds/deploys **all** docs to `gh-pages` (same pattern as Embedded-Sharepoint). CANspec is currently the only section (`/canspec/`); more docs can stage into `_site/` later.

On push (paths under `can/**`, `docs/**`, …) or `workflow_dispatch`:

1. Builds the single-file CANspec UI + JSON from `can/dbc/`
2. Deploys `_site/` to **`gh-pages`** (triggers **pages build and deployment**)
3. When CANspec UI sources change (or dispatch with release), publishes a **GitHub Release** with HTML only

Local (site maintainers only; do not commit outputs):

```bash
cd can/canspec-viewer
npm ci
npm run build-site          # dist/index.html only
npm run export-data:dist    # dist/canspec-data.json for local preview
npm run preview             # optional
```

## Live site

Pages: **Deploy from a branch** → `gh-pages` / `/ (root)`. CANspec: `/canspec/`.
