#pragma once

#include "is_const.h"
#include "is_reference.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_function;
#else
    template<class Tp>
    struct is_function : BOOL_CONSTANT(!is_const<const Tp>::value && !is_reference<Tp>::value) {
    };
#endif
}
