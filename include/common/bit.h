#pragma once

#include <stdint.h> /* NOLINT(modernize-deprecated-headers) */

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if defined(__GNUC__) && !(__cplusplus >= 201103L)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#endif

#ifndef CONSTEXPR_14
#if __cplusplus >= 201402L
#define CONSTEXPR_14 constexpr
#else
#define CONSTEXPR_14
#endif
#endif

#ifndef CONSTEXPR_11
#if __cplusplus >= 201103L
#define CONSTEXPR_11 constexpr
#else
#define CONSTEXPR_11
#endif
#endif

CONSTEXPR_14 int ctz32(uint32_t x) {
#if __has_builtin(__builtin_ctz)
    return __builtin_ctz(x);
#else
    CONSTEXPR_11 const char deBruijnBitPosition[32] = {
            0, 1, 28, 2, 29, 14, 24, 3,
            30, 22, 20, 15, 25, 17, 4, 8,
            31, 27, 13, 23, 21, 19, 16, 7,
            26, 12, 18, 6, 11, 5, 10, 9,
    };
    return deBruijnBitPosition[(x & -x) * 0x077CB531U >> 27];
#endif
}

CONSTEXPR_14 int ctz64(uint64_t x) {
#if __has_builtin(__builtin_ctzll)
    return __builtin_ctzll(x);
#else
    CONSTEXPR_11 char table[64] = {
            0, 1, 12, 2, 13, 22, 17, 3,
            14, 33, 23, 36, 18, 58, 28, 4,
            62, 15, 34, 26, 24, 48, 50, 37,
            19, 55, 59, 52, 29, 44, 39, 5,
            63, 11, 21, 16, 32, 35, 57, 27,
            61, 25, 47, 49, 54, 51, 43, 38,
            10, 20, 31, 56, 60, 46, 53, 42,
            9, 30, 45, 41, 8, 40, 7, 6,
    };
    return table[(x & -x) * 0x03f08c5392f756cdULL >> 58];
#endif
}

CONSTEXPR_14 int popcount32(uint32_t x) {
#if __has_builtin(__builtin_popcount)
    return __builtin_popcount(x);
#else
    x -= x >> 1 & 0x55555555;
    x = (x & 0x33333333) + (x >> 2 & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x += x >> 8;
    return (int) ((x + (x >> 16)) & 0x3F);
#endif
}

CONSTEXPR_14 int popcount64(uint64_t x) {
#if __has_builtin(__builtin_popcountll)
    return __builtin_popcountll(x);
#else
    x -= x >> 1 & 0x5555555555555555LL;
    x = (x & 0x3333333333333333LL) + (x >> 2 & 0x3333333333333333LL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FLL;
    x += x >> 8;
    x += x >> 16;
    return (int) ((x + (x >> 32)) & 0x7F);
#endif
}

#if __SIZEOF_INT128__ == 16

__extension__ CONSTEXPR_14 int ctz128(unsigned __int128 x) {
    return (uint64_t) x ? ctz64((uint64_t) x) : 64 + ctz64((uint64_t) (x >> 64));
}

__extension__ CONSTEXPR_14 int popcount128(unsigned __int128 x) {
    return popcount64((uint64_t) x) + popcount64((uint64_t) (x >> 64));
}

#endif

#if defined(__GNUC__) && !(__cplusplus >= 201103L)
#pragma GCC diagnostic pop
#endif
