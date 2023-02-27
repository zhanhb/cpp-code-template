#include <type_traits>

#if 1

#if __cplusplus >= 201402L
#define DEFINE_FUNCTION(declaration, content) decltype(auto) declaration noexcept(noexcept(content)) { return content; }
#else
#define DEFINE_FUNCTION(declaration, content) auto declaration noexcept(noexcept(content)) -> decltype(content) { return content; }
#endif

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr DEFINE_FUNCTION(Max(Tp && a, const Up &b), a < b && (a = b, true))

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr DEFINE_FUNCTION(Min(Tp && a, const Up &b), b < a && (a = b, true))

#else

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Max(Tp &&a, const Up &b) -> decltype(a < b && (a = b, true)) { return a < b && (a = b, true); }

template<class Tp, class Up = typename std::remove_reference<Tp>::type>
constexpr auto Min(Tp &&a, const Up &b) -> decltype(b < a && (a = b, true)) { return b < a && (a = b, true); }

#endif
