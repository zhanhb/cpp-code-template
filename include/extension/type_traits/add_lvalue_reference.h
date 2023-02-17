#pragma once

#include "lib_type_traits.h"
#include "../detail/referenceable.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::add_lvalue_reference;
#else
    namespace detail {
        // @formatter:off
        template<class Tp, bool = referenceable<Tp>::value>
        struct add_lvalue_reference_impl { typedef Tp &type; };
        template<class Tp>
        struct add_lvalue_reference_impl<Tp, false> { typedef Tp type; };
        // @formatter:on
    }
    template<class Tp>
    struct add_lvalue_reference : detail::add_lvalue_reference_impl<Tp> {
    };
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::add_lvalue_reference_t;
#elif __cplusplus >= 201103L
    template<class Tp> using add_lvalue_reference_t = typename add_lvalue_reference<Tp>::type;
#endif
}
