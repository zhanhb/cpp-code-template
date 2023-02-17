#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::conditional;
#else
    // @formatter:off
    template<bool, class Tp, class> struct conditional { typedef Tp type; };
    template<class Tp, class Up> struct conditional<false, Tp, Up> { typedef Up type; };
    // @formatter:on
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::conditional_t;
#elif __cplusplus >= 201103L
    template<bool value, class Tp, class Up> using conditional_t = typename conditional<value, Tp, Up>::type;
#endif
}
