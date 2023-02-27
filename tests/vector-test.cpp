#include "debug.h"
#include <bits/stdc++.h>

using namespace std;

#include "measure.h"

static long copy_assign;
static long copy_construct;
static long compare_cnt;

class Z {
    int x{}, y{};

public:

    explicit Z(int x, int y) : x(x), y(y) {}

    Z() = default;

    Z(const Z &) { ++copy_construct; }

    Z(Z &&) = default;

    Z &operator=(const Z &) {
        ++copy_assign;
        return *this;
    }

    friend bool operator<(Z a, Z b) { // NOLINT(performance-unnecessary-value-param)
        ++compare_cnt;
        return forward_as_tuple(a.x, a.y) < forward_as_tuple(b.x, b.y);
    }

};

int main(int argc, char **argv) {
    mt19937 gen{random_device{}()};
    using D = uniform_int_distribution<>;
//    for (int i = 0; i < 10; ++i) {
////        int n = D{1, 100}(gen);
//        int n = 100;
//        vector<int> v(n);
//        std::generate(v.begin(), v.end(), [&] { return D{1, 10000}(gen); });
//        measure();
//
//        out(s.minDistance(v, D{1, n}(gen)));
//    }
    {
        vector<vector<int> > vv;
        measure([&] {
            int n = 1e5;
            vv.reserve(n);
            for (int i = 0; i < n; ++i) {
                vv.emplace_back(vector<int>{D(1, 10000)(gen), D(1, 10000)(gen)});
            }
        });
        scoped_measure();
        sort(vv.begin(), vv.end(), [](auto x, auto y) { return x < y; }); // test vector<int> by value
    }
    {
        int n = 1e5;
        vector<Z> vv;
        vv.reserve(n);
        for (int i = 0; i < n; ++i) {
            vv.emplace_back(D{1, (int) 1e4}(gen), D{1, (int) 1e4}(gen));
        }
        scoped_measure();
        copy_assign = 0;
        copy_construct = 0;
        compare_cnt = 0;
        sort(vv.begin(), vv.end());
        out(copy_assign, copy_construct, compare_cnt);
    }
}
