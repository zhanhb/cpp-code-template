#pragma once

#include "type_traits/integral_constant.h"

#define PARAM_PACK(...) __VA_ARGS__

#if __cpp_variable_templates
#define BOOL_GET(var, ...) var<__VA_ARGS__>
#if __cpp_inline_variables >= 201606L
#define DEFINE_BOOL(temp, name, ...) template<temp> inline constexpr bool PARAM_PACK(name) = __VA_ARGS__;
#else
#define DEFINE_BOOL(temp, name, ...) template<temp> constexpr bool PARAM_PACK(name) = __VA_ARGS__;
#endif
#else /* __cpp_variable_templates */
#define BOOL_GET(var, ...) var<__VA_ARGS__>::value
#if __cplusplus >= 201103L
#define DEFINE_BOOL(temp, name, ...) template<temp> struct name final : BOOL_CONSTANT(__VA_ARGS__) {};
#else
#define DEFINE_BOOL(temp, name, ...) template<temp> struct name : BOOL_CONSTANT(__VA_ARGS__) {};
#endif
#endif /* __cpp_variable_templates */
