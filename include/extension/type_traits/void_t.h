#pragma once

#if __cplusplus >= 201103L

#include "lib_type_traits.h"
#include "../detail/first_type.h"

namespace extension {
#if LIB_TYPE_TRAITS && __cpp_lib_void_t
    using std::void_t;
#else
    template<class ...Args> using void_t = detail::first_type_t<void, Args...>;
#endif
}

#endif /* 201103L */
