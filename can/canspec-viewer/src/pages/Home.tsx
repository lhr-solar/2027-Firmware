import { useState } from "react";
import { IdPreview } from "../components/IdPreview";
import { Link } from "react-router-dom";
import { allNetworks } from "../data";
import { useLayoutOutlet } from "../layoutContext";
import { matchesQuery } from "../search";

export function Home() {
  const [preview, setPreview] = useState(false);
  const { data, q } = useLayoutOutlet();
  const networks = allNetworks(data).filter((n) =>
    matchesQuery(q, n.id, n.filename, String(n.messages.length)),
  );

  return (
    <div>
      <h1 className="page-title">Networks</h1>
      <p className="page-sub">
        Browse a bus or search the complete ID allocation.
      </p>

      {preview && <IdPreview data={data} onClose={() => setPreview(false)} />}
      <div className="home-actions">
        <button
          type="button"
          className="btn-primary"
          onClick={() => setPreview(true)}
        >
          ID preview
        </button>
        <Link className="btn-primary" to="/summary">
          All networks
        </Link>
      </div>

      <div className="grid-cards">
        {networks.map((n) => (
          <article key={n.id} className="network-card">
            <div className="network-card-head">
              <span className="network-card-title">{n.id}</span>
              <div className="network-card-meta">
                {n.messages.length} message{n.messages.length === 1 ? "" : "s"}
              </div>
            </div>
            <div className="network-card-actions">
              <Link className="ecu-chip" to={`/n/${encodeURIComponent(n.id)}`}>
                DBC viewer
              </Link>
              <Link
                className="ecu-chip"
                to={`/summary/${encodeURIComponent(n.id)}`}
              >
                Summary
              </Link>
            </div>
          </article>
        ))}
      </div>
      {networks.length === 0 && (
        <div className="empty-state">No networks match your filter.</div>
      )}
    </div>
  );
}
