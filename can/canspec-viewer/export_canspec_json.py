#!/usr/bin/env python3
"""Export can/dbc/*.dbc (or can/dbc/<vehicle>/*.dbc) to canspec-data.json for the CANspec UI."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any

try:
    import cantools
except ImportError:
    print("export_canspec_json: install cantools (pip install cantools)", file=sys.stderr)
    sys.exit(1)

_HERE = Path(__file__).resolve().parent
ROOT = _HERE.parent.parent
DBC_ROOT = ROOT / "can" / "dbc"
# Default: cwd/canspec-data.json — same folder as a released index.html when you cd there.
DEFAULT_OUT = Path("canspec-data.json")
DEFAULT_VEHICLE_ID = "HighNoon"


def _fmt_id(frame_id: int, is_extended: bool) -> str:
    if is_extended:
        return f"0x{frame_id:08X}"
    return f"0x{frame_id:03X}"


def _serialize_attributes(attrs: Any) -> dict[str, Any]:
    if not attrs:
        return {}
    out: dict[str, Any] = {}
    for key, attr in attrs.items():
        name = str(key)
        val = getattr(attr, "value", attr)
        if isinstance(val, (str, int, float, bool)) or val is None:
            out[name] = val
        else:
            out[name] = str(val)
    return out


def _choices_to_list(choices: Any) -> list[dict[str, Any]]:
    if not choices:
        return []
    items = []
    for k, v in choices.items():
        items.append({"value": int(k), "label": str(v)})
    items.sort(key=lambda x: x["value"])
    return items


def _conversion_to_dict(conv: Any) -> dict[str, Any]:
    t = type(conv).__name__
    d: dict[str, Any] = {"kind": t}
    if hasattr(conv, "scale"):
        d["scale"] = float(conv.scale)
    if hasattr(conv, "offset"):
        d["offset"] = float(conv.offset)
    if hasattr(conv, "is_float"):
        d["isFloat"] = bool(conv.is_float)
    ch = getattr(conv, "choices", None)
    if ch:
        d["choices"] = _choices_to_list(ch)
    return d


def _signal_to_dict(sig: Any, msg: Any) -> dict[str, Any]:
    conv = _conversion_to_dict(sig.conversion)
    sig_choices = _choices_to_list(sig.choices) if sig.choices else []
    if not sig_choices and conv.get("choices"):
        sig_choices = conv["choices"]

    if sig.is_float:
        data_type = "float"
    elif sig.is_signed:
        data_type = "signed"
    else:
        data_type = "unsigned"

    mux_ids = getattr(sig, "multiplexer_ids", None)
    mux_signal = getattr(sig, "multiplexer_signal", None)
    return {
        "name": sig.name,
        "message": msg.name,
        "startBit": int(sig.start),
        "lengthBits": int(sig.length),
        "byteOrder": str(getattr(sig, "byte_order", "little_endian")),
        "isSigned": bool(sig.is_signed),
        "isFloat": bool(sig.is_float),
        "dataType": data_type,
        "conversion": conv,
        "choices": sig_choices,
        "scale": float(sig.scale),
        "offset": float(sig.offset),
        "minimum": sig.minimum,
        "maximum": sig.maximum,
        "unit": sig.unit or "",
        "comment": (sig.comment or "") or "",
        "isMultiplexer": bool(getattr(sig, "is_multiplexer", False)),
        "multiplexerSignal": mux_signal,
        "multiplexerIds": [int(x) for x in mux_ids] if mux_ids else None,
        "spn": getattr(sig, "spn", None),
        "initial": sig.initial,
        "invalid": getattr(sig, "invalid", None),
        "rawInitial": getattr(sig, "raw_initial", None),
        "rawInvalid": getattr(sig, "raw_invalid", None),
    }


def _message_to_dict(msg: Any) -> dict[str, Any]:
    m_comment = getattr(msg, "comment", None) or ""
    signals = sorted(msg.signals, key=lambda s: (s.start, s.name))
    senders = list(msg.senders) if msg.senders else []
    m_attr = {}
    if msg.dbc and getattr(msg.dbc, "attributes", None):
        m_attr = _serialize_attributes(msg.dbc.attributes)

    return {
        "name": msg.name,
        "frameId": int(msg.frame_id),
        "hexId": _fmt_id(int(msg.frame_id), bool(msg.is_extended_frame)),
        "isExtended": bool(msg.is_extended_frame),
        "dlc": int(msg.length),
        "senders": senders,
        "cycleTimeMs": msg.cycle_time,
        "isFd": bool(msg.is_fd),
        "protocol": str(msg.protocol) if getattr(msg, "protocol", None) else None,
        "busName": msg.bus_name or "",
        "isMultiplexed": bool(getattr(msg, "is_multiplexed", False)),
        "attributes": m_attr,
        "comment": m_comment,
        "signals": [_signal_to_dict(s, msg) for s in signals],
    }


def _empty_network(dbc_path: Path) -> dict[str, Any]:
    """Blank / unreadable DBC still lists as a network with zero messages/nodes."""
    return {
        "id": dbc_path.stem,
        "filename": dbc_path.name,
        "nodes": [],
        "dbcAttributes": {},
        "valueTables": [],
        "messages": [],
    }


def _network_from_path(dbc_path: Path) -> dict[str, Any]:
    text = dbc_path.read_text(encoding="utf-8", errors="replace").strip()
    if not text:
        return _empty_network(dbc_path)
    try:
        return _network_to_dict(dbc_path)
    except Exception as e:
        print(f"warn {dbc_path.name}: {e} (exporting empty network)", file=sys.stderr)
        return _empty_network(dbc_path)


def _network_to_dict(dbc_path: Path) -> dict[str, Any]:
    db = cantools.database.load_file(dbc_path)
    nodes = [{"name": n.name} for n in db.nodes]
    dbc_attrs: dict[str, Any] = {}
    value_tables: list[dict[str, Any]] = []
    if db.dbc:
        dbc_attrs = _serialize_attributes(getattr(db.dbc, "attributes", None))
        vts = getattr(db.dbc, "value_tables", None) or {}
        for nm in sorted(vts.keys(), key=str.lower):
            value_tables.append({"name": nm, "entries": _choices_to_list(vts[nm])})

    messages = sorted(
        (_message_to_dict(m) for m in db.messages),
        key=lambda x: x["frameId"],
    )
    return {
        "id": dbc_path.stem,
        "filename": dbc_path.name,
        "nodes": nodes,
        "dbcAttributes": dbc_attrs,
        "valueTables": value_tables,
        "messages": messages,
    }


def _collect_vehicles(vehicle_id: str) -> list[dict[str, Any]]:
    flat = sorted(DBC_ROOT.glob("*.dbc"), key=lambda p: p.name.lower())
    if flat:
        return [
            {
                "id": vehicle_id,
                "networks": [_network_from_path(p) for p in flat],
            }
        ]

    vehicles: list[dict[str, Any]] = []
    for vehicle_dir in sorted(DBC_ROOT.iterdir(), key=lambda p: p.name.lower()):
        if not vehicle_dir.is_dir():
            continue
        networks = [
            _network_from_path(p)
            for p in sorted(vehicle_dir.glob("*.dbc"), key=lambda x: x.name.lower())
        ]
        if networks:
            vehicles.append({"id": vehicle_dir.name, "networks": networks})
    return vehicles


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "-o",
        "--out",
        type=Path,
        default=DEFAULT_OUT,
        help="Output JSON path (default: ./canspec-data.json — must sit next to index.html)",
    )
    parser.add_argument(
        "--vehicle-id",
        default=DEFAULT_VEHICLE_ID,
        help="Vehicle id when DBCs are flat under can/dbc (default: HighNoon)",
    )
    args = parser.parse_args()

    if not DBC_ROOT.is_dir():
        print(f"No DBC root at {DBC_ROOT}", file=sys.stderr)
        sys.exit(1)

    vehicles = _collect_vehicles(args.vehicle_id)
    if not vehicles:
        print(f"No .dbc files under {DBC_ROOT}", file=sys.stderr)
        sys.exit(1)

    out: Path = args.out
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({"vehicles": vehicles}), encoding="utf-8")
    print(f"Wrote {out}")


if __name__ == "__main__":
    main()
