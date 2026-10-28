# CAN timing and traffic estimates

Run from the repository root:

```sh
uv sync --locked
uv run --locked python can/analysis/calculate_load.py
uv run --locked python can/analysis/calculate_load.py --json /tmp/can-load.json
```

The DBCs own schedules and signal layouts; `../topology.yaml` owns bus settings,
directed forwarding, bridge names and outputs. DBC bus attributes are
synchronized copies; validation rejects drift.
`GenMsgSendType` defaults to `Cyclic`; every cyclic message must explicitly
assign a positive integer `GenMsgCycleTime` in milliseconds. `Triggered` messages may have an explicit repeat period. `None` marks reserved, non-transmittable WS22 IDs. Pending layouts
can have planned timing, but are never treated as approved zero-byte frames.
The tests and calculator reject inconsistent timing. These declarations do not
implement a firmware scheduler or choose receiver watchdog timeouts.

The edited review CSV was applied, including its instruction to match MPPT B/C
to A: power measurements 500 ms, status 1000 ms, SetMode 100 ms, and output
voltage/current limits 200 ms. TimestampSync is 10 ms. Vendor periods describe
the configured/intended traffic; the DBC cannot reconfigure a COTS device.

## Physical buses

| Bus | Format capability | Nominal bitrate | BRS |
|---|---|---:|---|
| CarCAN | CAN-FD | 1 Mbit/s | Off |
| DAqCAN | CAN-FD | 1 Mbit/s | Off |
| ChargerCAN | CAN-FD | 250 kbit/s | Off |
| LightingCAN | Classic CAN | 250 kbit/s | — |
| MotorCAN | Classic CAN | 250 kbit/s | — |
| SteeringCAN | Classic CAN | 500 kbit/s | — |
| ElconCAN | Classic CAN | 250 kbit/s | — |
| MPPTCAN | Classic CAN | 1 Mbit/s | — |

DBC globals `Baudrate`/`DataBaudrate` use bits/second. `VFrameFormat` describes
each frame and `CANFD_BRS=0` explicitly disables BRS. With BRS off the entire FD
frame runs at nominal bitrate. A CAN-FD-capable bus may also carry forwarded
classic frames; forwarding preserves the source frame's format and payload.

ChargerCAN mirrors BPSStatus, BPSPackData, BPSVoltageData, and BPSTemperatureData
from CarCAN, with identical IDs, layouts and periods. ElconInterfaceStatus also
uses FD on ChargerCAN; Elcon's private bus remains classic.

## Forwarding policy

`../topology.yaml` is the only routing configuration. Message selectors are
quoted hex IDs scoped to their source bus; transmitter selectors are DBC node
names. Multiple selector lists are ORed.

- No route forwards nothing; routes are directional.
- `exclude` (default) forwards all defined messages except matches. Empty lists
  forward all defined messages.
- `include` forwards only matches. Empty lists forward nothing.
- Unknown frames are not part of the DBC-derived routing table. Firmware must
  drop unknown IDs and implement the resolved filters; no firmware gateway is
  implemented by these scripts.
- Native identical messages win over forwarded copies. Conflicting same-ID
  definitions are errors. Frame-level loops, invalid bridge connections, unknown
  selectors, duplicate YAML keys and FD-to-classic forwards are rejected.
  Bridge names must be declared in each connected bus's DBC node list; output
  routes check the source bus only. There is no separate YAML node inventory.

Confirmed paths:

| Source | Destination | Bridge | Filter |
|---|---|---|---|
| MotorCAN | CarCAN | VCU | All |
| MPPTCAN | CarCAN | MPPTInterface | Exclude MPPTInterface-origin commands |
| SteeringCAN | CarCAN | DEI | All |
| LightingCAN | CarCAN | DEI | All |
| ElconCAN | ChargerCAN | ElconInterface | All |
| CarCAN | ChargerCAN | BPS | Include `"0x060"`, `"0x070"`, `"0x071"`, `"0x072"` |
| ChargerCAN | CarCAN | BPS | Exclude those four BPS IDs to prevent echo |
| CarCAN | TelemetryTCP | MIKA | All |
| DAqCAN | TelemetryTCP | MIKA | Exclude `"0x620"` (TimestampSync) |

The calculator follows every hop, so forwarded Elcon traffic contributes to
ChargerCAN occupancy as well as CarCAN. No DAqCAN-to-CarCAN route exists. The
internet stream contains CarCAN after forwarding plus filtered DAqCAN; excluded
MPPT commands never bypass CarCAN through a direct TCP route. JSON output keeps
native and routed bus totals separate and lists CarCAN/TCP message inventories.

Edit topology bus settings, then synchronize their DBC metadata:

```sh
uv run --locked python can/analysis/sync_bus_settings.py
uv run --locked python can/analysis/calculate_load.py
```

Synchronization validates changes in memory before writing and preserves IDs,
signal layouts and schedules. Moving oversized native frames to classic CAN
fails until their layouts are redesigned. Non-TCP outputs and routes that loop an individual frame
are intentionally unsupported. Opposite bus directions are allowed when their
filters prevent a frame from returning to a bus already on its forwarding path. No routing JSON needs to be maintained.

## Triggered messages and unfinished layouts

A trigger has no intrinsic frequency. Supply expected events/second using JSON:

```json
{
  "CarCAN/BPSCommand": 0.1,
  "CarCAN/PDUCommand": 0,
  "CarCAN/SweepMPPTs": 0.01,
  "MPPTCAN/MPPTAPerformSweep": 0.01,
  "MPPTCAN/MPPTASweepData": 1.28
}
```

```sh
uv run --locked python can/analysis/calculate_load.py --event-rates /path/to/event-rates.json
```

A sweep command and its response stream are separate rates: count every returned
sample. Unspecified triggered rates remain explicitly unresolved. An explicit
zero means an assumed idle stream. Overrides cannot enable reserved/pending
messages or replace a cyclic schedule. Pending messages have unknown final wire
length and remain listed separately rather than counted as zero-byte traffic.
Additional immediate sends of cyclic messages and fault bursts are not included.

## CAN occupancy model

For each bus the script sums frame time × frames/second. It accounts for standard
vs extended IDs, classic vs ISO CAN-FD, payload length, CRC size, fixed CRC stuff
bits, ACK, EOF and three intermission bits. Dynamic bit stuffing depends on live
payloads, so the report gives an unstuffed lower bound and a conservative
maximum-stuffing upper bound. The FD CRC is 17 bits through 16 payload bytes,
21 bits above 16 bytes, with its fixed stuff bits included.

The optional BRS model splits header/tail and data phases at bit boundaries;
real BRS/delimiter sample-point transitions are approximated. Current buses have
BRS off, so this approximation is not used. Error frames, retransmissions,
startup traffic and arbitration response-time guarantees require measurements
or additional analysis. Bus occupancy is not end-to-end latency.

Sources: [Bosch CAN-FD](https://www.bosch-semiconductors.com/products/ip-modules/can-protocols/can-fd/),
[Infineon ISO CAN-FD format](https://documentation.infineon.com/traveo/docs/yar1680597282748),
[Intrepid bit-stuffing explanation](https://guide.intrepidcs.com/docs/wavebps/can-can-fd-bus/can-can-fd-bus-bit-stuffing/).

## Internet estimate

CAN arbitration/CRC/stuffing bits do not go over TCP. The report separately shows:

1. CAN payload bytes only.
2. Serialized application bytes.
3. Uplink TCP/IP bytes, including segment headers.
4. Reverse TCP ACK bytes.

The serializer is not implemented here. Defaults are an explicit planning model:
10 metadata bytes/frame (ID, flags, length, timestamp), 20 bytes/batch, 20 ms
batching, 1500-byte IP MTU, IPv4/TCP headers of 20+20 bytes and one ACK/two
segments. Each batch is modeled with a partial final segment as needed; actual
message phasing, TCP coalescing and ACK behavior can change packet counts.
These numbers do not claim compatibility with last year's classic-only CANP
serializer (which cannot carry CAN-FD payloads unchanged).

Change assumptions to match the actual serializer and link:

```sh
uv run --locked python can/analysis/calculate_load.py --record-header-bytes 10 \
  --batch-header-bytes 20 --batch-ms 20 --mtu 1500 \
  --ip-header-bytes 20 --tcp-header-bytes 32 --ack-every 2 \
  --tls-record-bytes 22
```

TLS overhead is a supplied allowance per batch, not a full TLS record simulator.
TCP reconnects/retransmissions, radio/modem framing, GPS/video and unrelated
traffic are excluded. All kbps are decimal (1000 bits/second). Use the JSON
assumptions and unresolved list when assessing the estimate's practical limits.
