import { useState } from "react";
import { NavLink, Outlet, useLocation } from "react-router-dom";
import { useCanspecData } from "./useCanspecData";

export function Layout() {
  const {
    data,
    error,
    loading,
    branch,
    branches,
    entry,
    selectBranch,
    notice,
    catalogError,
    skipped,
  } = useCanspecData();
  const [q, setQ] = useState("");
  const location = useLocation();
  const onDbcViewer = location.pathname.startsWith("/n/");

  return (
    <div className="layout-root">
      <header className="topbar">
        <NavLink className="brand" to="/">
          <span className="brand-name">
            CAN Spec <span className="muted-sm">/ 2027</span>
          </span>
        </NavLink>
        <label className="branch-field">
          <span className="sr-only">Branch</span>
          <select
            aria-label="Branch"
            className="dbc-select"
            value={branch}
            onChange={(e) => selectBranch(e.target.value)}
            disabled={!branches.length}
          >
            {!branches.length && <option value="">Loading branches…</option>}
            {branches.map((item) => (
              <option key={item.name} value={item.name}>
                {item.name}
              </option>
            ))}
          </select>
          {entry?.commit && (
            <a
              className="commit-link mono"
              href={`https://github.com/lhr-solar/2027-Firmware/commit/${entry.commit}`}
              title="View source commit"
            >
              {entry.commit.slice(0, 7)}
            </a>
          )}
        </label>
        {!onDbcViewer && (
          <label className="search-field">
            <span className="sr-only">Search networks and messages</span>
            <input
              type="search"
              placeholder="Search…"
              value={q}
              onChange={(e) => setQ(e.target.value)}
            />
          </label>
        )}
        <nav className="topbar-actions" aria-label="Main">
          <NavLink
            className={({ isActive }) =>
              `doc-link${isActive ? " doc-link--active" : ""}`
            }
            to="/"
            end
          >
            Networks
          </NavLink>
          <NavLink
            className={({ isActive }) =>
              `doc-link${isActive ? " doc-link--active" : ""}`
            }
            to="/summary"
          >
            Summary
          </NavLink>
        </nav>
      </header>
      {(notice || catalogError || skipped.length > 0) && (
        <p className="branch-notice" role="status">
          {notice ||
            catalogError ||
            `Invalid DBC branches omitted: ${skipped.join(", ")}`}
        </p>
      )}
      <main className="main main-full">
        {loading ? (
          <div className="spinner-wrap">
            <div className="spinner" aria-label="Loading branch data" />
          </div>
        ) : error || !data ? (
          <div className="empty-state">
            <h1 className="page-title">CAN data unavailable</h1>
            <p className="err" role="alert">
              {error?.message || catalogError || "No branch data found."}
            </p>
            <p>Select another branch or retry after the next publication.</p>
          </div>
        ) : (
          <Outlet context={{ data, q, path: location.pathname }} />
        )}
      </main>
    </div>
  );
}
