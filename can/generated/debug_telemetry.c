#include "debug_telemetry.h"
#include "detail/debug_telemetry_raw.h"
#include <math.h>
#include <string.h>
#include <float.h>
_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128, "IEEE-754 binary32 is required");

const debug_telemetry_metadata_t debug_telemetry = {
    .reserved = 0,
};
