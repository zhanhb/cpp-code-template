// 17.4.1.2 Headers

// C
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
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

#if __cplusplus >= 201103L
#include <cfenv>
#include <cinttypes>
#include <cstdint>
#if __has_include(<cuchar>)
#include <cuchar>
#endif
#include <cwchar>
#include <cwctype>
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

#if __cplusplus >= 201402L
#if __has_include(<shared_mutex>)
#include <shared_mutex>
#endif
#endif

#if __cplusplus >= 201703L
#if __has_include(<any>)
#include <any>
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

#if __cplusplus >= 202002L
#if __has_include(<barrier>)
#include <barrier>
#endif
#if __has_include(<bit>)
#include <bit>
#endif
#if __has_include(<charconv>)
#include <charconv>
#endif
#if __has_include(<compare>)
#include <compare>
#endif
#if __has_include(<concepts>)
#include <concepts>
#endif
#if __cpp_impl_coroutine
#if __has_include(<coroutine>)
#include <coroutine>
#endif
#endif
#if __has_include(<format>)
#include <format>
#endif
#if __has_include(<latch>)
#include <latch>
#endif
#if __has_include(<numbers>)
#include <numbers>
#endif
#if __has_include(<ranges>)
#include <ranges>
#endif
#if __has_include(<semaphore>)
#include <semaphore>
#endif
#if __has_include(<source_location>)
#include <source_location>
#endif
#if __has_include(<span>)
#include <span>
#endif
#if __has_include(<stop_token>)
#include <stop_token>
#endif
#if __has_include(<syncstream>)
#include <syncstream>
#endif
#if __has_include(<version>)
#include <version>
#endif
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
