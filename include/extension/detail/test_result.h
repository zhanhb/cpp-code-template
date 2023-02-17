#pragma once

#include "../type_traits/integral_constant.h"

namespace extension {
    namespace detail {
        typedef char yes_type[2], no_type[3];
        // @formatter:off
        template<unsigned> struct test_result {};
        template<> struct test_result<sizeof(yes_type)> : true_type {};
        template<> struct test_result<sizeof(no_type)> : false_type {};
        // @formatter:on
    }
}
