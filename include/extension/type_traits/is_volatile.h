#pragma once

#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_volatile;
#else
    // @formatter:off
    template<class Tp> struct is_volatile : false_type {};
    template<class Tp> struct is_volatile<volatile Tp> : true_type {};
    // @formatter:on
#endif
}
