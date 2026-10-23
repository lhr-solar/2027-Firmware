"""Parse bus contracts and reject ambiguous names/layouts before emitting C."""

from dataclasses import dataclass
from pathlib import Path
import re

import cantools
from cantools.database.can.c_source import camel_to_snake_case

from signals import signal_spec

KEYWORDS = set(
    "auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while _Alignas _Alignof _Atomic _Bool _Complex _Generic _Imaginary _Noreturn _Static_assert _Thread_local".split()
)
FD_LENGTHS = (0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64)


def name(value: str) -> str:
    """DBC identifier -> readable C identifier; reject reserved/invalid output."""
    result = camel_to_snake_case(value)
    if not re.fullmatch(r"[a-z][a-z0-9_]*", result) or result in KEYWORDS:
        raise ValueError(f"{value!r}: unsupported C identifier {result!r}")
    return result


def unique(values, context: str) -> None:
    """Reject normalized collisions so generated identifiers cannot shadow fields."""
    seen = set()
    for value in values:
        identifier = name(value)
        if identifier in seen:
            raise ValueError(f"{context}: duplicate C identifier {identifier}")
        seen.add(identifier)


def wire_length(message) -> int:
    """Payload bytes -> legal transport length; added FD padding is zeroed."""
    return next(length for length in FD_LENGTHS if length >= message.length)


def nodes(database) -> dict:
    """Database -> sender groups; multi-sender messages appear in each group."""
    groups = {}
    for message in database.messages:
        for sender in message.senders or ["Unassigned"]:
            groups.setdefault(sender, []).append(message)
    return dict(sorted(groups.items()))


@dataclass
class Bus:
    """Normalized bus namespace plus parsed cantools database."""

    name: str
    database: object


def load_buses(paths: list[Path]) -> list[Bus]:
    """Paths -> checked bus models; zero-byte scaffold DBCs become empty buses."""
    unique((path.stem for path in paths), "buses")
    buses = []
    for path in sorted(paths):
        try:
            database = (
                cantools.database.load_file(path, strict=True)
                if path.stat().st_size
                else cantools.database.Database()
            )
            database.messages.sort(
                key=lambda message: (message.frame_id, message.is_extended_frame)
            )
            unique((message.name for message in database.messages), str(path))
            unique(nodes(database), f"{path}: nodes")
            ids = set()
            for message in database.messages:
                key = (message.frame_id, message.is_extended_frame)
                if key in ids:
                    raise ValueError(f"duplicate frame ID {key}")
                ids.add(key)
                if not 0 <= message.length <= (64 if message.is_fd else 8):
                    raise ValueError(
                        f"{message.name}: invalid payload size or missing FD flag"
                    )
                unique((signal.name for signal in message.signals), message.name)
                for signal in message.signals:
                    spec = signal_spec(signal)
                    if signal.choices:
                        unique(
                            (str(choice) for choice in signal.choices.values()),
                            signal.name,
                        )
                        if any(
                            not spec.raw_min <= value <= spec.raw_max
                            for value in signal.choices
                        ):
                            raise ValueError(
                                f"{signal.name}: choice outside permitted raw range"
                            )
            buses.append(Bus(name(path.stem), database))
        except (ValueError, cantools.errors.Error) as error:
            raise ValueError(f"{path}: {error}") from error
    validate_symbols(buses)
    return buses


def validate_symbols(buses: list[Bus]) -> None:
    """Reject cross-module C symbol collisions before producing uncompilable output."""
    symbols = set()

    def add(symbol):
        if symbol in symbols:
            raise ValueError(f"generated C symbol collision: {symbol}")
        symbols.add(symbol)

    for bus in buses:
        if bus.name == "can_types":
            raise ValueError(
                "bus name can_types conflicts with the shared runtime header"
            )
        add(bus.name)
        add(f"{bus.name}_metadata_t")
        for node in nodes(bus.database):
            add(f"{bus.name}_{name(node)}_t")
        for message in bus.database.messages:
            stem = f"{bus.name}_{name(message.name)}"
            for symbol in [
                f"{stem}_t",
                f"{stem}_pack",
                f"{stem}_unpack",
                f"{bus.name}_raw_{name(message.name)}_pack",
                f"{bus.name}_raw_{name(message.name)}_unpack",
                f"{bus.name}_raw_{name(message.name)}_init",
            ]:
                add(symbol)
            for signal in message.signals:
                if signal.choices:
                    add(f"{stem}_{name(signal.name)}_choice_t")
                    for choice in signal.choices.values():
                        add(f"{stem}_{name(signal.name)}_{name(str(choice))}".upper())
