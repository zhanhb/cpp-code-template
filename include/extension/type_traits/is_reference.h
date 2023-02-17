#pragma once

#include "is_same.h"
#include "remove_reference.h"

namespace extension {
#if LIB_TYPE_TRAITS
    using std::is_reference;
#else
    template<class Tp>
    struct is_reference : BOOL_CONSTANT(!is_same<typename remove_reference<Tp>::type, Tp>::value) {
    };
#endif
}
