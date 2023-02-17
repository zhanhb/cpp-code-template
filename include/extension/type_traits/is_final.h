#pragma once

#include "integral_constant.h"

namespace extension {
// @formatter:off
#if LIB_TYPE_TRAITS && __cpp_lib_is_final
    using std::is_final;
#elif __GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 7 || _MSC_VER >= 1800
    template<class Tp> struct is_final : BOOL_CONSTANT(__is_final(Tp)) {};
#elif __SUNPRO_CC >= 0x5130
    template<class Tp> struct is_final : BOOL_CONSTANT(__oracle_is_final(Tp)) {};
#elif defined(__has_feature)
#if __has_feature(is_final)
    template<class Tp> struct is_final : BOOL_CONSTANT(__is_final(Tp)) {};
#else
    template<class Tp> struct is_final : false_type {};
#endif
#else
    template<class Tp> struct is_final : false_type {};
#endif
// @formatter:on
}
