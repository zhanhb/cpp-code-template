#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::remove_const;
    using std::remove_volatile;
    using std::remove_cv;
#else
    // @formatter:off
    template<class Tp> struct remove_const { typedef Tp type; };
    template<class Tp> struct remove_const<Tp const> { typedef Tp type; };

    template<class Tp> struct remove_volatile { typedef Tp type; };
    template<class Tp> struct remove_volatile<Tp volatile> { typedef Tp type; };

    template<class Tp> struct remove_cv { typedef Tp type; };
    template<class Tp> struct remove_cv<Tp const> { typedef Tp type; };
    template<class Tp> struct remove_cv<Tp volatile> { typedef Tp type; };
    template<class Tp> struct remove_cv<Tp const volatile> { typedef Tp type; };
    // @formatter:on
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::remove_const_t;
    using std::remove_volatile_t;
    using std::remove_cv_t;
#elif __cplusplus >= 201103L
    template<class Tp> using remove_const_t = typename remove_const<Tp>::type;
    template<class Tp> using remove_volatile_t = typename remove_volatile<Tp>::type;
    template<class Tp> using remove_cv_t = typename remove_cv<Tp>::type;
#endif
}
