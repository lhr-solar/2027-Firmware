import type { CanMessage, CanspecPayload, ForwardRoute } from "./types";

export type Traffic = { source: string; message: CanMessage; fps: number };
const key = (m: CanMessage) => `${m.isExtended}:${m.frameId}`;

/** Frame inputs -> unstuffed/conservative wire bits including CRC, ACK and intermission. */
export function frameBits(m: CanMessage): [number, number] {
  if (!m.isFd) {
    if (m.dlc > 8)
      throw new Error(`${m.name}: classic CAN payload exceeds 8 bytes`);
    const bits = (m.isExtended ? 67 : 47) + 8 * m.dlc;
    return [
      bits,
      bits + Math.floor(((m.isExtended ? 54 : 34) + 8 * m.dlc - 1) / 4),
    ];
  }
  const prefix = m.isExtended ? 36 : 17;
  const body = 5 + 8 * m.dlc;
  const crc = m.dlc <= 16 ? 27 : 32;
  const bits = prefix + body + crc + 13;
  return [
    bits,
    bits + Math.floor((prefix - 1) / 4) + Math.floor((body + 3) / 4),
  ];
}

function permits(route: ForwardRoute, row: Traffic) {
  const f = route.filter;
  const match =
    (f?.messages ?? []).some((id) => Number(id) === row.message.frameId) ||
    (f?.transmitters ?? []).some((sender) =>
      row.message.senders.includes(sender),
    ) ||
    (f?.networks ?? []).includes(row.source);
  return (f?.mode ?? "exclude") === "include" ? match : !match;
}

/** DBC cyclic schedules + YAML -> transitively routed traffic, counted once per destination/ID.
 * Native mirror definitions take precedence. A per-frame path detects forwarding loops.
 */
export function calculateLoading(data: CanspecPayload) {
  const topology = data.topology;
  if (!topology)
    throw new Error(
      "This branch has no topology.yaml. Loading requires bus and forwarding settings.",
    );
  if (Object.values(topology.buses).some((bus) => bus.brs))
    throw new Error(
      "Loading currently requires BRS off; split data-phase timing is not modeled.",
    );
  const networks = data.vehicles.flatMap((v) => v.networks);
  const resolved = new Map<string, Map<string, Traffic>>();
  [...Object.keys(topology.buses), ...Object.keys(topology.outputs)].forEach(
    (name) => resolved.set(name, new Map()),
  );
  const omitted: string[] = [];
  const native = new Map<string, Traffic[]>();
  for (const [bus, spec] of Object.entries(topology.buses)) {
    const network = networks.find(
      (n) => n.filename === spec.dbc.split("/").pop(),
    );
    if (!network) throw new Error(`${bus}: DBC missing from snapshot`);
    const rows: Traffic[] = [];
    for (const message of network.messages) {
      if (Number(message.attributes.LayoutPending ?? 0)) {
        omitted.push(`${bus}/${message.name}: pending layout`);
        continue;
      }
      const type =
        message.sendType ?? (message.cycleTimeMs ? "Cyclic" : "Unspecified");
      if (type === "Disabled" || type === "None") continue;
      if (type !== "Cyclic") {
        omitted.push(`${bus}/${message.name}: ${type.toLowerCase()}`);
        continue;
      }
      if (!message.cycleTimeMs || message.cycleTimeMs <= 0)
        throw new Error(`${bus}/${message.name}: missing cycle time`);
      const row = { source: bus, message, fps: 1000 / message.cycleTimeMs };
      rows.push(row);
      resolved.get(bus)!.set(key(message), row);
    }
    native.set(bus, rows);
  }
  function visit(bus: string, row: Traffic, path: string[]) {
    if (path.includes(bus))
      throw new Error(
        `${row.message.name}: forwarding loop ${[...path, bus].join(" → ")}`,
      );
    const target = resolved.get(bus);
    if (!target) throw new Error(`Unknown route target ${bus}`);
    const previous = target.get(key(row.message));
    if (
      previous &&
      (previous.message.name !== row.message.name ||
        previous.message.dlc !== row.message.dlc ||
        previous.message.isFd !== row.message.isFd ||
        previous.fps !== row.fps)
    )
      throw new Error(`${bus}: conflicting forwarded ID ${row.message.hexId}`);
    if (topology!.buses[bus]?.format === "classic" && row.message.isFd)
      throw new Error(`${bus}: cannot carry CAN FD frame ${row.message.name}`);
    if (!previous) target.set(key(row.message), row);
    for (const route of topology!.buses[bus]?.forwards ?? []) {
      if (permits(route, row)) visit(route.to, row, [...path, bus]);
    }
  }
  for (const [bus, rows] of native) for (const row of rows) visit(bus, row, []);
  return { topology, resolved, native, omitted };
}

/** Routed rows and candidate bitrate -> wire occupancy percentage bounds. */
export function loadPercent(
  rows: Traffic[],
  bitrate: number,
): [number, number] {
  return rows.reduce<[number, number]>(
    (total, row) => {
      const bits = frameBits(row.message);
      return [
        total[0] + ((bits[0] * row.fps) / bitrate) * 100,
        total[1] + ((bits[1] * row.fps) / bitrate) * 100,
      ];
    },
    [0, 0],
  );
}

/** Routed TCP records -> kbps with explicit packed-record/batching assumptions. */
export function tcpRate(rows: Traffic[]) {
  const fps = rows.reduce((sum, row) => sum + row.fps, 0);
  const payload = rows.reduce((sum, row) => sum + row.fps * row.message.dlc, 0);
  const batches = Math.min(50, fps);
  const application = payload + fps * 10 + batches * 20;
  const segments = batches
    ? batches * Math.ceil(application / batches / 1460)
    : 0;
  return {
    payload: (payload * 8) / 1000,
    application: (application * 8) / 1000,
    uplink: ((application + segments * 40) * 8) / 1000,
    ack: ((segments / 2) * 40 * 8) / 1000,
  };
}
