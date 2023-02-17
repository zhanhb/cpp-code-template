#pragma once

#include "add_pointer.h"
#include "is_array.h"
#include "is_function.h"
#include "remove_cv.h"
#include "remove_extent.h"
#include "remove_reference.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::decay;
#else
    namespace detail {
        template<class Tp, bool = is_array<Tp>::value, bool = is_function<Tp>::value>
        struct decay_impl {
            typedef typename remove_cv<Tp>::type type;
        };
        template<class Tp>
        struct decay_impl<Tp, true, false> {
            typedef typename add_pointer<typename remove_extent<Tp>::type>::type type;
        };
        template<class Tp>
        struct decay_impl<Tp, false, true> {
            typedef typename add_pointer<Tp>::type type;
        };
    }
    template<class Tp>
    struct decay : detail::decay_impl<typename remove_reference<Tp>::type> {
    };
#endif

#if LIB_TYPE_TRAITS && __cpp_lib_transformation_trait_aliases
    using std::decay_t;
#elif __cplusplus >= 201103L
    template<class Tp> using decay_t = typename decay<Tp>::type;
#endif
}
