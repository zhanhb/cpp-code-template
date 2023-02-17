#pragma once

#include "integral_constant.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_lvalue_reference;
#else
    template<class>
    struct is_lvalue_reference : false_type {
    };
    template<class Tp>
    struct is_lvalue_reference<Tp &> : true_type {
    };
#endif
}
