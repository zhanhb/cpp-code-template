#pragma once

#include "integral_constant.h"
#include "is_lvalue_reference.h"
#include "is_reference.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_rvalue_reference;
#else
    template<class Tp>
    struct is_rvalue_reference : BOOL_CONSTANT(is_reference<Tp>::value && !is_lvalue_reference<Tp>::value) {
    };
#endif
}
