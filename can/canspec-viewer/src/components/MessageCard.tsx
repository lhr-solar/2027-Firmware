import type { IdSearchFormat } from "../search";
import type { CanMessage } from "../types";

const fmt = (value: unknown): string => (value == null ? "—" : String(value));

/** Compact message -> expandable signal layout, enums and documentation. */
export function MessageCard({
  msg,
  idFormat,
  expanded = false,
}: {
  msg: CanMessage;
  idFormat: IdSearchFormat;
  expanded?: boolean;
}) {
  return (
    <details className="msg-card" open={expanded || undefined}>
      <summary className="message-summary">
        <span className="msg-card-id">
          {idFormat === "decimal" ? msg.frameId : msg.hexId}
        </span>
        <span className="msg-card-name">{msg.name}</span>
        <span className="msg-card-ecu">{msg.senders.join(", ") || "—"}</span>
        <span className="msg-badge">
          {msg.isFd ? "FD" : "CAN"}
          {msg.isExtended ? " · ext" : ""}
        </span>
        <span className="muted-sm">
          {msg.dlc} B · {msg.signals.length} signals
        </span>
      </summary>
      {msg.comment && <p className="msg-card-comment">{msg.comment}</p>}
      <div className="table-scroll">
        {msg.signals.length ? (
          <table className="data data-compact signal-table">
            <thead>
              <tr>
                <th>Signal</th>
                <th>Wire</th>
                <th>Scale / offset</th>
                <th>Range</th>
                <th>Unit</th>
              </tr>
            </thead>
            <tbody>
              {msg.signals.map((s) => (
                <tr key={s.name}>
                  <td>
                    <strong>{s.name}</strong>
                    {(s.comment ||
                      s.choices.length > 0 ||
                      s.isMultiplexer ||
                      s.multiplexerSignal ||
                      s.initial != null ||
                      s.invalid != null ||
                      s.rawInitial != null ||
                      s.rawInvalid != null) && (
                      <details className="signal-notes">
                        <summary>
                          Details
                          {s.choices.length
                            ? ` · ${s.choices.length} values`
                            : ""}
                        </summary>
                        {s.comment && <p>{s.comment}</p>}
                        {s.choices.map((c) => (
                          <span className="choice-pair" key={c.value}>
                            <span className="cv">{c.value}</span> = {c.label}
                          </span>
                        ))}
                        {s.isMultiplexer && <p>Multiplexer selector</p>}
                        {s.multiplexerSignal && (
                          <p>
                            Selector {s.multiplexerSignal}:{" "}
                            {s.multiplexerIds?.join(", ")}
                          </p>
                        )}
                        {(s.initial != null ||
                          s.invalid != null ||
                          s.rawInitial != null ||
                          s.rawInvalid != null) && (
                          <p>
                            Initial {fmt(s.initial)} · invalid {fmt(s.invalid)}{" "}
                            · raw initial {fmt(s.rawInitial)} · raw invalid{" "}
                            {fmt(s.rawInvalid)}
                          </p>
                        )}
                      </details>
                    )}
                  </td>
                  <td className="mono nowrap">
                    {s.isFloat ? "f" : s.isSigned ? "i" : "u"}
                    {s.lengthBits} · @{s.startBit} ·{" "}
                    {s.byteOrder === "little_endian" ? "LE" : "BE"}
                  </td>
                  <td className="mono">
                    {fmt(s.scale)} / {fmt(s.offset)}
                  </td>
                  <td className="mono nowrap">
                    {fmt(s.minimum)} … {fmt(s.maximum)}
                  </td>
                  <td>{s.unit || "—"}</td>
                </tr>
              ))}
            </tbody>
          </table>
        ) : (
          <p className="muted-sm">Placeholder · no signals defined.</p>
        )}
      </div>
      <details className="signal-notes">
        <summary>Message metadata</summary>
        <p>
          {msg.isExtended ? "Extended 29-bit" : "Standard 11-bit"} ID ·{" "}
          {msg.isFd ? "CAN FD" : "Classic CAN"}
          {msg.cycleTimeMs != null ? ` · cycle ${msg.cycleTimeMs} ms` : ""}
          {msg.protocol ? ` · ${msg.protocol}` : ""}
          {msg.busName ? ` · ${msg.busName}` : ""}
        </p>
        <dl className="attr-dl">
          {Object.entries(msg.attributes).map(([key, value]) => (
            <div key={key} className="attr-row">
              <dt>{key}</dt>
              <dd>{fmt(value)}</dd>
            </div>
          ))}
        </dl>
      </details>
    </details>
  );
}
