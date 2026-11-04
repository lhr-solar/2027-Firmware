"""Emit checked physical/raw assignments; bit layout belongs to cantools."""

from dbc import name
from signals import signal_spec


def integer(value: int) -> str:
    """Python integer -> portable C constant, including both 64-bit endpoints."""
    if value == -(1 << 63):
        return "(-INT64_C(9223372036854775807) - 1)"
    if value < 0:
        return f"(-INT64_C({-value}))"
    return f"UINT64_C({value})" if value > (1 << 63) - 1 else f"INT64_C({value})"


def floating(value) -> str:
    """Decimal/int -> round-trip double literal for generated conversion arithmetic."""
    return repr(float(value))


def raw_type(signal) -> str:
    """DBC signal -> cantools' unpacked integer storage type."""
    bits = next(bits for bits in (8, 16, 32, 64) if bits >= signal.length)
    return f"{'int' if signal.is_signed else 'uint'}{bits}_t"


def assignment(signal, packing: bool) -> list[str]:
    """Signal and direction -> atomic-wrapper conversion statements.

    Packing clamps finite physical inputs and rounds raw values with ties away
    from zero. Receiving validates raw limits; callers never get partial updates.
    """
    spec = signal_spec(signal)
    field = name(signal.name)
    source = f"src->{field}" if packing else f"raw.{field}"
    target = f"raw.{field}" if packing else f"result.{field}"
    if signal.is_float:
        if packing:
            lines = ["{", f"    float value = {source};", "    if (!isfinite(value)) return CAN_INVALID;"]
            if signal.minimum is not None:
                lines.append(f"    if (value < {floating(signal.minimum)}) {{ value = (float){floating(signal.minimum)}; status = CAN_CLAMPED; }}")
            if signal.maximum is not None:
                lines.append(f"    if (value > {floating(signal.maximum)}) {{ value = (float){floating(signal.maximum)}; status = CAN_CLAMPED; }}")
            return lines + [f"    memcpy(&{target}, &value, sizeof(value));", "}"]
        lines = ["{", "    float value;", f"    memcpy(&value, &{source}, sizeof(value));", "    if (!isfinite(value)) return CAN_INVALID;"]
        if signal.minimum is not None:
            lines.append(f"    if (value < {floating(signal.minimum)}) return CAN_RANGE;")
        if signal.maximum is not None:
            lines.append(f"    if (value > {floating(signal.maximum)}) return CAN_RANGE;")
        return lines + [f"    {target} = value;", "}"]
    if not packing:
        result = []
        bits = int(
            raw_type(signal).removeprefix("uint").removeprefix("int").removesuffix("_t")
        )
        storage_low = -(1 << (bits - 1)) if signal.is_signed else 0
        storage_high = (1 << (bits - int(signal.is_signed))) - 1
        if spec.raw_min > storage_low:
            result.append(f"if ({source} < {integer(spec.raw_min)}) return CAN_RANGE;")
        if spec.raw_max < storage_high:
            result.append(f"if ({source} > {integer(spec.raw_max)}) return CAN_RANGE;")
        expression = (
            source
            if spec.identity
            else f"((double){source} * {floating(spec.scale)} + {floating(spec.offset)})"
        )
        result.append(f"{target} = ({spec.c_type}){expression};")
        return result
    if spec.identity:
        bits = int(
            spec.c_type.removeprefix("uint").removeprefix("int").removesuffix("_t")
        )
        signed = spec.c_type.startswith("int")
        low = -(1 << (bits - 1)) if signed else 0
        high = (1 << (bits - int(signed))) - 1
        result = ["{", f"    {spec.c_type} value = {source};"]
        if spec.raw_min > low:
            result += [
                f"    if (value < {integer(spec.raw_min)}) {{",
                f"        value = ({spec.c_type}){integer(spec.raw_min)}; status = CAN_CLAMPED;",
                "    }",
            ]
        if spec.raw_max < high:
            result += [
                f"    if (value > {integer(spec.raw_max)}) {{",
                f"        value = ({spec.c_type}){integer(spec.raw_max)}; status = CAN_CLAMPED;",
                "    }",
            ]
        result += [f"    {target} = ({raw_type(signal)})value;", "}"]
        return result
    low, high = sorted(
        [
            spec.raw_min * spec.scale + spec.offset,
            spec.raw_max * spec.scale + spec.offset,
        ]
    )
    # Clamp before inverse scaling, avoiding overflow for large finite inputs.
    return [
        "{",
        f"    double value = (double){source};",
        "    if (!isfinite(value)) return CAN_INVALID;",
        f"    if (value < {floating(low)}) {{ value = {floating(low)}; status = CAN_CLAMPED; }}",
        f"    if (value > {floating(high)}) {{ value = {floating(high)}; status = CAN_CLAMPED; }}",
        f"    double encoded = round((value - {floating(spec.offset)}) / {floating(spec.scale)});",
        f"    if (encoded < {floating(spec.raw_min)}) encoded = {floating(spec.raw_min)};",
        f"    if (encoded > {floating(spec.raw_max)}) encoded = {floating(spec.raw_max)};",
        f"    {target} = ({raw_type(signal)})encoded;",
        "}",
    ]
