#pragma once

#include "invoke.h"

namespace extension {
#if PREFER_LIB_INVOKE
    using std::mem_fn;
#else

    namespace detail {
        template<class Tp>
        class mem_fn_impl final {
            Tp pm;
        public:
            explicit constexpr mem_fn_impl(Tp pm) noexcept: pm(pm) {}

            template<class ...Args>
            inline constexpr auto operator()(Args &&... args) const
            noexcept(noexcept(extension::invoke(pm, static_cast<Args &&>(args)...))) ->
            decltype(extension::invoke(pm, static_cast<Args &&>(args)...)) {
                return extension::invoke(pm, static_cast<Args &&>(args)...);
            }
        };
    }

    template<class Tp, class Base>
    inline constexpr detail::mem_fn_impl<Tp Base::*> mem_fn(Tp Base::* pm) noexcept {
        return detail::mem_fn_impl<Tp Base::*>(pm);
    }

#endif

}
