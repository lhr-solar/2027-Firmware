"""Estimate native/forwarded CAN occupancy and telemetry TCP bandwidth from DBCs.

Run: python can/analysis/calculate_load.py --help
Results are bounded planning estimates; measured traffic is needed for actual
stuffing, event rates, arbitration latency, retransmissions and radio overhead.
"""

import argparse
import json
import math
from dataclasses import dataclass
from pathlib import Path

from contracts import attribute, load_databases, pending, send_type, wire_signature
from frames import frame_time
from topology import load_topology, resolve

ROOT = Path(__file__).resolve().parents[1]


@dataclass(frozen=True)
class Traffic:
    """A source message and its declared/assumed frames per second."""

    network: str
    message: object
    rate: float


def identity(message):
    """Physical frame key; preserve extended vs standard ID distinction."""
    return message.frame_id, message.is_extended_frame


def merge(rows):
    """Deduplicate shared frames; reject ambiguous same-ID messages on merged buses."""
    result = {}
    for row in rows:
        key = identity(row.message)
        if key in result:
            previous = result[key]
            if (
                wire_signature(previous.message) != wire_signature(row.message)
                or previous.rate != row.rate
            ):
                raise ValueError(
                    f"Conflicting merged ID {key}: {previous.network}/{previous.message.name}, {row.network}/{row.message.name}"
                )
        else:
            result[key] = row
    return list(result.values())


def traffic(databases, event_rates):
    """Cyclic schedules/event-rate assumptions -> active traffic and unresolved rows."""
    rows = []
    unresolved = []
    known = {(bus, m.name) for bus, db in databases.items() for m in db.messages}
    for key, rate in event_rates.items():
        if key not in known or rate < 0 or not math.isfinite(rate):
            raise ValueError(f"Invalid event-rate override: {key}={rate}")
    for bus, database in databases.items():
        for message in database.messages:
            kind = send_type(message)
            key = (bus, message.name)
            if kind == "None":
                if key in event_rates:
                    raise ValueError(f"Cannot enable reserved message: {key}")
                continue
            if pending(message):
                if key in event_rates:
                    raise ValueError(f"Cannot enable undefined layout: {key}")
                unresolved.append(
                    {
                        "network": bus,
                        "message": message.name,
                        "reason": "pending layout; not transmitted",
                    }
                )
                continue
            condition = attribute(message, "GenMsgSendCondition", "")
            if condition:
                if key in event_rates:
                    raise ValueError(f"Cannot override conditional cyclic schedule: {key}")
                unresolved.append({
                    "network": bus,
                    "message": message.name,
                    "reason": f"conditional cyclic; inactive in normal operation: {condition}",
                })
                continue
            if kind == "Cyclic":
                if key in event_rates:
                    raise ValueError(
                        f"Event rate cannot override cyclic schedule: {key}"
                    )
                rate = 1000 / message.cycle_time
            elif key in event_rates:
                rate = event_rates[key]
            else:
                unresolved.append(
                    {
                        "network": bus,
                        "message": message.name,
                        "reason": "triggered; event rate unspecified",
                    }
                )
                continue
            rows.append(Traffic(bus, message, rate))
    return rows, unresolved


def occupancy(rows, bitrate, data_bitrate):
    """Rows on a target bus -> lower/upper occupancy percentages and payload rate."""
    minimum = maximum = fps = payload = 0.0
    for row in rows:
        m = row.message
        brs = attribute(m, "CANFD_BRS", "0") == "1" if m.is_fd else False
        time = frame_time(
            m.length, m.is_extended_frame, m.is_fd, bitrate, data_bitrate, brs
        )
        minimum += row.rate * time.minimum
        maximum += row.rate * time.maximum
        fps += row.rate
        payload += row.rate * m.length * 8
    return {
        "frames_per_second": fps,
        "payload_kbps": payload / 1000,
        "minimum_load_percent": minimum * 100,
        "conservative_load_percent": maximum * 100,
    }


def tcp_bandwidth(
    rows,
    record_header_bytes,
    batch_header_bytes,
    batch_ms,
    mtu,
    ip_header_bytes,
    tcp_header_bytes,
    ack_every,
    tls_record_bytes,
):
    """Traffic -> CAN payload/application/TCP-IP rates under explicit serialization assumptions.

    Packs each batch's records into MTU-sized TCP segments. Counts a partial
    final segment per batch and reverse-path ACKs. Excludes retransmissions,
    TCP handshakes, Ethernet/cellular framing, encryption beyond supplied TLS
    overhead, keepalives and unrelated GPS/video traffic.
    """
    if min(record_header_bytes, batch_header_bytes, tls_record_bytes) < 0:
        raise ValueError("Serialization overhead must be nonnegative")
    if batch_ms <= 0 or ack_every <= 0 or ip_header_bytes < 20 or tcp_header_bytes < 20:
        raise ValueError("Invalid TCP/batching parameters")
    segment_payload = mtu - ip_header_bytes - tcp_header_bytes
    if segment_payload <= 0:
        raise ValueError("MTU must exceed IP + TCP headers")
    fps = sum(row.rate for row in rows)
    payload_bps = sum(row.rate * row.message.length for row in rows)
    if not fps:
        return {
            "frames_per_second": 0,
            "can_payload_kbps": 0,
            "application_kbps": 0,
            "tcp_ip_uplink_kbps": 0,
            "tcp_ip_reverse_ack_kbps": 0,
        }
    batches = min(1000 / batch_ms, fps)
    application_bps = (
        payload_bps + fps * record_header_bytes + batches * batch_header_bytes
    )
    encrypted_bps = application_bps + batches * tls_record_bytes
    segments_per_batch = math.ceil(encrypted_bps / batches / segment_payload)
    segments = batches * segments_per_batch
    headers = ip_header_bytes + tcp_header_bytes
    # Delayed-ACK behavior is an assumption, not a guaranteed peer behavior.
    acks = segments / ack_every
    return {
        "frames_per_second": fps,
        "can_payload_kbps": payload_bps * 8 / 1000,
        "application_kbps": application_bps * 8 / 1000,
        "tcp_ip_uplink_kbps": (encrypted_bps + segments * headers) * 8 / 1000,
        "tcp_ip_reverse_ack_kbps": acks * headers * 8 / 1000,
    }


def calculate(databases, topology, event_rates, args):
    """Validated source contracts and forwarding policy -> complete estimate report."""
    rows, unresolved = traffic(databases, event_rates)
    native = {}
    for bus, database in databases.items():
        bitrate = attribute(database, "Baudrate")
        if not isinstance(bitrate, int) or bitrate <= 0:
            raise ValueError(f"{bus}: missing positive Baudrate")
        data_rate = attribute(database, "DataBaudrate")
        native[bus] = occupancy(
            [row for row in rows if row.network == bus], bitrate, data_rate
        )
        native[bus]["bitrate"] = bitrate
        native[bus]["data_bitrate"] = data_rate
    resolved = resolve(
        topology,
        {bus: [row for row in rows if row.network == bus] for bus in databases},
        merge,
    )
    forwarded_buses = {}
    for bus, spec in topology["buses"].items():
        forwarded_buses[bus] = occupancy(
            resolved[bus], spec["bitrate"], spec.get("data_bitrate", spec["bitrate"])
        )
        forwarded_buses[bus]["native_unique_messages"] = len(
            [row for row in rows if row.network == bus]
        )
        forwarded_buses[bus]["forwarded_unique_messages"] = (
            len(resolved[bus]) - forwarded_buses[bus]["native_unique_messages"]
        )
    car_rows = resolved["CarCAN"]
    car_total = forwarded_buses["CarCAN"]
    internet = resolved["TelemetryTCP"]
    network = tcp_bandwidth(
        internet,
        args.record_header_bytes,
        args.batch_header_bytes,
        args.batch_ms,
        args.mtu,
        args.ip_header_bytes,
        args.tcp_header_bytes,
        args.ack_every,
        args.tls_record_bytes,
    )

    def inventory(items):
        return [
            {
                "source_network": row.network,
                "id": f"0x{row.message.frame_id:X}",
                "name": row.message.name,
                "frames_per_second": row.rate,
                "payload_bytes": row.message.length,
                "frame_format": "CAN FD" if row.message.is_fd else "CAN",
            }
            for row in sorted(
                items, key=lambda row: (row.message.frame_id, row.network)
            )
        ]

    return {
        "native_buses": native,
        "buses_with_forwarding": forwarded_buses,
        "car_can_messages": inventory(car_rows),
        "internet_messages": inventory(internet),
        "car_can_with_forwarding": car_total,
        "internet": network,
        "unresolved_traffic": unresolved,
        "assumptions": {
            "can_stuffing": "lower/upper bounds; no errors/retries",
            "forwarding": "preserve native frame format; deduplicate identical shared IDs",
            "tcp": {
                key: getattr(args, key)
                for key in (
                    "record_header_bytes",
                    "batch_header_bytes",
                    "batch_ms",
                    "mtu",
                    "ip_header_bytes",
                    "tcp_header_bytes",
                    "ack_every",
                    "tls_record_bytes",
                )
            },
            "scope": "CAN traffic only; excludes GPS/video and modem/link overhead",
        },
        "complete": not unresolved,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dbc-dir", type=Path, default=ROOT / "dbc")
    parser.add_argument("--topology", type=Path, default=ROOT / "topology.yaml")
    parser.add_argument(
        "--event-rates",
        type=Path,
        help='JSON mapping "Bus/Message" to expected events/second',
    )
    parser.add_argument("--json", type=Path, help="Write machine-readable report")
    parser.add_argument(
        "--record-header-bytes",
        type=int,
        default=10,
        help="Assumed packed ID/flags/length/timestamp bytes per frame (default 10)",
    )
    parser.add_argument("--batch-header-bytes", type=int, default=20)
    parser.add_argument("--batch-ms", type=float, default=20)
    parser.add_argument("--mtu", type=int, default=1500)
    parser.add_argument("--ip-header-bytes", type=int, default=20)
    parser.add_argument("--tcp-header-bytes", type=int, default=20)
    parser.add_argument("--ack-every", type=int, default=2)
    parser.add_argument(
        "--tls-record-bytes",
        type=int,
        default=0,
        help="TLS overhead per batch; zero means no TLS model",
    )
    args = parser.parse_args()
    databases = load_databases(args.dbc_dir)
    topology = load_topology(args.topology, databases)
    raw = json.loads(args.event_rates.read_text()) if args.event_rates else {}
    rates = {tuple(key.split("/", 1)): float(value) for key, value in raw.items()}
    report = calculate(databases, topology, rates, args)
    print(
        f"{'Network':<15} {'native/s':>10} {'total/s':>10} {'load lower':>12} {'load upper':>12}"
    )
    for bus, item in report["buses_with_forwarding"].items():
        native = report["native_buses"][bus]["frames_per_second"]
        print(
            f"{bus:<15} {native:10.2f} {item['frames_per_second']:10.2f} {item['minimum_load_percent']:11.2f}% {item['conservative_load_percent']:11.2f}%"
        )
    network = report["internet"]
    print(
        f"Internet CAN payload: {network['can_payload_kbps']:.2f} kbps; application: {network['application_kbps']:.2f} kbps"
    )
    print(
        f"TCP/IP uplink: {network['tcp_ip_uplink_kbps']:.2f} kbps; reverse ACKs: {network['tcp_ip_reverse_ack_kbps']:.2f} kbps"
    )
    if report["unresolved_traffic"]:
        print("PARTIAL ESTIMATE: unresolved events / pending layouts:")
        for item in report["unresolved_traffic"]:
            print(f"  {item['network']}/{item['message']}: {item['reason']}")
    for bus, item in report["buses_with_forwarding"].items():
        if item["conservative_load_percent"] > 100:
            print(f"OVER CAPACITY: {bus} exceeds 100% under conservative stuffing")
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
