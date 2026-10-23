"""Integration checks for branch publication lifecycle and DBC parsing failures."""

import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from cantools.database import UnsupportedDatabaseFormatError

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from publish_branches import publish

DBC = b"""VERSION ""
NS_ :
BS_:
BU_: Sensor
BO_ 1584 Reading: 8 Sensor
 SG_ Value : 0|16@1+ (0.1,0) [0|6553.5] "V" Vector__XXX
"""


class PublicationTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.repo = Path(self.directory.name) / "repo"
        self.repo.mkdir()
        self.out = Path(self.directory.name) / "site"
        self.git("init", "-q", "-b", "main")
        self.git("config", "user.name", "Test")
        self.git("config", "user.email", "test@example.com")
        self.write(DBC)
        self.git("add", ".")
        self.git("commit", "-qm", "main DBC")
        self.ref("main")

    def git(self, *args):
        return (
            subprocess.check_output(["git", "-C", str(self.repo), *args])
            .decode()
            .strip()
        )

    def ref(self, name):
        self.git("update-ref", f"refs/remotes/origin/{name}", "HEAD")

    def write(self, content):
        path = self.repo / "can/dbc/CarCAN.dbc"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(content)

    def feature(self, name="dev/dbc"):
        self.write(DBC.replace(b"1584", b"1585"))
        self.git("commit", "-qam", "changed DBC")
        self.ref(name)

    def test_publish_update_equal_main_and_delete(self):
        self.feature()
        self.ref("same/content")
        manifest = publish(self.repo, self.out)
        self.assertEqual(
            [b["name"] for b in manifest["branches"]],
            ["main", "dev/dbc", "same/content"],
        )
        self.assertEqual(len(list((self.out / "data").glob("*.json"))), 2)
        feature = manifest["branches"][1]
        data = json.loads((self.out / feature["dataUrl"]).read_text())
        self.assertEqual(
            data["vehicles"][0]["networks"][0]["messages"][0]["frameId"], 1585
        )
        # A non-DBC commit keeps the same immutable snapshot URL.
        (self.repo / "README").write_text("changed")
        self.git("add", ".")
        self.git("commit", "-qm", "documentation")
        self.ref("dev/dbc")
        updated = publish(self.repo, self.out)["branches"][1]
        self.assertEqual(updated["dataUrl"], feature["dataUrl"])
        self.assertNotEqual(updated["commit"], feature["commit"])
        # Main catches up; all equal branches disappear, including stored JSON.
        self.ref("main")
        manifest = publish(self.repo, self.out)
        self.assertEqual([b["name"] for b in manifest["branches"]], ["main"])
        self.assertEqual(len(list((self.out / "data").glob("*.json"))), 1)
        # Diverge again then delete the branch.
        self.write(DBC.replace(b"1584", b"1586"))
        self.git("commit", "-qam", "another DBC")
        self.ref("dev/dbc")
        manifest = publish(self.repo, self.out)
        stale_path = self.out / next(
            b["dataUrl"] for b in manifest["branches"] if b["name"] == "dev/dbc"
        )
        self.git("update-ref", "-d", "refs/remotes/origin/dev/dbc")
        manifest = publish(self.repo, self.out)
        self.assertNotIn("dev/dbc", [b["name"] for b in manifest["branches"]])
        self.assertFalse(stale_path.exists())

    def test_invalid_feature_omitted_invalid_main_fails(self):
        self.write(b"broken DBC")
        self.git("commit", "-qam", "invalid")
        self.ref("dev/invalid")
        manifest = publish(self.repo, self.out)
        self.assertEqual([b["name"] for b in manifest["branches"]], ["main"])
        self.assertEqual(manifest["errors"][0]["name"], "dev/invalid")
        self.ref("main")
        with self.assertRaises(UnsupportedDatabaseFormatError):
            publish(self.repo, self.out)

    def test_filename_change_creates_distinct_snapshot_and_blank_is_valid(self):
        self.git("mv", "can/dbc/CarCAN.dbc", "can/dbc/DAqCAN.dbc")
        (self.repo / "can/dbc/Empty.dbc").touch()
        self.git("add", ".")
        self.git("commit", "-qm", "rename bus")
        self.ref("bus/rename")
        manifest = publish(self.repo, self.out)
        self.assertEqual(len(manifest["branches"]), 2)
        payload = json.loads(
            (self.out / manifest["branches"][1]["dataUrl"]).read_text()
        )
        self.assertEqual(
            [n["id"] for n in payload["vehicles"][0]["networks"]], ["DAqCAN", "Empty"]
        )


if __name__ == "__main__":
    unittest.main()
