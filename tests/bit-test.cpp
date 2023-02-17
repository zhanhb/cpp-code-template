#include <bits/stdc++.h>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-macro-redefined"
#pragma push_macro("__has_builtin")
#define __has_builtin(x) 0 // NOLINT(bugprone-reserved-identifier)
#pragma GCC diagnostic pop
#endif

#include "common/bit.h"

#ifdef __GNUC__
#pragma pop_macro("__has_builtin")
#endif

#include "extension/bit/bit_cast.h"

int main() {
    for (uint32_t x = -100; x != 100; ++x) {
        assert(popcount64((uint64_t) x) == __builtin_popcountll((uint64_t) x));
        assert(popcount32(x) == __builtin_popcount(x));
        if (x) assert(ctz32(x) == __builtin_ctz(x));
    }
    for (uint64_t x = -100; x != 100; ++x) {
        assert(popcount64(x) == __builtin_popcountll(x));
        if (x) assert(ctz64(x) == __builtin_ctzll(x));
    }
    for (unsigned i = 0; i != sizeof(uint64_t) * CHAR_BIT; ++i) {
        uint64_t x = uint64_t(1) << i;
        assert(popcount64(x) == __builtin_popcountll(x));
        if (x) assert(ctz64(x) == __builtin_ctzll(x));
    }

    using extension::bit_cast;
    assert(0 == bit_cast<int>(0.f));
    assert(INT_MIN == bit_cast<int>(-0.f));
    assert(0 == bit_cast<int64_t>(0.));
    assert(INT64_MIN == bit_cast<int64_t>(-0.));
}
