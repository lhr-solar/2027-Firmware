# 2027 CAN databases

These seven draft databases implement the supplied **2026–2027 ID Allocation &
Drafting** export (edited October 22, 2026), incorporating the HighNoon DBCs from
Embedded-Sharepoint and the vendor references below. New team signal bit layouts
are proposed here; the source allocation did not specify start bits. They are
not wire-compatible with the old vehicle's team-defined messages.

## Networks and transmitters

| Database | Contents | Frames | Empty placeholders |
|---|---|---:|---:|
| CarCAN | Team critical/control, power, battery and body status | 18 | 1 |
| MotorCAN | VCU drive/power/reset commands and WS22 telemetry | 19 | 0 |
| MPPTCAN | Three SEC-B175-7A MPPTs; commands from MPPTInterface | 33 | 12 |
| SteeringCAN | LWS measurements; configuration sent by DEI | 2 | 0 |
| ChargerCAN | BPSLeader status and ElconInterface status | 2 | 0 |
| ElconCAN | ElconInterface commands and Elcon charger responses | 3 | 0 |
| TelemetryCAN | TelemetryLeader infrastructure and all 16 sensor boards | 19 | 17 |

Every message has exactly one declared transmitter. `External` is the command
injection tool/node for IDs 0x051–0x054. MPPTInterface receives BPSLeaderStatus
on **CarCAN** and produces MPPT commands on **MPPTCAN**; BPS is not a transmitter
on the private MPPT bus. DEI transmits all LWS configuration traffic; LWS remains
the transmitter of its own measurements.

BPSLeaderStatus (0x060) is intentionally present on both CarCAN and ChargerCAN
with identical signals and transmitter. This is the only repeated numeric ID
across the seven files. It describes BPSLeader transmitting on both networks;
any actual gateway implementation must match this routing assumption.

CarCAN, ChargerCAN and TelemetryCAN use CAN-FD, including short team frames.
The WS22, TPEE, Bosch and Elcon protocols remain **classic CAN**. Payloads use
legal CAN-FD wire lengths, with unused bits/bytes reserved as zero. No bus bit
rate or BRS setting is guessed; configure those in firmware to match hardware.

The pre-existing zero-byte DAqCAN.dbc, LightingCAN.dbc, bootloading.dbc and
telemetry_bebug.dbc are left untouched. TelemetryCAN is a new requested network,
not an unannounced rename of an old file. Lighting status lives on CarCAN.

## Shared encodings

| Quantity in team messages | Unit | Step | Offset / representation |
|---|---|---|---|
| Bulk voltages | V | 0.05 | 0 |
| Cell/tap voltages | mV | 1 | 2000; unsigned 12 bits, 2000–6095 mV |
| Currents | A | 0.01 | 0, except battery current below |
| Battery current | A | 0.01 | **−50; unsigned 14 bits, −50.00–113.83 A** |
| Battery temperatures | degC | 0.05 | 0; unsigned 11 bits, 0–102.35 degC |
| MIKA SoC temperature | degC | **0.5** | **−20; unsigned 8 bits, permitted −20–85 degC** |
| Fan tachometers | rpm | 5 | 0; unsigned 12 bits |
| Accelerator position | % | 100/255 | 0; unsigned 8 bits |
| Brake pressures | PSI | 1 | 0; unsigned 12 bits, permitted 0–3000 PSI |

Battery-current width and offset and the SoC-temperature exception are explicitly
preserved at the user's request. The 5 V offset from the draft's PDU/LVC voltage
fields is removed to standardize bulk voltage encoding; the existing 9-bit width
still represents the requested 5–20 V range. EVSE available current uses 12 bits
at 0.01 A/bit to retain 0–25.5 A with the standard current resolution.

BPS MPPT limits use the team voltage/current resolutions; MPPTInterface converts
them to the vendor command resolutions. This conversion does not replace device
limits or battery protection checks. BPS MPPTSetMode and MPPTInterface MPPTSetMode
use two bits to represent the vendor's Disabled/PO/ConstantInputVoltage modes.

**Vendor encodings are deliberate exceptions.** Changing a DBC cannot change a
COTS device's protocol: TPEE retains 0.01 V and 0.0005 A fixed-point values,
WS22 retains IEEE-754 floats, Bosch retains its angle/speed steps, and Elcon
retains its own voltage/current/temperature and cell-voltage encodings.

## Global value tables

`value_tables.json` is the shared enum catalog. Every standalone DBC contains
identical `VAL_TABLE_` definitions. Each enumerated signal has:

- Its usual `VAL_` mapping, required by common DBC readers.
- A `ValueTable` string attribute naming the shared global definition.

DBC globals are scoped to a file; a standalone DBC cannot import a table from
another DBC. The repeated definitions make each file independently usable, and
the validation tests detect divergence from the catalog. Reuse EnableDisable,
OnOff, FaultState, WarningState, Connection, Permission and the other shared
tables instead of defining signal-specific copies of equivalent enums.

## Empty messages and unallocated ranges

`LayoutPending=1` identifies an **undefined layout**. These messages have no
signals and a zero-byte placeholder payload. Zero length is a drafting marker,
not permission to transmit an empty frame. Owners must define the layout before
using these messages:

- CarCAN 0x072 SMDStatus: four SMB bitmaps are mentioned, but no widths/bit meanings.
- TelemetryCAN 0x620 timestamp/sync: no timestamp format supplied.
- All 16 telemetry sensor-board messages: no signals supplied.
- MPPT offsets 0x05–0x08: reserved here as empty placeholders for variable-length
  configuration-buffer protocols; they are not ordinary fixed-layout commands.

The CarCAN 0x054 sweep request is a **defined empty trigger**, not a pending
layout. WS22 reset and its two documented reserved frames are defined eight-byte
frames without named signals.

Unassigned/reserved ranges remain unassigned: no fabricated transmitting node or
fake message is created for each reserved ID. In particular, bootloader IDs
0x000–0x050 remain reserved until physical-bus placement and node-to-board mapping
are defined. No generic HMI, debug or future COTS traffic has been invented.

## Corrections and provenance

- Strain boards 1–6 use **0x63A–0x63F**, resolving the duplicate 0x63B with the
  user's approval. Telemetry expansion starts at 0x640.
- GPS **longitude is −180..180**, latitude **−90..90**. The draft interchanged them.
- TPEE SetMode/Sweep/VoltageLimit/CurrentLimit offsets are **0x09/0x0A/0x0B/0x0C**,
  confirming the new draft rather than last year's stale sweep/limit IDs.
  Sweep responses are five bytes; sweep start and end are both voltages.
  The current source's operating-mode enum includes MinimumInputVoltage=6,
  Fault=7; SetMode has Disabled=0, PO=1, ConstantInputVoltage=2.
- WS22 power command uses eight bytes with its current fraction at bits 32–63;
  the reset is eight bytes. Vendor percentage values are represented as `ratio`
  0.0–1.0, not 0–100. The old positive-only velocity bound is removed so reverse
  setpoints remain representable; application/controller operational limits apply.
- Bosch LWS angle is signed. Its OK bit is **1=valid**, not active-high fault.
  The undocumented legacy counter/checksum and resolution fields are removed:
  the Bosch sheet marks those bits as reserved. Configuration TX is DEI.
  Angle 0x7FFF and speed 0xFF remain decodable invalid sentinels; check OK/CAL/TRIM.
- Elcon IDs, five-byte control/status frames and the eight-byte multiplexed
  extension are retained from the **legacy HighNoon variant**. Its impossible
  cell-voltage and temperature bounds are corrected to representable physical
  ranges without changing wire bits. The currently published CAN-3865 protocol
  has different eight-byte extensions; confirm the installed charger firmware
  before choosing a different variant. This file does not claim CAN-3865 compliance.

References:

- Supplied 2026–2027 allocation export, edited October 22, 2026.
- [Legacy HighNoon DBCs](https://github.com/lhr-solar/Embedded-Sharepoint/tree/42c2d13eeddf2510327d549d5ab4cbdaee9637d3/can/vehicles/HighNoon).
- [TPEE Open-SEC CAN identifiers](https://github.com/TjitteS/OpenSmartEnergyConverter/blob/11bb2abe40cfa05160a197a04c03e1566eb88c51/Open-SEC%20Firmware/src/can.h),
  [wire packing/unpacking](https://github.com/TjitteS/OpenSmartEnergyConverter/blob/11bb2abe40cfa05160a197a04c03e1566eb88c51/Open-SEC%20Firmware/src/can.c),
  [control enums](https://github.com/TjitteS/OpenSmartEnergyConverter/blob/11bb2abe40cfa05160a197a04c03e1566eb88c51/Open-SEC%20Firmware/src/control.h),
  [MPPT modes](https://github.com/TjitteS/OpenSmartEnergyConverter/blob/11bb2abe40cfa05160a197a04c03e1566eb88c51/Open-SEC%20Firmware/src/mppt.h).
  These establish the protocol for the examined firmware revision, not the
  firmware revision currently flashed onto the team's hardware.
- [WS22 Appendix C](https://docs.prohelion.com/Motor_Controllers/WaveSculptor22/User_Manual/Appendix_C.html).
- [Bosch LWS datasheet](https://www.bosch-motorsport.com/content/downloads/Raceparts/Resources/pdf/Data%20sheet_191153675_Steering_Wheel_Angle_Sensor_LWS.pdf),
  cross-checked against the team's local 2021 copy.
- [Elcon CAN-3865](https://elconchargers.com/wp-content/uploads/2025/05/CAN-3865.pdf),
  consulted to identify variant differences, not substituted for the legacy format.

## Validation and codegen

From the repository root, with codegen's pinned cantools installed:

```sh
python -m unittest discover -s can/tests -v
```

Tests cover strict DBC parsing/layouts, transmitters, receiver declarations,
ID uniqueness, shared value tables, scaling exceptions, placeholders, vendor
byte vectors, each Elcon multiplexed page, GPS bounds and corrected strain IDs.
These are protocol tests, not hardware transmission tests.

The current custom codegen handles the six non-IEEE buses, but **MotorCAN's
required IEEE-754 signals are not yet supported by that generator**. Preserve
those signals in the DBC; do not convert WS22 floats into integers to bypass the
limitation. Python cantools can encode/decode all seven databases.
