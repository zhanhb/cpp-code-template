#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::remove_reference;
#elif __cplusplus >= 201103L
    // @formatter:off
    template<class Tp> struct remove_reference { using type = Tp; };
    template<class Tp> struct remove_reference<Tp &> { using type = Tp; };
    template<class Tp> struct remove_reference<Tp &&> { using type = Tp; };
#else
}
#include "add_lvalue_reference.h"
namespace extension {
    namespace detail {
        template<class Tp> struct remove_lvalue_reference { typedef Tp type; };
        template<class Tp> struct remove_lvalue_reference<Tp &> { typedef Tp type; };
    }
    // @formatter:on
    template<class Tp>
    struct remove_reference : detail::remove_lvalue_reference<typename add_lvalue_reference<Tp>::type> {
    };
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::remove_reference_t;
#elif __cplusplus >= 201103L
    template<class Tp> using remove_reference_t = typename remove_reference<Tp>::type;
#endif
}
