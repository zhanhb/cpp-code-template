#pragma once

#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_const;
#else
    // @formatter:off
    template<class Tp> struct is_const : false_type {};
    template<class Tp> struct is_const<const Tp> : true_type {};
    // @formatter:on
#endif
}
