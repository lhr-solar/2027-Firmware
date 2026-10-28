"""Topology policy checks covering empty filters, paths, loops and invalid edits."""

import shutil
import sys
import tempfile
import unittest
from copy import deepcopy
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "analysis"))
from contracts import load_databases
from sync_bus_settings import synchronize
from topology import (
    StrictLoader,
    load_topology,
    merge_messages,
    resolve,
    selected,
    validate,
)


class TopologyTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.databases = load_databases(ROOT / "dbc")
        cls.config = load_topology(ROOT / "topology.yaml", cls.databases)

    def test_empty_filters_and_ored_selectors(self):
        message = self.databases["DAqCAN"].get_message_by_name("TimestampSync")
        self.assertTrue(selected({}, message))
        self.assertTrue(selected({"mode": "exclude", "messages": []}, message))
        self.assertFalse(selected({"mode": "include", "messages": []}, message))
        self.assertFalse(selected({"messages": ["0x620"]}, message))
        self.assertTrue(selected({"mode": "include", "messages": ["0x620"]}, message))
        self.assertTrue(
            selected(
                {"mode": "include", "messages": ["0x621"], "transmitters": ["MIKA"]},
                message,
            )
        )

    def test_unknown_unquoted_hex_and_misspelled_filter(self):
        for rule in (
            {"messages": [0x620]},
            {"messages": ["TimestampSync"]},
            {"messages": ["0x20000000"]},
            {"messages": ["0x7FF"]},
            {"mode": "includes"},
            {"message": ["0x620"]},
            {"transmitters": ["not_a_node"]},
        ):
            config = deepcopy(self.config)
            config["buses"]["DAqCAN"]["forwards"][0]["filter"] = rule
            with self.assertRaises(ValueError):
                validate(config, self.databases)

    def test_bad_bridge_and_cycles(self):
        config = deepcopy(self.config)
        config["buses"]["MotorCAN"]["forwards"][0]["via"] = "MIKA"
        with self.assertRaisesRegex(ValueError, "connect both"):
            validate(config, self.databases)
        config = deepcopy(self.config)
        config["buses"]["CarCAN"]["forwards"].append({"to": "MotorCAN", "via": "VCU"})
        with self.assertRaisesRegex(ValueError, "loop"):
            validate(config, self.databases)

    def test_bps_forwarding_is_bidirectional_without_echo(self):
        forward = next(
            route
            for route in self.config["buses"]["CarCAN"]["forwards"]
            if route["to"] == "ChargerCAN"
        )
        backward = self.config["buses"]["ChargerCAN"]["forwards"][0]
        for identifier in (0x60, 0x70, 0x71, 0x72):
            message = self.databases["CarCAN"].get_message_by_frame_id(identifier)
            self.assertTrue(selected(forward["filter"], message))
            self.assertFalse(selected(backward["filter"], message))
            resolved = resolve(self.config, {"CarCAN": [message]}, merge_messages)
            self.assertEqual(resolved["ChargerCAN"], [message])
            self.assertEqual(resolved["TelemetryTCP"], [message])
        command = self.databases["CarCAN"].get_message_by_name("BPSCommand")
        self.assertFalse(selected(forward["filter"], command))
        # Removing the reverse exclusion must expose the real frame-level loop.
        config = deepcopy(self.config)
        config["buses"]["ChargerCAN"]["forwards"][0]["filter"]["messages"] = []
        with self.assertRaisesRegex(ValueError, "Forwarding loop for 0x60"):
            validate(config, self.databases)

    def test_dbc_setting_drift_and_duplicate_targets(self):
        config = deepcopy(self.config)
        config["buses"]["CarCAN"]["bitrate"] = 500000
        with self.assertRaisesRegex(ValueError, "bitrate differs"):
            validate(config, self.databases)
        config = deepcopy(self.config)
        config["buses"]["MotorCAN"]["forwards"] *= 2
        with self.assertRaisesRegex(ValueError, "duplicate route"):
            validate(config, self.databases)

    def test_fd_cannot_forward_to_classic(self):
        config = deepcopy(self.config)
        # Replace, rather than add, a route so there is no cycle.
        config["buses"]["MotorCAN"]["forwards"] = []
        config["buses"]["CarCAN"]["forwards"] = [{"to": "MotorCAN", "via": "VCU"}]
        with self.assertRaisesRegex(ValueError, "onto classic CAN"):
            validate(config, self.databases)

    def test_sync_is_idempotent_and_failure_does_not_write(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            shutil.copytree(ROOT / "dbc", root / "dbc")
            path = root / "topology.yaml"
            shutil.copyfile(ROOT / "topology.yaml", path)
            self.assertEqual(synchronize(path), [])
            text = path.read_text().replace("bitrate: 250000", "bitrate: 125000")
            path.write_text(text)
            self.assertTrue(synchronize(path))
            self.assertEqual(synchronize(path), [])
            snapshots = {p: p.read_bytes() for p in (root / "dbc").glob("*.dbc")}
            path.write_text(text.replace("format: can_fd", "format: classic"))
            with self.assertRaises(ValueError):
                synchronize(path)
            self.assertTrue(
                all(p.read_bytes() == content for p, content in snapshots.items())
            )

    def test_duplicate_yaml_keys_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "Duplicate YAML"):
            yaml.load("version: 1\nversion: 2\n", Loader=StrictLoader)


if __name__ == "__main__":
    unittest.main()
