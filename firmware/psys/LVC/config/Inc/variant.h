#pragma once

// Per-variant settings: the build defines BOARD_VARIANT_<NAME> for the variant being built
// (BOARD_VARIANTS in Board.cmake), and the values that differ between variants live here.
// TODO: drop the demo variants when real LVC firmware lands
#ifdef BOARD_VARIANT_BUILD1
#define BLINK_PERIOD_MS 500
#elif defined(BOARD_VARIANT_BUILD2)
#define BLINK_PERIOD_MS 250
#else
#error "Unknown LVC variant -- build with `make VARIANT=<name>` (see BOARD_VARIANTS in Board.cmake)"
#endif
