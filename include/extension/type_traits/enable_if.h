#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::enable_if;
#else
    template<bool, class Tp = void>
    struct enable_if {
        typedef Tp type;
    };
    template<class Tp>
    struct enable_if<false, Tp> {
    };
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::enable_if_t;
#elif __cplusplus >= 201103L
    template<bool value, class Tp = void> using enable_if_t = typename enable_if<value, Tp>::type;
#endif
}
