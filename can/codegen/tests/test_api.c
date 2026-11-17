/** @file test_api.c
 * @brief Host assertions for the generated API; oracle bytes come from Python cantools.
 * Input: generated TestBus codecs and oracle.h. Output: success or a failed assertion.
 */
#include "test_bus.h"
#include "oracle.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

int main(void) {
    test_bus_bps_t bps = {0};
    can_frame_t frame = {0};
    assert(test_bus.bps.cell_status.id == UINT32_C(0x18ff50e5));
    assert(test_bus.bps.cell_status.is_extended && test_bus.bps.cell_status.is_fd);
    assert(test_bus.bps.cell_status.payload_bytes == 64);
    assert(test_bus.bps.cell_status.dlc == 15);
    bps.cell_status.cell_voltage_mv = 3721;
    bps.cell_status.temperature = 25.25;
    bps.cell_status.state = TEST_BUS_CELL_STATUS_STATE_FAULT;
    bps.cell_status.tail = 0xa5;
    assert(test_bus_cell_status_pack(&frame, &bps.cell_status) == CAN_OK);
    assert(memcmp(frame.data, cell_oracle, 64) == 0);
    assert(frame.is_extended && frame.is_fd && frame.len == 64);
    assert(!frame.bitrate_switch);
    assert(test_bus_cell_status_unpack(&bps.cell_status, &frame) == CAN_OK);
    assert(bps.cell_status.cell_voltage_mv == 3721);
    assert(bps.cell_status.temperature == 25.5);
    assert(bps.cell_status.tail == 0xa5);

    bps.cell_status.cell_voltage_mv = 65000;
    bps.cell_status.temperature = -10000;
    assert(test_bus_cell_status_pack(&frame, &bps.cell_status) == CAN_CLAMPED);
    assert(test_bus_cell_status_unpack(&bps.cell_status, &frame) == CAN_OK);
    assert(bps.cell_status.cell_voltage_mv == 4500);
    assert(bps.cell_status.temperature == -40);
    bps.cell_status.cell_voltage_mv = 0;
    bps.cell_status.temperature = 10000;
    assert(test_bus_cell_status_pack(&frame, &bps.cell_status) == CAN_CLAMPED);
    assert(test_bus_cell_status_unpack(&bps.cell_status, &frame) == CAN_OK);
    assert(bps.cell_status.cell_voltage_mv == 2000);
    assert(bps.cell_status.temperature == 150);

    /* Nonfinite inputs and bad metadata must preserve caller-owned outputs. */
    can_frame_t saved = frame;
    bps.cell_status.temperature = NAN;
    assert(test_bus_cell_status_pack(&frame, &bps.cell_status) == CAN_INVALID);
    assert(memcmp(&saved, &frame, sizeof(frame)) == 0);
    bps.cell_status.temperature = INFINITY;
    assert(test_bus_cell_status_pack(&frame, &bps.cell_status) == CAN_INVALID);
    test_bus_cell_status_t before = bps.cell_status;
    frame.is_extended = false;
    assert(test_bus_cell_status_unpack(&bps.cell_status, &frame) == CAN_INVALID);
    assert(memcmp(&before, &bps.cell_status, sizeof(before)) == 0);
    frame = saved;
    frame.len = 63;
    assert(test_bus_cell_status_unpack(&bps.cell_status, &frame) == CAN_INVALID);
    frame = saved;
    frame.data[0] = 0xff;
    frame.data[1] = 0x0f; /* Raw 4095 exceeds the permitted cell operating range. */
    assert(test_bus_cell_status_unpack(&bps.cell_status, &frame) == CAN_RANGE);
    assert(memcmp(&before, &bps.cell_status, sizeof(before)) == 0);
    assert(test_bus_cell_status_pack(NULL, &bps.cell_status) == CAN_INVALID);
    assert(test_bus_cell_status_unpack(&bps.cell_status, NULL) == CAN_INVALID);

    /* Full-width identity integers must never travel through floating point. */
    bps.counter.value = UINT64_MAX;
    assert(test_bus_counter_pack(&frame, &bps.counter) == CAN_OK);
    for (size_t i = 0; i < 8; ++i)
        assert(frame.data[i] == 255);
    assert(test_bus_counter_unpack(&bps.counter, &frame) == CAN_OK);
    assert(bps.counter.value == UINT64_MAX);
    bps.signed_counter.value = INT64_MIN;
    assert(test_bus_signed_counter_pack(&frame, &bps.signed_counter) == CAN_OK);
    assert(test_bus_signed_counter_unpack(&bps.signed_counter, &frame) == CAN_OK);
    assert(bps.signed_counter.value == INT64_MIN);
    bps.signed_counter.value = INT64_MAX;
    assert(test_bus_signed_counter_pack(&frame, &bps.signed_counter) == CAN_OK);
    assert(test_bus_signed_counter_unpack(&bps.signed_counter, &frame) == CAN_OK);
    assert(bps.signed_counter.value == INT64_MAX);

    bps.signed_big.current = -10.75; /* Raw -1.5 rounds away from zero to -2. */
    bps.signed_big.negative_scale = -101;
    assert(test_bus_signed_big_pack(&frame, &bps.signed_big) == CAN_CLAMPED);
    assert(memcmp(frame.data, signed_oracle, 8) == 0);
    assert(test_bus_signed_big_unpack(&bps.signed_big, &frame) == CAN_OK);
    assert(bps.signed_big.current == -11);
    assert(bps.signed_big.negative_scale == -100);

    bps.padded.tail = 42;
    assert(test_bus_padded_pack(&frame, &bps.padded) == CAN_OK);
    assert(frame.len == 12 && test_bus.bps.padded.dlc == 9);
    assert(frame.data[8] == 42);
    for (size_t i = 9; i < 64; ++i)
        assert(frame.data[i] == 0);
    assert(test_bus_padded_unpack(&bps.padded, &frame) == CAN_OK);

    bps.mux.mode = 0;
    bps.mux.first = 17;
    bps.mux.second = NAN; /* Inactive branch is not validated or encoded. */
    assert(test_bus_mux_pack(&frame, &bps.mux) == CAN_OK);
    assert(frame.data[0] == 0 && frame.data[1] == 17);
    assert(test_bus_mux_unpack(&bps.mux, &frame) == CAN_OK);
    assert(bps.mux.first == 17 && bps.mux.second == 0);
    bps.mux.mode = 1;
    bps.mux.second = 12.25;
    assert(test_bus_mux_pack(&frame, &bps.mux) == CAN_OK);
    assert(frame.data[1] == 25);
    assert(test_bus_mux_unpack(&bps.mux, &frame) == CAN_OK);
    assert(bps.mux.second == 12.5 && bps.mux.first == 0);
    bps.mux.mode = 2;
    saved = frame;
    assert(test_bus_mux_pack(&frame, &bps.mux) == CAN_INVALID);
    assert(memcmp(&saved, &frame, sizeof(frame)) == 0);
    frame.data[0] = 2;
    assert(test_bus_mux_unpack(&bps.mux, &frame) == CAN_INVALID);

    /* Standard and extended frames can share numeric IDs without being interchangeable. */
    bps.extended_small.value = 7;
    assert(test_bus_extended_small_pack(&frame, &bps.extended_small) == CAN_OK);
    assert(frame.id == 0x123 && frame.is_extended && !frame.is_fd);
    frame.is_extended = false;
    assert(test_bus_extended_small_unpack(&bps.extended_small, &frame) == CAN_INVALID);
    frame.is_extended = true;
    frame.bitrate_switch = true;
    assert(test_bus_extended_small_unpack(&bps.extended_small, &frame) == CAN_INVALID);

    assert(test_bus_empty_pack(&frame, &bps.empty) == CAN_OK);
    assert(frame.len == 0);
    assert(test_bus_empty_unpack(&bps.empty, &frame) == CAN_OK);
    bps.wide.value = 0;
    assert(test_bus_wide_pack(&frame, &bps.wide) == CAN_CLAMPED);
    assert(test_bus_wide_unpack(&bps.wide, &frame) == CAN_OK);
    assert(bps.wide.value == 1);
    bps.wide.value = UINT64_MAX;
    assert(test_bus_wide_pack(&frame, &bps.wide) == CAN_CLAMPED);
    assert(test_bus_wide_unpack(&bps.wide, &frame) == CAN_OK);
    assert(bps.wide.value == UINT64_C(9223372036854775807));
    bps.narrow.value = 255;
    assert(test_bus_narrow_pack(&frame, &bps.narrow) == CAN_CLAMPED);
    assert(test_bus_narrow_unpack(&bps.narrow, &frame) == CAN_OK);
    assert(bps.narrow.value == 100);
    frame.data[0] = 255;
    assert(test_bus_narrow_unpack(&bps.narrow, &frame) == CAN_RANGE);

    bps.nested.mode = 0;
    bps.nested.sub_mode = 1;
    bps.nested.value = 31;
    assert(test_bus_nested_pack(&frame, &bps.nested) == CAN_OK);
    assert(frame.data[0] == 0 && frame.data[1] == 1 && frame.data[2] == 31);
    assert(test_bus_nested_unpack(&bps.nested, &frame) == CAN_OK);
    assert(bps.nested.value == 31);
    bps.nested.mode = 1;
    bps.nested.sub_mode = 255; /* Inactive selector is ignored. */
    bps.nested.other = 42;
    assert(test_bus_nested_pack(&frame, &bps.nested) == CAN_OK);
    assert(test_bus_nested_unpack(&bps.nested, &frame) == CAN_OK);
    assert(bps.nested.other == 42 && bps.nested.value == 0);
    bps.small_fd.value = 9;
    assert(test_bus_small_fd_pack(&frame, &bps.small_fd) == CAN_OK);
    assert(frame.is_fd && !frame.is_extended && frame.len == 8);
    frame.bitrate_switch = true;
    assert(test_bus_small_fd_unpack(&bps.small_fd, &frame) == CAN_OK);
    assert(test_bus.bps.max_id.id == UINT32_C(0x1fffffff));
    assert(test_bus_max_id_pack(&frame, &bps.max_id) == CAN_OK);
    assert(test_bus_max_id_unpack(&bps.max_id, &frame) == CAN_OK);

    for (size_t i = 0; i < sizeof(vectors) / sizeof(vectors[0]); ++i) {
        bps.cell_status.cell_voltage_mv = vectors[i].mv;
        bps.cell_status.temperature = vectors[i].temperature;
        bps.cell_status.state = vectors[i].state;
        bps.cell_status.tail = vectors[i].tail;
        assert(test_bus_cell_status_pack(&frame, &bps.cell_status) == CAN_OK);
        assert(memcmp(frame.data, vectors[i].bytes, 64) == 0);
        assert(test_bus_cell_status_unpack(&bps.cell_status, &frame) == CAN_OK);
        assert(bps.cell_status.temperature == vectors[i].temperature);
        assert(bps.cell_status.state == vectors[i].state); /* Unknown enum values survive. */
    }
    return 0;
}
