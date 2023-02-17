#pragma once

#include <cstddef>
#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::remove_extent;
#else
    // @formatter:off
    template<class Tp> struct remove_extent { typedef Tp type; };
    template<class Tp> struct remove_extent<Tp[]> { typedef Tp type; };
    template<class Tp, std::size_t Size> struct remove_extent<Tp[Size]> { typedef Tp type; };
    // @formatter:on
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::remove_extent_t;
#elif __cplusplus >= 201103L
    template<class Tp> using remove_extent_t = typename remove_extent<Tp>::type;
#endif
}
