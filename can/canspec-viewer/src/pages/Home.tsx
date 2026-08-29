import { Link } from "react-router-dom";
import { allNetworks } from "../data";
import { useLayoutOutlet } from "../layoutContext";
import { matchesQuery } from "../search";

export function Home() {
  const { data, q } = useLayoutOutlet();
  const networks = allNetworks(data).filter((n) =>
    matchesQuery(q, n.id, n.filename, String(n.messages.length)),
  );

  return (
    <div>
      <h1 className="page-title">Networks</h1>
      <p className="page-sub">Pick a bus for the DBC viewer, open the message summary, or browse everything.</p>

      <div className="home-actions">
        <Link className="btn-primary" to="/summary">
          All networks
        </Link>
      </div>

      <div className="grid-cards">
        {networks.map((n) => (
          <article key={n.id} className="network-card">
            <div className="network-card-head">
              <div className="card-label">Network</div>
              <span className="network-card-title">{n.id}</span>
              <div className="network-card-meta">
                {n.filename}
                <span className="dot-sep">·</span>
                {n.messages.length} message{n.messages.length === 1 ? "" : "s"}
              </div>
            </div>
            <div className="network-card-actions">
              <Link className="ecu-chip" to={`/n/${encodeURIComponent(n.id)}`}>
                DBC viewer
              </Link>
              <Link className="ecu-chip" to={`/summary/${encodeURIComponent(n.id)}`}>
                Summary
              </Link>
            </div>
          </article>
        ))}
      </div>
      {networks.length === 0 && <div className="empty-state">No networks match your filter.</div>}
    </div>
  );
}
