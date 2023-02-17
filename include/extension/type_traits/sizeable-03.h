#pragma once

#if __cplusplus && __cplusplus < 201103L

#include <cstddef>
#include "integral_constant.h"

#define SIZEABLE(...) extension::integral_constant<std::size_t, sizeof(__VA_ARGS__)>

#endif
