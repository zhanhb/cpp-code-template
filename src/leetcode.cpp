#include <bits/stdc++.h>
//#include "leetcode/list-node.h"
//#include "leetcode/tree-node.h"
#include "debug.h"

using namespace std;

#include "solution.hpp"
#include "measure.h"
#include "misc/reverse.h"
#include "string-repeat.h"

template<class CharT, class Traits>
class scope_locale {

    struct nop {
        template<typename T>
        constexpr void operator()(T const &) const noexcept {}
    };

    std::unique_ptr<std::basic_ios<CharT, Traits>, nop> ios;
    std::locale loc;
public:
    scope_locale(std::basic_ios<CharT, Traits> &ios, const std::locale &loc) : ios(&ios), loc(ios.imbue(loc)) {}

    scope_locale(std::basic_ios<CharT, Traits> &ios, const char *name) : scope_locale(ios, std::locale(name)) {}

    ~scope_locale() {
        if (ios) ios->imbue(loc), ios = nullptr;
    }
};

int main() {
    scope_locale sl_out(cout, "");
    scope_locale sl_err(cerr, "");
    scope_locale sl_wout(wcout, "");
    scope_locale sl_werr(wcerr, "");
    [[maybe_unused]] typedef uniform_int_distribution<> D;
    [[maybe_unused]] mt19937 gen{random_device{}()};

    std::cout << std::boolalpha << std::setprecision(std::numeric_limits<double>::max_digits10 + 1);

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    std::cerr.tie(nullptr), std::clog.tie(nullptr);

    vector<int> m((int *) nullptr, (int *) nullptr, allocator<int>{}, nullptr);

    auto s = measure([] { return Solution{}; });
    (void) s;
}
