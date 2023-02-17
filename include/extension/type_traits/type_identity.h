#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS && __cpp_lib_type_identity
    using std::type_identity;
    using std::type_identity_t;
#else
    template<class Tp>
    struct type_identity {
        typedef Tp type;
    };
#if __cplusplus >= 201103L
    template<class Tp> using type_identity_t = typename type_identity<Tp>::type;
#endif
#endif
}
