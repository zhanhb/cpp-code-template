#pragma once

#include "is_reference.h"
#include "../detail/referenceable.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::add_rvalue_reference;
#else
    namespace detail {
        template<class Tp, bool = referenceable<Tp>::value && !is_reference<Tp>::value>
        struct add_rvalue_reference_impl {
#if __cplusplus >= 201103L
            typedef Tp &&type;
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-extensions"
            typedef Tp &&type;
#pragma clang diagnostic pop
#else
            typedef Tp type;
#endif
        };
        template<class Tp>
        struct add_rvalue_reference_impl<Tp, false> {
            typedef Tp type;
        };
    }
    template<class Tp>
    struct add_rvalue_reference : detail::add_rvalue_reference_impl<Tp> {
    };
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::add_rvalue_reference_t;
#elif __cplusplus >= 201103L
    template<class Tp> using add_rvalue_reference_t = typename add_rvalue_reference<Tp>::type;
#endif
}
