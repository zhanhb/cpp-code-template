#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::remove_pointer;
#else
    // @formatter:off
    template<class Tp> struct remove_pointer { typedef Tp type; };
    template<class Tp> struct remove_pointer<Tp *> { typedef Tp type; };
    template<class Tp> struct remove_pointer<Tp *const> { typedef Tp type; };
    template<class Tp> struct remove_pointer<Tp *volatile> { typedef Tp type; };
    template<class Tp> struct remove_pointer<Tp *const volatile> { typedef Tp type; };
    // @formatter:on
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::remove_pointer_t;
#elif __cplusplus >= 201103L
    template<class Tp> using remove_pointer_t = typename remove_pointer<Tp>::type;
#endif
}
