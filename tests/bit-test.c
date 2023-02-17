#include <assert.h>

#ifdef __cplusplus
#error this file should not be treat as c++ source file
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-macro-redefined"
#pragma push_macro("__has_builtin")
#define __has_builtin(x) 0 /* NOLINT(bugprone-reserved-identifier) */
#pragma GCC diagnostic pop
#endif

#include "common/bit.h"

#ifdef __GNUC__
#pragma pop_macro("__has_builtin")
#endif

#include <limits.h>

int main(void) {
    uint32_t ui32;
    uint64_t ui64;
    unsigned i;
    for (ui32 = -100; ui32 != 100; ++ui32) {
        assert(popcount64((uint64_t) ui32) == __builtin_popcountll((uint64_t) ui32));
        assert(popcount32(ui32) == __builtin_popcount(ui32));
        if (ui32) assert(ctz32(ui32) == __builtin_ctz(ui32));
    }
    for (ui64 = -100; ui64 != 100; ++ui64) {
        assert(popcount64(ui64) == __builtin_popcountll(ui64));
        if (ui64) assert(ctz64(ui64) == __builtin_ctzll(ui64));
    }
    for (i = 0; i != sizeof(uint64_t) * CHAR_BIT; ++i) {
        ui64 = UINT64_C(1) << i;
        assert(popcount64(ui64) == __builtin_popcountll(ui64));
        if (ui64) assert(ctz64(ui64) == __builtin_ctzll(ui64));
    }
    return 0;
}
