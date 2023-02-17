#pragma once

#include <iterator>
#include "../config.h"

namespace extension {
#if __cpp_lib_make_reverse_iterator
    using std::make_reverse_iterator;
#else

    template<class Iter>
    inline CONSTEXPR_11 std::reverse_iterator<Iter> make_reverse_iterator(Iter i) {
        return std::reverse_iterator<Iter>(i);
    }

#endif
}
