#pragma once

#include "first_type.h"
#include "test_result.h"

namespace extension {
    namespace detail {
        template<class Tp>
        class referenceable {
            // @formatter:off
            template<class Up> static yes_type *test(typename first_type<int, Up &>::type) { return 0; } // NOLINT(modernize-use-nullptr)
            template<class Up> static no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)
            // @formatter:on
        public:
            enum {
                value = test_result<sizeof(*referenceable::test<Tp>(0))>::value
            };
        };
    }
}
