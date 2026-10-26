"""Analytical/reference checks for schedules, forwarding and network rate estimates."""

import sys
import unittest
from argparse import Namespace
from copy import deepcopy
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "analysis"))
from calculate_load import Traffic, calculate, merge, tcp_bandwidth, traffic
from contracts import (
    attribute,
    load_databases,
    send_type,
    validate_timing,
)
from frames import frame_time
from topology import load_topology, validate


def options():
    return Namespace(
        record_header_bytes=10,
        batch_header_bytes=20,
        batch_ms=20,
        mtu=1500,
        ip_header_bytes=20,
        tcp_header_bytes=20,
        ack_every=2,
        tls_record_bytes=0,
    )


class LoadAnalysisTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.databases = load_databases(ROOT / "dbc")
        cls.topology = load_topology(ROOT / "topology.yaml", cls.databases)

    def test_physical_buses(self):
        rates = {
            "CarCAN": 1000000,
            "DAqCAN": 1000000,
            "ChargerCAN": 250000,
            "ElconCAN": 250000,
            "LightingCAN": 250000,
            "MotorCAN": 250000,
            "MPPTCAN": 1000000,
            "SteeringCAN": 500000,
        }
        for bus, db in self.databases.items():
            self.assertEqual(attribute(db, "Baudrate"), rates[bus])
            fd = bus in ("CarCAN", "DAqCAN", "ChargerCAN")
            self.assertEqual(attribute(db, "BusType"), "CAN FD" if fd else "CAN")
            for message in db.messages:
                self.assertEqual(message.is_fd, fd, (bus, message.name))
                self.assertEqual(attribute(message, "CANFD_BRS"), "0")
                self.assertLessEqual(message.length, 64 if fd else 8)

    def test_strict_schedule_and_selected_csv_values(self):
        car = self.databases["CarCAN"]
        self.assertEqual(car.get_message_by_name("BPSStatus").cycle_time, 200)
        self.assertEqual(car.get_message_by_name("PedalsStatus").cycle_time, 100)
        self.assertEqual(car.get_message_by_name("BPSPackData").cycle_time, 50)
        self.assertEqual(
            self.databases["DAqCAN"].get_message_by_name("TimestampSync").cycle_time, 10
        )
        for message in car.messages:
            validate_timing(message)
        bad = deepcopy(car.get_message_by_name("BPSStatus"))
        del bad.dbc.attributes["GenMsgCycleTime"]
        with self.assertRaisesRegex(ValueError, "explicit positive"):
            validate_timing(bad)
        bad = deepcopy(car.get_message_by_name("BPSCommand"))
        self.assertEqual(send_type(bad), "Triggered")
        bad.cycle_time = 10
        with self.assertRaisesRegex(ValueError, "cannot have"):
            validate_timing(bad)
        for prefix in ("MPPTA", "MPPTB", "MPPTC"):
            for suffix, period in (
                ("PowerMeasurements", 500),
                ("Status", 1000),
                ("SetMode", 100),
                ("SetOutputVoltageLimit", 200),
                ("SetOutputCurrentLimit", 200),
            ):
                self.assertEqual(
                    self.databases["MPPTCAN"]
                    .get_message_by_name(prefix + suffix)
                    .cycle_time,
                    period,
                )
        for name in ("WS22Reserved0A", "WS22Reserved0D"):
            self.assertEqual(
                send_type(self.databases["MotorCAN"].get_message_by_name(name)), "None"
            )

    def test_charger_four_identical_bps_contracts(self):
        for name in (
            "BPSStatus",
            "BPSPackData",
            "BPSVoltageData",
            "BPSTemperatureData",
        ):
            source = self.databases["CarCAN"].get_message_by_name(name)
            mirror = self.databases["ChargerCAN"].get_message_by_name(name)
            self.assertEqual(mirror.frame_id, source.frame_id)
            self.assertEqual(mirror.length, source.length)
            self.assertEqual(mirror.cycle_time, source.cycle_time)
            self.assertEqual(
                [
                    (s.name, s.start, s.length, s.scale, s.offset)
                    for s in mirror.signals
                ],
                [
                    (s.name, s.start, s.length, s.scale, s.offset)
                    for s in source.signals
                ],
            )

    def test_reference_frame_occupancy(self):
        classic = frame_time(8, False, False, 1000000)
        self.assertAlmostEqual(classic.minimum, 111e-6)
        self.assertAlmostEqual(classic.maximum, 135e-6)
        self.assertAlmostEqual(frame_time(8, True, False, 1000000).minimum, 131e-6)
        fd = frame_time(64, False, True, 1000000)
        self.assertAlmostEqual(fd.minimum, 579e-6)
        self.assertGreater(fd.maximum, fd.minimum)
        self.assertAlmostEqual(
            frame_time(64, False, True, 250000).minimum, fd.minimum * 4
        )
        self.assertLess(
            frame_time(64, False, True, 1000000, 4000000, True).minimum, fd.minimum
        )
        for length, fd in ((9, False), (10, True), (65, True)):
            with self.assertRaises(ValueError):
                frame_time(length, False, fd, 1000000)
        with self.assertRaises(ValueError):
            frame_time(8, False, True, 1000000, brs=True)

    def test_car_forwarding_and_filtered_daq_tcp(self):
        report = calculate(self.databases, self.topology, {}, options())
        # Car 84 + charger interface 10 + lighting 10 + motor 66 + steering 100
        # + MPPT power/status 9 + Elcon 3. Mirrored BPS frames add no second copy.
        self.assertEqual(report["car_can_with_forwarding"]["frames_per_second"], 282)
        self.assertEqual(report["internet"]["frames_per_second"], 1169)
        self.assertEqual(
            report["buses_with_forwarding"]["ChargerCAN"]["frames_per_second"], 53
        )
        car = report["car_can_messages"]
        tcp = report["internet_messages"]
        self.assertFalse(any(row["source_network"] == "DAqCAN" for row in car))
        self.assertFalse(any(row["name"] == "TimestampSync" for row in tcp))
        for prefix in ("MPPTA", "MPPTB", "MPPTC"):
            for suffix in ("SetMode", "SetOutputVoltageLimit", "SetOutputCurrentLimit"):
                self.assertNotIn(prefix + suffix, {row["name"] for row in car})
                self.assertNotIn(prefix + suffix, {row["name"] for row in tcp})
        self.assertEqual(
            len([row for row in tcp if row["name"] == "BPSVoltageData"]), 1
        )
        self.assertFalse(report["complete"])
        self.assertIn(
            "pending layout; not transmitted",
            {row["reason"] for row in report["unresolved_traffic"]},
        )

    def test_event_rates_and_unknown_layouts(self):
        rows, unresolved = traffic(self.databases, {("CarCAN", "SweepMPPTs"): 2})
        event = next(row for row in rows if row.message.name == "SweepMPPTs")
        self.assertEqual(event.rate, 2)
        self.assertNotIn(
            ("CarCAN", "SweepMPPTs"),
            {(row["network"], row["message"]) for row in unresolved},
        )
        for key, rate in (
            (("CarCAN", "SMDStatus"), 1),
            (("MotorCAN", "WS22Reserved0A"), 1),
            (("CarCAN", "BPSStatus"), 1),
            (("CarCAN", "typo"), 1),
            (("CarCAN", "SweepMPPTs"), -1),
            (("CarCAN", "SweepMPPTs"), float("nan")),
        ):
            with self.assertRaises(ValueError):
                traffic(self.databases, {key: rate})

    def test_tcp_hand_calculation_and_batching(self):
        message = self.databases["CarCAN"].get_message_by_name("BPSPackData")
        rows = [Traffic("CarCAN", message, 100)]  # 1200 payload bytes/second
        report = tcp_bandwidth(rows, 10, 20, 20, 1500, 20, 20, 2, 0)
        self.assertAlmostEqual(report["can_payload_kbps"], 9.6)
        # 1200 payload + 1000 record headers + 1000 batch headers bytes/s
        self.assertAlmostEqual(report["application_kbps"], 25.6)
        # 50 TCP segments/s, each with 40 IP/TCP header bytes
        self.assertAlmostEqual(report["tcp_ip_uplink_kbps"], 41.6)
        self.assertAlmostEqual(report["tcp_ip_reverse_ack_kbps"], 8)
        slower = tcp_bandwidth(rows, 10, 20, 100, 1500, 20, 20, 2, 0)
        self.assertLess(slower["tcp_ip_uplink_kbps"], report["tcp_ip_uplink_kbps"])
        with self.assertRaises(ValueError):
            tcp_bandwidth(rows, 10, 20, 20, 30, 20, 20, 2, 0)

    def test_conflicting_frames_and_bad_filters_fail(self):
        source = self.databases["CarCAN"].get_message_by_name("BPSStatus")
        clone = deepcopy(source)
        clone.signals[0].length += 1
        with self.assertRaisesRegex(ValueError, "Conflicting merged"):
            merge([Traffic("CarCAN", source, 5), Traffic("ChargerCAN", clone, 5)])
        config = deepcopy(self.topology)
        config["buses"]["DAqCAN"]["forwards"][0]["filter"]["messages"] = ["0x7FF"]
        with self.assertRaisesRegex(ValueError, "unknown/ambiguous"):
            validate(config, self.databases)


if __name__ == "__main__":
    unittest.main()
