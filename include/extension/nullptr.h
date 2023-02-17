#pragma once

#if __cplusplus >= 201103L
namespace extension {
    typedef decltype(nullptr) nullptr_t;
}
#else

#include <cstddef>

#if defined(nullptr)
namespace extension {
    using std::nullptr_t;
}
#else

#include "config.h"

namespace extension {
    class nullptr_t {
        MAYBE_UNUSED void *ptr_for_size;
    public:
        inline nullptr_t() : ptr_for_size(reinterpret_cast<void *>(0)) {}

        inline operator void *() const { return 0; } // NOLINT(google-explicit-constructor,modernize-use-nullptr)

        template<class Tp>
        inline operator Tp *() const { return reinterpret_cast<Tp *>(0); } // NOLINT(google-explicit-constructor)

        template<class Tp, class Up>
        inline operator Tp Up::*() const { return 0; } // NOLINT(google-explicit-constructor)

        friend inline bool operator==(nullptr_t, nullptr_t) { return true; }

        friend inline bool operator!=(nullptr_t, nullptr_t) { return false; }
    };
}

#define nullptr (extension::nullptr_t())

#endif
#endif
