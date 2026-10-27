import { useLayoutOutlet } from "../layoutContext";
import { calculateLoading, loadPercent } from "../loading";
import type { Traffic } from "../loading";

function Inventory({ rows }: { rows: Traffic[] }) {
  return (
    <details>
      <summary>
        {rows.length} cyclic messages ·{" "}
        {rows.reduce((sum, row) => sum + row.fps, 0).toFixed(1)} frames/s
      </summary>
      <div className="table-scroll">
        <table>
          <thead>
            <tr>
              <th>ID</th>
              <th>Message</th>
              <th>Source bus</th>
              <th>Frames/s</th>
            </tr>
          </thead>
          <tbody>
            {[...rows]
              .sort((a, b) => a.message.frameId - b.message.frameId)
              .map((row) => (
                <tr key={`${row.message.isExtended}:${row.message.frameId}`}>
                  <td>{row.message.hexId}</td>
                  <td>{row.message.name}</td>
                  <td>{row.source}</td>
                  <td>{row.fps.toFixed(1)}</td>
                </tr>
              ))}
          </tbody>
        </table>
      </div>
    </details>
  );
}

export function LoadingPage() {
  const { data, q } = useLayoutOutlet();
  let report;
  try {
    report = calculateLoading(data);
  } catch (error) {
    return (
      <div>
        <h1 className="page-title">Bus loading</h1>
        <p className="err">{String(error)}</p>
      </div>
    );
  }
  const { topology, resolved, native, omitted } = report;
  return (
    <div>
      <h1 className="page-title">Bus loading</h1>
      <p className="page-sub">
        Cyclic traffic, including every permitted forwarding hop. Blue =
        alternative bitrate; orange = assigned bitrate. Bars show conservative
        stuffing; labels show unstuffed–conservative bounds.
      </p>
      <p className="page-sub">
        Identical mirrored IDs count once per bus/output. Triggered messages and
        pending layouts are excluded; errors and retries are additional.
      </p>
      <div className="loading-grid">
        {Object.entries(topology.buses)
          .filter(([name]) => name.toLowerCase().includes(q.toLowerCase()))
          .map(([name, bus]) => {
            const rows = [...resolved.get(name)!.values()];
            const rates = [
              ...new Set([125000, 250000, 500000, 1000000, bus.bitrate]),
            ].sort((a, b) => a - b);
            return (
              <article className="network-card" key={name}>
                <h2>{name}</h2>
                <p className="page-sub">
                  {bus.format === "can_fd" ? "CAN FD · BRS off" : "Classic CAN"}{" "}
                  · {native.get(name)!.length} native +{" "}
                  {rows.length - native.get(name)!.length} forwarded
                </p>
                <div
                  className="load-chart"
                  role="group"
                  aria-label={`${name} bus loading by bitrate`}
                >
                  {rates.map((rate) => {
                    const [low, high] = loadPercent(rows, rate);
                    return (
                      <div
                        className={`load-row${rate === bus.bitrate ? " assigned" : ""}`}
                        key={rate}
                      >
                        <span>
                          {rate >= 1000000 ? `${rate / 1000000}M` : `${rate / 1000}k`}{rate === bus.bitrate ? " ★" : ""}
                        </span>
                        <div className="load-track">
                          <div
                            className={`load-bar${high > 100 ? " overloaded" : ""}`}
                            style={{ width: `${Math.min(high, 100)}%` }}
                          />
                        </div>
                        <strong className={high > 100 ? "load-over" : ""}>
                          {low.toFixed(1)}–{high.toFixed(1)}%
                        </strong>
                      </div>
                    );
                  })}
                  <div className="load-axis">
                    Shared scale: 0–100% · stripes = over capacity
                  </div>
                </div>
                <Inventory rows={rows} />
              </article>
            );
          })}
      </div>
      {Object.entries(topology.outputs)
        .filter(([, output]) => output.transport === "tcp")
        .map(([name]) => {
          const rows = [...resolved.get(name)!.values()],
            payloadKbps = rows.reduce((sum, row) => sum + row.fps * row.message.dlc * 8 / 1000, 0);
          return (
            <article className="network-card tcp-card" key={name}>
              <h2>{name}</h2>
              <div className="tcp-rate">
                {payloadKbps.toFixed(2)} <small>kbps CAN payload</small>
              </div>
              <p className="page-sub">
                Message data only, including all permitted upstream forwarding paths.
              </p>
              <Inventory rows={rows} />
            </article>
          );
        })}
      <details className="loading-omitted">
        <summary>
          {omitted.length} excluded triggered / undefined messages
        </summary>
        <ul>
          {omitted.map((item) => (
            <li key={item}>{item}</li>
          ))}
        </ul>
      </details>
    </div>
  );
}
