#pragma once

#if !defined(__cplusplus) || __cplusplus < 201103L
#error C++ 11 required
#endif

#include <functional>
#include <tuple>
#include <type_traits>

template<class First, class ...Rest>
class std::hash<std::tuple<First, Rest...> > {

    typedef std::tuple<First, Rest...> target_type;
    typedef std::tuple_size<target_type> tuple_size;

    constexpr static size_t do_combine(size_t seed, size_t h) {
        return seed ^ (h + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }

    template<size_t Ip, class Tp>
    constexpr static size_t calc(Tp &&t) {
        typedef std::hash<typename std::remove_const<typename std::tuple_element<Ip, target_type>::type>::type> base;
        return base()(std::get<Ip>(std::forward<Tp>(t)));
    }

    template<size_t Ip, class Tp>
    constexpr static typename std::enable_if<Ip + 1 == tuple_size::value, size_t>::type
    combine(size_t seed, Tp &&t) {
        return do_combine(seed, calc<Ip>(std::forward<Tp>(t)));
    }

    template<size_t Ip, class Tp>
    constexpr static typename std::enable_if<Ip + 1 < tuple_size::value, size_t>::type
    combine(size_t seed, Tp &&t) {
        return combine<Ip + 1>(do_combine(seed, calc<Ip>(std::forward<Tp>(t))), std::forward<Tp>(t));
    }

public:

    template<class Tp>
    constexpr size_t operator()(Tp &&t) const {
        return combine<0>(0, std::forward<Tp>(t));
    }
};
