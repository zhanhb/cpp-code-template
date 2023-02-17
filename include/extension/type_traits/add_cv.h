#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::add_const;
    using std::add_volatile;
    using std::add_cv;
#else
    // @formatter:off
    template<class Tp> struct add_const { typedef const Tp type; };
    template<class Tp> struct add_volatile { typedef volatile Tp type; };
    template<class Tp> struct add_cv { typedef const volatile Tp type; };
    // @formatter:on
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::add_const_t;
    using std::add_volatile_t;
    using std::add_cv_t;
#elif __cplusplus >= 201103L
    template<class Tp> using add_const_t = typename add_const<Tp>::type;
    template<class Tp> using add_volatile_t = typename add_volatile<Tp>::type;
    template<class Tp> using add_cv_t = typename add_cv<Tp>::type;
#endif
}
