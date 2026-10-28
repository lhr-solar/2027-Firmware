"""Validate and resolve directed, native-first forwarding from topology.yaml."""

import re
from pathlib import Path

import yaml
from contracts import attribute, wire_signature


class StrictLoader(yaml.SafeLoader):
    """Reject duplicate YAML keys instead of silently losing a route or setting."""

    def construct_mapping(self, node, deep=False):
        keys = [self.construct_object(key, deep=deep) for key, _ in node.value]
        if len(keys) != len(set(keys)):
            raise ValueError("Duplicate YAML key")
        return super().construct_mapping(node, deep=deep)


def fields(value, allowed, required, label):
    """Fail on missing/unknown fields so misspelled filters cannot forward all."""
    if (
        not isinstance(value, dict)
        or set(value) - set(allowed)
        or set(required) - set(value)
    ):
        raise ValueError(f"{label}: expected {required}; allowed fields {allowed}")


def selected(rule, message):
    """Hex-ID/transmitter selectors are ORed; exclude is the default filter mode."""
    match = message.frame_id in {int(value, 16) for value in rule.get("messages", [])}
    match = match or bool(set(message.senders) & set(rule.get("transmitters", [])))
    return not match if rule.get("mode", "exclude") == "exclude" else match


def merge_messages(messages):
    """Native-first definitions -> one copy per unambiguous standard/extended ID."""
    result = {}
    for message in messages:
        key = (message.frame_id, message.is_extended_frame)
        if key in result and wire_signature(result[key]) != wire_signature(message):
            raise ValueError(f"Conflicting merged frame ID {message.frame_id:#x}")
        result.setdefault(key, message)
    return list(result.values())


def resolve(config, native, merge):
    """Resolve each source frame through filtered routes, rejecting actual loops.

    Opposite bus directions are valid when their filters cannot echo a frame.
    Native entries precede forwarded arrivals; the merge callback validates and
    deduplicates identical definitions or rate-bearing rows at each endpoint.
    """
    resolved = {
        name: list(native.get(name, []))
        for name in (*config["buses"], *config["outputs"])
    }

    def forward(source, item, path):
        message = getattr(item, "message", item)
        for route in config["buses"].get(source, {}).get("forwards", []):
            if not selected(route.get("filter", {}), message):
                continue
            target = route["to"]
            if target in path:
                raise ValueError(
                    f"Forwarding loop for {message.frame_id:#x} ({message.name}): "
                    + " -> ".join((*path, target))
                )
            resolved[target].append(item)
            forward(target, item, (*path, target))

    for source in config["buses"]:
        for item in native.get(source, []):
            forward(source, item, (source,))
    return {endpoint: merge(items) for endpoint, items in resolved.items()}


def validate(config, databases):
    """Check references, hardware paths, selectors, frame compatibility and drift."""
    fields(
        config,
        ("version", "buses", "outputs"),
        ("version", "buses", "outputs"),
        "topology",
    )
    if config["version"] != 1:
        raise ValueError("Unsupported topology version")
    for name in ("buses", "outputs"):
        if not isinstance(config[name], dict):
            raise TypeError(f"{name} must be a mapping")
    if set(config["buses"]) != set(databases):
        raise ValueError("Topology buses must match the populated DBC set")
    if set(config["buses"]) & set(config["outputs"]):
        raise ValueError("Bus/output names must be distinct")
    endpoints = set(config["buses"]) | set(config["outputs"])
    for name, output in config["outputs"].items():
        fields(output, ("transport",), ("transport",), name)
        if output["transport"] != "tcp":
            raise ValueError(f"{name}: only TCP outputs are supported")
    for bus, spec in config["buses"].items():
        fields(
            spec,
            ("dbc", "format", "bitrate", "brs", "data_bitrate", "forwards"),
            ("dbc", "format", "bitrate"),
            bus,
        )
        if (
            spec["format"] not in ("classic", "can_fd")
            or type(spec["bitrate"]) is not int
            or spec["bitrate"] <= 0
        ):
            raise ValueError(f"{bus}: invalid format/bitrate")
        if type(spec.get("brs", False)) is not bool:
            raise ValueError(f"{bus}: brs must be true/false")
        if spec["format"] == "classic" and (spec.get("brs") or "data_bitrate" in spec):
            raise ValueError(f"{bus}: classic CAN has no BRS/data bitrate")
        if spec.get("brs") and (
            type(spec.get("data_bitrate")) is not int or spec["data_bitrate"] <= 0
        ):
            raise ValueError(f"{bus}: BRS requires data_bitrate")
        database = databases[bus]
        if attribute(database, "Baudrate") != spec["bitrate"]:
            raise ValueError(f"{bus}: DBC bitrate differs from topology")
        if attribute(database, "BusType") != (
            "CAN FD" if spec["format"] == "can_fd" else "CAN"
        ):
            raise ValueError(f"{bus}: DBC bus format differs from topology")
        if spec["format"] == "can_fd" and attribute(
            database, "DataBaudrate"
        ) != spec.get("data_bitrate", spec["bitrate"]):
            raise ValueError(f"{bus}: DBC data bitrate differs from topology")
        for message in database.messages:
            if message.is_fd != (spec["format"] == "can_fd") or (
                attribute(message, "CANFD_BRS", "0") == "1"
            ) != spec.get("brs", False):
                raise ValueError(
                    f"{bus}/{message.name}: native frame format/BRS differs from topology"
                )
        routes = spec.get("forwards", [])
        if not isinstance(routes, list):
            raise TypeError(f"{bus}: forwards must be a list")
        targets = set()
        for route in routes:
            fields(route, ("to", "via", "filter"), ("to", "via"), f"{bus} route")
            target = route["to"]
            bridge = route["via"]
            if target not in endpoints or target in targets:
                raise ValueError(f"{bus}: unknown/duplicate route target {target}")
            targets.add(target)
            source_nodes = {node.name for node in database.nodes}
            target_nodes = (
                {node.name for node in databases[target].nodes}
                if target in databases
                else source_nodes
            )
            if bridge not in source_nodes & target_nodes:
                raise ValueError(
                    f"{bus}->{target}: {bridge} does not connect both endpoints in the DBCs"
                )
            rule = route.get("filter", {})
            fields(rule, ("mode", "messages", "transmitters"), (), f"{bus} filter")
            if rule.get("mode", "exclude") not in ("include", "exclude"):
                raise ValueError(f"{bus}: unknown filter mode")
            for key in ("messages", "transmitters"):
                values = rule.get(key, [])
                if not isinstance(values, list) or any(
                    not isinstance(value, str) for value in values
                ):
                    raise ValueError(
                        f"{bus}: {key} must be a list of strings; quote hex IDs"
                    )
                if len(values) != len(set(values)):
                    raise ValueError(f"{bus}: duplicate {key} selector")
            for value in rule.get("messages", []):
                if (
                    not re.fullmatch(r"0x[0-9a-fA-F]{1,8}", value)
                    or int(value, 16) > 0x1FFFFFFF
                ):
                    raise ValueError(f"{bus}: invalid hex message ID {value}")
    definitions = resolve(
        config,
        {bus: database.messages for bus, database in databases.items()},
        merge_messages,
    )
    for bus, spec in config["buses"].items():
        known = definitions[bus]
        for message in known:
            if spec["format"] == "classic" and (message.is_fd or message.length > 8):
                raise ValueError(
                    f"{bus}: cannot forward FD frame {message.name} onto classic CAN"
                )
        for route in spec.get("forwards", []):
            rule = route.get("filter", {})
            for value in rule.get("messages", []):
                matches = [
                    message for message in known if message.frame_id == int(value, 16)
                ]
                if len(matches) != 1:
                    raise ValueError(f"{bus}: unknown/ambiguous message ID {value}")
            for sender in rule.get("transmitters", []):
                if sender not in {
                    sender for message in known for sender in message.senders
                }:
                    raise ValueError(f"{bus}: unknown transmitter selector {sender}")
    return config


def load_topology(path: Path, databases):
    """A safe, duplicate-key-checked YAML manifest -> validated topology."""
    config = yaml.load(path.read_text(), Loader=StrictLoader)
    fields(
        config,
        ("version", "buses", "outputs"),
        ("version", "buses", "outputs"),
        "topology",
    )
    for bus, spec in config["buses"].items():
        fields(
            spec,
            ("dbc", "format", "bitrate", "brs", "data_bitrate", "forwards"),
            ("dbc", "format", "bitrate"),
            bus,
        )
        expected = path.parent / "dbc" / f"{bus}.dbc"
        if (
            not isinstance(spec["dbc"], str)
            or (path.parent / spec["dbc"]).resolve() != expected.resolve()
        ):
            raise ValueError(f"{bus}: dbc must reference dbc/{bus}.dbc")
    return validate(config, databases)
