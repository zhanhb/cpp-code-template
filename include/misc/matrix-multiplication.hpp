#include <array>

using std::array;

template<size_t M, size_t P>
array<int, P> operator*(const array<int, M> &a, const array<array<int, P>, M> &b) {
    array<int, P> res{};
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < P; ++j) res[j] = (res[j] + (int64_t) a[i] * b[i][j]) % MOD;
    }
    return res;
}

template<size_t N, size_t M, size_t P>
array<array<int, P>, N> operator*(const array<array<int, M>, N> &a, const array<array<int, P>, M> &b) {
    // intentionally not initialized to improve performance
    array<array<int, P>, N> res; // NOLINT(cppcoreguidelines-pro-type-member-init)
    for (int i = 0; i < N; ++i) res[i] = a[i] * b;
    return res;
}

//    array<array<int, 2>, 1> arr = {{1, 1}};
//    array<array<int, 2>, 2> mul{
//            0, 1,
//            1, 1,
//    };
//    out(mul);
//    for (int i = 0; i < 10; ++i) {
//        arr = arr * mul;
//        out(arr);
//    }
