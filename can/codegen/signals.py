"""Signal types and representable bounds, shared by validation and C emission."""

from dataclasses import dataclass
from decimal import Decimal, ROUND_CEILING, ROUND_FLOOR, localcontext


@dataclass(frozen=True)
class SignalSpec:
    """A checked physical C type and inclusive raw limits for one DBC signal."""

    c_type: str
    raw_min: int
    raw_max: int
    scale: Decimal
    offset: Decimal
    identity: bool


def signal_spec(signal) -> SignalSpec:
    """DBC signal -> safe physical C type and raw bounds.

    Decimal endpoints avoid Python float rounding. Scaled conversions must fit
    double's exact-integer domain; identity integers preserve all 64 wire bits.
    """
    with localcontext() as context:
        context.prec = 80
        return _signal_spec(signal)


def _signal_spec(signal) -> SignalSpec:
    if signal.is_float:
        raise ValueError(f"{signal.name}: IEEE float wire signals are not supported")
    if not 1 <= signal.length <= 64:
        raise ValueError(f"{signal.name}: signal width must be 1..64")
    scale = Decimal(str(signal.scale))
    offset = Decimal(str(signal.offset))
    if not scale.is_finite() or not offset.is_finite() or scale == 0:
        raise ValueError(f"{signal.name}: scale must be finite/nonzero; offset finite")
    low = -(1 << (signal.length - 1)) if signal.is_signed else 0
    high = (1 << (signal.length - int(signal.is_signed))) - 1
    for bound, is_minimum in [(signal.minimum, True), (signal.maximum, False)]:
        if bound is None:
            continue
        bound = Decimal(str(bound))
        if not bound.is_finite():
            raise ValueError(f"{signal.name}: nonfinite physical bound")
        raw = (bound - offset) / scale
        lower = is_minimum == (scale > 0)
        if lower:
            low = max(low, int(raw.to_integral_value(rounding=ROUND_CEILING)))
        else:
            high = min(high, int(raw.to_integral_value(rounding=ROUND_FLOOR)))
    if low > high:
        raise ValueError(f"{signal.name}: no representable value within DBC limits")
    identity = scale == 1 and offset == 0
    if signal.is_multiplexer and not identity:
        raise ValueError(f"{signal.name}: multiplexors require scale 1 and offset 0")
    if signal.choices and not identity:
        raise ValueError(f"{signal.name}: value tables require scale 1 and offset 0")
    physical = sorted([Decimal(low) * scale + offset, Decimal(high) * scale + offset])
    if (
        not identity
        and max(abs(Decimal(low)), abs(Decimal(high)), *map(abs, physical)) > 2**53 - 1
    ):
        raise ValueError(
            f"{signal.name}: scaled conversion exceeds exact double integer range"
        )
    integral = (
        scale == scale.to_integral_value() and offset == offset.to_integral_value()
    )
    c_type = "double"
    if integral:
        signed = physical[0] < 0
        for bits in (8, 16, 32, 64):
            minimum = -(1 << (bits - 1)) if signed else 0
            maximum = (1 << (bits - int(signed))) - 1
            if minimum <= physical[0] and physical[1] <= maximum:
                c_type = f"{'int' if signed else 'uint'}{bits}_t"
                break
        else:
            raise ValueError(f"{signal.name}: physical range exceeds C integer types")
    return SignalSpec(c_type, low, high, scale, offset, identity)
