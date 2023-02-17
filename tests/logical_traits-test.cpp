#include "extension/type_traits/logical_traits.h"
#include "static_assert.h"

namespace logical_traits_classic {
    template<class...>
    struct conjunction : std::true_type {
    };
    template<class Tp>
    struct conjunction<Tp> : Tp {
    };
    template<class B1, class... Bn>
    struct conjunction<B1, Bn...> : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {
    };

    template<class...>
    struct disjunction : std::false_type {
    };
    template<class Tp>
    struct disjunction<Tp> : Tp {
    };
    template<class B1, class... Bn>
    struct disjunction<B1, Bn...> : std::conditional<bool(B1::value), B1, disjunction<Bn...> >::type {
    };
}

using extension::conjunction;
using extension::disjunction;
template<class ...Args> using old_conjunction = logical_traits_classic::conjunction<Args...>;
template<class ...Args> using old_disjunction = logical_traits_classic::disjunction<Args...>;

#if __cpp_lib_logical_traits && PREFER_LIB_LOGICAL_TRAITS

#define STATIC_ASSERT_CONJUNCTION(...) \
STATIC_ASSERT_SAME_AS(old_conjunction<__VA_ARGS__>::type, conjunction<__VA_ARGS__>::type)
#define STATIC_ASSERT_DISJUNCTION(...) \
STATIC_ASSERT_SAME_AS(old_disjunction<__VA_ARGS__>::type, disjunction<__VA_ARGS__>::type)

#else

#define STATIC_ASSERT_CONJUNCTION(...) \
STATIC_ASSERT_V(std::is_base_of, extension::detail::and_t<__VA_ARGS__>, old_conjunction<__VA_ARGS__>)
#define STATIC_ASSERT_DISJUNCTION(...) \
STATIC_ASSERT_V(std::is_base_of, extension::detail::or_t<__VA_ARGS__>, old_disjunction<__VA_ARGS__>)

#endif

#define STATIC_ASSERT_LOGICAL_TRAITS(...) \
STATIC_ASSERT_CONJUNCTION(__VA_ARGS__); \
STATIC_ASSERT_DISJUNCTION(__VA_ARGS__)

typedef std::integral_constant<int, 0> zero;
typedef std::integral_constant<int, 1> one;
typedef std::integral_constant<int, 2> two;

#define test2(...) __VA_ARGS__

#if __cpp_lib_logical_traits && PREFER_LIB_LOGICAL_TRAITS
#ifdef __clang__
#undef test2
#define test2(...)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wnarrowing"
#endif
#endif

#define gen0(...) STATIC_ASSERT_LOGICAL_TRAITS(__VA_ARGS__);
#define append_1d(...) gen0(zero, __VA_ARGS__) gen0(one, __VA_ARGS__) test2(gen0(two, __VA_ARGS__))
#define append_2d(...) append_1d(zero, __VA_ARGS__) append_1d(one, __VA_ARGS__) test2(append_1d(two, __VA_ARGS__))
#define append_3d(...) append_2d(zero, __VA_ARGS__) append_2d(one, __VA_ARGS__) test2(append_2d(two, __VA_ARGS__))

#define gen1() gen0(zero) gen0(one) test2(gen0(two))
#define gen2() append_1d(zero) append_1d(one) test2(append_1d(two))
#define gen3() append_2d(zero) append_2d(one) test2(append_2d(two))
#define gen4() append_3d(zero) append_3d(one) test2(append_3d(two))

gen0()
gen1()
gen2()
gen3()
gen4()

test2(STATIC_ASSERT_DISJUNCTION(two, int);)

typedef std::integral_constant<int, 3> three;
typedef std::integral_constant<int, 4> four;

test2(STATIC_ASSERT_COND(conjunction<three, four>::value == 4);)

typedef std::true_type yes;
typedef std::false_type no;
STATIC_ASSERT_LOGICAL_TRAITS();
STATIC_ASSERT_LOGICAL_TRAITS(yes);
STATIC_ASSERT_LOGICAL_TRAITS(no);

STATIC_ASSERT_LOGICAL_TRAITS(zero);
STATIC_ASSERT_LOGICAL_TRAITS(one);
STATIC_ASSERT_LOGICAL_TRAITS(two);

STATIC_ASSERT_CONJUNCTION(no, void);
STATIC_ASSERT_CONJUNCTION(zero, void);

STATIC_ASSERT_DISJUNCTION(yes, int);
STATIC_ASSERT_DISJUNCTION(one, int);

STATIC_ASSERT_V(std::is_base_of, std::true_type, extension::negation<zero>);
STATIC_ASSERT_V(std::is_base_of, std::false_type, extension::negation<one>);
test2(STATIC_ASSERT_V(std::is_base_of, std::false_type, extension::negation<two>);)

struct final_yes final : yes {
};
struct final_no final : no {
};

struct empty {
};

struct final_empty final {
};

STATIC_ASSERT_V(std::is_base_of, empty, conjunction<final_yes, empty>);
STATIC_ASSERT_V(std::is_base_of, empty, conjunction<final_yes, final_yes, empty>);
STATIC_ASSERT_V(std::is_base_of, no, conjunction<no, final_empty>);
STATIC_ASSERT_V(std::is_base_of, no, conjunction<no, final_yes, no>);

STATIC_ASSERT_V(std::is_base_of, empty, extension::disjunction<final_no, empty>);
STATIC_ASSERT_V(std::is_base_of, yes, extension::disjunction<yes, empty>);

int main() {
}
