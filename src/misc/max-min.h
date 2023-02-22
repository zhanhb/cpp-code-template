#pragma once

#include <type_traits>

#if 0

#define INVOKE_RETURN(...) noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Max(Tp &&a, const Up &b) INVOKE_RETURN(a < b && (a = b, true))

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Min(Tp &&a, const Up &b) INVOKE_RETURN(b < a && (a = b, true))

#else

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Max(Tp &&a, const Up &b) -> decltype(a < b && (a = b, true)) { return a < b && (a = b, true); }

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Min(Tp &&a, const Up &b) -> decltype(b < a && (a = b, true)) { return b < a && (a = b, true); }

#endif
