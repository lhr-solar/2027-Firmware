/** @file bootloading.h
 * @brief Generated physical-value API. Edit the DBC, never this file.
 * Pack: physical fields -> zero-padded frame; unpack: frame -> physical fields.
 * Pack returns CAN_OK/CAN_CLAMPED on success. Negative status leaves output unchanged.
 * Initialize instances with {0}; only active multiplexed fields are processed.
 */
#pragma once
#include "can_types.h"

/** Immutable metadata grouped by sender, for ID/filter/driver configuration. */
typedef struct {
    uint8_t reserved;
} bootloading_metadata_t;
extern const bootloading_metadata_t bootloading;
