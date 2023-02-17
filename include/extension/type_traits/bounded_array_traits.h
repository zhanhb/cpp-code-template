#pragma once

#include <cstddef>
#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS && __cpp_lib_bounded_array_traits
    using std::is_bounded_array;
    using std::is_unbounded_array;
#else
    template<class>
    struct is_bounded_array : false_type {
    };
    template<class Tp, std::size_t Np>
    struct is_bounded_array<Tp[Np]> : true_type {
    };
    template<class>
    struct is_unbounded_array : false_type {
    };
    template<class Tp>
    struct is_unbounded_array<Tp[]> : true_type {
    };
#endif
}
