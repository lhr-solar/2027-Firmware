"""Validate the authored 2027 bus contracts without requiring vehicle hardware."""

import json
import struct
import unittest
from pathlib import Path

import cantools

ROOT = Path(__file__).resolve().parents[1] / "dbc"
BUSES = (
    "CarCAN",
    "MotorCAN",
    "MPPTCAN",
    "SteeringCAN",
    "ChargerCAN",
    "ElconCAN",
    "DAqCAN",
    "LightingCAN",
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
        for bus in ("CarCAN", "ChargerCAN", "DAqCAN", "LightingCAN"):
            for message in self.databases[bus].messages:
                for signal in message.signals:
                    if signal.unit in ("V", "A", "rpm"):
                        self.assertEqual(
                            signal.scale,
                            0.5
                            if signal.name.startswith("WheelRPM")
                            else {"V": 0.05, "A": 0.01, "rpm": 5}[signal.unit],
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
        current = self.message("CarCAN", 0x70).get_signal_by_name("BatteryCurrent")
        self.assertEqual(
            (current.length, current.scale, current.offset, current.is_signed),
            (14, 0.01, -50, False),
        )
        self.assertAlmostEqual(current.maximum, 113.83)
        temperature = self.message("CarCAN", 0x82).get_signal_by_name("SoCTemperature")
        self.assertEqual(
            (temperature.length, temperature.minimum, temperature.maximum), (8, -20, 85)
        )

    def test_only_documented_shared_bps_ids_repeat(self):
        seen = {}
        for bus, database in self.databases.items():
            for message in database.messages:
                key = (message.frame_id, message.is_extended_frame)
                if key in seen:
                    previous_bus, previous = seen[key]
                    self.assertIn(
                        key,
                        ((0x60, False), (0x70, False), (0x71, False), (0x72, False)),
                    )
                    self.assertEqual({previous_bus, bus}, {"CarCAN", "ChargerCAN"})
                    self.assertEqual(message.name, previous.name)
                    self.assertEqual(message.length, previous.length)
                    self.assertEqual(message.cycle_time, previous.cycle_time)
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
        self.assertEqual(len(placeholders), 18)
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
        for identifier, location in [(0x690, "Front"), (0x691, "Rear")]:
            message = self.message("DAqCAN", identifier)
            self.assertEqual(message.senders, [f"StrainGauge{location}"])
            self.assertEqual(message.name, f"StrainGauge{location}Data")
        identifiers = {
            message.frame_id for message in self.databases["DAqCAN"].messages
        }
        self.assertFalse(identifiers.intersection(range(0x63C, 0x640)))
        status = self.message("DAqCAN", 0x621)
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

    def test_mika_camera_and_fan_fields(self):
        message = self.message("CarCAN", 0x82)
        names = {signal.name for signal in message.signals}
        self.assertIn("WarningRearCameraDisconnected", names)
        self.assertNotIn("WarningCameraDisconnected", names)
        driver = message.get_signal_by_name("DriverCameraConnected")
        self.assertEqual(driver.length, 1)
        self.assertEqual(driver.dbc.attributes["ValueTable"].value, "Connection")
        tach = message.get_signal_by_name("CM5FanTach")
        self.assertEqual(
            (tach.length, tach.scale, tach.offset, tach.unit), (12, 5, 0, "rpm")
        )
        discrete = [signal for signal in message.signals if signal.choices]
        continuous = [signal for signal in message.signals if signal.unit]
        self.assertLess(
            max(signal.start for signal in discrete),
            min(signal.start for signal in continuous),
        )

    def test_bps_cell_sum_precedes_precharge_voltages(self):
        message = self.message("CarCAN", 0x70)
        voltages = [signal for signal in message.signals if signal.unit == "V"]
        self.assertEqual(
            [signal.name for signal in voltages[:5]],
            [
                "CellSumVoltage",
                "MainBatteryVoltage",
                "MotorBatteryVoltage",
                "MainVoltage",
                "MotorVoltage",
            ],
        )
        signal = message.get_signal_by_name("CellSumVoltage")
        self.assertEqual((signal.length, signal.scale, signal.offset), (12, 0.05, 0))
        self.assertGreaterEqual(signal.maximum, 32 * 6.095)
        values = {item.name: item.minimum for item in message.signals}
        values["CellSumVoltage"] = 134.4
        frame = message.encode(values)
        self.assertEqual(
            (int.from_bytes(frame, "little") >> signal.start) & 0xFFF, 2688
        )
        charger = self.message("ChargerCAN", 0x70).get_signal_by_name("CellSumVoltage")
        self.assertEqual(
            (charger.start, charger.length, charger.scale), (signal.start, 12, 0.05)
        )

    def test_bps_pack_split_and_data_block(self):
        measurements = {
            "CellSumVoltage",
            "MainBatteryVoltage",
            "MotorBatteryVoltage",
            "MainVoltage",
            "MotorVoltage",
            "BatteryCurrent",
        }
        for bus in ("CarCAN", "ChargerCAN"):
            status = self.message(bus, 0x60)
            pack = self.message(bus, 0x70)
            self.assertEqual(pack.name, "BPSPackData")
            self.assertEqual(pack.senders, ["BPS"])
            self.assertTrue(pack.is_fd)
            self.assertEqual((status.length, pack.length), (16, 12))
            self.assertEqual({signal.name for signal in pack.signals}, measurements)
            self.assertFalse(measurements & {signal.name for signal in status.signals})
            for name in (
                "BatteryFanTach1",
                "BatteryFanTach2",
                "BPSFanTach",
                "MPPTVoltageLimit",
                "MPPTCurrentLimit",
                "DriveOK",
                "ChargeOK",
            ):
                self.assertIn(name, {signal.name for signal in status.signals})
            self.assertEqual(status.get_signal_by_name("HVPositiveContactor").start, 40)
            # Known raw values verify the new frame boundaries and padding.
            raw = {signal.name: 0 for signal in pack.signals}
            raw.update(CellSumVoltage=2688, BatteryCurrent=5000)
            expected = 2688 | (5000 << 60)
            frame = pack.encode(raw, scaling=False)
            self.assertEqual(frame, expected.to_bytes(12, "little"))
            self.assertEqual(pack.decode(frame, scaling=False)["BatteryCurrent"], 5000)
            self.assertEqual(pack.cycle_time, 50)
        for identifier, name in (
            (0x70, "BPSPackData"),
            (0x71, "BPSVoltageData"),
            (0x72, "BPSTemperatureData"),
            (0x73, "SMDStatus"),
        ):
            self.assertEqual(self.message("CarCAN", identifier).name, name)

        for identifier, summaries, starts, length, counts in (
            (0x71, ("MinCellVoltage", "MaxCellVoltage"), (384, 396), 64, (1000, 2000)),
            (
                0x72,
                ("MaxBatteryTemperature", "AverageBatteryTemperature"),
                (352, 363),
                48,
                (800, 600),
            ),
        ):
            message = self.message("CarCAN", identifier)
            self.assertEqual(message.length, length)
            self.assertEqual(len(message.signals), 34)
            self.assertEqual(
                [signal.name for signal in message.signals[-2:]], list(summaries)
            )
            raw = {signal.name: 0 for signal in message.signals}
            expected = 0
            for name, start, count in zip(summaries, starts, counts):
                signal = message.get_signal_by_name(name)
                self.assertEqual(signal.start, start)
                raw[name] = count
                expected |= count << start
                self.assertNotIn(
                    name,
                    {signal.name for signal in self.message("CarCAN", 0x60).signals},
                )
                self.assertNotIn(
                    name,
                    {signal.name for signal in self.message("CarCAN", 0x70).signals},
                )
            self.assertEqual(
                message.encode(raw, scaling=False), expected.to_bytes(length, "little")
            )

    def test_bps_command_echo_mirrors_command(self):
        command = self.message("CarCAN", 0x51)
        echo = self.message("CarCAN", 0x74)
        self.assertEqual(echo.name, "BPSCommandEcho")
        self.assertEqual(echo.senders, ["BPS"])
        self.assertEqual((echo.length, echo.is_fd), (command.length, command.is_fd))
        self.assertEqual(echo.cycle_time, 1000)
        for source, mirrored in zip(command.signals, echo.signals, strict=True):
            self.assertEqual(
                (
                    source.name,
                    source.start,
                    source.length,
                    source.byte_order,
                    source.scale,
                    source.offset,
                    source.is_signed,
                    source.minimum,
                    source.maximum,
                    source.choices,
                ),
                (
                    mirrored.name,
                    mirrored.start,
                    mirrored.length,
                    mirrored.byte_order,
                    mirrored.scale,
                    mirrored.offset,
                    mirrored.is_signed,
                    mirrored.minimum,
                    mirrored.maximum,
                    mirrored.choices,
                ),
            )
        for values in (
            {
                "Mode": 0,
                "MPPTChargeScheme": 0,
                "VoltageOverrideMask": 0,
                "TemperatureOverrideMask": 0,
            },
            {
                "Mode": 1,
                "MPPTChargeScheme": 1,
                "VoltageOverrideMask": 0xA5A5A5A5,
                "TemperatureOverrideMask": 0xFFFFFFFF,
            },
        ):
            self.assertEqual(echo.encode(values), command.encode(values))
            self.assertEqual(
                echo.decode(command.encode(values), decode_choices=False), values
            )

    def test_team_signal_category_order(self):
        """All team layouts place fault bits, warnings, states, then measurements."""

        def group(signal):
            attribute = signal.dbc.attributes.get("ValueTable") if signal.dbc else None
            table = attribute.value if attribute else None
            if signal.name.startswith("Fault") or table in ("FaultState", "Watchdog"):
                return 0
            if signal.name.startswith("Warning") or table == "WarningState":
                return 1
            if (
                table
                or signal.length == 1
                or (not signal.unit and signal.name != "SatellitesInUse")
            ):
                return 2
            return 3

        for bus in ("CarCAN", "ChargerCAN", "DAqCAN", "LightingCAN"):
            for message in self.databases[bus].messages:
                sequence = [
                    group(signal)
                    for signal in sorted(
                        message.signals, key=lambda signal: signal.start
                    )
                ]
                self.assertEqual(sequence, sorted(sequence), (bus, message.name))

    def test_bootloader_board_coverage_and_addressing(self):
        allocation = json.loads((ROOT.parent / "bootloader_ids.json").read_text())
        boards = allocation["boards"]
        self.assertEqual(len(boards), 20)
        self.assertEqual(allocation["schema_version"], 1)
        self.assertEqual(int(allocation["addressing"]["host_can_id"], 0), 0)
        excluded = {
            "External",
            "WS22",
            "LWS",
            "Elcon",
            "MPPTA",
            "MPPTB",
            "MPPTC",
            "ElconInterface",
            "TelemetryLeader",
            "MIKA",
        }
        attached = {}
        for bus, database in self.databases.items():
            for node in database.nodes:
                if node.name not in excluded:
                    attached.setdefault(node.name, set()).add(bus)
        self.assertEqual({board["name"] for board in boards}, set(attached))
        self.assertEqual(len({board["node_id"] for board in boards}), len(boards))
        addresses = {0}
        application_ids = {
            message.frame_id
            for database in self.databases.values()
            for message in database.messages
        }
        for board in boards:
            node = board["node_id"]
            self.assertTrue(1 <= node <= 40)
            rx, tx = int(board["rx_can_id"], 0), int(board["tx_can_id"], 0)
            self.assertEqual((rx, tx), (node, node + 0x28))
            self.assertNotIn(rx, addresses)
            self.assertNotIn(tx, addresses)
            self.assertNotIn(rx, application_ids)
            self.assertNotIn(tx, application_ids)
            addresses.update((rx, tx))
            self.assertEqual(set(board["can_buses"]), attached[board["name"]])
        used = {board["node_id"] for board in boards}
        reserved = set(allocation["reserved_node_ids"])
        self.assertFalse(used & reserved)
        self.assertEqual(used | reserved, set(range(1, 41)))
        self.assertFalse(any("SMB" in board["name"] for board in boards))

    def test_consolidated_physical_nodes(self):
        for database in self.databases.values():
            self.assertFalse(
                {node.name for node in database.nodes}
                & {"TelemetryLeader", "BottomShellLighting"}
            )
            for message in database.messages:
                self.assertNotIn("TelemetryLeader", message.senders)
                self.assertNotIn("BottomShellLighting", message.senders)
                for signal in message.signals:
                    self.assertEqual(len(signal.receivers), len(set(signal.receivers)))
                    self.assertNotIn("TelemetryLeader", signal.receivers)
                    self.assertNotIn("BottomShellLighting", signal.receivers)
        self.assertEqual(self.message("LightingCAN", 0x80).senders, ["DEI"])
        hmi = self.message("CarCAN", 0x82)
        self.assertEqual(hmi.name, "MIKAStatus")
        self.assertEqual(hmi.senders, ["MIKA"])
        self.assertEqual(self.message("DAqCAN", 0x621).senders, ["MIKA"])
        self.assertNotIn(
            "MIKAWatchdog",
            {signal.name for signal in self.message("DAqCAN", 0x623).signals},
        )
        allocation = json.loads((ROOT.parent / "bootloader_ids.json").read_text())
        boards = {board["name"]: board for board in allocation["boards"]}
        self.assertFalse(
            set(boards)
            & {"MIKA", "TelemetryLeader", "BottomShellLighting", "ElconInterface"}
        )
        self.assertTrue({5, 9, 11, 12}.issubset(allocation["reserved_node_ids"]))
        self.assertEqual(boards["BPS"]["node_id"], 1)
        self.assertEqual(boards["BPS"]["rx_can_id"], "0x001")
        self.assertEqual(boards["BPS"]["tx_can_id"], "0x029")
        for bus, identifier, name in (
            ("CarCAN", 0x60, "BPSStatus"),
            ("ChargerCAN", 0x60, "BPSStatus"),
        ):
            status = self.message(bus, identifier)
            self.assertEqual(status.name, name)
            self.assertEqual(status.senders, ["BPS"])
        self.assertEqual(boards["DEI"]["node_id"], 7)
        self.assertEqual(boards["TopShellLighting"]["node_id"], 10)
        self.assertEqual(boards["StrainGaugeRear"]["node_id"], 24)
        self.assertEqual(self.message("ChargerCAN", 0x600).senders, ["ElconInterface"])

    def test_lighting_bus_ownership(self):
        car = self.databases["CarCAN"]
        lighting = self.databases["LightingCAN"]
        self.assertFalse({message.frame_id for message in car.messages} & {0x80, 0x81})
        self.assertNotIn("TopShellLighting", {node.name for node in car.nodes})
        self.assertEqual(
            {message.frame_id for message in lighting.messages}, {0x80, 0x81}
        )
        self.assertEqual(lighting.dbc.attributes["DBName"].value, "LightingCAN")
        self.assertEqual(self.message("LightingCAN", 0x80).senders, ["DEI"])
        self.assertEqual(
            self.message("LightingCAN", 0x81).senders, ["TopShellLighting"]
        )
        self.assertTrue(all(not message.is_fd for message in lighting.messages))
        self.assertEqual(
            self.message("LightingCAN", 0x80).encode(
                {"Sync": 1, "LeftSignalOn": 1, "RightSignalOn": 0, "HeadlightsOn": 1}
            ),
            b"\x0b",
        )
        self.assertEqual(
            self.message("LightingCAN", 0x81).encode(
                {"BrakeOn": 1, "LeftSignalOn": 0, "RightSignalOn": 1}
            ),
            b"\x05",
        )
        allocation = json.loads((ROOT.parent / "bootloader_ids.json").read_text())
        boards = {board["name"]: board for board in allocation["boards"]}
        self.assertEqual(boards["TopShellLighting"]["can_buses"], ["LightingCAN"])
        self.assertEqual(boards["TopShellLighting"]["node_id"], 10)
        self.assertEqual(
            boards["DEI"]["can_buses"], ["CarCAN", "LightingCAN", "SteeringCAN"]
        )
        self.assertEqual(boards["BPS"]["node_id"], 1)
        self.assertEqual(boards["BPS"]["rx_can_id"], "0x001")
        self.assertEqual(boards["BPS"]["tx_can_id"], "0x029")
        for bus, identifier, name in (
            ("CarCAN", 0x60, "BPSStatus"),
            ("ChargerCAN", 0x60, "BPSStatus"),
        ):
            status = self.message(bus, identifier)
            self.assertEqual(status.name, name)
            self.assertEqual(status.senders, ["BPS"])
        self.assertEqual(boards["DEI"]["node_id"], 7)

    def test_legacy_timestamp_sync_and_names(self):
        sync = self.message("DAqCAN", 0x620)
        self.assertEqual(sync.name, "TimestampSync")
        self.assertEqual(sync.senders, ["MIKA"])
        self.assertEqual(sync.length, 2)
        self.assertTrue(sync.is_fd)
        self.assertEqual(len(sync.signals), 1)
        timestamp = sync.get_signal_by_name("Timestamp")
        self.assertEqual(
            (
                timestamp.start,
                timestamp.length,
                timestamp.byte_order,
                timestamp.is_signed,
            ),
            (0, 16, "little_endian", False),
        )
        self.assertEqual((timestamp.scale, timestamp.offset), (1, 0))
        self.assertFalse(timestamp.unit)
        self.assertIn("unresolved", timestamp.comment)
        self.assertEqual(sync.encode({"Timestamp": 0x1234}), b"\x34\x12")
        self.assertEqual(sync.encode({"Timestamp": 65535}), b"\xff\xff")
        self.assertFalse(sync.dbc.attributes["LayoutPending"].value)
        self.assertEqual(self.message("DAqCAN", 0x621).name, "TelemetryStatus")

    def test_legacy_linpot_adc_calibration(self):
        for identifier in (0x670, 0x671, 0x672):
            message = self.message("DAqCAN", identifier)
            self.assertEqual(message.length, 12)
            self.assertFalse(message.dbc.attributes["LayoutPending"].value)
            self.assertEqual(len(message.signals), 6)
            raw = {"Timestamp": 0x1234}
            packed = 0x1234
            for channel, count in enumerate((0, 1, 2048, 4095), 1):
                signal = message.get_signal_by_name(f"LinpotPosition{channel}")
                self.assertEqual(
                    (signal.start, signal.length, signal.byte_order),
                    (16 + (channel - 1) * 12, 12, "little_endian"),
                )
                self.assertEqual(
                    (signal.scale, signal.offset, signal.unit), (0.0188885, 1, "mm")
                )
                raw[signal.name] = count
                packed |= count << signal.start
            raw["LinpotPositionAverage"] = 1536
            packed |= 1536 << 64
            frame = message.encode(raw, scaling=False)
            self.assertEqual(frame, packed.to_bytes(12, "little"))
            decoded = message.decode(frame, decode_choices=False)
            for channel, count in enumerate((0, 1, 2048, 4095), 1):
                self.assertAlmostEqual(
                    decoded[f"LinpotPosition{channel}"], count * 0.0188885 + 1
                )
            self.assertEqual(message.encode(decoded), frame)

    def test_measurement_batches_and_logical_ids(self):
        groups = [(0x630, "Chassis", ("Acceleration", "Gyro", "Magnetometer"))]
        groups += [
            (base, board, ("Acceleration", "Gyro", "WheelSpeed"))
            for base, board in (
                (0x640, "UprightFR"),
                (0x650, "UprightFL"),
                (0x660, "UprightRear"),
            )
        ]
        for base, board, measurements in groups:
            for index, measurement in enumerate(measurements):
                message = self.message("DAqCAN", base + index)
                self.assertEqual(message.name, f"{board}{measurement}Data")
                self.assertLessEqual(message.length, 64)
                interval = message.get_signal_by_name("SampleIntervalTicks")
                self.assertEqual(
                    (interval.start, interval.length, interval.minimum), (16, 16, 1)
                )
                raw = {"Timestamp": 0x1234, "SampleIntervalTicks": 5}
                packed = 0x1234 | (5 << 16)
                samples = [
                    signal for signal in message.signals if signal.name not in raw
                ]
                self.assertEqual(len(samples), 5 if measurement == "WheelSpeed" else 15)
                for signal in samples:
                    if signal.name.endswith("Average"):
                        value = 1 if signal.is_signed else 3
                    else:
                        sample = int(signal.name[-1]) - 1
                        value = (
                            (-6, -2, 4, 8)[sample]
                            if signal.is_signed
                            else (0, 2, 4, 6)[sample]
                        )
                    raw[signal.name] = value
                    packed |= (value & ((1 << signal.length) - 1)) << signal.start
                frame = message.encode(raw, scaling=False)
                self.assertEqual(frame, packed.to_bytes(message.length, "little"))
                self.assertEqual(message.encode(message.decode(frame)), frame)

    def test_no_signal_receiver_assignments(self):
        for bus, database in self.databases.items():
            for message in database.messages:
                for signal in message.signals:
                    self.assertEqual(
                        signal.receivers, [], (bus, message.name, signal.name)
                    )

    def test_bps_fault_snapshots_and_voltage_faults(self):
        for bus in ("CarCAN", "ChargerCAN"):
            status = self.databases[bus].get_message_by_name("BPSStatus")
            names = {signal.name for signal in status.signals}
            self.assertFalse(any("VoltageNotPresent" in name for name in names))
            for name, bit in (("FaultMainVoltage", 19), ("FaultMotorVoltage", 20),
                              ("FaultMainPrechargeVoltage", 21), ("FaultMotorPrechargeVoltage", 22)):
                signal = status.get_signal_by_name(name)
                self.assertEqual((signal.start, signal.length), (bit, 1))
                self.assertEqual(signal.dbc.attributes["ValueTable"].value, "FaultState")
        car = self.databases["CarCAN"]
        for identifier, name, length in ((0x75, "BPSFaultSnapshot", 16),
                                         (0x76, "BPSFaultVoltageSnapshot", 64),
                                         (0x77, "BPSFaultTemperatureSnapshot", 64)):
            message = car.get_message_by_frame_id(identifier)
            self.assertEqual((message.name, message.length, message.cycle_time), (name, length, 1000))
            self.assertEqual(message.senders, ["BPS"])
            self.assertTrue(message.is_fd)
            self.assertEqual(message.dbc.attributes["GenMsgSendCondition"].value,
                             "AnyBPSFaultActive && FaultSnapshotValid")
            self.assertEqual(message.get_signal_by_name("SnapshotId").length, 16)
            self.assertIn("frozen", message.comment)
        for snapshot_name, live_name, prefix in (
            ("BPSFaultSnapshot", "BPSPackData", "BatteryCurrent"),
            ("BPSFaultVoltageSnapshot", "BPSVoltageData", "CellVoltage"),
            ("BPSFaultTemperatureSnapshot", "BPSTemperatureData", "CellTemperature"),
        ):
            snapshot = car.get_message_by_name(snapshot_name)
            live = car.get_message_by_name(live_name)
            for signal in snapshot.signals:
                if signal.name.startswith(prefix):
                    source = live.get_signal_by_name(signal.name)
                    self.assertEqual((signal.length, signal.scale, signal.offset, signal.minimum, signal.maximum),
                                     (source.length, source.scale, source.offset, source.minimum, source.maximum))
        for name in ("BPSFaultVoltageSnapshot", "BPSFaultTemperatureSnapshot"):
            message = car.get_message_by_name(name)
            self.assertEqual(message.get_signal_by_name("SampleValidMask").length, 32)
            self.assertEqual(message.get_signal_by_name("SampleAgeMs").length, 16)

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
