#pragma once

#if __cplusplus >= 201103L

#include <chrono>

#else

#include <ctime>

#endif

#include <iostream>

class [[maybe_unused]] Measure {
    const char *file;
    int line;
#if __cplusplus >= 201103L
    std::chrono::time_point<std::chrono::high_resolution_clock> start{};

    inline static std::chrono::time_point<std::chrono::high_resolution_clock> get_now() {
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

    explicit Measure(const char *file, int line, const std::string &name) noexcept: Measure(file, line) {}

    ~Measure() { // NOLINT(modernize-use-equals-default)
        long double escaped = get_diff_as<long double>(); // NOLINT(modernize-use-auto)
        std::cerr << file << ":" << line << ": " << escaped << "s" << std::endl;
    }
};

template<class Tp>
auto wrapper_measure(const char *file, int line, Tp &&tp) -> decltype(tp()) {
    Measure sm(file, line);
    return tp();
}

#define CONCAT(a, b) a ## b
#define EXPAND_CONCAT(a, b) CONCAT(a, b)
#define scoped_measure() Measure EXPAND_CONCAT(measure, __LINE__)(__FILE__, __LINE__)
#define measure(...) wrapper_measure(__FILE__, __LINE__, __VA_ARGS__)
