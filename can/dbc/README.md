# 2027 CAN databases

These eight draft databases implement the supplied **2026–2027 ID Allocation &
Drafting** export (edited October 22, 2026), incorporating the HighNoon DBCs from
Embedded-Sharepoint and the vendor references below. New team signal bit layouts
are proposed here; the source allocation did not specify start bits. They are
not wire-compatible with the old vehicle's team-defined messages.

## Networks and transmitters

| Database | Contents | Frames | Empty placeholders |
|---|---|---:|---:|
| CarCAN | Team critical/control, power, battery and HMI status | 16 | 1 |
| MotorCAN | VCU drive/power/reset commands and WS22 telemetry | 19 | 0 |
| MPPTCAN | Three SEC-B175-7A MPPTs; commands from MPPTInterface | 33 | 12 |
| SteeringCAN | LWS measurements; configuration sent by DEI | 2 | 0 |
| LightingCAN | Bottom-shell status from DEI; top-shell status from TopShellLighting | 2 | 0 |
| ChargerCAN | BPS status and ElconInterface status | 2 | 0 |
| ElconCAN | ElconInterface commands and Elcon charger responses | 3 | 0 |
| DAqCAN | MIKA infrastructure and all 12 sensor boards | 23 | 5 |

Every message has exactly one declared transmitter. `External` is the command
injection tool/node for IDs 0x051–0x054. MPPTInterface receives BPSStatus
on **CarCAN** and produces MPPT commands on **MPPTCAN**; BPS is not a transmitter
on the private MPPT bus. DEI transmits all LWS configuration traffic; LWS remains
the transmitter of its own measurements.

BPSStatus (0x060) is intentionally present on both CarCAN and ChargerCAN
with identical signals and transmitter. This is the only repeated numeric ID
across the eight files. It describes BPS transmitting on both networks;
any actual gateway implementation must match this routing assumption.

CarCAN, ChargerCAN, DAqCAN and LightingCAN use CAN-FD, including short team frames.
The WS22, TPEE, Bosch and Elcon protocols remain **classic CAN**. Payloads use
legal CAN-FD wire lengths, with unused bits/bytes reserved as zero. No bus bit
rate or BRS setting is guessed; configure those in firmware to match hardware.

The telemetry/sensor bus is **DAqCAN**, using the formerly empty DAqCAN.dbc
scaffold. TelemetryCAN.dbc is removed at the user's request. The empty
bootloading.dbc and telemetry_bebug.dbc scaffolds remain untouched. Both lighting
messages live on **LightingCAN**: DEI transmits BottomShellLightingStatus at
**0x080**, and TopShellLighting transmits TopShellLightingStatus at **0x081**.
Their IDs and payload layouts are retained. Bottom-shell circuitry is on DEI,
not a separate CAN node. TopShellLighting is attached only to LightingCAN; its
former direct CarCAN DEI-status subscription is removed. DEI is attached to
CarCAN, SteeringCAN and LightingCAN. MIKA is the combined HMI
and telemetry board: it transmits MIKAStatus (0x082) on CarCAN and
TimestampSync/TelemetryStatus/OnBoardWatchdog on DAqCAN. Its former
self-watchdog bit is reserved rather than monitoring a second board identity.

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

## Signal organization and October 23 updates

Team-defined messages follow **faults → warnings → discrete signals → continuous
signals**, in both bit layout and displayed signal order. Category boundaries
are byte-aligned. Status fields using FaultState belong with faults; multi-state
status fields belong with the discrete states. Measured counts such as satellites
in use belong with continuous measurements. Fixed vendor layouts are unchanged.
This reorganization changes bit positions in the draft team layouts; regenerate
consumers together rather than mixing versions.

- BPSStatus includes `CellSumVoltage` **before** all pre/post-precharge
  voltage measurements. It uses unsigned 12-bit bulk encoding (0.05 V/bit,
  offset 0; 0–204.75 V), covering the sum of all 32 representable cell taps.
  This is added identically on CarCAN and ChargerCAN; the status frame remains 32 bytes.
- MIKA's rear-camera warning is explicitly `WarningRearCameraDisconnected`.
  A separate one-bit `DriverCameraConnected` uses the shared Connection table.
- MIKA includes `CM5FanTach`: unsigned 12-bit, 5 rpm/bit, 0–20475 rpm.
  Its discrete connection bits precede frame rate, temperature and fan speed;
  the frame is now 6 bytes. The SoC-temperature encoding remains unchanged.

## Legacy timestamp and linpot formats

`TimestampSync` at **0x620**, TX MIKA, now uses the calibrated HighNoon sync
layout: **two bytes**, unsigned 16-bit `Timestamp` at bit 0, little-endian,
scale 1, offset 0. The tick period, epoch and broadcast period remain unresolved.
The counter wraps every 65536 ticks; do not label it milliseconds or seconds.
The message remains CAN-FD under this year's bus convention. The status message
at **0x621** is named `TelemetryStatus`, also transmitted by MIKA.

Suspension FR/FL/Rear at **0x670/0x671/0x672** each contain an unsigned 16-bit
raw timestamp followed by **four unsigned 12-bit ADC/linpot values** and their
12-bit average, in twelve bytes. Fields are named `LinpotPosition1` through
`LinpotPosition4`, followed by `LinpotPositionAverage`. The average is across
the four physical channels and must be supplied by the sender. This year's wire order is uniformly little-endian. Each channel uses
last year's calibrated conversion:

```text
position_mm = raw_adc * 0.0188885 + 1
```

The 0..4095 raw domain represents **1..78.3494075 mm**, with approximately
0.01889 mm per count. These bounds are calculated from the calibration rather
than copying inconsistent old min/max labels. Quantization is not sensor accuracy.
The user confirmed the same linpots/measuring scheme is in use this year.

The calibrated definitions were found in the local Electron-v2 and Data Sandbox
copies of Embedded-Sharepoint's `can/dbc/HighNoon/DAqCAN.dbc`; Photon contains an
older unitless/raw copy. The calibrated copy also includes signed accel/gyro
fields with real scales, unlike Photon's copy. The new IMU/gyro/magnetometer layouts are defined below.

Firmware sending ADC counts must use a **raw packing path**; the current public
codegen API expects physical millimetres. Passing counts directly to that API
would apply the inverse calibration again. Raw-ADC convenience APIs are separate
codegen work; they have not been added by this DBC update.

## Receiver assignments

All signal receiver lists are intentionally empty in every DBC. `Vector__XXX`
is the required DBC syntax placeholder for an unspecified receiver, not a real
node or subscription. Message TX nodes and bus node inventories remain defined;
consumer subscriptions and gateway behavior belong in firmware configuration.

## Bootloader IDs

`../bootloader_ids.json` allocates **20 eligible custom CAN boards**, with one
global, stable node ID per board even when it connects to multiple buses.
Node IDs **5, 9, 11, 12 and 25–40** are reserved; remaining assignments retain
their original IDs. Standard 11-bit arbitration IDs follow the supplied scheme:

- Reserved BootloaderHost ID: **0x000** (payload/host protocol not defined here).
- Board RX: **NodeID**.
- Board TX: **NodeID + 0x28**.

RX/TX are relative to the board. `can_buses` lists physical attachments from the
DBCs, not enabled bootloader transports. The user's explicit exclusions are:

- BottomShellLighting: integrated on DEI; no independent firmware target.
- ElconInterface: excluded from bootloader allocation; retained in the DBCs.
- MIKA and its former TelemetryLeader identity: one physical board, named MIKA
  in the DBCs; neither needs CAN bootloading.
- SMBs (no CAN), COTS devices, and the external host.

Do not renumber assigned IDs when boards are removed. The JSON is an allocation,
not an implementation of bootloading or routing.

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
- Five telemetry placeholders: three blackbody boards and two strain boards.
- MPPT offsets 0x05–0x08: reserved here as empty placeholders for variable-length
  configuration-buffer protocols; they are not ordinary fixed-layout commands.

The CarCAN 0x054 sweep request is a **defined empty trigger**, not a pending
layout. WS22 reset and its two documented reserved frames are defined eight-byte
frames without named signals.

Unassigned/reserved ranges remain unassigned: no fabricated transmitting node or
fake message is created for each reserved ID. Bootloader IDs 0x000–0x050
remain reserved for the separate allocation in `../bootloader_ids.json`;
bootloader transport, message layouts and gateway behavior are not defined here. No generic HMI, debug or future COTS traffic has been invented.

## Corrections and provenance

- There are **two strain boards**: Front at **0x690**, Rear at **0x691**.
  Telemetry IDs are grouped by board within 0x630–0x6DF.
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

The current custom codegen handles the seven non-IEEE buses, but **MotorCAN's
required IEEE-754 signals are not yet supported by that generator**. Preserve
those signals in the DBC; do not convert WS22 floats into integers to bypass the
limitation. Python cantools can encode/decode all eight databases.

## Telemetry allocation and sample batches

| IDs | Measurements |
|---|---|
| 0x630–0x632 | Chassis acceleration, gyro, magnetometer |
| 0x640–0x642 | Front-right upright acceleration, gyro, wheel speed |
| 0x650–0x652 | Front-left upright acceleration, gyro, wheel speed |
| 0x660–0x662 | Rear upright acceleration, gyro, wheel speed |
| 0x670–0x672 | Front-right, front-left, rear suspension linpots |
| 0x680–0x682 | Blackbody boards 1–3 |
| 0x690–0x691 | Front and rear strain gauges |

Other slots in 0x630–0x6DF remain unallocated. Infrastructure retains
TimestampSync 0x620, TelemetryStatus 0x621, and OnBoardWatchdog 0x623.
Bootloader node IDs are independent and unchanged.

Acceleration, gyro, magnetometer and wheel-speed frames contain four consecutive
samples per axis/value and a sender-computed arithmetic average. Number suffixes
follow the quantity, for example `AccelerationX1` through `AccelerationX4`, then
`AccelerationXAverage`. Packing stores the supplied average; it does not compute
it. Round averages to the nearest representable step, with raw half-step ties
away from zero. Send only complete four-sample batches.

`Timestamp` identifies sample 1. `SampleIntervalTicks` is a positive 16-bit gap
between successive samples, using the same clock. Sample times are Timestamp,
Timestamp + interval, +2 intervals, +3 intervals, modulo 65536. Tick duration
remains unresolved. Corresponding acceleration/gyro batches from one board share
timing; magnetometer batches can use their own cadence.

| Quantity | Encoding | Range |
|---|---|---|
| Chassis acceleration | signed 13-bit, 0.001 g | −4.096–4.095 g |
| Upright acceleration | signed 15-bit, 0.001 g | −16.384–16.383 g |
| Gyro | signed 16-bit, 0.02 deg/s | −655.36–655.34 deg/s |
| Magnetometer | signed 12-bit, 0.15 uT | −307.2–307.05 uT |
| Wheel speed | unsigned 12-bit, 0.5 rpm | 0–2047.5 rpm |

Wheel speed deliberately uses finer resolution than fan tachometers (5 rpm).
All measurement frames fit CAN-FD's 64-byte limit.
