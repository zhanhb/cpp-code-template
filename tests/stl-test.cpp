#include <cassert>
#include <deque>
#include <map>
#include <vector>

using namespace std;

void test_vector() {
    vector<int> v{1};
    auto p = v.begin();
    assert(v.capacity() == 1);
    v.resize(2);
    assert(v.capacity() == 2);
    assert(p != v.begin());
}

void test_map() {
    map<int, int> mp;
    mp[1] = 2;
    mp[2] = 3;
    auto e = mp.end();
    mp.erase(prev(mp.end()));
    assert(e == mp.end());
    mp.clear();
    assert(e == mp.end());
}

void test_deque() {
    deque<int> dq{1, 2, 3, 4, 5};
    auto p = next(dq.begin());
    dq.erase(dq.begin());
    assert(p == dq.begin());
}

int main() {
    test_deque();
    test_map();
    test_vector();
}