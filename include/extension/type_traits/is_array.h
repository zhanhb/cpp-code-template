#pragma once

#include <cstddef>
#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_array;
#else
    template<class>
    struct is_array : false_type {
    };
    template<class Tp>
    struct is_array<Tp[]> : true_type {
    };
    template<class Tp, std::size_t Size>
    struct is_array<Tp[Size]> : true_type {
    };
#endif
}
