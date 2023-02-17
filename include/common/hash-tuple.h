#pragma once

#if !(__cplusplus >= 201103L)
#error C++ 11 required
#else

#include <functional>
#include <tuple>
#include <type_traits>

namespace hash_tuple_detail {

    template<class Key, class = std::hash<typename std::remove_const<Key>::type>, class = void>
    struct has_enabled_hash : std::false_type {
    };
    template<class Key, class Hash>
    struct has_enabled_hash<Key, Hash,
            typename std::enable_if<std::is_convertible<
                    // it's an undefined behavior to work with is_invocable_r
                    // to avoid implicit instantiation of undefined template
                    decltype(std::declval<Hash>()(std::declval<Key const &>())),
                    std::size_t
            >::value>::type
    > : std::integral_constant<bool,
            std::is_default_constructible<Hash>::value &&
            std::is_destructible<Hash>::value
    > {
    };

    template<class Tp, class...>
    using enable_hash_helper_impl = Tp;

    template<class Tp, class ...Keys>
    using enable_hash_helper = enable_hash_helper_impl<Tp,
            typename std::enable_if<has_enabled_hash<Keys>::value>::type...
    >;

    template<class Tp, std::size_t tuple_size = std::tuple_size<Tp>::value>
    struct hash_tuple_impl {
#if __cplusplus < 201703L
        typedef std::size_t result_type;
        typedef Tp argument_type;
#elif __cplusplus < 202002L
        [[deprecated]] typedef std::size_t result_type;
        [[deprecated]] typedef Tp argument_type;
#endif

    private:
        template<std::size_t Ip>
        using target_hash = std::hash<typename std::remove_const<typename std::tuple_element<Ip, Tp>::type>::type>;

        template<std::size_t Ip, class Up>
        static constexpr std::size_t calc(Up &&up)
        noexcept(noexcept(target_hash<Ip>()(std::get<Ip>(std::forward<Up>(up))))) {
            return target_hash<Ip>()(std::get<Ip>(std::forward<Up>(up)));
        }

#if __cpp_lib_integer_sequence

        static constexpr std::size_t do_combine(std::size_t &seed, std::size_t h) noexcept {
            return seed ^= h + 0x9E3779B9 + (seed << 6) + (seed >> 2);
        }

        template<class ...Args>
        static constexpr int swallow(Args &&...) noexcept { return 0; }

        template<class Up, std::size_t ...Ip>
        static constexpr std::size_t calculate(Up &&up, std::index_sequence<Ip...>)
        noexcept(noexcept(swallow(do_combine(std::declval<std::size_t &>(), calc<Ip>(std::forward<Up>(up)))...))) {
            std::size_t hash = 0;
            hash_tuple_impl::swallow(hash_tuple_impl::do_combine(hash, calc<Ip>(std::forward<Up>(up)))...);
            return hash;
        }

    public:

        template<class Up>
        constexpr std::size_t operator()(Up &&up) const
        noexcept(noexcept(hash_tuple_impl::calculate(std::forward<Up>(up), std::make_index_sequence<tuple_size>{}))) {
            return hash_tuple_impl::calculate(std::forward<Up>(up), std::make_index_sequence<tuple_size>{});
        }

#else

        static constexpr std::size_t do_combine(std::size_t seed, std::size_t h) noexcept {
            return seed ^ (h + 0x9E3779B9 + (seed << 6) + (seed >> 2));
        }

        template<std::size_t Ip, class Up>
        static constexpr typename std::enable_if<Ip + 1 == tuple_size, std::size_t>::type
        combine(std::size_t seed, Up &&up)
        noexcept(noexcept(hash_tuple_impl::do_combine(seed, calc<Ip>(std::forward<Up>(up))))) {
            return hash_tuple_impl::do_combine(seed, calc<Ip>(std::forward<Up>(up)));
        }

        template<std::size_t Ip, class Up>
        static constexpr typename std::enable_if<(Ip + 1 < tuple_size), std::size_t>::type
        combine(std::size_t seed, Up &&up)
        noexcept(noexcept(combine<Ip + 1>(hash_tuple_impl::do_combine(seed, calc<Ip>(std::forward<Up>(up))), std::forward<Up>(up)))) {
            return combine<Ip + 1>(hash_tuple_impl::do_combine(seed, calc<Ip>(std::forward<Up>(up))), std::forward<Up>(up));
        }

    public:

        template<class Up>
        constexpr std::size_t operator()(Up &&up) const
        noexcept(noexcept(combine<0>(0, std::forward<Up>(up)))) {
            return combine<0>(0, std::forward<Up>(up));
        }

#endif
    };
}

namespace std {
    // @formatter:off
    template<class First, class ...Rest>
    struct hash<hash_tuple_detail::enable_hash_helper<std::tuple<First, Rest...>, First, Rest...> > : // NOLINT(cert-dcl58-cpp)
            hash_tuple_detail::hash_tuple_impl<std::tuple<First, Rest...> > {
    };
    // @formatter:on
}

#endif
