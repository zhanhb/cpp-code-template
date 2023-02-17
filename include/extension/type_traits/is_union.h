#pragma once

#include "integral_constant.h"

namespace extension {
// @formatter:off
#if LIB_TYPE_TRAITS
    using std::is_union;
#elif _MSC_VER >= 1500 || __GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 3 || defined(__ghs__) && __GHS_VERSION_NUMBER >= 600 || defined(__CODEGEARC__)
    template<class Tp> struct is_union : BOOL_CONSTANT(__is_union(Tp)) {};
#elif __DMC__ >= 0x848
    template<class Tp> struct is_union : BOOL_CONSTANT(bool(__typeinfo(Tp) & 0x400)) {};
#elif __MSL_CPP__ >= 0x8000
}
#include <msl_utility>
namespace extension {
    template<class Tp> struct is_union : BOOL_CONSTANT(Metrowerks::is_union<Tp>::value) {};
#elif defined(__has_feature)
#if __has_feature(is_union)
    template<class Tp> struct is_union : BOOL_CONSTANT(__is_union(Tp)) {};
#else
    template<class Tp> struct is_union : false_type {};
#endif
#else
    template<class Tp> struct is_union : false_type {};
#endif
// @formatter:on
}
