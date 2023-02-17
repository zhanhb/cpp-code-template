#include <cstdint>
#include <vector>

class Combine {
    std::vector<int> fac, inv;

    static int mod_inverse(int x) {
        int64_t ans = 1;
        for (int t; (t = MOD % x); x = t) ans = ans * (MOD - MOD / x) % MOD;
        return int(ans);
    }

public:
    explicit Combine(int n) : fac(n), inv(n) {
        for (int i = fac[0] = 1; i < n; ++i) fac[i] = fac[i - 1] * int64_t(i) % MOD;
        inv[n - 1] = mod_inverse(fac[n - 1]);
        for (int i = n; --i > 0;) inv[i - 1] = inv[i] * int64_t(i) % MOD;
        assert(inv[0] == 1);
    }

    int operator()(int n, int m) const {
        assert(0 <= m && m <= n);
        return int64_t(fac[n]) * inv[m] % MOD * inv[n - m] % MOD;
    }
} const C(N); // NOLINT(cert-err58-cpp)
