#pragma once

#if __cplusplus >= 201103L

#include "../config.h"
#include "../functional/invoke.h"

namespace extension {
#if LIB_TYPE_TRAITS && __cpp_lib_is_invocable && PREFER_LIB_INVOKE
    using std::invoke_result;
    using std::invoke_result_t;
    using std::is_invocable;
    using std::is_invocable_r;
    using std::is_nothrow_invocable;
    using std::is_nothrow_invocable_r;
#else

    namespace detail {
        template<class, class...>
        struct invocable {
        };
        template<class ...Args>
        struct invocable<decltype(void(extension::invoke(std::declval<Args>()...))), Args...> {
            typedef decltype(extension::invoke(std::declval<Args>()...)) type;
        };

        template<class, class, class...>
        struct invocable_r {
            MAYBE_UNUSED typedef false_type invocable, nothrow;
        };
        template<class Ret, class ...Args>
        struct invocable_r<decltype(void(extension::invoke_r<Ret>(std::declval<Args>()...))), Ret, Args...> {
            MAYBE_UNUSED typedef true_type invocable;
            MAYBE_UNUSED typedef BOOL_CONSTANT(noexcept(extension::invoke_r<Ret>(std::declval<Args>()...))) nothrow;
        };
    }

    template<class Fn, class ...Args>
    struct invoke_result : detail::completely_apply<detail::invocable, void, Fn, Args...> {
    };
    template<class Fn, class ...Args> using invoke_result_t = typename invoke_result<Fn, Args...>::type;
    template<class Ret, class Fn, class ...Args>
    struct is_invocable_r : detail::completely_apply<detail::invocable_r, void, Ret, Fn, Args...>::invocable {
    };
    template<class Ret, class Fn, class ...Args>
    struct is_nothrow_invocable_r : detail::completely_apply<detail::invocable_r, void, Ret, Fn, Args...>::nothrow {
    };
    template<class Fn, class ...Args>
    struct is_invocable : is_invocable_r<void, Fn, Args...>::type {
    };
    template<class Fn, class ...Args>
    struct is_nothrow_invocable : is_nothrow_invocable_r<void, Fn, Args...>::type {
    };

#endif
}
#endif
