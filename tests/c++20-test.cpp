#include <tuple>
#include <type_traits>

using namespace std;

template<class>
struct is_block : false_type {
};
template<class Rp, class ...Args>
struct is_block<Rp (^)(Args...)> : true_type {
};

template<class _Tp, class... _Args>
inline constexpr bool one_of_v = (std::is_same<_Tp, _Args>::value || ...);

template<class Tp, class ...Args>
constexpr auto call(Tp &&tp, Args &&...args) ->
decltype(std::apply(std::forward<Tp>(tp), forward_as_tuple<Args...>(args...)))
//decltype(std::forward<Tp>(tp)(std::forward<Args>(args)...))
{
    return std::apply(std::forward<Tp>(tp), forward_as_tuple<Args...>(args...));
//    return std::forward<Tp>(tp)(std::forward<Args>(args)...);
}

// C++17 auto 形参声明
template<auto n>
constexpr auto f() -> std::pair<decltype(n), decltype(n)> { // auto 不能从花括号初始化器列表推导
    return {n, n};
}

int add(int a, int b, int c) {
    return a + b + c;
}

int main() {
    [](auto &&...) {}(add, f<1>);
}