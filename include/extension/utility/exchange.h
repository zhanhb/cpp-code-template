#pragma once

#if __cplusplus >= 201103L

#include <utility>

namespace extension {
#if __cpp_lib_exchange_function
    using std::exchange;
#else

    template<class Tp, class Up = Tp>
    inline Tp exchange(Tp &obj, Up &&new_value)
    noexcept(std::is_nothrow_move_constructible<Tp>::value && std::is_nothrow_assignable<Tp &, Up>::value) {
        Tp old_value = std::move(obj);
        obj = std::forward<Up>(new_value);
        return old_value;
    }

#endif
}

#endif
