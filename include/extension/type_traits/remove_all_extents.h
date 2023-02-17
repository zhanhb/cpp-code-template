#pragma once

#include <cstddef>
#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::remove_all_extents;
#else
    template<class Tp>
    struct remove_all_extents {
        typedef Tp type;
    };
    template<class Tp>
    struct remove_all_extents<Tp[]> {
        typedef typename remove_all_extents<Tp>::type type;
    };
    template<class Tp, std::size_t Size>
    struct remove_all_extents<Tp[Size]> {
        typedef typename remove_all_extents<Tp>::type type;
    };
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::remove_all_extents_t;
#elif __cplusplus >= 201103L
    template<class Tp> using remove_all_extents_t = typename remove_all_extents<Tp>::type;
#endif
}
