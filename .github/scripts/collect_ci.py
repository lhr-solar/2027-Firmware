#!/usr/bin/env python3

# Aggregate the other CI workflows into one pass/fail result.

import json
import os
import sys
import time
import urllib.error
import urllib.request

API = "https://api.github.com"

# a finished run with one of these conclusions is not a failure
OK = {"success", "skipped", "neutral"}


def env(name: str, default: str | None = None) -> str:
    value = os.environ.get(name) or default
    if value is None:
        sys.exit(f"error: {name} is not set")
    return value


def get(url: str, token: str) -> dict:
    req = urllib.request.Request(url, headers={
        "Accept": "application/vnd.github+json",
        "Authorization": f"Bearer {token}",
        "X-GitHub-Api-Version": "2022-11-28",
    })
    with urllib.request.urlopen(req, timeout=30) as resp:
        return json.load(resp)


def latest_runs(repo: str, sha: str, event: str, token: str) -> dict[str, dict]:
    """Newest run per workflow filename, for the given commit."""
    url = f"{API}/repos/{repo}/actions/runs?head_sha={sha}&per_page=100"
    if event:
        url += f"&event={event}"
    runs: dict[str, dict] = {}
    for run in get(url, token).get("workflow_runs", []):
        name = os.path.basename(run.get("path", ""))
        best = runs.get(name)
        if best is None or run["run_number"] > best["run_number"]:
            runs[name] = run
    return runs


def main() -> int:
    token = env("GH_TOKEN")
    repo = env("GITHUB_REPOSITORY")
    sha = env("HEAD_SHA")
    event = os.environ.get("EVENT", "")
    wanted = [w.strip() for w in env("WORKFLOWS").splitlines() if w.strip()]
    if not wanted:
        sys.exit("error: WORKFLOWS is empty")

    deadline = time.time() + float(env("TIMEOUT_MINUTES", "120")) * 60
    grace_until = time.time() + float(env("GRACE_SECONDS", "90"))
    poll = float(env("POLL_SECONDS", "20"))

    results: dict[str, dict | None] = {}
    while True:
        try:
            runs = latest_runs(repo, sha, event, token)
        except (urllib.error.URLError, TimeoutError) as exc:
            # a blip in the API should not fail a gate that is otherwise green
            print(f"warning: {exc}; retrying", flush=True)
            runs = None

        if runs is not None:
            results = {name: runs.get(name) for name in wanted}
            waiting = [
                name for name, run in results.items()
                if (run is None and time.time() < grace_until)
                or (run is not None and run["status"] != "completed")
            ]
            if not waiting:
                break
            print(f"waiting on: {', '.join(sorted(waiting))}", flush=True)

        if time.time() >= deadline:
            print("error: timed out waiting for workflow runs", file=sys.stderr)
            break
        time.sleep(poll)

    lines = ["### CI collector", "", "| workflow | result |", "| --- | --- |"]
    failed, pending = [], []
    for name in wanted:
        run = results.get(name)
        if run is None:
            state = "not triggered"
        elif run["status"] != "completed":
            state = f"still {run['status']}"
            pending.append(name)
        else:
            state = run["conclusion"] or "unknown"
            if state not in OK:
                failed.append(name)
        link = f"[{name}]({run['html_url']})" if run else name
        lines.append(f"| {link} | {state} |")

    summary = "\n".join(lines) + "\n"
    print(summary)
    if path := os.environ.get("GITHUB_STEP_SUMMARY"):
        with open(path, "a") as f:
            f.write(summary)

    for name in failed:
        print(f"::error title=CI failed::{name} did not pass")
    for name in pending:
        print(f"::error title=CI incomplete::{name} did not finish in time")
    return 1 if failed or pending else 0


if __name__ == "__main__":
    sys.exit(main())
