"""CAN occupancy bounds, including intermission and ISO CAN-FD CRC overhead.

Static DBCs cannot predict payload-dependent dynamic stuffing. Return an
unstuffed lower bound and a conservative maximum-stuffing upper bound instead
of claiming an exact measured utilization. Error/retry traffic is additional.
"""

from dataclasses import dataclass


@dataclass(frozen=True)
class Occupancy:
    """One frame's minimum and conservative maximum wire time in seconds."""

    minimum: float
    maximum: float


def frame_time(
    length: int,
    extended: bool,
    fd: bool,
    bitrate: int,
    data_bitrate: int | None = None,
    brs: bool = False,
) -> Occupancy:
    """Frame properties and bitrates -> occupancy bounds, including three IFS bits.

    FD BRS/CRC delimiter sample points are approximated at bit boundaries;
    bounds are for traffic planning, not arbitration response-time analysis.
    """
    if bitrate <= 0 or (data_bitrate is not None and data_bitrate <= 0):
        raise ValueError("Bitrates must be positive")
    if length not in (
        (0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64) if fd else range(9)
    ):
        raise ValueError(
            f"Invalid {'FD' if fd else 'classic CAN'} payload length: {length}"
        )
    if not fd:
        stuffable = (54 if extended else 34) + 8 * length
        minimum_bits = (67 if extended else 47) + 8 * length
        return Occupancy(
            minimum_bits / bitrate, (minimum_bits + (stuffable - 1) // 4) / bitrate
        )
    if brs and data_bitrate is None:
        raise ValueError("BRS requires an explicit data-phase bitrate")
    prefix = 36 if extended else 17  # SOF through BRS
    body = 5 + 8 * length  # ESI, DLC, payload
    crc = 17 if length <= 16 else 21
    crc_field = 4 + crc + (6 if length <= 16 else 7)  # SBC/parity + fixed stuff
    tail = 13  # CRC delimiter, ACK, EOF, intermission
    data_rate = data_bitrate if brs else bitrate
    minimum = (prefix + tail) / bitrate + (body + crc_field) / data_rate
    # Independent phase ceilings also cover a run crossing the BRS boundary.
    maximum = minimum + ((prefix - 1) // 4) / bitrate + ((body + 3) // 4) / data_rate
    return Occupancy(minimum, maximum)
