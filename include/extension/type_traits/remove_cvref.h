#pragma once

#include "lib_type_traits.h"
#include "remove_cv.h"
#include "remove_reference.h"

namespace extension {
#if LIB_TYPE_TRAITS && __cpp_lib_remove_cvref
    using std::remove_cvref;
    using std::remove_cvref_t;
#else
    template<class Tp>
    struct remove_cvref {
        typedef typename remove_cv<typename remove_reference<Tp>::type>::type type;
    };
#if __cplusplus >= 201103L
    template<class Tp> using remove_cvref_t = typename remove_cvref<Tp>::type;
#endif
#endif
}
