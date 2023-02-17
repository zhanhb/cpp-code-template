#pragma once

#if __cplusplus >= 201103L

#include <chrono>

#else

#include <ctime>

#endif

#include <iostream>
#include "extension/functional/invoke.h"

class Measure {
    const char *file;
    int line;
#if __cplusplus >= 201103L
    std::chrono::high_resolution_clock::time_point start{};

    inline static std::chrono::high_resolution_clock::time_point get_now() {
        return std::chrono::high_resolution_clock::now();
    }

    template<class Tp>
    inline Tp get_diff_as() { return std::chrono::duration<Tp>(get_now() - start).count(); }

#else
    clock_t start;

    inline static clock_t get_now() { return clock(); }

    template<class Tp>
    inline Tp get_diff_as() { return (get_now() - start) / Tp(CLOCKS_PER_SEC); }

#endif

public:
    explicit Measure(const char *file, int line) : file(file), line(line), start(get_now()) {}

    ~Measure() {
        long double escaped = get_diff_as<long double>(); // NOLINT(modernize-use-auto)
        std::cerr << file << ":" << line << ": " << escaped << "s" << std::endl;
    }
};

#if __cplusplus >= 201103L

template<class Fn, class ...Args>
auto wrapper_measure(const char *file, int line, Fn &&fn, Args &&...args) ->
decltype(std::forward<Fn>(fn)(std::forward<Args>(args)...)) {
    Measure sm(file, line);
    return std::forward<Fn>(fn)(std::forward<Args>(args)...);
}

#else

template<class Tp>
Tp wrapper_measure(const char *file, int line, Tp (*fun)()) {
    Measure sm(file, line);
    return fun();
}

#endif

#define CONCAT(a, b) a ## b
#define EXPAND_CONCAT(a, b) CONCAT(a, b)
#define scoped_measure() Measure EXPAND_CONCAT(measure, __LINE__)(__FILE__, __LINE__)
#define measure(...) wrapper_measure(__FILE__, __LINE__, __VA_ARGS__)
