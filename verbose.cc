#include "misc/bit.h"
#include "cstdio"
#include "misc/hash-tuple.h"

void hello(unsigned x) {
    for (unsigned p = x; p; p &= p - 1) {
        printf("%d\n", ctz(p));
    }
}

size_t hash_tuple(std::tuple<int, int> tp) {
    return std::hash<decltype(tp)>()(tp);
}
