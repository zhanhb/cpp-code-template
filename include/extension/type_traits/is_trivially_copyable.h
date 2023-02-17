#pragma once

#include "integral_constant.h"

namespace extension {
// @formatter:off
#if LIB_TYPE_TRAITS && !(defined(__GNUC__) && !(__GNUC__ >= 5) && !defined(__clang__))
    using std::is_trivially_copyable;
#elif defined(__clang__) && defined(__has_feature)
#if __has_feature(is_trivially_copyable)
    template<class Tp> struct is_trivially_copyable : BOOL_CONSTANT(__is_trivially_copyable(Tp)) {};
#elif __has_feature(is_trivial)
    template<class Tp> struct is_trivially_copyable : BOOL_CONSTANT(__is_trivial(Tp) || __is_pod(Tp)) {};
#else
    template<class Tp> struct is_trivially_copyable : BOOL_CONSTANT(__is_pod(Tp)) {};
#endif
#elif __GNUC__ >= 5
    template<class Tp> struct is_trivially_copyable : BOOL_CONSTANT(__is_trivially_copyable(Tp)) {};
#elif __GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 7
    template<class Tp> struct is_trivially_copyable : BOOL_CONSTANT(__is_trivial(Tp) || __is_pod(Tp)) {};
#elif defined(__GNUC__)
    template<class Tp> struct is_trivially_copyable : BOOL_CONSTANT(__is_pod(Tp)) {};
#else
}
#include "is_pod.h"
namespace extension {
    template<class Tp> struct is_trivially_copyable : is_pod<Tp>::type {};
#endif
// @formatter:on
}
