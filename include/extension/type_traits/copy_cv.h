#pragma once

namespace extension {
    // @formatter:off
    template<class From, class To> struct copy_cv { typedef To type; };
    template<class From, class To> struct copy_cv<From const, To> { typedef const To type; };
    template<class From, class To> struct copy_cv<From volatile, To> { typedef volatile To type; };
    template<class From, class To> struct copy_cv<From const volatile, To> { typedef const volatile To type; };
    // @formatter:on

#if __cplusplus >= 201103L
    template<class From, class To> using copy_cv_t = typename copy_cv<From, To>::type;
#endif
}
