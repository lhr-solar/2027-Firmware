"""Load DBC timing/physical contracts and resolve forwarding rules strictly."""

from pathlib import Path

import cantools


def attribute(owner, name, default=None):
    """DBC owner + attribute name -> assigned/default attribute value."""
    if owner.dbc is None:
        return default
    item = owner.dbc.attributes.get(name)
    definition = (
        owner.dbc.attribute_definitions.get(name)
        if hasattr(owner.dbc, "attribute_definitions")
        else None
    )
    value = (
        item.value
        if item is not None
        else definition.default_value
        if definition
        else default
    )
    if item is not None and item.definition.type_name == "ENUM":
        return item.definition.choices[int(value)]
    return value


def send_type(message):
    """Message -> Cyclic/Triggered/None; missing cyclic is the authoring default."""
    return attribute(message, "GenMsgSendType", "Cyclic")


def pending(message):
    """Pending drafts are not approved transmit layouts, including zero-DLC ones."""
    return bool(attribute(message, "LayoutPending", 0))


def validate_timing(message):
    """Reject missing cyclic periods and periods on triggered/disabled messages."""
    kind = send_type(message)
    explicit = message.dbc.attributes.get("GenMsgCycleTime")
    if kind == "Cyclic":
        if (
            explicit is None
            or not isinstance(explicit.value, int)
            or explicit.value <= 0
        ):
            raise ValueError(
                f"{message.name}: cyclic message needs explicit positive GenMsgCycleTime"
            )
        if message.cycle_time != explicit.value:
            raise ValueError(f"{message.name}: inconsistent cycle time")
    elif kind in ("Triggered", "None"):
        if explicit is not None or message.cycle_time is not None:
            raise ValueError(f"{message.name}: {kind} cannot have a cycle time")
    else:
        raise ValueError(f"{message.name}: unsupported send type {kind}")


def load_databases(root: Path):
    """Flat DBC directory -> parsed buses with validated timing and frame formats."""
    databases = {}
    for path in sorted(root.glob("*.dbc")):
        if not path.stat().st_size:
            continue
        database = cantools.database.load_file(path, strict=True)
        for message in database.messages:
            validate_timing(message)
            if message.length > (64 if message.is_fd else 8):
                raise ValueError(
                    f"{path.stem}/{message.name}: payload exceeds frame format"
                )
        databases[path.stem] = database
    return databases


def wire_signature(message):
    """Complete identity of a native/forwarded wire contract, excluding bus bitrate."""
    return (
        message.name,
        message.length,
        message.is_fd,
        message.cycle_time,
        tuple(message.senders),
        attribute(message, "CANFD_BRS", "0"),
        send_type(message),
        tuple(
            (
                signal.name,
                signal.start,
                signal.length,
                signal.byte_order,
                signal.is_signed,
                signal.is_float,
                signal.scale,
                signal.offset,
                signal.minimum,
                signal.maximum,
            )
            for signal in message.signals
        ),
    )
