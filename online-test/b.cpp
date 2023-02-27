#include <cstdio>
#include <vector>
#include <set>

using namespace std;

int solve(vector<int> &v, int m) {
    int ans = 0;
    multiset<int, greater<int> > rev;
    multiset<int> st;
    for (int x: v) {
        if (x >= m) ++ans;
        else {
            st.insert(x);
            rev.insert(x);
        }
    }
    for (auto p = rev.begin(); p != rev.end();) {
        int x = *p;
        p = rev.erase(p);
        st.erase(st.find(x));
        auto q2 = st.lower_bound(m - x);
        if (q2 == st.end()) break;
        int y = *q2;

        auto q = rev.find(y);
        if (q == p) p = rev.erase(p);
        else rev.erase(q);

        st.erase(st.find(y));
        ++ans;
    }
    return ans;
}

int main() {
    int n;
    while (1 == scanf("%d", &n)) {
        vector<int> v(n);
        for (int i = 0; i < n; ++i) {
            int x;
            scanf("%d", &v[i]);
        }
        int m;
        scanf("%d", &m);
        printf("%d\n", solve(v, m));
    }
}
