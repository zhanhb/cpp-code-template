#pragma once

#include "debug.h"

namespace impl_verify {
#if __cplusplus >= 201103L
    namespace detail {
        template<class Tp, class = Tp, class = bool>
        struct has_equal_to : std::false_type {
        };
        template<class Tp, class Up>
        struct has_equal_to<Tp, Up, decltype(bool(std::declval<Tp>() == std::declval<Up>()))
        > : std::true_type {
        };
    }

    template<class CharT1, class Traits1, class CharT2, class Traits2, class Rp, class Ep = Rp>
    inline typename std::enable_if<detail::has_equal_to<const Ep &, const Rp &>::value>::type
#else

    template<class CharT1, class Traits1, class CharT2, class Traits2, class Rp, class Ep>
    inline void
#endif
    do_verify(
            const char *file, int line, const char *name,
            std::basic_ostream<CharT1, Traits1> &out,
            std::basic_ostream<CharT2, Traits2> &log,
            const Ep &expected, const Rp &result) {
        if (expected == result) {
            out << file << ":" << line << ": " << name << ": " << result << std::endl;
        } else {
            log << file << ":" << line << ": " << name << ":" << std::endl;
            log << "expected \"" << expected << "\", but got \"" << result << '"' << std::endl;
            abort();
        }
    }
}

#define verify(...) impl_verify::do_verify(__FILE__, __LINE__, #__VA_ARGS__, std::cout, std::cerr, __VA_ARGS__)
