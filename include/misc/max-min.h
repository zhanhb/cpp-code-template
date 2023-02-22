#include <type_traits>

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Max(Tp &&a, const Up &b) -> decltype(a < b && (a = b, true)) { return a < b && (a = b, true); }

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Min(Tp &&a, const Up &b) -> decltype(b < a && (a = b, true)) { return b < a && (a = b, true); }
