#pragma once

#if !defined(__cplusplus) || __cplusplus < 201103L
#error C++ 11 required
#endif

#include <functional>
#include <tuple>
#include <type_traits>

namespace hash_tuple_detail {

#if __cplusplus >= 201703L
    using std::void_t;
#else
    template<class...>
    struct void_t_impl {
        typedef void type;
    };
    template<class ...Args> using void_t = typename void_t_impl<Args...>::type;
#endif

    template<class Key, class = std::hash<Key>, class = void>
    struct has_enabled_hash : std::false_type {
    };
    template<class Key, class Hash>
    struct has_enabled_hash<Key, Hash, void_t<
            // to avoid implicit instantiation of undefined template
            decltype(size_t(Hash()(std::declval<Key const &>())))
    > > : std::integral_constant<bool,
            std::is_copy_constructible<Hash>::value &&
            std::is_move_constructible<Hash>::value &&
            std::is_default_constructible<Hash>::value &&
            std::is_destructible<Hash>::value
    > {
    };

    template<class Type, class>
    using enable_hash_helper_imp = Type;

    template<class Type, class ...Keys>
    using enable_hash_helper = enable_hash_helper_imp<Type,
            void_t<typename std::enable_if<has_enabled_hash<Keys>::value>::type...>
    >;
}

template<class First, class ...Rest>
class std::hash<hash_tuple_detail::enable_hash_helper<
        std::tuple<First, Rest...>,
        typename std::remove_const<First>::type,
        typename std::remove_const<Rest>::type...
> > {

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
