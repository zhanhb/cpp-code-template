#include <array>

using std::array;

template<size_t Mp, size_t Op>
array<int, Op> operator*(const array<int, Mp> &a, const array<array<int, Op>, Mp> &b) {
    array<int, Op> res{};
    for (unsigned i = 0; i != Mp; ++i) {
        for (unsigned j = 0; j != Op; ++j) {
            res[j] = (res[j] + (int64_t) a[i] * b[i][j]) % MOD;
        }
    }
    return res;
}

template<size_t Np, size_t Mp, size_t Op>
array<array<int, Op>, Np> operator*(const array<array<int, Mp>, Np> &a, const array<array<int, Op>, Mp> &b) {
    // intentionally not initialized to improve performance
    array<array<int, Op>, Np> res; // NOLINT(cppcoreguidelines-pro-type-member-init)
    for (int i = 0; i < Np; ++i) res[i] = a[i] * b;
    return res;
}

//    array<array<int, 2>, 1> arr = {{1, 1}};
//    array<array<int, 2>, 2> mul{
//            0, 1,
//            1, 1,
//    };
//    OUT(mul);
//    for (int i = 0; i < 10; ++i) {
//        arr = arr * mul;
//        OUT(arr);
//    }
