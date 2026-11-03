import { useEffect, useMemo, useRef, useState } from "react";
import { allNetworks } from "../data";
import { matchesQuery } from "../search";
import type { CanspecPayload } from "../types";

/** Selected branch -> searchable ID/device/bus allocation, including shared IDs. */
export function IdPreview({
  data,
  onClose,
}: {
  data: CanspecPayload;
  onClose: () => void;
}) {
  const dialog = useRef<HTMLDialogElement>(null);
  const [query, setQuery] = useState("");
  useEffect(() => {
    const element = dialog.current;
    element?.showModal();
    return () => element?.close();
  }, []);
  const rows = useMemo(
    () =>
      allNetworks(data)
        .flatMap((bus) =>
          bus.messages.map((message) => ({ bus: bus.id, message })),
        )
        .sort(
          (a, b) =>
            a.message.frameId - b.message.frameId || a.bus.localeCompare(b.bus),
        )
        .filter(({ bus, message }) =>
          matchesQuery(
            query,
            message.hexId,
            String(message.frameId),
            message.senders.join(" "),
            bus,
            message.name,
          ),
        ),
    [data, query],
  );

  return (
    <dialog
      ref={dialog}
      className="id-preview"
      onKeyDown={(event) => {
        if (event.key === "Escape") {
          event.preventDefault();
          onClose();
        }
      }}
      onCancel={(event) => {
        event.preventDefault();
        onClose();
      }}
      onClick={(event) => {
        if (event.target === event.currentTarget) onClose();
      }}
      aria-labelledby="id-preview-title"
    >
      <div className="modal-head">
        <h2 id="id-preview-title">ID allocation</h2>
        <button
          type="button"
          className="ecu-chip"
          onClick={onClose}
          aria-label="Close ID preview"
        >
          Close ×
        </button>
      </div>
      <label className="search-field">
        <span className="sr-only">Search IDs, devices, buses or messages</span>
        <input
          autoFocus
          type="search"
          placeholder="Search ID, device, bus or message…"
          value={query}
          onChange={(event) => setQuery(event.target.value)}
        />
      </label>
      <p className="page-sub">
        {rows.length} messages · sorted numerically · shared IDs appear once per
        bus
      </p>
      <div className="modal-scroll">
        <table className="data data-compact">
          <thead>
            <tr>
              <th>ID</th>
              <th>Device (TX)</th>
              <th>Bus</th>
              <th>Message</th>
            </tr>
          </thead>
          <tbody>
            {rows.map(({ bus, message }) => (
              <tr key={`${bus}-${message.frameId}`}>
                <td className="mono">
                  <span className="hex-id">{message.hexId}</span>
                  <span className="id-dec"> / {message.frameId}</span>
                </td>
                <td>{message.senders.join(", ") || "—"}</td>
                <td>{bus}</td>
                <td>{message.name}</td>
              </tr>
            ))}
          </tbody>
        </table>
        {!rows.length && <p className="empty-state">No matching IDs.</p>}
      </div>
    </dialog>
  );
}
