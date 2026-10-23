"""Validate the authored 2027 bus contracts without requiring vehicle hardware."""

import json
from pathlib import Path
import struct
import unittest

import cantools

ROOT = Path(__file__).resolve().parents[1] / "dbc"
BUSES = (
    "CarCAN",
    "MotorCAN",
    "MPPTCAN",
    "SteeringCAN",
    "ChargerCAN",
    "ElconCAN",
    "TelemetryCAN",
)


class DatabaseTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.databases = {
            bus: cantools.database.load_file(ROOT / f"{bus}.dbc", strict=True)
            for bus in BUSES
        }
        cls.tables = {
            name: {int(key): value for key, value in values.items()}
            for name, values in json.loads(
                (ROOT / "value_tables.json").read_text()
            ).items()
        }

    def message(self, bus, identifier):
        return self.databases[bus].get_message_by_frame_id(identifier)

    def test_transmitters_and_lengths(self):
        """Every message has one declared transmitter and a legal wire length."""
        for bus, database in self.databases.items():
            identifiers = set()
            declared = {node.name for node in database.nodes}
            for message in database.messages:
                with self.subTest(bus=bus, message=message.name):
                    self.assertEqual(len(message.senders), 1)
                    self.assertIn(message.senders[0], declared)
                    self.assertNotEqual(message.senders[0], "Vector__XXX")
                    key = (message.frame_id, message.is_extended_frame)
                    self.assertNotIn(key, identifiers)
                    identifiers.add(key)
                    lengths = (
                        (0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64)
                        if message.is_fd
                        else range(9)
                    )
                    self.assertIn(message.length, lengths)
                    for signal in message.signals:
                        self.assertTrue(set(signal.receivers).issubset(declared))

    def test_shared_value_tables(self):
        """Global tables and each signal's ValueTable link match the canonical catalog."""
        for bus, database in self.databases.items():
            actual = {
                name: {int(key): str(value) for key, value in values.items()}
                for name, values in database.dbc.value_tables.items()
            }
            self.assertEqual(actual, self.tables, bus)
            for message in database.messages:
                for signal in message.signals:
                    if signal.choices:
                        key = signal.dbc.attributes["ValueTable"].value
                        self.assertEqual(
                            {
                                value: str(label)
                                for value, label in signal.choices.items()
                            },
                            self.tables[key],
                            (bus, message.name, signal.name),
                        )

    def test_team_resolution_and_explicit_exceptions(self):
        """Standardize physical steps while preserving the user's two compact encodings."""
        for bus in ("CarCAN", "ChargerCAN", "TelemetryCAN"):
            for message in self.databases[bus].messages:
                for signal in message.signals:
                    if signal.unit in ("V", "A", "rpm"):
                        self.assertEqual(
                            signal.scale, {"V": 0.05, "A": 0.01, "rpm": 5}[signal.unit]
                        )
                    if signal.unit == "mV":
                        self.assertEqual(
                            (signal.length, signal.scale, signal.offset), (12, 1, 2000)
                        )
                    if signal.unit == "degC":
                        expected = (
                            (0.5, -20) if signal.name == "SoCTemperature" else (0.05, 0)
                        )
                        self.assertEqual((signal.scale, signal.offset), expected)
        current = self.message("CarCAN", 0x60).get_signal_by_name("BatteryCurrent")
        self.assertEqual(
            (current.length, current.scale, current.offset, current.is_signed),
            (14, 0.01, -50, False),
        )
        self.assertAlmostEqual(current.maximum, 113.83)
        temperature = self.message("CarCAN", 0x82).get_signal_by_name("SoCTemperature")
        self.assertEqual(
            (temperature.length, temperature.minimum, temperature.maximum), (8, -20, 85)
        )

    def test_only_documented_cross_bus_id_repeat(self):
        """The shared BPS status is the sole duplicate ID across the seven networks."""
        seen = {}
        for bus, database in self.databases.items():
            for message in database.messages:
                key = (message.frame_id, message.is_extended_frame)
                if key in seen:
                    previous_bus, previous = seen[key]
                    self.assertEqual(key, (0x60, False))
                    self.assertEqual({previous_bus, bus}, {"CarCAN", "ChargerCAN"})
                    self.assertEqual(message.senders, previous.senders)
                    self.assertEqual(
                        [
                            (s.name, s.start, s.length, s.scale, s.offset)
                            for s in message.signals
                        ],
                        [
                            (s.name, s.start, s.length, s.scale, s.offset)
                            for s in previous.signals
                        ],
                    )
                seen[key] = (bus, message)

    def test_placeholders_are_explicit(self):
        placeholders = []
        for bus, database in self.databases.items():
            for message in database.messages:
                if message.dbc.attributes["LayoutPending"].value:
                    self.assertEqual(message.signals, [])
                    self.assertEqual(message.length, 0)
                    self.assertIn("PLACEHOLDER", message.comment)
                    placeholders.append((bus, message.frame_id))
        self.assertEqual(len(placeholders), 30)
        self.assertFalse(
            self.message("CarCAN", 0x54).dbc.attributes["LayoutPending"].value
        )
        self.assertEqual(
            self.message("CarCAN", 0x54).length, 0
        )  # Defined empty sweep trigger.

    def test_tpee_vendor_bytes_and_command_ids(self):
        """SEC-B175-7A commands use offsets 9/A/B/C and signed big-endian fixed point."""
        for label, base in [("A", 0x200), ("B", 0x220), ("C", 0x240)]:
            self.assertEqual(
                self.message("MPPTCAN", base + 9).encode({"Mode": 1}), b"\x01"
            )
            sweep = self.message("MPPTCAN", base + 0xA)
            self.assertEqual(
                sweep.encode(
                    {"StartVoltage": 30, "EndVoltage": 150, "SampleCount": 128}
                ),
                struct.pack(">hhB", 3000, 15000, 128),
            )
            self.assertEqual(
                self.message("MPPTCAN", base + 0xB).encode({"VoltageLimit": 175}),
                struct.pack(">h", 17500),
            )
            self.assertEqual(
                self.message("MPPTCAN", base + 0xC).encode({"CurrentLimit": 7}),
                struct.pack(">h", 14000),
            )
            data = self.message("MPPTCAN", base + 2)
            self.assertEqual(data.length, 5)
            self.assertEqual(
                data.encode({"SampleIndex": 3, "Current": 2.5, "Voltage": 100}),
                struct.pack(">Bhh", 3, 5000, 10000),
            )
            for offset in (5, 6, 7, 8, 9, 10, 11, 12):
                self.assertEqual(
                    self.message("MPPTCAN", base + offset).senders, ["MPPTInterface"]
                )
            self.assertEqual(self.message("MPPTCAN", base).senders, ["MPPT" + label])

    def test_ws22_vendor_bytes(self):
        drive = self.message("MotorCAN", 0x401)
        self.assertEqual(
            drive.encode(
                {"MC_MotorVelocitySetpoint": -20000, "MC_MotorCurrentSetpoint": 0.5}
            ),
            struct.pack("<ff", -20000, 0.5),
        )
        power = self.message("MotorCAN", 0x402)
        self.assertEqual(
            power.encode({"MC_MotorPowerSetpoint": 0.75}), struct.pack("<ff", 0, 0.75)
        )
        self.assertEqual(self.message("MotorCAN", 0x403).encode({}), bytes(8))
        self.assertTrue(drive.get_signal_by_name("MC_MotorVelocitySetpoint").is_float)
        self.assertFalse(drive.is_fd)

    def test_lws_and_elcon_vendor_bytes(self):
        self.assertEqual(self.message("SteeringCAN", 0x7C0).senders, ["DEI"])
        self.assertEqual(
            self.message("SteeringCAN", 0x7C0).encode({"Command": 3}), b"\x03\x00"
        )
        lws = self.message("SteeringCAN", 0x2B0)
        self.assertEqual(lws.senders, ["LWS"])
        self.assertEqual(
            lws.encode(
                {
                    "Angle": -12.3,
                    "AngularSpeed": 40,
                    "OK": 1,
                    "Calibrated": 1,
                    "Trimmed": 1,
                }
            ),
            struct.pack("<hBBB", -123, 10, 7, 0),
        )
        elcon = self.message("ElconCAN", 0x1806E5F4)
        self.assertTrue(elcon.is_extended_frame)
        self.assertFalse(elcon.is_fd)
        self.assertEqual(elcon.senders, ["ElconInterface"])
        self.assertEqual(
            elcon.encode(
                {"Elcon_VOL_SET": 130, "Elcon_CUR_SET": 7, "Elcon_CONTROL_FLAG": 0}
            ),
            struct.pack(">HHB", 1300, 70, 0),
        )

    def test_corrected_strain_ids_and_gps(self):
        for index in range(6):
            self.assertEqual(
                self.message("TelemetryCAN", 0x63A + index).senders,
                [f"StrainGauge{index + 1}"],
            )
        status = self.message("TelemetryCAN", 0x621)
        self.assertEqual(
            (
                status.get_signal_by_name("GPSLongitude").minimum,
                status.get_signal_by_name("GPSLongitude").maximum,
            ),
            (-180, 180),
        )
        self.assertEqual(
            (
                status.get_signal_by_name("GPSLatitude").minimum,
                status.get_signal_by_name("GPSLatitude").maximum,
            ),
            (-90, 90),
        )

    def test_every_defined_layout_roundtrips(self):
        """Exercise every active multiplexed branch, not only the non-multiplexed frames."""

        def cases(message, tree):
            result = [{}]
            for entry in tree:
                if isinstance(entry, str):
                    signal = message.get_signal_by_name(entry)
                    value = (
                        next(iter(signal.choices))
                        if signal.choices
                        else signal.minimum
                        if signal.minimum is not None
                        else 0
                    )
                    result = [dict(item, **{entry: value}) for item in result]
                else:
                    for selector, branches in entry.items():
                        result = [
                            dict(item, **{selector: value}, **branch)
                            for item in result
                            for value, subtree in branches.items()
                            for branch in cases(message, subtree)
                        ]
            return result

        for bus, database in self.databases.items():
            for message in database.messages:
                if message.dbc.attributes["LayoutPending"].value:
                    continue
                for signals in cases(message, message.signal_tree):
                    with self.subTest(bus=bus, message=message.name, signals=signals):
                        frame = message.encode(signals)
                        decoded = message.decode(frame, decode_choices=False)
                        self.assertEqual(message.encode(decoded), frame)


if __name__ == "__main__":
    unittest.main()
