import { useEffect, useState } from "react";
import { CanspecDataError, loadCanspecData } from "./data";
import type { CanspecPayload } from "./types";

export function useCanspecData() {
  const [data, setData] = useState<CanspecPayload | null>(null);
  const [error, setError] = useState<CanspecDataError | Error | null>(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    let cancelled = false;
    loadCanspecData()
      .then((d) => {
        if (!cancelled) {
          setData(d);
          setError(null);
        }
      })
      .catch((e: unknown) => {
        if (!cancelled) {
          setError(e instanceof Error ? e : new Error("Failed to load data"));
        }
      })
      .finally(() => {
        if (!cancelled) setLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, []);

  return { data, error, loading };
}
