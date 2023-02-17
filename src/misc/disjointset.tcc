#include <algorithm>
#include <numeric>
#include <vector>

class DisjointSet {
    std::vector<int> father{}, rank{}, cnt{};
public:
    explicit DisjointSet(int n) : father(n), rank(n), cnt(n, 1) {
        std::iota(father.begin(), father.end(), 0);
    }

    int getf(int x) {
        int &u = father[x];
        return u == x ? u : u = getf(u);
    }

    void merge(int x, int y) {
        x = getf(x), y = getf(y);
        if (x == y) return;
        if (rank[x] == rank[y]) ++rank[x];
        if (rank[x] < rank[y]) {
            father[x] = y;
            cnt[y] += cnt[x];
            cnt[x] = 0;
        } else {
            father[y] = x;
            cnt[x] += cnt[y];
            cnt[y] = 0;
        }
    }
};
