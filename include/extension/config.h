#pragma once

#ifndef __has_cpp_attribute
#define __has_cpp_attribute(...) 0
#endif

#ifndef IF_CXX_11
#if __cplusplus >= 201103L
#define IF_CXX_11(...) __VA_ARGS__
#else
#define IF_CXX_11(...)
#endif
#endif

#ifndef IF_CXX_14
#if __cplusplus >= 201402L
#define IF_CXX_14(...) __VA_ARGS__
#else
#define IF_CXX_14(...)
#endif
#endif

#ifndef CONSTEXPR_11
#define CONSTEXPR_11 IF_CXX_11(constexpr)
#endif

#ifndef CONSTEXPR_14
#define CONSTEXPR_14 IF_CXX_14(constexpr)
#endif

#ifndef DEPRECATED
#if __cplusplus >= 201402L && __has_cpp_attribute(deprecated)
#define DEPRECATED [[deprecated]]
#elif defined(__GNUC__)
#define DEPRECATED __attribute__((deprecated))
#else
#define DEPRECATED
#endif
#endif

#ifndef MAYBE_UNUSED
#if __cplusplus >= 201703L && __has_cpp_attribute(maybe_unused)
#define MAYBE_UNUSED [[maybe_unused]]
#elif defined(__GNUC__)
#define MAYBE_UNUSED __attribute__((unused))
#else
#define MAYBE_UNUSED
#endif
#endif

#ifndef NODISCARD
#if __cplusplus >= 201703L && __has_cpp_attribute(nodiscard)
#define NODISCARD [[nodiscard]]
#elif defined(__GNUC__)
#define NODISCARD __attribute__((warn_unused_result))
#else
#define NODISCARD
#endif
#endif
