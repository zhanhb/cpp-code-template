#pragma once

#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_void;
#else
    // @formatter:off
    template<class> struct is_void : false_type {};
    template<> struct is_void<void> : true_type {};
    template<> struct is_void<const void> : true_type {};
    template<> struct is_void<volatile void> : true_type {};
    template<> struct is_void<const volatile void> : true_type {};
    // @formatter:on
#endif
}
