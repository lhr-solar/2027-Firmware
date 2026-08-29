import { useState } from "react";
import { NavLink, Outlet, useLocation } from "react-router-dom";
import { CANSPEC_DATA_FILENAME, CanspecDataError } from "./data";
import { useCanspecData } from "./useCanspecData";

function MissingDataPanel({ error }: { error: Error }) {
  const kind = error instanceof CanspecDataError ? error.kind : "other";
  const isMissing = kind === "missing" || kind === "invalid";

  return (
    <div className="main">
      <div className="missing-data">
        <h1 className="page-title">CAN data not loaded</h1>
        <p className="err">{error.message}</p>

        {isMissing && (
          <>
            <p className="missing-data-lead">
              Put the generated file here (same folder as this page’s <code>index.html</code>):
            </p>
            <pre className="missing-data-path">{CANSPEC_DATA_FILENAME}</pre>
            <p className="page-sub">
              Full path example when serving the CANspec release folder:
            </p>
            <pre className="missing-data-path">…/canspec/{CANSPEC_DATA_FILENAME}</pre>
            <p className="page-sub">Generate it from the repo DBCs (do not commit the JSON on source branches):</p>
            <pre className="missing-data-cmd">{`pip install -r can/canspec-viewer/requirements.txt
python3 can/canspec-viewer/export_canspec_json.py -o ./${CANSPEC_DATA_FILENAME}`}</pre>
            <p className="page-sub">
              Then serve that folder (e.g. <code>python3 -m http.server</code>) so the page can fetch the
              JSON. On GitHub Pages this file is published automatically to{" "}
              <code>/canspec/{CANSPEC_DATA_FILENAME}</code>.
            </p>
          </>
        )}
      </div>
    </div>
  );
}

export function Layout() {
  const { data, error, loading } = useCanspecData();
  const [q, setQ] = useState("");
  const location = useLocation();
  const onDbcViewer = location.pathname.startsWith("/n/");

  if (loading) {
    return (
      <div className="spinner-wrap">
        <div className="spinner" aria-label="Loading" />
      </div>
    );
  }

  if (error || !data) {
    return <MissingDataPanel error={error ?? new Error("Unable to load CAN data.")} />;
  }

  return (
    <div className="layout-root">
      <header className="topbar">
        <div className="brand">
          <span className="brand-title">Firmware</span>
          <span className="brand-name">CAN Spec</span>
        </div>
        {!onDbcViewer ? (
          <label className="search-field">
            <span className="sr-only">Search</span>
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" aria-hidden>
              <path
                d="M10.5 18a7.5 7.5 0 110-15 7.5 7.5 0 010 15zM16.5 16.5L21 21"
                stroke="currentColor"
                strokeWidth="1.6"
                strokeLinecap="round"
              />
            </svg>
            <input
              type="search"
              placeholder="Filter networks and messages…"
              value={q}
              onChange={(e) => setQ(e.target.value)}
              autoComplete="off"
            />
          </label>
        ) : (
          <div className="topbar-spacer" aria-hidden />
        )}
        <div className="topbar-actions">
          <NavLink className={({ isActive }) => `doc-link${isActive ? " doc-link--active" : ""}`} to="/" end>
            Networks
          </NavLink>
          <NavLink
            className={({ isActive }) => `doc-link${isActive ? " doc-link--active" : ""}`}
            to="/summary"
          >
            Summary
          </NavLink>
        </div>
      </header>
      <div className="app-shell">
        <main className="main main-full">
          <Outlet context={{ data, q, path: location.pathname }} />
        </main>
      </div>
    </div>
  );
}
