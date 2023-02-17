#pragma once

#ifdef __has_include
#if __has_include(<bit>)

#include <bit>

#endif
#endif

#if __cpp_lib_bit_cast
namespace extension {
    using std::bit_cast;
}
#elif __cplusplus >= 201103L

#include <cstring>
#include <type_traits>
#include "../config.h"
#include "../type_traits/is_trivially_copyable.h"

namespace extension {

    template<class To, class From>
    NODISCARD typename std::enable_if<
            sizeof(To) == sizeof(From) &&
            is_trivially_copyable<From>::value &&
            is_trivially_copyable<To>::value,
            To
    >::type CONSTEXPR_14 bit_cast(const From &src) noexcept {
        alignas(To) unsigned char buf[sizeof(To) ? sizeof(To) : 1];
        std::memcpy(buf, &src, sizeof(To));
        return *reinterpret_cast<To *>(buf);
    }
}

#else

#include "../type_traits/enable_if.h"

namespace extension {
    template<class To, class From>
    inline typename extension::enable_if<sizeof(To) == sizeof(From), To>::type bit_cast(const From &src) {
        struct bit_cast_union {
            From from;
            To to;

            inline explicit bit_cast_union(const From &src) : from(src) {}
        };
        return bit_cast_union(src).to;
    }
}

#endif
