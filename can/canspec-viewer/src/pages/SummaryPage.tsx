import { useMemo } from "react";
import { Link, useParams } from "react-router-dom";
import { allNetworks, findNetworkById } from "../data";
import { useLayoutOutlet } from "../layoutContext";
import { matchesQuery } from "../search";
import type { CanMessage } from "../types";

type Row = {
  networkId: string;
  msg: CanMessage;
};

function senderLabel(msg: CanMessage): string {
  if (!msg.senders.length) return "—";
  return msg.senders.join(", ");
}

export function SummaryPage() {
  const { networkId } = useParams();
  const { data, q } = useLayoutOutlet();
  const networks = useMemo(() => allNetworks(data), [data]);
  const activeNetwork = useMemo(
    () => (networkId ? findNetworkById(data, networkId) : undefined),
    [data, networkId],
  );

  const rows: Row[] = useMemo(() => {
    if (networkId && !activeNetwork) return [];
    const source = activeNetwork ? [activeNetwork] : networks;
    const out: Row[] = [];
    for (const n of source) {
      for (const msg of n.messages) {
        if (
          !matchesQuery(
            q,
            msg.hexId,
            String(msg.frameId),
            msg.name,
            senderLabel(msg),
            n.id,
            String(msg.signals.length),
          )
        ) {
          continue;
        }
        out.push({ networkId: n.id, msg });
      }
    }
    out.sort((a, b) => a.msg.frameId - b.msg.frameId || a.msg.name.localeCompare(b.msg.name));
    return out;
  }, [activeNetwork, networkId, networks, q]);

  if (networkId && !activeNetwork) {
    return <p className="err">Unknown network: {networkId}</p>;
  }

  return (
    <div>
      <h1 className="page-title">Message summary</h1>
      <p className="page-sub">
        Frame ID, sender, message name, and signal count
        {activeNetwork ? ` — ${activeNetwork.id}` : " — all networks"}.
      </p>

      <div className="summary-bus-bar" role="tablist" aria-label="Network filter">
        <Link
          className={`ecu-chip${!networkId ? " ecu-chip-on" : ""}`}
          to="/summary"
          role="tab"
          aria-selected={!networkId}
        >
          All networks
        </Link>
        {networks.map((n) => (
          <Link
            key={n.id}
            className={`ecu-chip${networkId === n.id ? " ecu-chip-on" : ""}`}
            to={`/summary/${encodeURIComponent(n.id)}`}
            role="tab"
            aria-selected={networkId === n.id}
          >
            {n.id}
          </Link>
        ))}
      </div>

      <div className="table-wrap summary-table-wrap">
        <table className="data data-compact summary-table">
          <thead>
            <tr>
              {!activeNetwork && <th>Bus</th>}
              <th>ID</th>
              <th>Sender</th>
              <th>Message</th>
              <th># Signals</th>
            </tr>
          </thead>
          <tbody>
            {rows.map(({ networkId: bus, msg }) => (
              <tr key={`${bus}-${msg.frameId}-${msg.name}`}>
                {!activeNetwork && (
                  <td>
                    <Link className="mono-link" to={`/n/${encodeURIComponent(bus)}`}>
                      {bus}
                    </Link>
                  </td>
                )}
                <td className="mono">{msg.hexId}</td>
                <td>{senderLabel(msg)}</td>
                <td>
                  <Link
                    className="mono-link"
                    to={`/n/${encodeURIComponent(bus)}`}
                    title="Open DBC viewer"
                  >
                    {msg.name}
                  </Link>
                </td>
                <td className="mono">{msg.signals.length}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
      {rows.length === 0 && <div className="empty-state">No messages match your filter.</div>}
    </div>
  );
}
