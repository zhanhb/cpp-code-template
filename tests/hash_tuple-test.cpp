#include "common/hash-tuple.h"
#include "debug.h"
#include <bits/stdc++.h>
#include "static_assert.h"

using namespace std;

int main() {
    {
        typedef tuple<int, int, int> K;
        unordered_map<K, int> mp;

        mp[make_tuple(1, 2, 3)] = 1;
        OUT(mp);
    }
    {
        typedef tuple<int, tuple<int, int>, int> K;
        unordered_map<K, int> mp;
        mp[forward_as_tuple(1, make_tuple(2, 3), 4)] = 1;
        OUT(mp);
    }
    {
        typedef tuple<tuple<int, int>, int, int> K;
        unordered_map<K, int> mp;
        mp[make_tuple(make_tuple(1, 2), 3, 4)] = 1;
        OUT(mp);
    }
//    STATIC_ASSERT_NOT_V(hash_tuple_detail::has_enabled_hash, tuple<char>);
    STATIC_ASSERT_V(hash_tuple_detail::has_enabled_hash, tuple<char, char>);
    STATIC_ASSERT_V(std::is_empty, hash<tuple<char, char> >);
    STATIC_ASSERT_V(
            std::is_default_constructible,
            hash<
                    tuple<
                            char,
                            unsigned char,
                            short,
                            unsigned short,
                            int,
                            unsigned,
                            long,
                            unsigned long,
                            tuple<
                                    double,
                                    long
                            >,
                            int,
                            int
                    >
            >);
    STATIC_ASSERT_NOT_V(hash_tuple_detail::has_enabled_hash, tuple<>);
//    STATIC_ASSERT_NOT_V(hash_tuple_detail::has_enabled_hash, tuple<int>);
    STATIC_ASSERT_V(hash_tuple_detail::has_enabled_hash, tuple<int, int>);
    STATIC_ASSERT_NOT_V(hash_tuple_detail::has_enabled_hash, tuple<int &, int>);
    STATIC_ASSERT_V(hash_tuple_detail::has_enabled_hash, tuple<tuple<int, int>, int>);
    STATIC_ASSERT_NOT_V(hash_tuple_detail::has_enabled_hash, tuple<std::atomic_int, int>);
}
