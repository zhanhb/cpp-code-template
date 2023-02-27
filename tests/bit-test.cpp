#include <cassert>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbuiltin-macro-redefined"
#pragma push_macro("__has_builtin")
#undef __has_builtin
#define __has_builtin(x) 0 // NOLINT(bugprone-reserved-identifier)
#pragma clang diagnostic pop

#include "misc/bit.h"

#pragma pop_macro("__has_builtin")

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
}
