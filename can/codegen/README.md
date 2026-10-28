# DBC → C codegen

Generate standalone, allocation-free C11 codecs from the canonical bus DBCs.
The public API accepts **physical values**, handles scaling/offsets, and packs
into a transport-independent frame. The pinned cantools backend handles bit
layout in a private codec. No STM32 HAL, RTOS, or Python dependency is needed
at firmware runtime.

## Generate

From the repository root, using Python 3.10 or newer:

```sh
uv sync --locked
uv run --locked python can/codegen/generate_can_headers.py -o can/generated
```

By default this reads every `can/dbc/*.dbc`. To select buses explicitly:

```sh
uv run --locked python can/codegen/generate_can_headers.py \
  can/dbc/CarCAN.dbc can/dbc/MotorCAN.dbc -o build/generated/can
```

Each DBC describes one physical bus. Empty scaffold files generate valid empty
bus modules. They do not supply vehicle messages; the examples below use the
**test fixture**, not an assigned vehicle protocol.

Output:

```text
can/generated/
  can_types.h              Shared frame, status and metadata types
  car_can.h / car_can.c     Physical fields, sender groups, pack/unpack
  motor_can.h / motor_can.c
  detail/*_raw.h / *.c      Private raw-bit codecs
  .can-codegen-files       Manifest for removing obsolete generated files
```

Use a dedicated output directory. Generation preserves unrelated files, removes
obsolete files listed in its previous manifest, and avoids rewriting unchanged
C files. Outputs contain no timestamps. Commit all generated files together;
never hand-edit them. CI regenerates and compares every file, including the
manifest, on every push and pull request. Fix a mismatch by running the command
above and committing the result. On a mismatch, CI also uploads a
`regenerated-can-files` artifact for review; it never pushes commits automatically.

Add `can/` as a CMake subdirectory and link the physical buses a board uses:
`target_link_libraries(board PRIVATE can_car can_charger)`. Available targets:
`can_car`, `can_charger`, `can_daq`, `can_elcon`, `can_lighting`, `can_motor`,
`can_mppt`, `can_steering`. Each includes its public and private codecs and math
library. Include the bus header (or `can.h` for all declarations). CI checks the
shared package for every board PR; a board must opt into the buses it uses.

## Use the API

For `tests/fixtures/TestBus.dbc`, generation produces:

```c
#include "test_bus.h"

test_bus_bps_t bps = {0};
can_frame_t frame;

bps.cell_status.cell_voltage_mv = 3721;
bps.cell_status.temperature = 25.25;
bps.cell_status.state = TEST_BUS_CELL_STATUS_STATE_NORMAL;

can_status_t status = test_bus_cell_status_pack(&frame, &bps.cell_status);
if (status >= CAN_OK) {
    /* frame is usable; CAN_CLAMPED means at least one input was clipped. */
    /* Choose frame.bitrate_switch if the bus uses BRS, then send via your driver. */
}

/* Immutable metadata is grouped by the DBC's sender node. */
uint32_t id = test_bus.bps.cell_status.id;

/* Receiving is explicit and uses caller-owned storage. */
status = test_bus_cell_status_unpack(&bps.cell_status, &frame);
if (status == CAN_OK) {
    uint16_t millivolts = bps.cell_status.cell_voltage_mv;
    (void)millivolts;
}
(void)id;
```

Message structs can also be used directly without a sender container. Messages
with multiple senders appear in each sender group; the codec/type is shared.
Messages without a sender appear under `unassigned`. These groups describe
message ownership, not a board's receive subscriptions. No mutable global
message instances are generated; independent instances can be used by different
tasks, subject to normal synchronization if an instance is shared.

Names normalize to snake_case. Ambiguous normalized names, C keywords in fields,
and conflicting public symbols fail generation rather than silently renaming.
Generated public comments describe units, conversion, permitted raw limits,
inputs/outputs and error behavior.

## Physical values and quantization

The DBC controls units and the equation:

```text
physical = raw × scale + offset
raw = round((physical − offset) / scale)
```

A 12-bit unsigned cell signal with scale **1**, offset **2000**, and unit **mV**
accepts `3721` and encodes raw `1721`. Its bit capacity is 2000–6095 mV; declaring
DBC limits 2000–4500 mV narrows the permitted range. The API uses `uint16_t`.
Defining the same wire encoding with unit V, scale 0.001 and offset 2 instead
produces a `double` field accepting `3.721`. Units are never inferred or converted
from their labels.

Integral scales and offsets use the smallest physical integer field that fits
the permitted decoded range. Fractional scales or offsets use `double`, even
when individual physical inputs happen to be integers. Integer variables of
other widths can be assigned using normal C conversion rules; values must fit
the generated field type **before assignment**. A packer cannot detect a value
that already wrapped during assignment to an undersized C field.

Packing:

- Rounds to nearest raw step; exact ties go away from zero in raw space.
- Clamps finite inputs to the reachable range permitted by bit width and DBC
  min/max. Non-grid endpoints use the nearest representable endpoint *inside*
  the limits. Returns `CAN_CLAMPED` when the input exceeds that reachable range.
- Returns `CAN_OK` for ordinary quantization within the reachable range.
- Rejects NaN/infinity and invalid multiplexors with `CAN_INVALID`.
- Leaves the input message unchanged, so unpack the resulting frame if you need
  to inspect the effective transmitted values.

Fractional arithmetic follows C double precision. Identity integer signals use
integer arithmetic throughout, including full-width signed/unsigned 64-bit values.
Scaled conversions exceeding double's exact-integer domain are rejected during
generation. IEEE binary32 wire signals with identity scaling use C `float`, with finite-value
validation and DBC bounds; other IEEE widths/scaling and scaled value-table
signals are rejected explicitly. No byte-level encoding is inferred from host struct layout.

Value tables produce named C enums (or typed-width integer macros for values
outside the C11 enum range). Signal fields retain their integer storage type,
so unknown values can be received, inspected and forwarded without losing them.

## Frame validation and CAN-FD

Both 11-bit standard IDs and 29-bit extended IDs are supported, including the
maximum extended ID. `frame.id` is the actual arbitration ID without the DBC
extended marker. Numeric IDs may repeat across distinct buses or standard/extended
formats; duplicate IDs within the same bus/format fail generation.

CAN-FD is selected by the DBC frame-format metadata, **not** by guessing from
payload length. FD frames of eight bytes or fewer remain FD. Classic payloads
must be at most eight bytes; FD payloads at most 64.

Metadata exposes DBC payload bytes, legal wire bytes and numeric DLC separately.
For example, a 9-byte DBC payload uses 12 wire bytes and DLC 9. Packing zeroes
all unused bytes. BRS defaults to false and is selected explicitly by the caller;
a HAL adapter translates the numeric DLC to the HAL-specific constant.

Unpacking checks ID, standard/extended format, classic/FD format, exact wire
length, multiplexor validity and active signal ranges. Unknown value-table
entries within range remain valid. Nonzero padding is ignored. Invalid received
ranges return `CAN_RANGE`; malformed metadata or multiplexors return `CAN_INVALID`.
**Every negative result preserves the entire destination.** Only active
multiplexed fields are converted; inactive fields are zeroed on successful unpack.
Simple and nested multiplexing, both endian orders, signed signals, negative
scales and cross-byte signal layouts are supported.

## Implementation modules and tests

- `generate_can_headers.py`: command-line arguments and diagnostics.
- `dbc.py`: parsing, namespaces, bus/frame validation.
- `signals.py`: physical types and exact representable endpoint calculations.
- `conversion.py`: checked physical/raw assignment emission.
- `emit.py`: public API and private backend C emission.
- `runtime/can_types.h`: small shared C frame/status contract, copied to output.

```sh
uv run --locked python -m unittest discover -s can/codegen/tests -v
```

Tests compile generated C with strict warnings and address/undefined-behavior
sanitizers, then execute API assertions and 200 deterministic byte vectors from
Python cantools. They also check empty production scaffolds, CLI diagnostics,
name collisions, unsupported contracts, reproducibility and stale-file cleanup.
These are host codec tests, not on-hardware CAN transmission tests.
