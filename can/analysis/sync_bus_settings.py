"""Synchronize topology bus settings into DBC attributes without changing signals.

Run from the repository root: python can/analysis/sync_bus_settings.py
All proposed updates are validated in memory before any file is written.
"""

from copy import deepcopy
from pathlib import Path

import yaml
from cantools.database.can.attribute import Attribute
from contracts import load_databases
from topology import StrictLoader, load_topology

ROOT = Path(__file__).resolve().parents[1]


def synchronize(path=ROOT / "topology.yaml"):
    """Topology manifest -> checked DBC bus/format metadata; return changed paths."""
    path = Path(path)
    databases = load_databases(path.parent / "dbc")
    config = yaml.load(path.read_text(), Loader=StrictLoader)
    # Reuse the canonical enum definitions, including CAN-FD frame-format values.
    definitions = deepcopy(databases["CarCAN"].dbc.attribute_definitions)
    for bus, spec in config["buses"].items():
        database = databases[bus]
        fd = spec["format"] == "can_fd"
        for key in ("Baudrate", "DataBaudrate", "CANFD_BRS", "VFrameFormat"):
            database.dbc.attribute_definitions[key] = deepcopy(definitions[key])
        for key, value in (
            ("Baudrate", spec["bitrate"]),
            ("BusType", "CAN FD" if fd else "CAN"),
        ):
            database.dbc.attributes[key] = Attribute(
                value, database.dbc.attribute_definitions[key]
            )
        if fd:
            database.dbc.attributes["DataBaudrate"] = Attribute(
                spec.get("data_bitrate", spec["bitrate"]),
                database.dbc.attribute_definitions["DataBaudrate"],
            )
        else:
            database.dbc.attributes.pop("DataBaudrate", None)
        database.dbc.attribute_definitions["CANFD_BRS"].default_value = "0"
        for message in database.messages:
            if message.length > (64 if fd else 8):
                raise ValueError(
                    f"{bus}/{message.name}: cannot fit selected frame format"
                )
            message.is_fd = fd
            for key, value in (
                (
                    "VFrameFormat",
                    (15 if message.is_extended_frame else 14)
                    if fd
                    else (1 if message.is_extended_frame else 0),
                ),
                ("CANFD_BRS", int(spec.get("brs", False))),
            ):
                message.dbc.attributes[key] = Attribute(
                    value, database.dbc.attribute_definitions[key]
                )
        database.refresh()
    load_topology(path, databases)
    changed = []
    for bus, database in databases.items():
        output = path.parent / config["buses"][bus]["dbc"]
        content = database.as_dbc_string(
            sort_signals=lambda signals: sorted(
                signals, key=lambda signal: signal.start
            )
        )
        content = (
            "\n".join(line.rstrip() for line in content.splitlines()).rstrip() + "\n"
        )
        if output.read_text() != content:
            output.write_text(content)
            changed.append(output)
    return changed


if __name__ == "__main__":
    print(f"Synchronized {len(synchronize())} DBC files from topology.yaml")
