#pragma once

#if __cplusplus && __cplusplus < 201103L

#include "../type_traits/add_rvalue_reference.h"

namespace extension {
    template<class Tp>
    typename add_rvalue_reference<Tp>::type declval() throw(); // NOLINT(readability-redundant-declaration)
}

#endif
