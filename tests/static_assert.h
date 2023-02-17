#pragma once

#define STATIC_ASSERT_DO_CONCAT(a, b, c) a ## b ## c
#define STATIC_ASSERT_EXPAND_CONCAT(a, b, c) STATIC_ASSERT_DO_CONCAT(a, b, c)

#if __cpp_static_assert >= 201411L
#define STATIC_ASSERT_COND(...) static_assert(__VA_ARGS__)
#define STATIC_ASSERT_MSG(...) static_assert(__VA_ARGS__)
#elif __cplusplus >= 201103L
#define STATIC_ASSERT_MSG(...) static_assert(__VA_ARGS__)
#define STATIC_ASSERT_COND(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#elif defined(__clang__)
#define STATIC_ASSERT_MSG(...) _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wc11-extensions\"") _Static_assert(__VA_ARGS__) _Pragma("clang diagnostic pop")
#define STATIC_ASSERT_COND(...) STATIC_ASSERT_MSG(__VA_ARGS__, #__VA_ARGS__)
#elif __cplusplus
namespace extension {
    // @formatter:off
    template<bool> struct STATIC_ASSERTION_FAILURE {};
    template<> struct STATIC_ASSERTION_FAILURE<true> : extension::true_type {};
    template<class Tp, bool = Tp::value> struct STATIC_ASSERTION_TRUE {};
    template<class Tp> struct STATIC_ASSERTION_TRUE<Tp, true> : extension::true_type {};
    template<class Tp, bool = Tp::value> struct STATIC_ASSERTION_FALSE {};
    template<class Tp> struct STATIC_ASSERTION_FALSE<Tp, false> : extension::true_type {};
    // @formatter:on
}

#include "extension/config.h"

#define STATIC_ASSERT_COND(...) MAYBE_UNUSED typedef BOOL_CONSTANT(extension::STATIC_ASSERTION_FAILURE<(__VA_ARGS__)>::value) STATIC_ASSERT_EXPAND_CONCAT(____static_assert_typedef_, __LINE__, ____)
#define STATIC_ASSERT_TRUE_TYPE(...) MAYBE_UNUSED typedef BOOL_CONSTANT(extension::STATIC_ASSERTION_TRUE<__VA_ARGS__>::value) STATIC_ASSERT_EXPAND_CONCAT(____static_assert_typedef_, __LINE__, ____)
#define STATIC_ASSERT_FALSE_TYPE(...) MAYBE_UNUSED typedef BOOL_CONSTANT(extension::STATIC_ASSERTION_FALSE<__VA_ARGS__>::value) STATIC_ASSERT_EXPAND_CONCAT(____static_assert_typedef_, __LINE__, ____)
#else
#define STATIC_ASSERT_COND(...) _Static_assert(__VA_ARGS__)
#define STATIC_ASSERT_MSG(...) _Static_assert(__VA_ARGS__)
#endif

#ifdef STATIC_ASSERT_MSG
#define STATIC_ASSERT_V(tpl, ...) STATIC_ASSERT_MSG(tpl<__VA_ARGS__>::value, #tpl "_v<" #__VA_ARGS__ ">")
#define STATIC_ASSERT_NOT_V(tpl, ...) STATIC_ASSERT_MSG(!tpl<__VA_ARGS__>::value, "!" #tpl "_v<" #__VA_ARGS__ ">")
#define STATIC_ASSERT_CONCEPT(concept, ...) STATIC_ASSERT_MSG(CONCEPT_GET(concept, __VA_ARGS__), #concept "<" #__VA_ARGS__ ">")
#define STATIC_ASSERT_NOT_CONCEPT(concept, ...) STATIC_ASSERT_MSG(!CONCEPT_GET(concept, __VA_ARGS__), "!" #concept "<" #__VA_ARGS__ ">")
#else
#define STATIC_ASSERT_V(tpl, ...) STATIC_ASSERT_TRUE_TYPE(tpl<__VA_ARGS__>)
#define STATIC_ASSERT_NOT_V(tpl, ...) STATIC_ASSERT_FALSE_TYPE(tpl<__VA_ARGS__>)
#define STATIC_ASSERT_CONCEPT(concept, ...) STATIC_ASSERT_V(concept, __VA_ARGS__)
#define STATIC_ASSERT_NOT_CONCEPT(concept, ...) STATIC_ASSERT_NOT_V(concept, __VA_ARGS__)
#endif

#if LIB_TYPE_TRAITS
#define STATIC_ASSERT_SAME_AS(...) STATIC_ASSERT_V(std::is_same, __VA_ARGS__)
#define STATIC_ASSERT_NOT_SAME_AS(...) STATIC_ASSERT_NOT_V(std::is_same, __VA_ARGS__)
#else

#include "extension/type_traits/is_same.h"

#define STATIC_ASSERT_SAME_AS(...) STATIC_ASSERT_V(extension::is_same, __VA_ARGS__)
#define STATIC_ASSERT_NOT_SAME_AS(...) STATIC_ASSERT_NOT_V(extension::is_same, __VA_ARGS__)
#endif
