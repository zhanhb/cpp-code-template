#pragma once

#if __cplusplus >= 201103L

#include <functional>

namespace extension {
#if __cpp_lib_unwrap_ref && PREFER_LIB_INVOKE
    using std::unwrap_reference;
    using std::unwrap_ref_decay;
    using std::unwrap_reference_t;
    using std::unwrap_ref_decay_t;
#else
    template<class Tp>
    struct unwrap_reference {
        typedef Tp type;
    };
    template<class Tp>
    struct unwrap_reference<std::reference_wrapper<Tp> > {
        typedef Tp &type;
    };
#if !PREFER_LIB_INVOKE
    template<class>
    struct reference_wrapper;
    template<class Tp>
    struct unwrap_reference<reference_wrapper<Tp> > {
        typedef Tp &type;
    };
#endif

    template<class Tp>
    struct unwrap_ref_decay : unwrap_reference<typename std::decay<Tp>::type> {
    };

    template<class Tp> using unwrap_reference_t = typename unwrap_reference<Tp>::type;
    template<class Tp> using unwrap_ref_decay_t = typename unwrap_ref_decay<Tp>::type;
#endif
}

#endif
