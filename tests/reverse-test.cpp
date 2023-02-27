#include "misc/reverse.h"
#include "debug.h"

#define println(...) out(__VA_ARGS__)

class MyContainer {
    int a[4]{1, 2, 3, 4};
public:

    MyContainer() {
        println("constructed");
    }

    MyContainer(const MyContainer &) noexcept {
        assert(false);
    }

    MyContainer(MyContainer &&) noexcept {
        println("move constructed");
    }

    std::reverse_iterator<const int *> rbegin() {
        return std::reverse_iterator<const int *>(a + (sizeof(a) / sizeof(a[0])));
    }

    std::reverse_iterator<const int *> rend() {
        return std::reverse_iterator<const int *>(a);
    }

    ~MyContainer() {
        println("~MyContainer");
    }
};

#include <bits/stdc++.h>

using namespace std;

void test_case1() {
    out("0");
    MyContainer my;
    out(reverse(my));
    out("1");
    out(reverse(MyContainer{}));
    out("2");
    reverse(MyContainer{});
    out("3");
    for (auto x: reverse(MyContainer{})) {
        out(x);
    }
    out("4");
}

void test_case2() {
    vector<int> v{1, 2, 3, 4, 5};
    out(reverse(v));
    out(reverse(std::move(v)));
}

int main() {
    test_case1();
    set<int> st;
    auto p = st.begin();
    auto r = reverse_iterator<remove_reference<decltype(p)>::type>(p);

    {
        int a = 10;
        static_assert(is_same<decltype(max(a, 10)), const int &>::value, ""); // NOLINT(modernize-unary-static-assert)
        static_assert(is_same<decltype(a), int>::value, ""); // NOLINT(modernize-unary-static-assert)
        static_assert(is_same<decltype(st.rbegin()), decltype(r)>::value, ""); // NOLINT(modernize-unary-static-assert)
    }

    test_case2();

    int a[5] = {1, 2, 3, 4};
    out(reverse(a));
    out(reverse(initializer_list<int>{1, 2, 3, 4}));
    auto b = reverse(a);

//    auto a = reverse(generate());
//    auto b = generate();
//    assert(equal(a.begin(), a.end(), b.rbegin()));
}
