#pragma once

#include <type_traits>

template<class Tp, class Up>
constexpr typename std::enable_if<std::is_assignable<Tp, Up>::value, bool>::type
Max(Tp &&a, Up &&b) { return a < b && (a = b, true); }

template<class Tp, class Up>
constexpr typename std::enable_if<std::is_assignable<Tp, Up>::value, bool>::type
Min(Tp &&a, Up &&b) { return b < a && (a = b, true); }
