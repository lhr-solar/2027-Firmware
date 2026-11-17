"""Metadata labels survive the DBC/JSON boundary, including reserved messages."""

import sys
import unittest
from pathlib import Path

import cantools

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from export_canspec_json import _message_to_dict
from topology_data import topology_payload

DBC = """VERSION ""
NS_ :
BS_:
BU_: Device
BO_ 96 CyclicFrame: 1 Device
 SG_ Value : 0|8@1+ (1,0) [0|255] "" Vector__XXX
BO_ 97 EventFrame: 0 Device
BO_ 98 ReservedFrame: 0 Device
BA_DEF_ BO_ "GenMsgSendType" ENUM "Cyclic","Triggered","None";
BA_DEF_ BO_ "GenMsgCycleTime" INT 0 65535;
BA_DEF_DEF_ "GenMsgSendType" "Cyclic";
BA_ "GenMsgSendType" BO_ 96 0;
BA_ "GenMsgCycleTime" BO_ 96 200;
BA_ "GenMsgSendType" BO_ 97 1;
BA_ "GenMsgSendType" BO_ 98 2;
"""


class MetadataTests(unittest.TestCase):
    def test_message_type_and_period(self):
        db = cantools.database.load_string(DBC, database_format="dbc")
        frames = [_message_to_dict(message) for message in db.messages]
        self.assertEqual(
            [(m["sendType"], m["cycleTimeMs"]) for m in frames],
            [("Cyclic", 200), ("Triggered", None), ("Disabled", None)],
        )

    def test_topology_loader_rejects_duplicate_keys_and_unquoted_ids(self):
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            topology_payload(b"version: 1\nversion: 2\n")
        with self.assertRaisesRegex(ValueError, "strings"):
            topology_payload(b"""version: 1
buses:
  A:
    format: can_fd
    bitrate: 1000000
    forwards:
      - to: TCP
        via: Device
        filter:
          messages: [0x620]
outputs:
  TCP:
    transport: tcp
""")


if __name__ == "__main__":
    unittest.main()
