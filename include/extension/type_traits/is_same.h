#pragma once

#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_same;
#else
    // @formatter:off
    template<class, class> struct is_same : false_type {};
    template<class Tp> struct is_same<Tp, Tp> : true_type {};
    // @formatter:on
#endif
}
