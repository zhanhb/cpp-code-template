#include <bits/stdc++.h>
//#include "leetcode/list-node.h"
//#include "leetcode/tree-node.h"
#include "debug.h"

using namespace std;

#include "solution.hpp"
#include "measure.h"
#include "string-repeat.h"

int main() {
    [[maybe_unused]] typedef uniform_int_distribution<> D;
    [[maybe_unused]] mt19937 gen{random_device{}()};

    std::clog << std::boolalpha << std::setprecision(std::numeric_limits<double>::max_digits10 + 1);

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    std::cerr.tie(nullptr), std::clog.tie(nullptr);

    auto s = measure([] { return Solution{}; });

}
