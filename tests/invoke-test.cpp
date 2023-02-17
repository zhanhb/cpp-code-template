#include "extension/type_traits/is_invocable.h"
#include "verify.h"
#include "static_assert.h"

#if __cplusplus >= 201103L

struct closure {
    int val;

    explicit closure(int val) : val(val) {}

    int noexcept_get() const noexcept { return val; } // NOLINT(modernize-use-nodiscard)

    int get() const { return val; } // NOLINT(modernize-use-nodiscard)

    int apply(int x) const { return val + x; } // NOLINT(modernize-use-nodiscard)
};

void test1() {
    closure y(8);
    auto const cy = std::ref(y);
    STATIC_ASSERT_COND(noexcept(extension::invoke(&closure::val, y)));
    verify(8, extension::invoke(&closure::val, y));
    auto const ptr = &closure::val;
    STATIC_ASSERT_COND(noexcept(extension::invoke(std::ref(ptr), y)));
    STATIC_ASSERT_COND(noexcept(extension::invoke(extension::mem_fn(ptr), y)));
    verify(8, extension::invoke(std::ref(ptr), y));
    STATIC_ASSERT_COND(noexcept(extension::invoke_r<int64_t>(&closure::val, y)));
    verify(8, extension::invoke_r<int64_t>(&closure::val, y));
    STATIC_ASSERT_COND(noexcept(extension::invoke(&closure::val, cy)));
    verify(8, extension::invoke(&closure::val, cy));
    STATIC_ASSERT_COND(noexcept(extension::invoke(&closure::val, &y)));
    verify(8, extension::invoke(&closure::val, &y));
    auto const fn = &closure::noexcept_get;
    auto ref_fn = std::ref(fn);
#if __cplusplus >= 201703L
    STATIC_ASSERT_COND(noexcept(extension::invoke(fn, y)));
    STATIC_ASSERT_COND(noexcept(extension::invoke(fn, cy)));
    STATIC_ASSERT_COND(noexcept(extension::invoke(ref_fn, y)));
#else
    STATIC_ASSERT_COND(!noexcept(extension::invoke(fn, y)));
    STATIC_ASSERT_COND(!noexcept(extension::invoke(fn, cy)));
    STATIC_ASSERT_COND(!noexcept(extension::invoke(ref_fn, y)));
#endif
    verify(8, extension::invoke(ref_fn, y));
    STATIC_ASSERT_COND(!noexcept(extension::invoke(&closure::get, cy)));
    verify(8, extension::invoke(&closure::get, cy));
    verify(8, extension::invoke(&closure::get, y));
    verify(7, extension::invoke(&closure::get, closure(7)));
    verify(11, extension::invoke(&closure::apply, closure(6), 5));
}

template<class ...Args>
static inline std::true_type
is_invocable_test(decltype(extension::invoke(std::declval<Args>()...), 0));

template<class...>
static inline std::false_type is_invocable_test(...);

template<class Ret, class ...Args>
static inline std::true_type
is_invocable_r_test(decltype(extension::invoke_r<Ret>(std::declval<Args>()...), 0));

template<class...>
static inline std::false_type is_invocable_r_test(...);

#define STATIC_ASSERT_IS_INVOCABLE(...) static_assert(decltype(is_invocable_test<__VA_ARGS__>(0))::value, "is_invocable_v<" #__VA_ARGS__ ">")
#define STATIC_ASSERT_NOT_INVOCABLE(...) static_assert(!decltype(is_invocable_test<__VA_ARGS__>(0))::value, "!is_invocable_v<" #__VA_ARGS__ ">")
#define STATIC_ASSERT_IS_INVOCABLE_R(...) static_assert(decltype(is_invocable_r_test<__VA_ARGS__>(0))::value, "is_invocable_r_v<" #__VA_ARGS__ ">")
#define STATIC_ASSERT_NOT_INVOCABLE_R(...) static_assert(!decltype(is_invocable_r_test<__VA_ARGS__>(0))::value, "!is_invocable_r_v<" #__VA_ARGS__ ">")
#define STATIC_ASSERT_INVOKE_RESULT(result, ...) static_assert(std::is_same<result, typename extension::invoke_result<__VA_ARGS__>::type>::value, "is_same_v<" #result ", invoke_result_t<" #__VA_ARGS__ "> >")

#ifdef __GNUC__
#pragma GCC diagnostic push
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wzero-length-array"
#else
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#endif
STATIC_ASSERT_IS_INVOCABLE(void(int(&)[0]), int(&)[0]);
STATIC_ASSERT_INVOKE_RESULT(void, void(int(&)[0]), int(&)[0]);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

STATIC_ASSERT_IS_INVOCABLE(int());
STATIC_ASSERT_NOT_INVOCABLE(int() const);

template<class>
struct changeable;

STATIC_ASSERT_NOT_INVOCABLE_R(void, changeable<int>, int); // incomplete type

template<>
struct changeable<int> {
    void operator()(int) noexcept {}
};

STATIC_ASSERT_IS_INVOCABLE_R(void, changeable<int>, int); // should be changed

struct Fn {
    std::true_type operator()(std::input_iterator_tag) const { return {}; }

    std::false_type operator()(...) const { return {}; }
};

struct incomplete_type;

STATIC_ASSERT_NOT_INVOCABLE(Fn, incomplete_type);


struct incomplete_type : std::forward_iterator_tag {
};

STATIC_ASSERT_IS_INVOCABLE(Fn, incomplete_type);
STATIC_ASSERT_V(
        std::is_same, std::true_type,
        decltype(extension::invoke(std::declval<Fn>(), std::declval<incomplete_type>())));
STATIC_ASSERT_INVOKE_RESULT(std::true_type, Fn, incomplete_type);

void test2() {
    STATIC_ASSERT_V(extension::is_invocable, changeable<int>, int);
    struct {
        void operator()() const {
        }
    } op;
    extension::invoke(op);
}

int main() {
    test1();
    test2();
}

#else

int main() {
    return 0;
}

#endif
