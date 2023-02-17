#pragma once

namespace extension { // NOLINT(modernize-concat-nested-namespaces)
    namespace detail {
#if __cplusplus >= 201103L
        template<class Tp, class...>
#else
        template<class Tp, class = void, class = void, class = void, class = void>
#endif
        struct first_type {
            typedef Tp type;
        };
#if __cplusplus >= 201103L
        template<class Tp, class ...Rest> using first_type_t = typename first_type<Tp, Rest...>::type;
#endif
    }
}
