#pragma once

#include "lib_type_traits.h"

#if LIB_TYPE_TRAITS
namespace extension {
    using std::is_class;
}
#else

#include "is_union.h"
#include "../detail/test_result.h"

namespace extension {
    namespace detail {
        template<class Tp>
        class is_class_test {
            // @formatter:off
            template<class Up> static yes_type *test(int Up::*) { return 0; } // NOLINT(modernize-use-nullptr)
            template<class Up> static no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)
            // @formatter:on
        public:
            enum {
                value = test_result<sizeof(*is_class_test::test<Tp>(0))>::value && !is_union<Tp>::value
            };
        };
    }
    template<class Tp>
    struct is_class : BOOL_CONSTANT(detail::is_class_test<Tp>::value) {
    };
}

#endif
