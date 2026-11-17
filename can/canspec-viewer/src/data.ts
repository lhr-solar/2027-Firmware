import type { CanNetwork, CanspecPayload } from "./types";

/** Exact relative URL the UI fetches. JSON must sit next to index.html. */
export const CANSPEC_DATA_FILENAME = "canspec-data.json";

export function canspecDataUrl(): string {
  const base = import.meta.env.BASE_URL;
  if (base === "./" || base === ".") {
    return `./${CANSPEC_DATA_FILENAME}`;
  }
  const withSlash = base.endsWith("/") ? base : `${base}/`;
  return `${withSlash}${CANSPEC_DATA_FILENAME}`;
}

export class CanspecDataError extends Error {
  readonly kind: "missing" | "invalid" | "other";

  constructor(kind: CanspecDataError["kind"], message: string) {
    super(message);
    this.name = "CanspecDataError";
    this.kind = kind;
  }
}

export async function loadCanspecData(
  filename = CANSPEC_DATA_FILENAME,
): Promise<CanspecPayload> {
  const url = new URL(filename, new URL(canspecDataUrl(), document.baseURI));
  let res: Response;
  try {
    res = await fetch(url);
  } catch {
    throw new CanspecDataError(
      "missing",
      `Could not fetch ${CANSPEC_DATA_FILENAME} (network/file error).`,
    );
  }
  if (res.status === 404) {
    throw new CanspecDataError(
      "missing",
      `${CANSPEC_DATA_FILENAME} was not found.`,
    );
  }
  if (!res.ok) {
    throw new CanspecDataError(
      "other",
      `Failed to load CAN data (${res.status}).`,
    );
  }
  try {
    const payload = (await res.json()) as CanspecPayload;
    if (
      !Array.isArray(payload.vehicles) ||
      payload.vehicles.some((v) => !Array.isArray(v.networks))
    ) {
      throw new Error("Invalid CAN data shape");
    }
    return payload;
  } catch {
    throw new CanspecDataError(
      "invalid",
      `${CANSPEC_DATA_FILENAME} is not valid JSON.`,
    );
  }
}

/** Flatten vehicle wrappers — UI is network-first (single vehicle in practice). */
export function allNetworks(data: CanspecPayload): CanNetwork[] {
  const byId = new Map<string, CanNetwork>();
  for (const v of data.vehicles) {
    for (const n of v.networks) {
      byId.set(n.id, n);
    }
  }
  return [...byId.values()].sort((a, b) => a.id.localeCompare(b.id));
}

export function findNetworkById(
  data: CanspecPayload,
  networkId: string,
): CanNetwork | undefined {
  return allNetworks(data).find((n) => n.id === networkId);
}
