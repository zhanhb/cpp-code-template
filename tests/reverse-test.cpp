#include "common/reverse.h"
#include "debug.h"
#include <cassert>

#define println(...) OUT(__VA_ARGS__)

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
        println(__func__);
    }
};

#include <bits/stdc++.h>

void test_case1() {
    OUT("0");
    MyContainer my;
    OUT(reverse(my));
    OUT("1");
    OUT(reverse(MyContainer{}));
    OUT("2");
    reverse(MyContainer{});
    OUT("3");
    for (auto x: reverse(MyContainer{})) {
        OUT(x);
    }
    OUT("4");
}

void test_case2() {
    std::vector<int> v{1, 2, 3, 4, 5};
    OUT(reverse(v));
    OUT(reverse(std::move(v)));
}

void test_case3() {
    int a[5] = {1, 2, 3, 4};
    OUT(reverse(a));
    OUT(reverse(std::initializer_list<int>{1, 2, 3, 4}));
    auto rev = reverse(a);
    OUT(rev);
}

int main() {
    test_case1();
    test_case2();
    test_case3();
}
