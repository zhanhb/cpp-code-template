#include <cassert>

template<class Tree, class Pred>
inline int search(int n, Tree &&tree, Pred &&test, int l) {
    assert(0 <= l && l < n);
    l += n;
    int stack[40], top = 0, id = 0;
    for (int r = n << 1; l < r; l >>= 1, r >>= 1) {
        if (l & 1) {
            if (!test(tree[l])) {
                id = l;
                break;
            }
            ++l;
        }
        if (r & 1) stack[top++] = --r;
    }
    if (!id) {
        while (top) {
            int x = stack[--top];
            if (test(tree[x])) continue;
            id = x;
            break;
        }
        if (!id) return n;
    }
    while (id < n) if (test(tree[id <<= 1])) ++id;
    return id - n;
}
