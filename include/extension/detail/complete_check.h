#pragma once

#if __cplusplus >= 201103L

#include <cstddef>
#include "first_type.h"
#include "../type_traits/bounded_array_traits.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_function.h"
#include "../type_traits/is_void.h"
#include "../type_traits/remove_reference.h"
#include "../type_traits/logical_traits.h"

namespace extension { // NOLINT(modernize-concat-nested-namespaces)
    namespace detail {
        struct complete_unbounded_test {
            template<class Tp, std::size_t = sizeof(Tp)>
            static true_type test(int) { return {}; }

            template<class Tp, class Up = typename remove_reference<Tp>::type>
            static or_t<is_function<Up>, is_unbounded_array<Up>, is_void<Up> > test(...) { return {}; }
        };

        template<class Tp, bool value = decltype(complete_unbounded_test::test<Tp>(0))::value>
        struct is_complete_or_unbounded : BOOL_CONSTANT(value) {
        };

        template<class Tp>
        struct require_complete : is_complete_or_unbounded<Tp> {
            static_assert(require_complete::value, "must be a complete class or an unbounded array");
        };

        // g++ 8.5.0, 12.2.0 13.1.0 reports "error: invalid use of pack expansion expression"
        // @see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=86356
        // just simply disable feature fold expressions while compiling with g++
#if __cpp_fold_expressions && !(defined(__GNUC__) && !defined(__clang__))
        template<class Tp, class ...Args>
        using enable_if_all_t = enable_if_t<(... && Args::value), Tp>;
#else
        template<class Tp, class ...Args>
        using enable_if_all_t = first_type_t<Tp, enable_if_t<Args::value>...>;
#endif
        template<template<class...> class Tpl, class ...Args>
        using completely_apply = Tpl<enable_if_t<require_complete<Args>::value, Args>...>;
    }
}

#endif
