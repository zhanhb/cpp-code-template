#pragma once

#ifdef LIB_TYPE_TRAITS
#if __cplusplus >= 201103L
#elif defined(__GNUC__) && !defined(__clang__) && LIB_TYPE_TRAITS
#undef LIB_TYPE_TRAITS
#define LIB_TYPE_TRAITS 0
#endif
#elif __cplusplus >= 201103L
#define LIB_TYPE_TRAITS 1
#elif defined(__clang__)
#define LIB_TYPE_TRAITS 1
#else
#define LIB_TYPE_TRAITS 0
#endif

#if LIB_TYPE_TRAITS

#include <type_traits>

#endif
