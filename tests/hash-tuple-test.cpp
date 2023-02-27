#include "misc/hash-tuple.h"
#include "debug.h"
#include <bits/stdc++.h>

using namespace std;

class good_id {
    static int g_id;
    std::shared_ptr<int> ptr;
public:
    good_id() : ptr(std::make_shared<int>(++g_id)) {}

    good_id(const good_id &another) noexcept: ptr(another.ptr) {
        out("copied constructed");
    }

    good_id(good_id &&another) noexcept: ptr(std::move(another).ptr) {
        out("moved constructed");
    }

    good_id &operator=(const good_id &another) noexcept { // NOLINT(bugprone-unhandled-self-assignment)
        ptr = another.ptr;
        out("copied");
        return *this;
    }

    good_id &operator=(good_id &&another) noexcept {
        ptr = std::move(another.ptr);
        out("moved");
        return *this;
    }

    explicit operator int() const {
        return bool(ptr) ? *ptr : 0;
    }

    friend inline bool operator==(const good_id &a, const good_id &b) { return a.ptr == b.ptr; }

    friend inline bool operator!=(const good_id &a, const good_id &b) { return !(a == b); }
};

int good_id::g_id = 0;

template<>
struct std::hash<good_id> {
    size_t operator()(const good_id &good) const {
        return int(good);
    }
};

int main() {
    typedef typename std::tuple_element<0, tuple<int &&> const>::type ele_t;
    static_assert(std::is_same<ele_t, int &&>::value, ""); // NOLINT(modernize-unary-static-assert)
    {
        hash<tuple<good_id, good_id> > h;
        good_id a, b, c;

        assert(int(a) == 1);
        assert(int(b) == 2);
        assert(int(c) == 3);

        auto tmp = forward_as_tuple(std::move(a), std::move(b));

        h(tmp);

        assert(int(get<0>(tmp)) == 1);
        assert(int(get<1>(tmp)) == 2);
        std::unordered_map<tuple<good_id, good_id>, int> mp;
        mp.emplace(std::move(tmp), 1);
    }

    {
        typedef tuple<int, int, int> K;
        unordered_map<K, int> mp;

        mp[make_tuple(1, 2, 3)] = 1;
        out(mp);
    }
    {
        typedef tuple<int, tuple<int, int>, int> K;
        unordered_map<K, int> mp;
        mp[make_tuple(1, make_tuple(2, 3), 4)] = 1;
        out(mp);
    }
    {
        typedef tuple<tuple<int, int>, int, int> K;
        unordered_map<K, int> mp;
        mp[make_tuple(make_tuple(1, 2), 3, 4)] = 1;
        out(mp);
    }
    out(sizeof(hash<tuple<char> >)); // 8
    static_assert(std::is_default_constructible<hash<tuple< // NOLINT(modernize-unary-static-assert)
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
    > > >::value, "");

    static_assert(!hash_tuple_detail::has_enabled_hash<tuple<> >::value, ""); // NOLINT(modernize-unary-static-assert)
    static_assert(hash_tuple_detail::has_enabled_hash<tuple<int> >::value, ""); // NOLINT(modernize-unary-static-assert)
    static_assert(!hash_tuple_detail::has_enabled_hash<tuple<int &> >::value,
                  ""); // NOLINT(modernize-unary-static-assert)
    static_assert(hash_tuple_detail::has_enabled_hash<tuple<tuple<int> > >::value,
                  ""); // NOLINT(modernize-unary-static-assert)
    static_assert(!hash_tuple_detail::has_enabled_hash<tuple<std::atomic_int> >::value,
                  ""); // NOLINT(modernize-unary-static-assert)

}
