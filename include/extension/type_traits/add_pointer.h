#pragma once

#include "is_void.h"
#include "remove_reference.h"
#include "../detail/referenceable.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::add_pointer;
#else
    namespace detail {
        template<class Tp, bool = referenceable<Tp>::value || is_void<Tp>::value>
        struct add_pointer_impl {
            typedef Tp *type;
        };
        template<class Tp>
        struct add_pointer_impl<Tp, false> {
            typedef Tp type;
        };
    }
    template<class Tp>
    struct add_pointer : detail::add_pointer_impl<typename remove_reference<Tp>::type> {
    };
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::add_pointer_t;
#elif __cplusplus >= 201103L
    template<class Tp> using add_pointer_t = typename add_pointer<Tp>::type;
#endif
}
