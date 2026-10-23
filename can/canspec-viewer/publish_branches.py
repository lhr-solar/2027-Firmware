"""Reconcile remote DBC snapshots into a complete static viewer publication."""

import argparse
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

from export_canspec_json import _network_from_path


def git(repo: Path, *args: str) -> bytes:
    """Git arguments -> checked output; branch contents are data, never executed."""
    return subprocess.check_output(["git", "-C", str(repo), *args])


def snapshot(repo: Path, ref: str) -> dict[str, bytes]:
    """Commit/ref -> flat DBC file bytes, rejecting symlinks and submodules."""
    files = {}
    entries = git(repo, "ls-tree", "-rz", ref, "--", "can/dbc/")
    for entry in entries.split(b"\0"):
        if not entry:
            continue
        metadata, raw_path = entry.split(b"\t", 1)
        path = raw_path.decode("utf-8")
        if not path.endswith(".dbc") or len(Path(path).parts) != 3:
            continue
        mode, kind, oid = metadata.split()
        if mode not in (b"100644", b"100755") or kind != b"blob":
            raise ValueError(f"Not a regular DBC file: {path}")
        files[Path(path).name] = git(repo, "cat-file", "blob", oid.decode())
    return files


def fingerprint(files: dict[str, bytes]) -> str:
    """Sorted filenames + exact contents -> stable, filename-sensitive snapshot ID."""
    digest = hashlib.sha256()
    for name, content in sorted(files.items()):
        digest.update(name.encode() + b"\0" + str(len(content)).encode() + b"\0")
        digest.update(content)
    return digest.hexdigest()


def export(files: dict[str, bytes]) -> dict:
    """DBC bytes -> viewer data, using only this trusted checkout's exporter."""
    with tempfile.TemporaryDirectory() as directory:
        networks = []
        for name, content in sorted(files.items()):
            path = Path(directory) / name
            path.write_bytes(content)
            networks.append(_network_from_path(path))
    return {"vehicles": [{"id": "2027", "networks": networks}]}


def publish(repo: Path, output: Path, baseline: str = "main") -> dict:
    """Fetched origin branches -> main plus distinct branch DBC snapshots and manifest.

    Rebuild the managed data directory each time, so deleted/equal branches leave
    neither selector entries nor orphaned JSON. Main must parse; invalid feature
    branches are omitted with a manifest error rather than showing empty buses.
    """
    import shutil

    refs = (
        git(repo, "for-each-ref", "--format=%(refname)", "refs/remotes/origin/")
        .decode()
        .splitlines()
    )
    refs = [ref for ref in refs if not ref.endswith("/HEAD")]
    base_ref = f"refs/remotes/origin/{baseline}"
    if base_ref not in refs:
        raise ValueError(f"Missing baseline branch: {baseline}")
    base_files = snapshot(repo, base_ref)
    base_hash = fingerprint(base_files)
    data_dir = output / "data"
    if data_dir.exists():
        shutil.rmtree(data_dir)
    data_dir.mkdir(parents=True)
    branches = []
    errors = []
    for ref in sorted(refs, key=lambda ref: (ref != base_ref, ref)):
        branch = ref.removeprefix("refs/remotes/origin/")
        if branch == "gh-pages":
            continue
        try:
            files = snapshot(repo, ref)
            content_hash = fingerprint(files)
            if ref != base_ref and (not files or content_hash == base_hash):
                continue
            path = data_dir / f"{content_hash}.json"
            if not path.exists():
                payload = export(files)
                path.write_text(json.dumps(payload), encoding="utf-8")
            if ref == base_ref:
                (output / "canspec-data.json").write_bytes(path.read_bytes())
            branches.append(
                {
                    "name": branch,
                    "commit": git(repo, "rev-parse", ref).decode().strip(),
                    "dbcHash": content_hash,
                    "dataUrl": f"data/{content_hash}.json",
                }
            )
        except Exception as error:
            if ref == base_ref:
                raise
            errors.append({"name": branch, "error": str(error)})
            print(f"Skipped invalid DBC branch {branch!r}: {error}")
    manifest = {"defaultBranch": baseline, "branches": branches, "errors": errors}
    (output / "branches.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8"
    )
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path.cwd())
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--baseline", default="main")
    args = parser.parse_args()
    manifest = publish(args.repo, args.out, args.baseline)
    print(f"Published {len(manifest['branches'])} branch entries")


if __name__ == "__main__":
    main()
