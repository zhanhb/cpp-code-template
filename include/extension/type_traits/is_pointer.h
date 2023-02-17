#pragma once

#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_pointer;
#else
    // @formatter:off
    template<class Tp> struct is_pointer : false_type {};
    template<class Tp> struct is_pointer<Tp *> : true_type {};
    template<class Tp> struct is_pointer<Tp *const> : true_type {};
    template<class Tp> struct is_pointer<Tp *volatile> : true_type {};
    template<class Tp> struct is_pointer<Tp *const volatile> : true_type {};
    // @formatter:on
#endif
}
