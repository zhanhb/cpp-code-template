#pragma once

#if __cplusplus >= 201103L

#include "lib_type_traits.h"

#if LIB_TYPE_TRAITS && __cpp_lib_is_nothrow_convertible
namespace extension {
    using std::is_nothrow_convertible;
}
#else

#include "integral_constant.h"
#include "logical_traits.h"
#include "../detail/complete_check.h"

namespace extension {
    namespace detail {
        template<class Tp>
        static inline void accept(Tp) noexcept; // NOLINT(readability-redundant-declaration)

        template<class From, class To, class = void>
        struct nothrow_convertible_impl : and_t<std::is_void<From>, std::is_void<To> > {
        };
        template<class From, class To>
        struct nothrow_convertible_impl<From, To, decltype(detail::accept<To>(std::declval<From>()))>
                : BOOL_CONSTANT(noexcept(detail::accept<To>(std::declval<From>()))) {
        };
    }

    template<class From, class To>
    struct is_nothrow_convertible : detail::completely_apply<detail::nothrow_convertible_impl, From, To>::type {
    };
}

#endif /* __cpp_lib_is_nothrow_convertible */

#endif /* 201103L */
