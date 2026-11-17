import type { CanMessage, CanNetwork } from "./types";

export function busType(network: CanNetwork): string {
  const value = network.dbcAttributes.BusType;
  return typeof value === "string" && value ? value : "Unspecified bus type";
}

export function busBitrate(network: CanNetwork): string {
  const value = network.dbcAttributes.Baudrate;
  if (typeof value !== "number" || value <= 0) return "Bitrate unspecified";
  return value >= 1_000_000
    ? `${value / 1_000_000} Mbit/s`
    : `${value / 1000} kbit/s`;
}

export function messageType(message: CanMessage): string {
  return message.sendType || (message.cycleTimeMs ? "Cyclic" : "Unspecified");
}

export function cycleTime(message: CanMessage): string {
  if (message.cycleTimeMs != null && message.cycleTimeMs > 0)
    return `${message.cycleTimeMs} ms`;
  return messageType(message) === "Triggered" ? "No cycle" : "—";
}

export function messageTiming(message: CanMessage): string {
  const kind = messageType(message);
  return kind === "Cyclic" ? `${kind} · ${cycleTime(message)}` : kind;
}
