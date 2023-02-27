#include <compare>

template<typename T, int N> requires (N > 0) /*...*/
class Stack {
    static_assert(N > 0, "N > 0 is required");
};

_LIBCPP_BEGIN_NAMESPACE_STD
template<class Tp>
constexpr strong_ordering strong_order(const Tp &lhs, const Tp &rhs) {
    return lhs <=> rhs;
}

template<class Tp>
constexpr partial_ordering partial_order(const Tp &lhs, const Tp &rhs) {
    return lhs <=> rhs;
}
_LIBCPP_END_NAMESPACE_STD
