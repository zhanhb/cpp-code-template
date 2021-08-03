#include <algorithm>
#include <numeric>
#include <vector>

class DisjointSet {
    std::vector<int> father;
public:
    explicit DisjointSet(int n) : father(n) {
        std::iota(father.begin(), father.end(), 0);
    }

    int getf(int x) {
        int &u = father[x];
        return u == x ? u : u = getf(u);
    }

    void merge(int x, int y) {
        x = getf(x), y = getf(y);
        if (x == y) return;
        father[x] = y;
    }
};
