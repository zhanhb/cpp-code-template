#pragma once

#if !defined(__cplusplus) || __cplusplus < 201103L
#error C++ 11 required
#endif

#include <functional>
#include <tuple>
#include <type_traits>

namespace hash_tuple_detail {

    using std::false_type;
    using std::true_type;
    using std::tuple;

    template<class, class>
    struct is_in;
    template<class Tp>
    struct is_in<Tp, tuple<> > : false_type {
    };
    template<class Tp, class ...Rest>
    struct is_in<Tp, tuple<Tp, Rest...> > : true_type {
    };
    template<class Tp, class Head, class ...Rest>
    struct is_in<Tp, tuple<Head, Rest...> > : is_in<Tp, tuple<Rest...> >::type {
    };

    template<class Tp, class Current, class = typename is_in<Tp, Current>::type>
    struct prepend_unique;
    template<class Tp, class ...Current>
    struct prepend_unique<Tp, tuple<Current...>, true_type> {
        typedef tuple<Current...> type;
    };
    template<class Tp, class ...Current>
    struct prepend_unique<Tp, tuple<Current...>, false_type> {
        typedef tuple<Tp, Current...> type;
    };

    template<class ...>
    struct remove_duplicate;
    template<>
    struct remove_duplicate<> {
        typedef tuple<> type;
    };
    template<class Head, class ...Rest>
    struct remove_duplicate<Head, Rest...> {
        typedef typename prepend_unique<Head, typename remove_duplicate<Rest...>::type>::type type;
    };

    template<class>
    struct base;
    template<class ...Args>
    struct base<tuple<Args...> > : virtual std::hash<Args> ... {
    };
}

template<class First, class ...Rest>
class std::hash<std::tuple<First, Rest...> > :
        hash_tuple_detail::base<typename hash_tuple_detail::remove_duplicate<First, Rest...>::type> {

    typedef std::tuple<First, Rest...> target_type;
    typedef std::tuple_size<target_type> tuple_size;

    constexpr static size_t do_combine(size_t seed, size_t h) {
        return seed ^ (h + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }

    template<size_t Ip, class Tp>
    constexpr size_t calc(Tp &&t) const {
        typedef std::hash<typename std::remove_const<typename std::tuple_element<Ip, target_type>::type>::type> base;
        return base::operator()(std::get<Ip>(std::forward<Tp>(t)));
    }

    template<size_t Ip, class Tp>
    constexpr typename std::enable_if<Ip + 1 == tuple_size::value, size_t>::type
    combine(size_t seed, Tp &&t) const {
        return do_combine(seed, calc<Ip>(std::forward<Tp>(t)));
    }

    template<size_t Ip, class Tp>
    constexpr typename std::enable_if<Ip + 1 < tuple_size::value, size_t>::type
    combine(size_t seed, Tp &&t) const {
        return combine<Ip + 1>(do_combine(seed, calc<Ip>(std::forward<Tp>(t))), std::forward<Tp>(t));
    }

public:

    template<class Tp>
    constexpr size_t operator()(Tp &&t) const {
        return combine<0>(0, std::forward<Tp>(t));
    }
};
