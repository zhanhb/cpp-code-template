#pragma once

#include "lib_type_traits.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::integral_constant;
    using std::true_type;
    using std::false_type;
#if __cpp_lib_bool_constant
#define BOOL_CONSTANT(...) std::bool_constant<(__VA_ARGS__)>
    using std::bool_constant;
#else /* __cpp_lib_bool_constant */
#define BOOL_CONSTANT(...) std::integral_constant<bool, (__VA_ARGS__)>
#if __cplusplus >= 201103L
    template<bool value> using bool_constant = BOOL_CONSTANT(value);
#endif /* 201103L */
#endif /* __cpp_lib_bool_constant */
#else /* LIB_TYPE_TRAITS */

    template<class Tp, Tp val>
    struct integral_constant {
        typedef Tp value_type;
        typedef integral_constant type;

#if __cplusplus >= 201103L

        // @formatter:off
        enum : Tp { value = val };
        // @formatter:on
        constexpr operator Tp() const noexcept { return value; } // NOLINT(google-explicit-constructor)
        constexpr value_type operator()() const noexcept { return value; }

#else /* 201103L */

        // @formatter:off
        enum { value = val };
        // @formatter:on
        inline operator Tp() const throw() { return value; } // NOLINT(google-explicit-constructor)
        inline value_type operator()() const throw() { return value; }

#endif /* 201103L */
    };

#if __cplusplus >= 201103L
    template<bool value> using bool_constant = integral_constant<bool, value>;
#define BOOL_CONSTANT(...) extension::bool_constant<(__VA_ARGS__)>
#else /* 201103L */
#define BOOL_CONSTANT(...) extension::integral_constant<bool, (__VA_ARGS__)>
#endif /* 201103L */
    typedef BOOL_CONSTANT(true) true_type;
    typedef BOOL_CONSTANT(false) false_type;

#endif /* LIB_TYPE_TRAITS */
}
