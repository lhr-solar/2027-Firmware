"""Render public bus APIs and private cantools codecs as deterministic C files."""

from copy import deepcopy
from pathlib import Path
import re

from cantools.database.can import c_source
from cantools.database.conversion import BaseConversion

from conversion import assignment, integer
from dbc import FD_LENGTHS, name, nodes, wire_length
from signals import signal_spec


def comment(value) -> str:
    """DBC prose -> safe single-line C comment content."""
    return " ".join(str(value or "").replace("*/", "* /").split())


def conversions(message, packing: bool) -> list[str]:
    """Multiplexing tree -> assignments for active signals only, in selector order."""
    signals = {signal.name: signal for signal in message.signals}

    def walk(tree):
        lines = []
        for item in tree:
            if isinstance(item, str):
                lines.extend(assignment(signals[item], packing))
            else:
                for selector, branches in item.items():
                    lines.extend(assignment(signals[selector], packing))
                    lines.append(f"switch (raw.{name(selector)}) {{")
                    for value, subtree in sorted(branches.items()):
                        lines.append(f"case {integer(value)}:")
                        lines.extend("    " + line for line in walk(subtree))
                        lines.append("    break;")
                    lines.extend(["default: return CAN_INVALID;", "}"])
        return lines

    return walk(message.signal_tree)


def header(bus) -> str:
    """Checked bus -> physical message structs, sender containers and public API."""
    prefix = bus.name
    lines = [
        f"/** @file {prefix}.h",
        " * @brief Generated physical-value API. Edit the DBC, never this file.",
        " * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.",
        " * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.",
        " * Initialize instances with {0}; only active multiplexed fields are processed.",
        " */",
        "#pragma once",
        '#include "can_types.h"',
        "",
    ]
    for message in bus.database.messages:
        stem = f"{prefix}_{name(message.name)}"
        for signal in message.signals:
            if signal.choices:
                constants = [
                    (f"{stem}_{name(signal.name)}_{name(str(label))}".upper(), value)
                    for value, label in sorted(signal.choices.items())
                ]
                lines.append(
                    f"/** Named raw values for {comment(signal.name)}; unknown values remain integers. */"
                )
                if all(-(1 << 31) <= value <= (1 << 31) - 1 for _, value in constants):
                    lines += ["typedef enum {"]
                    lines += [f"    {key} = {value}," for key, value in constants]
                    lines += [f"}} {stem}_{name(signal.name)}_choice_t;"]
                else:
                    lines += [
                        f"#define {key} ({integer(value)})" for key, value in constants
                    ]
        lines += [
            f"/** {comment(message.comment) or comment(message.name)} */",
            "typedef struct {",
        ]
        for signal in message.signals:
            spec = signal_spec(signal)
            encoding = ("IEEE-754 binary32" if signal.is_float else
                        f"physical=raw*{spec.scale}+{spec.offset}; raw=[{spec.raw_min},{spec.raw_max}]")
            description = f"{signal.name}; unit={signal.unit or 'none'}; {encoding}. {signal.comment or ''}"
            lines += [
                f"    /** {comment(description)} */",
                f"    {spec.c_type} {name(signal.name)};",
            ]
        if not message.signals:
            lines.append(
                "    uint8_t reserved; /**< Empty payload placeholder; ignored by codec. */"
            )
        lines += [
            f"}} {stem}_t;",
            "",
            "/** Physical src -> frame. Nearest raw step, ties away from zero; finite inputs clamp.",
            " * Negative status preserves dst; CAN_CLAMPED still produces a sendable frame. */",
            f"can_status_t {stem}_pack(can_frame_t* dst, const {stem}_t* src);",
            "/** Matching frame -> physical dst. Checks ID, frame type, length and active signal ranges.",
            " * Negative status preserves dst; unknown value-table entries remain numeric. */",
            f"can_status_t {stem}_unpack({stem}_t* dst, const can_frame_t* src);",
            "",
        ]
    for node, messages in nodes(bus.database).items():
        lines.append("typedef struct {")
        lines.extend(
            f"    {prefix}_{name(message.name)}_t {name(message.name)};"
            for message in messages
        )
        lines += [f"}} {prefix}_{name(node)}_t;", ""]
    lines += [
        "/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */",
        "typedef struct {",
    ]
    for node, messages in nodes(bus.database).items():
        lines += ["    struct {"]
        lines.extend(
            f"        can_message_info_t {name(message.name)};" for message in messages
        )
        lines.append(f"    }} {name(node)};")
    if not bus.database.messages:
        lines.append("    uint8_t reserved;")
    lines += [
        f"}} {prefix}_metadata_t;",
        f"extern const {prefix}_metadata_t {prefix};",
        "",
    ]
    return "\n".join(lines)


def source(bus) -> str:
    """Checked bus -> wrappers that stage output locally to preserve failure atomicity."""
    prefix = bus.name
    lines = [
        f'#include "{prefix}.h"',
        f'#include "detail/{prefix}_raw.h"',
        "#include <math.h>",
        "#include <string.h>",
        "#include <float.h>",
        "_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, \"IEEE-754 binary32 is required\");",
        "",
    ]
    lines.append(f"const {prefix}_metadata_t {prefix} = {{")
    for node, messages in nodes(bus.database).items():
        lines.append(f"    .{name(node)} = {{")
        for message in messages:
            wire = wire_length(message)
            lines.append(
                f"        .{name(message.name)} = {{ .id = UINT32_C({message.frame_id}), "
                f".payload_bytes = {message.length}, .wire_bytes = {wire}, "
                f".dlc = {FD_LENGTHS.index(wire)}, .is_extended = {str(message.is_extended_frame).lower()}, "
                f".is_fd = {str(message.is_fd).lower()} }},"
            )
        lines.append("    },")
    if not bus.database.messages:
        lines.append("    .reserved = 0,")
    lines += ["};", ""]
    for message in bus.database.messages:
        stem = f"{prefix}_{name(message.name)}"
        raw = f"{prefix}_raw_{name(message.name)}"
        lines += [
            f"can_status_t {stem}_pack(can_frame_t* dst, const {stem}_t* src) {{",
            "    if (dst == NULL || src == NULL) return CAN_INVALID;",
            f"    struct {raw}_t raw = {{0}};",
            "    can_status_t status = CAN_OK;",
        ]
        lines += ["    " + line for line in conversions(message, True)]
        lines += [
            f"    can_frame_t result = {{ .id = UINT32_C({message.frame_id}),",
            f"        .len = {wire_length(message)}, .is_extended = {str(message.is_extended_frame).lower()},",
            f"        .is_fd = {str(message.is_fd).lower()} }};",
            f"    if ({raw}_pack(result.data, &raw, sizeof(result.data)) < 0) return CAN_INVALID;",
            "    *dst = result;",
            "    return status;",
            "}",
            "",
            f"can_status_t {stem}_unpack({stem}_t* dst, const can_frame_t* src) {{",
            "    if (dst == NULL || src == NULL) return CAN_INVALID;",
            f"    if (src->id != UINT32_C({message.frame_id}) || src->len != {wire_length(message)} ||",
            f"        src->is_extended != {str(message.is_extended_frame).lower()} ||",
            f"        src->is_fd != {str(message.is_fd).lower()} ||",
            "        (!src->is_fd && src->bitrate_switch)) return CAN_INVALID;",
            f"    struct {raw}_t raw = {{0}};",
            f"    if ({raw}_unpack(&raw, src->data, src->len) < 0) return CAN_INVALID;",
            f"    {stem}_t result = {{0}};",
        ]
        lines += ["    " + line for line in conversions(message, False)]
        lines += ["    *dst = result;", "    return CAN_OK;", "}", ""]
    return "\n".join(lines)


def generate(buses, output: Path) -> None:
    """Validated buses + dedicated directory -> deterministic public/private C modules.

    Render everything before writing, so invalid inputs cannot leave partial output.
    A manifest removes only files previously owned by this generator on regeneration.
    """
    files = {"can_types.h": (Path(__file__).parent / "runtime/can_types.h").read_text()}
    files["can.h"] = "#pragma once\n" + "".join(f'#include "{bus.name}.h"\n' for bus in buses)
    for bus in buses:
        prefix = bus.name
        files[f"{prefix}.h"] = header(bus)
        files[f"{prefix}.c"] = source(bus)
        raw_database = deepcopy(bus.database)
        # Physical conversion belongs to our checked wrapper. Removing physical bounds
        # also avoids backend range helpers reversing limits for negative scales.
        for message in raw_database.messages:
            for signal in message.signals:
                signal.minimum = signal.maximum = None
                if signal.is_float:
                    signal.is_signed = False
                signal.conversion = BaseConversion.factory(1, 0, signal.choices)
        raw_header, raw_source, _, _ = c_source.generate(
            raw_database,
            f"{prefix}_raw",
            f"{prefix}_raw.h",
            f"{prefix}_raw.c",
            "unused.c",
            floating_point_numbers=False,
        )
        for suffix, text in [("h", raw_header), ("c", raw_source)]:
            text = re.sub(
                r" @brief This (?:header|source) file was generated by cantools version [^\n]+",
                " @brief Generated by pinned cantools; timestamp omitted for reproducibility.",
                text,
            )
            files[f"detail/{prefix}_raw.{suffix}"] = text
    manifest = output / ".can-codegen-files"
    previous = manifest.read_text().splitlines() if manifest.exists() else []
    output.mkdir(parents=True, exist_ok=True)
    for relative, text in files.items():
        text = "\n".join(line.rstrip() for line in text.splitlines()).rstrip() + "\n"
        target = output / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        if not target.exists() or target.read_text() != text:
            target.write_text(text)
    for relative in set(previous) - files.keys():
        target = output / relative
        if target.resolve().is_relative_to(output.resolve()) and target.is_file():
            target.unlink()
    manifest.write_text("\n".join(sorted(files)) + "\n")
