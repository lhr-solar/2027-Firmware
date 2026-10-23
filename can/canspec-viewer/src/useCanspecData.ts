import { useEffect, useState } from "react";
import { loadCanspecData } from "./data";
import type { CanspecPayload } from "./types";

export interface BranchEntry {
  name: string;
  commit: string;
  dbcHash: string;
  dataUrl: string;
}
interface BranchManifest {
  defaultBranch: string;
  branches: BranchEntry[];
  errors?: { name: string; error: string }[];
}

const local: BranchManifest = {
  defaultBranch: "local",
  branches: [
    { name: "local", commit: "", dbcHash: "", dataUrl: "canspec-data.json" },
  ],
};

/** Published branch manifest -> selectable snapshots; local exports need no manifest. */
export function useCanspecData() {
  const [manifest, setManifest] = useState<BranchManifest | null>(null);
  const [branch, setBranch] = useState(
    () => new URL(location.href).searchParams.get("branch") ?? "",
  );
  const [notice, setNotice] = useState("");
  const [data, setData] = useState<CanspecPayload | null>(null);
  const [error, setError] = useState<Error | null>(null);
  const [loading, setLoading] = useState(true);
  const [catalogError, setCatalogError] = useState("");

  useEffect(() => {
    let cancelled = false;
    const refresh = async () => {
      try {
        const response = await fetch(
          new URL("branches.json", document.baseURI),
          { cache: "no-store" },
        );
        if (!response.ok && response.status !== 404)
          throw new Error("Unable to refresh branch list.");
        const next: BranchManifest =
          response.status === 404 ? local : await response.json();
        if (
          !next.branches?.length ||
          !next.branches.some((b) => b.name === next.defaultBranch)
        ) {
          throw new Error("Invalid branch list.");
        }
        if (!cancelled) {
          setManifest(next);
          setCatalogError("");
        }
      } catch (value) {
        if (!cancelled) {
          setCatalogError(
            value instanceof Error
              ? value.message
              : "Unable to refresh branches.",
          );
          setLoading(false);
        }
      }
    };
    void refresh();
    const timer = window.setInterval(() => void refresh(), 60_000);
    return () => {
      cancelled = true;
      window.clearInterval(timer);
    };
  }, []);

  useEffect(() => {
    if (!manifest || manifest.branches.some((item) => item.name === branch))
      return;
    if (branch)
      setNotice(
        `“${branch}” is unavailable or matches ${manifest.defaultBranch}. Showing ${manifest.defaultBranch}.`,
      );
    setBranch(manifest.defaultBranch);
  }, [manifest, branch]);

  const entry = manifest?.branches.find((item) => item.name === branch);
  useEffect(() => {
    if (!entry) return;
    let cancelled = false;
    setLoading(true);
    setData(null);
    setError(null);
    loadCanspecData(entry.dataUrl)
      .then((next) => {
        if (!cancelled) setData(next);
      })
      .catch((value: unknown) => {
        if (!cancelled)
          setError(
            value instanceof Error
              ? value
              : new Error("Unable to load branch data."),
          );
      })
      .finally(() => {
        if (!cancelled) setLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, [entry?.dataUrl]);

  useEffect(() => {
    if (!branch) return;
    const url = new URL(location.href);
    if (branch === "local") url.searchParams.delete("branch");
    else url.searchParams.set("branch", branch);
    history.replaceState(history.state, "", url);
  }, [branch]);

  return {
    data,
    error,
    loading,
    branch,
    branches: manifest?.branches ?? [],
    entry,
    notice,
    catalogError,
    skipped: manifest?.errors?.map((item) => item.name) ?? [],
    selectBranch: (name: string) => {
      setNotice("");
      setBranch(name);
    },
  };
}
