#if defined(__has_include_next)
#if __has_include_next(<bits/stdc++.h>)
#pragma GCC system_header
#include_next <bits/stdc++.h>
#ifndef BITS_STDCPP_H
#define BITS_STDCPP_H
#endif
#endif
#endif

#ifndef BITS_STDCPP_H
#define BITS_STDCPP_H

// C
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#ifndef __has_include
#define __has_include(...) 0
#endif

#if __cplusplus >= 201103L
// @formatter:off
#include <cfenv>
#include <cinttypes>
#include <cstdint>
#if __has_include(<cuchar>)
#include <cuchar>
#endif
#include <cwchar>
#include <cwctype>
// @formatter:on
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

#endif

// @formatter:off
#if __cplusplus >= 201402L
#if __has_include(<shared_mutex>)
#include <shared_mutex>
#endif
#endif

#if __cplusplus >= 201703L
#if __has_include(<any>)
#include <any>
#endif
#if __has_include(<charconv>)
#include <charconv>
#endif
#if __has_include(<execution>)
#include <execution>
#endif
#if __has_include(<filesystem>)
#include <filesystem>
#endif
#if __has_include(<optional>)
#include <optional>
#endif
#if __has_include(<memory_resource>)
#include <memory_resource>
#endif
#if __has_include(<string_view>)
#include <string_view>
#endif
#if __has_include(<variant>)
#include <variant>
#endif
#endif

#if __has_include(<version>)
#include <version>
#endif

#if __cpp_lib_barrier
#include <barrier>
#endif
#if __cpp_lib_bit_cast || __cpp_lib_bitops || __cpp_lib_byteswap || __cpp_lib_endian || __cpp_lib_int_pow2
#include <bit>
#endif
#if __cpp_impl_three_way_comparison
#include <compare>
#endif
#if __cpp_lib_concepts
#include <concepts>
#endif
#if __cpp_impl_coroutine && __cpp_lib_coroutine
#include <coroutine>
#endif
#if __cpp_lib_format
#include <format>
#endif
#if __cpp_lib_latch
#include <latch>
#endif
#if __cpp_lib_math_constants
#include <numbers>
#endif
#if __cpp_lib_ranges
#include <ranges>
#endif
#if __cpp_lib_semaphore
#include <semaphore>
#endif
#if __cpp_lib_source_location
#include <source_location>
#endif
#if __cpp_lib_span
#include <span>
#endif
#if __cpp_lib_jthread
#include <stop_token>
#endif
#if __cpp_lib_syncbuf
#include <syncstream>
#endif

#if __cplusplus > 202002L
#if __has_include(<expected>)
#include <expected>
#endif
#if __has_include(<flat_map>)
#include <flat_map>
#endif
#if __has_include(<flat_set>)
#include <flat_set>
#endif
#if __has_include(<generator>)
#include <generator>
#endif
#if __has_include(<mdspan>)
#include <mdspan>
#endif
#if __has_include(<print>)
#include <print>
#endif
#if __has_include(<spanstream>)
#include <spanstream>
#endif
#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif
#endif

#if 201103L <= __cplusplus && __cplusplus <= 201402L
#if __has_include(<ccomplex>)
#include <ccomplex>
#endif
#if __has_include(<codecvt>)
#include <codecvt>
#endif
#if __has_include(<cstdalign>)
#include <cstdalign>
#endif
#if __has_include(<cstdbool>)
#include <cstdbool>
#endif
#if __has_include(<ctgmath>)
#include <ctgmath>
#endif
#endif

#if __cplusplus <= 201703L
#if __has_include(<ciso646>)
#include <ciso646>
#endif
#endif
// @formatter:on

#endif /* BITS_STDCPP_H */
