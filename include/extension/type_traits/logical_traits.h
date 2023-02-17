#pragma once

#if __cplusplus >= 201103L

#include <cstddef>
#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS && __cpp_lib_logical_traits && PREFER_LIB_LOGICAL_TRAITS
    using std::conjunction;
    using std::disjunction;
    using std::negation;
#else
    namespace detail {
        template<std::size_t>
        struct junctions {
            template<template<class, std::size_t> class Eval, class, class Tp, class ...Rest>
            using apply = typename Eval<Tp, sizeof...(Rest)>::template apply<Eval, Tp, Rest...>;
        };
        template<>
        struct junctions<1> {
            template<template<class, std::size_t> class, class, class Tp> using apply = Tp;
        };
        template<>
        struct junctions<0> {
            template<template<class, std::size_t> class, class Tp, class...> using apply = Tp;
        };

        template<class Tp, std::size_t value> using eval_conjunction = junctions<bool(Tp::value) ? value : 0>;
        template<class Tp, std::size_t value> using eval_disjunction = junctions<bool(Tp::value) ? 0 : value>;

        template<class ...Args> using and_t = typename junctions<sizeof...(Args)>::template apply<eval_conjunction, true_type, Args...>;
        template<class ...Args> using or_t = typename junctions<sizeof...(Args)>::template apply<eval_disjunction, false_type, Args...>;
    }

    template<class ...Args>
    struct conjunction : detail::and_t<Args...> {
    };
    template<class ...Args>
    struct disjunction : detail::or_t<Args...> {
    };
    template<class Tp>
    struct negation : BOOL_CONSTANT(!bool(Tp::value)) {
    };
#endif

    // disable specializations
    static_assert(conjunction<negation<disjunction<> > >::value, "logical traits");
}

#endif /* 201103L */
