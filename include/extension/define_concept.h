#pragma once

#include "define_bool.h"
#include "type_traits.h"

#if __cplusplus >= 202002L || !(defined(__GNUC__) && !defined(__clang__) && !defined(__EDG__))
#define VA_OPT_TEST_PICK_UP(a, b, c, ...) c
#define IS_VA_OPT_SUPPORTED(...) VA_OPT_TEST_PICK_UP(__VA_OPT__(0,0),1,0,)
#else
#define IS_VA_OPT_SUPPORTED(...) 0
#endif

#if __cpp_concepts

#define DEFINE_CONCEPT_UNSAFE(temp, name, ...) template<temp> concept name = PARAM_PACK(__VA_ARGS__);
#if IS_VA_OPT_SUPPORTED(?)
#define TYPE_CONCEPT(type, concept, ...) concept __VA_OPT__(<__VA_ARGS__>) type
#else
#define TYPE_CONCEPT(type, concept, ...) concept<__VA_ARGS__> type
#endif
#define REQUIRE_CONCEPT(concept, ...) requires(concept<__VA_ARGS__>)
#define ENABLE_IF_CONCEPT(content, ...) content

#else /* __cpp_concepts */

#define DEFINE_CONCEPT_UNSAFE(temp, name, ...) DEFINE_BOOL(PARAM_PACK(temp), PARAM_PACK(name), PARAM_PACK(__VA_ARGS__))
#define TYPE_CONCEPT(type, ...) class type
#define REQUIRE_CONCEPT(...)

#if LIB_TYPE_TRAITS
#define ENABLE_IF_CONCEPT(content, ...) typename std::enable_if<CONCEPT_GET(__VA_ARGS__), content>::type
#else
#define ENABLE_IF_CONCEPT(content, ...) typename extension::enable_if<CONCEPT_GET(__VA_ARGS__), content>::type
#endif

#endif /* __cpp_concepts */

#define CONCEPT_GET(concept, ...) BOOL_GET(PARAM_PACK(concept), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT1_UNSAFE(name, type1, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(class type1), name, PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT2_UNSAFE(name, type1, type2, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(class type1, class type2), name, PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT3_UNSAFE(name, type1, type2, type3, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(class type1, class type2, class type3), name, PARAM_PACK(__VA_ARGS__))

#if __cplusplus >= 201103L

#if __cpp_concepts

#if IS_VA_OPT_SUPPORTED(?)
#define IMPL_EXPR_IS_TO_PRD(exp, chain, ...) requires { { exp } -> chain __VA_OPT__(<__VA_ARGS__>); }
#else
#define IMPL_EXPR_IS_TO_PRD(exp, chain, ...) requires { { exp } -> chain<__VA_ARGS__>; }
#endif

#define DEFINE_CONCEPT_PREPEND_COND_DETAILS(temp1, temp2, temp3, name, reuse, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_EXP_DETAILS(temp1, temp2, temp3, name, reuse, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, requires { PARAM_PACK(__VA_ARGS__); })
#define DEFINE_CONCEPT_PREPEND_EXP_IS_DETAILS(temp1, temp2, temp3, name, reuse, exp, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, IMPL_EXPR_IS_TO_PRD(PARAM_PACK(exp), __VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_COND_EXP_DETAILS(temp1, temp2, temp3, name, reuse, cond, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, (cond) && requires { PARAM_PACK(__VA_ARGS__); })

#define DEFINE_CONCEPT_APPEND_COND_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_APPEND_EXP_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, requires { PARAM_PACK(__VA_ARGS__); })
#define DEFINE_CONCEPT_APPEND_EXP_IS_DETAIL(temp1, temp2, name, reuse, exp, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, IMPL_EXPR_IS_TO_PRD(PARAM_PACK(exp), __VA_ARGS__))
#define DEFINE_CONCEPT_APPEND_COND_EXP_DETAIL(temp1, temp2, name, reuse, cond, ...) DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), name, (cond) && requires { PARAM_PACK(__VA_ARGS__); })

#else /* __cpp_concepts */

#define IMPL_CONCEPT_STRUCT_COMMON(temp1, name, temp2, content) \
template<temp1> struct name final : std::false_type {};         \
template<temp2> struct name<content> final : std::true_type {};

#define IMPL_CONCEPT_BOTH_COMMON(temp3, s_name, temp2, impl, temp1, name, param)                        \
IMPL_CONCEPT_STRUCT_COMMON(PARAM_PACK(temp3), PARAM_PACK(s_name), PARAM_PACK(temp2), PARAM_PACK(impl))  \
DEFINE_CONCEPT_UNSAFE(PARAM_PACK(temp1), PARAM_PACK(name), PARAM_PACK(s_name)<param>::value)

#define IMPL_COND_TO_VOID(...) typename std::enable_if<(__VA_ARGS__)>::type
#define IMPL_EXPR_TO_VOID(...) decltype(void((__VA_ARGS__)))
#if IS_VA_OPT_SUPPORTED(?)
#define IMPL_EXPR_IS_TO_VOID(exp, chain, ...) typename std::enable_if<CONCEPT_GET(chain, decltype((exp)) __VA_OPT__(,) PARAM_PACK(__VA_ARGS__))>::type
#elif defined(_MSC_VER) && !defined(__clang__)
#define IMPL_EXPR_IS_TO_VOID(exp, chain, ...) typename std::enable_if<CONCEPT_GET(chain, decltype((exp)), __VA_ARGS__)>::type
#else
#define IMPL_EXPR_IS_TO_VOID(exp, chain, ...) typename std::enable_if<CONCEPT_GET(chain, decltype((exp)), ##__VA_ARGS__)>::type
#endif

#if __cpp_lib_void_t
#define IMPL_COND_EXPR_TO_VOID(cond, ...) std::void_t<typename std::enable_if<(cond)>::type, decltype(__VA_ARGS__)>
#else
#define IMPL_COND_EXPR_TO_VOID(cond, ...) extension::void_t<typename std::enable_if<(cond)>::type, decltype(__VA_ARGS__)>
#endif

#define IMPL_CONCEPT_PREPEND(temp1, temp2, temp3, name, reuse1, reuse2, void_exp)   \
IMPL_CONCEPT_BOTH_COMMON(                                                           \
    PARAM_PACK(class, temp3),                                                       \
    PARAM_PACK(CONCEPT_STRUCT_ ## name),                                            \
    PARAM_PACK(temp2),                                                              \
    PARAM_PACK(void_exp, reuse2),                                                   \
    PARAM_PACK(temp1),                                                              \
    PARAM_PACK(name),                                                               \
    PARAM_PACK(void, reuse1))

#define DEFINE_CONCEPT_PREPEND_COND_DETAILS(temp1, temp2, temp3, name, reuse, ...) IMPL_CONCEPT_PREPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp3), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_COND_TO_VOID(__VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_EXP_DETAILS(temp1, temp2, temp3, name, reuse, ...) IMPL_CONCEPT_PREPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp3), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_EXPR_TO_VOID(__VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_EXP_IS_DETAILS(temp1, temp2, temp3, name, reuse, exp, ...) IMPL_CONCEPT_PREPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp3), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_EXPR_IS_TO_VOID(PARAM_PACK(exp), __VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_COND_EXP_DETAILS(temp1, temp2, temp3, name, reuse, cond, ...) IMPL_CONCEPT_PREPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp3), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_COND_EXPR_TO_VOID(PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__)))

#define IMPL_CONCEPT_APPEND(temp1, temp2, name, reuse1, reuse2, void_exp)   \
IMPL_CONCEPT_BOTH_COMMON(                                                   \
    PARAM_PACK(temp2, class),                                               \
    PARAM_PACK(CONCEPT_STRUCT_ ## name),                                    \
    PARAM_PACK(temp2),                                                      \
    PARAM_PACK(reuse2, void_exp),                                           \
    PARAM_PACK(temp1),                                                      \
    PARAM_PACK(name),                                                       \
    PARAM_PACK(reuse1, void))

#define DEFINE_CONCEPT_APPEND_COND_DETAIL(temp1, temp2, name, reuse, ...) IMPL_CONCEPT_APPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_COND_TO_VOID(__VA_ARGS__))
#define DEFINE_CONCEPT_APPEND_EXP_DETAIL(temp1, temp2, name, reuse, ...) IMPL_CONCEPT_APPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_EXPR_TO_VOID(__VA_ARGS__))
#define DEFINE_CONCEPT_APPEND_EXP_IS_DETAIL(temp1, temp2, name, reuse, exp, ...) IMPL_CONCEPT_APPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_EXPR_IS_TO_VOID(PARAM_PACK(exp), __VA_ARGS__))
#define DEFINE_CONCEPT_APPEND_COND_EXP_DETAIL(temp1, temp2, name, reuse, cond, ...) IMPL_CONCEPT_APPEND(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(reuse), IMPL_COND_EXPR_TO_VOID(PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__)))

#endif /* __cpp_concepts */

#define DEFINE_CONCEPT_PREPEND_COND_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_PREPEND_COND_DETAILS(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_EXP_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_PREPEND_EXP_DETAILS(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_EXP_IS_DETAIL(temp1, temp2, name, reuse, exp, ...) DEFINE_CONCEPT_PREPEND_EXP_IS_DETAILS(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT_PREPEND_COND_EXP_DETAIL(temp1, temp2, name, reuse, cond, ...) DEFINE_CONCEPT_PREPEND_COND_EXP_DETAILS(PARAM_PACK(temp1), PARAM_PACK(temp2), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))

#define DEFINE_CONCEPT_PREPEND_COND(temp, name, reuse, ...) DEFINE_CONCEPT_PREPEND_COND_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_EXP(temp, name, reuse, ...) DEFINE_CONCEPT_PREPEND_EXP_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_PREPEND_EXP_IS(temp, name, reuse, exp, ...) DEFINE_CONCEPT_PREPEND_EXP_IS_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT_PREPEND_COND_EXP(temp, name, reuse, cond, ...) DEFINE_CONCEPT_PREPEND_COND_EXP_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))

#if CONCEPT_PREFER_PREPEND
#define DEFINE_CONCEPT_COND_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_PREPEND_COND_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_EXP_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_PREPEND_EXP_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_EXP_IS_DETAIL(temp1, temp2, name, reuse, exp, ...) DEFINE_CONCEPT_PREPEND_EXP_IS_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT_COND_EXP_DETAIL(temp1, temp2, name, reuse, cond, ...) DEFINE_CONCEPT_PREPEND_COND_EXP_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))
#else /* CONCEPT_PREFER_PREPEND */
#define DEFINE_CONCEPT_COND_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_APPEND_COND_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_EXP_DETAIL(temp1, temp2, name, reuse, ...) DEFINE_CONCEPT_APPEND_EXP_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_EXP_IS_DETAIL(temp1, temp2, name, reuse, exp, ...) DEFINE_CONCEPT_APPEND_EXP_IS_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT_COND_EXP_DETAIL(temp1, temp2, name, reuse, cond, ...) DEFINE_CONCEPT_APPEND_COND_EXP_DETAIL(PARAM_PACK(temp1), PARAM_PACK(temp2), name, PARAM_PACK(reuse), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))
#endif /* CONCEPT_PREFER_PREPEND */

#define DEFINE_CONCEPT_COND(temp, name, reuse, ...) DEFINE_CONCEPT_COND_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_EXP(temp, name, reuse, ...) DEFINE_CONCEPT_EXP_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT_EXP_IS(temp, name, reuse, exp, ...) DEFINE_CONCEPT_EXP_IS_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT_COND_EXP(temp, name, reuse, cond, ...) DEFINE_CONCEPT_COND_EXP_DETAIL(PARAM_PACK(temp), PARAM_PACK(temp), name, PARAM_PACK(reuse), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT1_COND(name, type1, ...) DEFINE_CONCEPT_COND(PARAM_PACK(class type1), name, PARAM_PACK(type1), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT1_EXP(name, type1, ...) DEFINE_CONCEPT_EXP(PARAM_PACK(class type1), name, PARAM_PACK(type1), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT1_EXP_IS(name, type1, exp, ...) DEFINE_CONCEPT_EXP_IS(PARAM_PACK(class type1), name, PARAM_PACK(type1), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT1_COND_EXP(name, type1, cond, ...) DEFINE_CONCEPT_COND_EXP(PARAM_PACK(class type1), name, PARAM_PACK(type1), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT2_COND(name, type1, type2, ...) DEFINE_CONCEPT_COND(PARAM_PACK(class type1, class type2), name, PARAM_PACK(type1, type2), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT2_EXP(name, type1, type2, ...) DEFINE_CONCEPT_EXP(PARAM_PACK(class type1, class type2), name, PARAM_PACK(type1, type2), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT2_EXP_IS(name, type1, type2, exp, ...) DEFINE_CONCEPT_EXP_IS(PARAM_PACK(class type1, class type2), name, PARAM_PACK(type1, type2), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT2_COND_EXP(name, type1, type2, cond, ...) DEFINE_CONCEPT_COND_EXP(PARAM_PACK(class type1, class type2), name, PARAM_PACK(type1, type2), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT3_COND(name, type1, type2, type3, ...) DEFINE_CONCEPT_COND(PARAM_PACK(class type1, class type2, class type3), name, PARAM_PACK(type1, type2, type3), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT3_EXP(name, type1, type2, type3, ...) DEFINE_CONCEPT_EXP(PARAM_PACK(class type1, class type2, class type3), name, PARAM_PACK(type1, type2, type3), PARAM_PACK(__VA_ARGS__))
#define DEFINE_CONCEPT3_EXP_IS(name, type1, type2, type3, exp, ...) DEFINE_CONCEPT_EXP_IS(PARAM_PACK(class type1, class type2, class type3), name, PARAM_PACK(type1, type2, type3), PARAM_PACK(exp), __VA_ARGS__)
#define DEFINE_CONCEPT3_COND_EXP(name, type1, type2, type3, cond, ...) DEFINE_CONCEPT_COND_EXP(PARAM_PACK(class type1, class type2, class type3), name, PARAM_PACK(type1, type2, type3), PARAM_PACK(cond), PARAM_PACK(__VA_ARGS__))

#endif /* 201103L */

#define CONCEPT_IF_1(concept, type1, ...) ENABLE_IF_CONCEPT(PARAM_PACK(__VA_ARGS__), concept, type1)
#define CONCEPT_IF_2(concept, type1, type2, ...) ENABLE_IF_CONCEPT(PARAM_PACK(__VA_ARGS__), concept, type1, type2)
#define CONCEPT_IF_3(concept, type1, type2, type3, ...) ENABLE_IF_CONCEPT(PARAM_PACK(__VA_ARGS__), concept, type1, type2, type3)
#define CONCEPT_IF_4(concept, type1, type2, type3, type4, ...) ENABLE_IF_CONCEPT(PARAM_PACK(__VA_ARGS__), concept, type1, type2, type3, type4)
#define CONCEPT_IF_5(concept, type1, type2, type3, type4, type5, ...) ENABLE_IF_CONCEPT(PARAM_PACK(__VA_ARGS__), concept, type1, type2, type3, type4, type5)
