import { useMemo, useState } from "react";
import { Link } from "react-router-dom";
import { allNetworks } from "../data";
import { useLayoutOutlet } from "../layoutContext";
import type { CanTopology, ForwardRoute } from "../types";

type Edge = ForwardRoute & { source: string; id: string };
type Point = { x: number; y: number };
const BOX_W = 180,
  BOX_H = 70,
  STEP_X = 290,
  STEP_Y = 104;

function rate(value: number) {
  return value >= 1_000_000
    ? `${value / 1_000_000} Mbit/s`
    : `${value / 1000} kbit/s`;
}
function selectors(edge: Edge) {
  return [
    ...(edge.filter?.messages ?? []),
    ...(edge.filter?.transmitters ?? []),
    ...(edge.filter?.networks ?? []),
  ];
}
function policy(edge: Edge) {
  const count = selectors(edge).length;
  const mode = edge.filter?.mode ?? "exclude";
  return count
    ? `${mode} · ${count} selectors`
    : mode === "exclude"
      ? "exclude · forward all"
      : "include · forward none";
}

/** Reverse distance to an output lays out arbitrary bus names without following cycles. */
function layout(topology: CanTopology, edges: Edge[]) {
  const names = [
    ...Object.keys(topology.buses),
    ...Object.keys(topology.outputs),
  ];
  const distance = new Map<string, number>(
    Object.keys(topology.outputs).map((name) => [name, 0]),
  );
  for (let pass = 0; pass < names.length; pass++) {
    for (const edge of edges) {
      const target = distance.get(edge.to);
      if (
        target !== undefined &&
        (distance.get(edge.source) ?? Infinity) > target + 1
      )
        distance.set(edge.source, target + 1);
    }
  }
  const max = Math.max(1, ...distance.values());
  const columns = new Map<number, string[]>();
  names.forEach((name) => {
    const column = max - (distance.get(name) ?? max);
    columns.set(column, [...(columns.get(column) ?? []), name]);
  });
  const height = Math.max(
    440,
    ...[...columns.values()].map((items) => items.length * STEP_Y + 90),
  );
  const positions = new Map<string, Point>();
  columns.forEach((items, column) => {
    items.sort(
      (a, b) =>
        edges.filter((e) => e.to === b || e.source === b).length -
          edges.filter((e) => e.to === a || e.source === a).length ||
        a.localeCompare(b),
    );
    items.forEach((name, index) => {
      const offset =
        index === 0 ? 0 : Math.ceil(index / 2) * (index % 2 ? 1 : -1);
      positions.set(name, {
        x: 35 + column * STEP_X,
        y: (height - BOX_H) / 2 + offset * STEP_Y,
      });
    });
  });
  return { positions, width: max * STEP_X + BOX_W + 70, height };
}

export function TopologyPage() {
  const { data, q } = useLayoutOutlet();
  const topology = data.topology;
  const [selection, setSelection] = useState({
    kind: "endpoint",
    id: "CarCAN",
  });
  const [zoom, setZoom] = useState(1);
  const networks = useMemo(() => allNetworks(data), [data]);
  const edges = useMemo(
    () =>
      Object.entries(topology?.buses ?? {}).flatMap(([source, bus]) =>
        (bus.forwards ?? []).map((route) => ({
          ...route,
          source,
          id: `${source}->${route.to}`,
        })),
      ),
    [topology],
  );
  const diagram = useMemo(
    () => (topology ? layout(topology, edges) : null),
    [topology, edges],
  );
  if (!topology || !diagram)
    return (
      <div className="empty-state">
        <h1 className="page-title">No topology on this branch</h1>
        <p>
          This branch has no <code>can/topology.yaml</code>. Choose a branch
          that defines the network topology.
        </p>
        <Link className="ecu-chip" to="/">
          Browse DBCs
        </Link>
      </div>
    );
  const selectedEdge =
    selection.kind === "route"
      ? edges.find((edge) => edge.id === selection.id)
      : undefined;
  const bus = topology.buses[selection.id];
  const output = topology.outputs[selection.id];
  const network = networks.find((item) => item.id === selection.id);
  const query = q.trim().toLowerCase();
  const edgeMatches = (edge: Edge) =>
    !query ||
    `${edge.source} ${edge.to} ${edge.via} ${policy(edge)} ${selectors(edge).join(" ")}`
      .toLowerCase()
      .includes(query);
  const endpointMatches = (name: string) =>
    !query ||
    name.toLowerCase().includes(query) ||
    edges.some(
      (edge) => (edge.source === name || edge.to === name) && edgeMatches(edge),
    );
  const messageName = (source: string, id: string) =>
    networks
      .find((item) => item.id === source)
      ?.messages.find((message) => message.frameId === Number(id))?.name;
  const describe = (edge: Edge) =>
    `${edge.source} → ${edge.to}\nBridge: ${edge.via}\n${policy(edge)}${selectors(edge).length ? `\n${selectors(edge).join(", ")}` : ""}`;
  const focusRoute = (edge: Edge) =>
    setSelection({ kind: "route", id: edge.id });
  const matchingRoutes = edges.filter(edgeMatches);

  return (
    <section className="topology-page">
      <div className="topology-heading">
        <div>
          <h1 className="page-title">Network topology</h1>
          <p className="page-sub">
            {Object.keys(topology.buses).length} buses ·{" "}
            {Object.keys(topology.outputs).length} output · {edges.length}{" "}
            directed routes · from topology.yaml
          </p>
        </div>
        <div className="topology-zoom" role="group" aria-label="Graph zoom">
          <button
            className="ecu-chip"
            onClick={() => setZoom((value) => Math.max(0.65, value - 0.15))}
            aria-label="Zoom out"
          >
            −
          </button>
          <button
            className="ecu-chip"
            onClick={() => setZoom(1)}
            aria-label="Reset zoom"
          >
            {Math.round(zoom * 100)}%
          </button>
          <button
            className="ecu-chip"
            onClick={() => setZoom((value) => Math.min(1.6, value + 0.15))}
            aria-label="Zoom in"
          >
            +
          </button>
        </div>
      </div>
      <div className="topology-legend">
        <span>
          <i className="legend-dot fd" /> CAN-FD
        </span>
        <span>
          <i className="legend-dot classic" /> Classic CAN
        </span>
        <span>
          <i className="legend-dot output" /> TCP output
        </span>
        <span className="muted-sm">
          Arrows show forwarding. Select a bus or route for details.
        </span>
      </div>
      {query && (
        <p className="page-sub" role="status">
          {matchingRoutes.length} matching routes; unrelated connections are
          dimmed.
        </p>
      )}
      <div className="topology-workspace">
        <div className="topology-canvas" aria-label="Scrollable topology graph">
          <svg
            viewBox={`0 0 ${diagram.width} ${diagram.height}`}
            style={{
              width: `${Math.max(800, diagram.width) * zoom}px`,
              height: `${diagram.height * zoom}px`,
            }}
            role="group"
            aria-label="Directed CAN network graph"
          >
            <defs>
              {["include", "exclude"].map((mode) => (
                <marker
                  id={`arrow-${mode}`}
                  key={mode}
                  viewBox="0 0 10 10"
                  refX="9"
                  refY="5"
                  markerWidth="7"
                  markerHeight="7"
                  orient="auto-start-reverse"
                >
                  <path
                    d="M 0 0 L 10 5 L 0 10 z"
                    fill={mode === "include" ? "#fbbf24" : "#5eead4"}
                  />
                </marker>
              ))}
            </defs>
            {edges.map((edge) => {
              const start = diagram.positions.get(edge.source)!,
                end = diagram.positions.get(edge.to)!;
              const direction = end.x >= start.x ? 1 : -1;
              const x1 = start.x + (direction > 0 ? BOX_W : 0),
                x2 = end.x + (direction > 0 ? 0 : BOX_W);
              const reciprocal = edges.some(
                (other) => other.source === edge.to && other.to === edge.source,
              );
              const bend = reciprocal ? (direction > 0 ? -38 : 38) : 0;
              const y1 =
                start.y +
                BOX_H / 2 +
                (reciprocal ? (direction > 0 ? -14 : 14) : 0);
              const y2 =
                end.y +
                BOX_H / 2 +
                (reciprocal ? (direction > 0 ? -14 : 14) : 0);
              const path = `M${x1},${y1} C${x1 + direction * 65},${y1 + bend} ${x2 - direction * 65},${y2 + bend} ${x2},${y2}`;
              const mode = edge.filter?.mode ?? "exclude";
              const active =
                selection.kind === "route"
                  ? selection.id === edge.id
                  : selection.id === edge.source || selection.id === edge.to;
              return (
                <g
                  key={edge.id}
                  className={`topology-edge ${mode}${active ? " active" : ""}${!edgeMatches(edge) ? " dimmed" : ""}`}
                >
                  <path
                    className="edge-line"
                    d={path}
                    markerEnd={`url(#arrow-${mode})`}
                  />
                  <path
                    className="edge-hit"
                    d={path}
                    role="button"
                    tabIndex={0}
                    aria-label={`${edge.source} to ${edge.to} via ${edge.via}, ${policy(edge)}`}
                    onClick={() => focusRoute(edge)}
                    onKeyDown={(event) => {
                      if (event.key === "Enter" || event.key === " ") {
                        event.preventDefault();
                        focusRoute(edge);
                      }
                    }}
                  >
                    <title>{describe(edge)}</title>
                  </path>
                </g>
              );
            })}
            {[...diagram.positions].map(([name, point]) => {
              const spec = topology.buses[name],
                sink = topology.outputs[name];
              const kind = spec
                ? spec.format === "can_fd"
                  ? "fd"
                  : "classic"
                : "output";
              const metadata = spec
                ? `${spec.format === "can_fd" ? "CAN-FD" : "Classic CAN"} · ${rate(spec.bitrate)}`
                : `${sink.transport.toUpperCase()} output`;
              return (
                <g
                  key={name}
                  transform={`translate(${point.x},${point.y})`}
                  className={`topology-node ${kind}${selection.kind === "endpoint" && selection.id === name ? " active" : ""}${!endpointMatches(name) ? " dimmed" : ""}`}
                  role="button"
                  tabIndex={0}
                  aria-label={`${name}, ${metadata}`}
                  onClick={() => setSelection({ kind: "endpoint", id: name })}
                  onKeyDown={(event) => {
                    if (event.key === "Enter" || event.key === " ") {
                      event.preventDefault();
                      setSelection({ kind: "endpoint", id: name });
                    }
                  }}
                >
                  <title>
                    {name}\n{metadata}
                    {spec?.format === "can_fd"
                      ? `\nBRS ${spec.brs ? "on" : "off"}`
                      : ""}
                  </title>
                  <rect width={BOX_W} height={BOX_H} rx="8" />
                  <text x="14" y="25" className="node-name">
                    {name}
                  </text>
                  <text x="14" y="43">
                    {metadata}
                  </text>
                  <text x="14" y="58" className="node-sub">
                    {spec?.format === "can_fd"
                      ? `BRS ${spec.brs ? "on" : "off"}`
                      : spec
                        ? "Native classic frames"
                        : "CarCAN + filtered DAqCAN"}
                  </text>
                </g>
              );
            })}
          </svg>
        </div>
        <aside
          className="topology-inspector"
          aria-label="Topology details"
          aria-live="polite"
        >
          {selectedEdge ? (
            <>
              <div className="detail-kicker">Forwarding route</div>
              <h2>
                {selectedEdge.source} → {selectedEdge.to}
              </h2>
              <dl className="attr-dl">
                <div className="attr-row">
                  <dt>Bridge</dt>
                  <dd>{selectedEdge.via}</dd>
                </div>
                <div className="attr-row">
                  <dt>Mode</dt>
                  <dd
                    className={`filter-${selectedEdge.filter?.mode ?? "exclude"}`}
                  >
                    {policy(selectedEdge)}
                  </dd>
                </div>
              </dl>
              {!selectors(selectedEdge).length && (
                <p className="page-sub">
                  {selectedEdge.filter?.mode === "include"
                    ? "Empty include list: forward nothing."
                    : "Empty exclude list: forward all defined messages."}
                </p>
              )}
              {!!selectedEdge.filter?.messages?.length && (
                <details className="topology-details">
                  <summary>
                    Message IDs ({selectedEdge.filter.messages.length})
                  </summary>
                  <ul>
                    {selectedEdge.filter.messages.map((id) => (
                      <li key={id}>
                        <code>{id}</code>{" "}
                        <span>
                          {messageName(selectedEdge.source, id) ??
                            "Forwarded message"}
                        </span>
                      </li>
                    ))}
                  </ul>
                </details>
              )}
              {!!selectedEdge.filter?.transmitters?.length && (
                <details className="topology-details">
                  <summary>
                    Transmitters ({selectedEdge.filter.transmitters.length})
                  </summary>
                  <ul>
                    {selectedEdge.filter.transmitters.map((sender) => (
                      <li key={sender}>{sender}</li>
                    ))}
                  </ul>
                </details>
              )}
              {!!selectedEdge.filter?.networks?.length && (
                <details className="topology-details">
                  <summary>
                    Networks ({selectedEdge.filter.networks.length})
                  </summary>
                  <ul>
                    {selectedEdge.filter.networks.map((name) => (
                      <li key={name}>{name}</li>
                    ))}
                  </ul>
                </details>
              )}
              <p className="topology-note">
                Matching selectors are combined with OR. Forwarding preserves
                ID, payload and frame format; duplicate native frames are
                suppressed.
              </p>
            </>
          ) : bus || output ? (
            <>
              <div className="detail-kicker">{bus ? "CAN bus" : "Output"}</div>
              <h2>{selection.id}</h2>
              <dl className="attr-dl">
                {bus ? (
                  <>
                    <div className="attr-row">
                      <dt>Format</dt>
                      <dd>
                        {bus.format === "can_fd" ? "CAN-FD" : "Classic CAN"}
                      </dd>
                    </div>
                    <div className="attr-row">
                      <dt>Bitrate</dt>
                      <dd>{rate(bus.bitrate)}</dd>
                    </div>
                    {bus.format === "can_fd" && (
                      <div className="attr-row">
                        <dt>BRS</dt>
                        <dd>
                          {bus.brs
                            ? `On · ${rate(bus.data_bitrate ?? bus.bitrate)} data`
                            : "Off"}
                        </dd>
                      </div>
                    )}
                    <div className="attr-row">
                      <dt>DBC</dt>
                      <dd>
                        <Link
                          className="mono-link"
                          to={`/n/${encodeURIComponent(selection.id)}`}
                        >
                          {bus.dbc.split("/").pop()}
                        </Link>
                      </dd>
                    </div>
                  </>
                ) : (
                  <div className="attr-row">
                    <dt>Transport</dt>
                    <dd>{output.transport.toUpperCase()}</dd>
                  </div>
                )}
              </dl>
              {network && (
                <details className="topology-details">
                  <summary>Native nodes ({network.nodes.length})</summary>
                  <ul>
                    {network.nodes.map((node) => (
                      <li key={node.name}>{node.name}</li>
                    ))}
                  </ul>
                </details>
              )}
              <details className="topology-details" open>
                <summary>Connected routes</summary>
                <div className="route-list">
                  {edges
                    .filter(
                      (edge) =>
                        edge.source === selection.id ||
                        edge.to === selection.id,
                    )
                    .map((edge) => (
                      <button key={edge.id} onClick={() => focusRoute(edge)}>
                        <span>
                          {edge.source} → {edge.to}
                        </span>
                        <small>
                          {edge.via} · {policy(edge)}
                        </small>
                      </button>
                    ))}
                </div>
              </details>
            </>
          ) : (
            <p>Select a bus or arrow to inspect it.</p>
          )}
        </aside>
      </div>
      <details className="topology-route-table">
        <summary>All routes ({edges.length})</summary>
        <div className="table-scroll">
          <table className="data data-compact">
            <thead>
              <tr>
                <th>From</th>
                <th>To</th>
                <th>Bridge</th>
                <th>Filter</th>
                <th>Details</th>
              </tr>
            </thead>
            <tbody>
              {matchingRoutes.map((edge) => (
                <tr key={edge.id}>
                  <td>{edge.source}</td>
                  <td>{edge.to}</td>
                  <td>{edge.via}</td>
                  <td>{policy(edge)}</td>
                  <td>
                    <button
                      className="ecu-chip"
                      onClick={() => focusRoute(edge)}
                    >
                      Inspect route
                    </button>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </details>
    </section>
  );
}
