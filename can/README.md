# CAN

- `dbc/` — one canonical DBC per physical bus.
- `codegen/` — DBC → typed C packing/unpacking API.
- `canspec-viewer/` — compact dark CAN specification browser.

## Viewer

The viewer opens directly on the bus list, with no vehicle selector. Choose a
branch in the header, then a bus. Messages expand into signal tables; search
also opens matching messages. Sender/search filters and ID format controls are
under **Filters & ID format**. Enums, comments and metadata remain expandable.
**ID preview** on the main page opens a searchable, numerically sorted allocation
table: ID → transmitting device → bus. Shared IDs appear separately per bus.

```sh
pip install -r can/canspec-viewer/requirements.txt
cd can/canspec-viewer
npm ci
npm run build
npm run preview
```

For live UI editing, generate `public/canspec-data.json` and run `npm run dev`.
Without a branch manifest, the selector shows `local`. The single-file HTML also
works with an exported `canspec-data.json` served alongside it. Generated files,
`dist/`, and `node_modules/` must not be committed to source branches.

## Branch publication

`.github/workflows/publish-can-viewer.yml` builds the viewer and publishes it to
GitHub Pages at `/2027-Firmware/canspec/`. It runs on DBC/viewer pushes across
branches, branch creation/deletion, manual dispatch, and an hourly reconciliation.
The scheduled run covers older branches that do not yet contain the workflow.
The workflow and viewer code must first be merged into the default branch.

Every run fetches and prunes all branch refs, then publishes a complete snapshot:

- `main` is always included, even when its DBC files are intentionally blank.
- Other branches appear only when their current DBC filenames/content differ
  from `main`. Comparison uses file contents, not commit ancestry or SHA equality.
- Each unique DBC snapshot gets one `data/<sha256>.json`. Branches sharing the
  same content share that file. `branches.json` maps names to snapshot URLs and
  source commits. `canspec-data.json` remains a main-only compatibility export.
- Deleted branches, branches matching main, and their unused JSON disappear on
  the next publication. Updating main also reconciles every branch.
- Invalid nonblank DBCs fail parsing. Invalid feature branches are omitted and
  reported in the manifest/UI; invalid main data fails the publication so the
  previous deployed site remains available. Blank scaffold DBCs are valid.
- Only DBC bytes are read from feature branches. UI/exporter code always comes
  from the trusted default-branch checkout.

The UI refreshes the branch list every minute. If a selected branch disappears,
it falls back to main with a notice. A `?branch=dev%2Fexample` URL preserves branch
selection across reloads and can be shared. Snapshot files are content-addressed,
so switching branches does not reuse another branch's cached data.

### GitHub setup

In repository **Settings → Pages**, choose **GitHub Actions** as the source.
The `github-pages` environment must allow deployments from the source branches
that trigger this workflow; a main-only environment rule would block feature
branch DBC publication. Use the workflow's repository read / Pages write / OIDC
permissions; no personal access token is needed. Generated data lives only in
the Pages artifact, not in source commits or a separate `gh-pages` branch.
This replaces the old, absent `build-docs` / branch-based publishing setup.

The publication concurrency group serializes site updates. Each run reconciles
current refs rather than relying on its triggering commit, so queued push and
delete events cannot restore an obsolete branch entry.

To generate a branch publication locally after fetching origin refs:

```sh
python can/canspec-viewer/publish_branches.py --out can/canspec-viewer/dist
python -m unittest discover -s can/canspec-viewer/tests -v
```
