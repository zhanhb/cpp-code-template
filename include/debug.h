#ifndef DEBUG_H
#define DEBUG_H

#ifndef __cplusplus
#error C++ required
#endif

#include <fstream>
#include <iostream>
#include <locale>
#include <valarray>

#if __cplusplus >= 201103L

#include <tuple>

#if __cplusplus >= 201703L

#include <optional>
#include <variant>

#endif /* 201703L */
#endif

namespace debug {

#if __cplusplus >= 201103L
    namespace detail {
#if __cplusplus >= 201703L
        using std::bool_constant;
        using std::void_t;
#else /* __cplusplus < 201703L */
        template<bool b> using bool_constant = std::integral_constant<bool, b>;
        template<class...>
        struct void_t_impl {
            typedef void type;
        };
        template<class ...Args> using void_t = typename void_t_impl<Args...>::type;
#endif /* 201703L */
#if __cplusplus >= 201402L
        using std::is_final;
#else
#if defined(__GNUC__) || _MSC_VER >= 1800
        template<class Tp> using is_final = bool_constant<__is_final(Tp)>;
#elif __SUNPRO_CC >= 0x5130
        template<class Tp> using is_final = bool_constant<__oracle_is_final(Tp)>;
#else /* __SUNPRO_CC >= 0x5130 */
        template<class> using is_final = std::false_type;
#endif /* __GNUC__ || _MSC_VER */
#endif /* 201402L */

        template<class, template<class...> class>
        struct is_specialization_of : std::false_type {
        };
        template<template<class...> class Template, class... Args>
        struct is_specialization_of<Template<Args...>, Template> : std::true_type {
        };

        template<typename Tp, typename Up>
        class apply_cvref {
            static_assert(!std::is_reference<Up>::value, "");
            static_assert(!std::is_const<Up>::value, "");
            static_assert(!std::is_volatile<Up>::value, "");

            using Rp = typename std::remove_reference<Tp>::type;
            using U1 = typename std::conditional<std::is_const<Rp>::value, typename std::add_const<Up>::type, Up>::type;
            using U2 = typename std::conditional<std::is_volatile<Rp>::value, typename std::add_volatile<U1>::type, U1>::type;
            using U3 = typename std::conditional<std::is_lvalue_reference<Tp>::value, typename std::add_lvalue_reference<U2>::type, U2>::type;
            using U4 = typename std::conditional<std::is_rvalue_reference<Tp>::value, typename std::add_rvalue_reference<U3>::type, U3>::type;

        public:
            typedef U4 type;
        };

        template<class Tp, bool = std::is_move_constructible<Tp>::value, class = void>
        struct range_iterator {
        };
        template<class Tp>
        struct range_iterator<Tp, true, void_t<
                decltype(bool(std::declval<Tp &>() != std::declval<Tp &>())),
                decltype(++std::declval<Tp &>()),
                decltype(*std::declval<Tp &>())> > {
            using reference = decltype(*std::declval<Tp &>());
        };

        template<class, class = void>
        struct is_range : std::false_type {
        };
        template<class Tp>
        struct is_range<Tp, void_t<typename range_iterator<typename std::common_type<
                decltype(std::begin(std::declval<Tp &>())),
                decltype(std::end(std::declval<Tp &>()))
        >::type>::reference> > : std::true_type {
        };

        template<class, class = void>
        struct is_map_impl : std::false_type {
        };
        template<class Tp>
        struct is_map_impl<Tp, void_t<
                typename Tp::key_type,
                typename Tp::mapped_type,
                typename Tp::value_type>
        > : std::conditional<
                is_range<Tp>::value,
                typename is_specialization_of<typename Tp::value_type, std::pair>::type,
                std::false_type
        >::type {
        };
        template<class Tp>
        struct is_map : is_map_impl<typename std::remove_cv<Tp>::type> {
        };

        template<class Tp>
        struct is_collection : bool_constant<is_range<Tp>::value && !is_map<Tp>::value> {
        };

        template<class, class = void>
        struct is_tuple_impl : std::false_type {
        };
        template<class Tp>
        struct is_tuple_impl<Tp, void_t<decltype(std::tuple_size<Tp>::value)> > :
                std::is_integral<decltype(std::tuple_size<Tp>::value)>::type {
        };
        template<class Tp>
        struct is_tuple : is_tuple_impl<Tp>::type {
        };

        template<class Tp, bool = std::is_abstract<Tp>::value || is_final<Tp>::value>
        class has_member_c_test {

            template<class Up>
            constexpr static typename std::is_member_object_pointer<decltype(&Up::c)>::type test(int) { return {}; }

            template<class>
            constexpr static std::false_type test(...) { return {}; }

        public:
            using type = decltype(test<Tp>(0));
        };

        template<class Tp>
        class has_member_c_test<Tp, false> : Tp {

            template<class Up>
            constexpr static typename std::is_member_object_pointer<decltype(&Up::c)>::type test(int) { return {}; }

            template<class>
            constexpr static std::false_type test(...) { return {}; }

        public:
            using type = decltype(test<has_member_c_test>(0));
        };

        template<class Tp>
        struct has_member_c : has_member_c_test<Tp>::type {
        };

        template<class, class = void>
        struct container_type {
        };
        template<class Tp>
        struct container_type<Tp, void_t<typename Tp::container_type> > {
            using type = typename Tp::container_type;
        };

        template<class Tp,
                bool = std::is_abstract<Tp>::value || is_final<Tp>::value,
                bool = has_member_c<Tp>::value>
        struct typeof_member_c {
        };
        template<class Tp>
        struct typeof_member_c<Tp, true, true> {
            using type = decltype(Tp::c);
        };
        template<class Tp>
        struct typeof_member_c<Tp, false, true> : Tp {
            using type = decltype(typeof_member_c::c);
        };

        template<class, class = void>
        struct adapter_c_type {
        };
        template<class Tp>
        struct adapter_c_type<Tp, void_t<
                typename container_type<Tp>::type,
                typename typeof_member_c<Tp>::type>
        > : std::enable_if<
                std::is_same<
                        typename container_type<Tp>::type,
                        typename typeof_member_c<Tp>::type
                >::value,
                typename container_type<Tp>::type
        > {
        };

        template<class Tp, class = void>
        struct is_adapter : std::false_type {
        };
        template<class Tp>
        struct is_adapter<Tp, void_t<typename adapter_c_type<Tp>::type> >
                : is_range<typename adapter_c_type<Tp>::type> {
        };

        template<class Tp>
        struct is_char : bool_constant<
                std::is_same<Tp, char>::value
                || std::is_same<Tp, unsigned char>::value
                || std::is_same<Tp, signed char>::value
                || std::is_same<Tp, signed char>::value
                || std::is_same<Tp, wchar_t>::value
                || std::is_same<Tp, char16_t>::value
                || std::is_same<Tp, char32_t>::value
        > {
        };
#if __cplusplus >= 202002L
        template<>
        struct is_char<char8_t> : std::true_type {
        };
#endif
        template<class>
        struct is_string : std::false_type {
        };
        template<class Tp>
        struct is_string<Tp *> : is_char<typename std::remove_const<Tp>::type>::type {
        };
        template<class CharT, class Traits, class Alloc>
        struct is_string<std::basic_string<CharT, Traits, Alloc> > : is_char<CharT>::type {
        };
        template<template<class, class> class BasicStringView, class CharT>
        struct is_string<BasicStringView<CharT, std::char_traits<CharT> > > : is_char<CharT>::type {
        };

#if __cpp_lib_variant

        template<class Tp, bool = detail::is_specialization_of<
                typename std::remove_cv<typename std::remove_reference<Tp>::type>::type,
                std::variant
        >::value, class = void>
        struct sfinae_variant_size {
        };
        template<class Tp>
        struct sfinae_variant_size<Tp, true, void_t<typename std::variant_size<typename std::remove_reference<Tp>::type>::type> >
                : std::variant_size<typename std::remove_reference<Tp>::type>::type {
        };
#endif
    }

    template<class Tp> using is_adapter = typename detail::is_adapter<typename std::remove_reference<Tp>::type>::type;
    template<class Tp> using is_collection = typename detail::is_collection<typename std::remove_reference<Tp>::type>::type;
    template<class Tp> using is_map = typename detail::is_map<typename std::remove_reference<Tp>::type>::type;
    template<class Tp> using is_range = typename detail::is_range<typename std::remove_reference<Tp>::type>::type;
    template<class Tp> using is_string = typename detail::is_string<typename std::decay<Tp>::type>::type;
    template<class Tp> using is_tuple = typename detail::is_tuple<typename std::remove_reference<Tp>::type>::type;

    template<class Tp, bool = std::is_abstract<Tp>::value || detail::is_final<Tp>::value>
    struct adapter_helper {
        template<class Up>
        static auto get_container(Up &&adapter) -> decltype((std::forward<Up>(adapter).c)) {
            return std::forward<Up>(adapter).c;
        }
    };

    template<class Tp>
    struct adapter_helper<Tp, false> : Tp {
        template<class Up>
        static auto get_container(Up &&adapter) ->
        decltype((std::declval<typename detail::apply_cvref<Up &&, adapter_helper>::type>().c)) {
            return reinterpret_cast<
                    typename detail::apply_cvref<Up &&, adapter_helper>::type
                    >(std::forward<Up>(adapter)).c;
        }
    };

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &write_adapter(std::basic_ostream<CharT, Traits> &, Adapter &&);

    template<class CharT, class Traits, class Coll>
    inline std::basic_ostream<CharT, Traits> &
    write_collection(std::basic_ostream<CharT, Traits> &, Coll &&);

    template<class CharT, class Traits, class Map>
    inline std::basic_ostream<CharT, Traits> &
    write_map(std::basic_ostream<CharT, Traits> &, Map &&);

    template<class CharT, class Traits, class Tp>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &, Tp &&, char = ':');

    template<size_t Ip = 0, class Tp, size_t Np = std::tuple_size<typename std::remove_reference<Tp>::type>::value, class CharT, class Traits>
    inline typename std::enable_if<Ip == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &, Tp &&);

    template<size_t Ip = 0, class Tp, size_t Np = std::tuple_size<typename std::remove_reference<Tp>::type>::value, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &, Tp &&);

    template<size_t Ip = 0, class Tp, size_t Np = std::tuple_size<typename std::remove_reference<Tp>::type>::value, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 < Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &, Tp &&);

#if __cpp_lib_variant

    template<class CharT, class Traits, class Variant>
    inline std::basic_ostream<CharT, Traits> &write_variant(std::basic_ostream<CharT, Traits> &, Variant &&);

#endif

#else

    template<class Tp>
    struct adapter_helper : Tp {
        template<class Up>
        static typename Tp::container_type const &get_container(const Up &adapter) {
            return reinterpret_cast<const adapter_helper &>(adapter).c;
        }
    };

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &write_adapter(std::basic_ostream<CharT, Traits> &, const Adapter &);

    template<class CharT, class Traits, class Coll>
    inline std::basic_ostream<CharT, Traits> &
    write_collection(std::basic_ostream<CharT, Traits> &, const Coll &);

    template<class CharT, class Traits, class Map>
    inline std::basic_ostream<CharT, Traits> &
    write_map(std::basic_ostream<CharT, Traits> &, const Map &);

    template<class CharT, class Traits, class T1, class T2>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &, const std::pair<T1, T2> &, char = ':');

#endif
}

#include "write-wide.h"

#if __cplusplus >= 201103L

template<class CharT, class Traits, class Adapter>
inline typename std::enable_if<
        debug::is_adapter<Adapter>::value,
        std::basic_ostream<CharT, Traits> &
>::type operator<<(std::basic_ostream<CharT, Traits> &out, Adapter &&adapter) {
    return debug::write_adapter(out, std::forward<Adapter>(adapter));
}

template<class CharT, class Traits, class Coll>
inline typename std::enable_if<
        debug::is_collection<Coll>::value && !debug::is_string<Coll>::value,
        std::basic_ostream<CharT, Traits> &
>::type operator<<(std::basic_ostream<CharT, Traits> &out, Coll &&c) {
    return debug::write_collection(out, std::forward<Coll>(c));
}

template<class CharT, class Traits, class Map>
inline typename std::enable_if<
        debug::is_map<Map>::value,
        std::basic_ostream<CharT, Traits> &
>::type operator<<(std::basic_ostream<CharT, Traits> &out, Map &&map) {
    return debug::write_map(out, std::forward<Map>(map));
}

template<class CharT, class Traits, class Tp>
inline typename std::enable_if<
        debug::is_tuple<Tp>::value && !debug::is_range<Tp>::value,
        std::basic_ostream<CharT, Traits> &
>::type operator<<(std::basic_ostream<CharT, Traits> &out, Tp &&tuple) {
    return debug::write_tuple(out << '[', std::forward<Tp>(tuple)) << ']';
}

#if __cpp_lib_variant

template<class CharT, class Traits, class Variant>
inline typename std::enable_if<
        bool(debug::detail::sfinae_variant_size<typename std::remove_reference<Variant>::type>::value),
        std::basic_ostream<CharT, Traits> &
>::type operator<<(std::basic_ostream<CharT, Traits> &out, Variant &&var) {
    return debug::write_variant(out, std::forward<Variant>(var));
}

#endif
#else

#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>

template<class CharT, class Traits, class Tp, class Alloc>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::deque<Tp, Alloc> &dq) {
    return debug::write_collection(out, dq);
}

template<class CharT, class Traits, class Tp, class Alloc>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::list<Tp, Alloc> &l) {
    return debug::write_collection(out, l);
}

template<class CharT, class Traits, class Key, class Val, class Compare, class Alloc>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::map<Key, Val, Compare, Alloc> &mp) {
    return debug::write_map(out, mp);
}

template<class CharT, class Traits, class Key, class Val, class Compare, class Alloc>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::multimap<Key, Val, Compare, Alloc> &mp) {
    return debug::write_map(out, mp);
}

template<class CharT, class Traits, class Tp, class Compare, class Alloc>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::multiset<Tp, Compare, Alloc> &st) {
    return debug::write_collection(out, st);
}

template<class CharT, class Traits, class Tp, class Compare, class Alloc>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::set<Tp, Compare, Alloc> &st) {
    return debug::write_collection(out, st);
}

template<class CharT, class Traits, class Tp, class Alloc>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::vector<Tp, Alloc> &v) {
    return debug::write_collection(out, v);
}

template<class CharT, class Traits, class Tp, class Container, class Compare>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::priority_queue<Tp, Container, Compare> &q) {
    return debug::write_adapter(out, q);
}

template<class CharT, class Traits, class Tp, class Container>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::queue<Tp, Container> &q) {
    return debug::write_adapter(out, q);
}

template<class CharT, class Traits, class Tp, class Container>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::stack<Tp, Container> &s) {
    return debug::write_adapter(out, s);
}

template<class CharT, class Traits, class T1, class T2>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::pair<T1, T2> &p) {
    return debug::write_pair(out << '[', p, ',') << ']';
}

#endif

#if __cplusplus >= 201703L

template<class CharT, class Traits, class Tp>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::optional<Tp> &o) {
    return o.has_value() ? out << *o : out << "null";
}

#endif

#if __cplusplus >= 202002L

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, std::partial_ordering po) {
    if (po == 0) return out << "\"==\"";
    else if (po < 0) return out << "\"<\"";
    else if (po > 0) return out << "\">\"";
    else return out << "\"!=\"";
}

#endif

namespace debug {

    template<class CharT, class Traits, class It>
    inline std::basic_ostream<CharT, Traits> &
    write_collection_impl(std::basic_ostream<CharT, Traits> &out, It first, It last) {
        if (first != last) {
            for (out << '[';; out << ',') {
                out << *first;
                ++first;
                if (first != last) continue;
                return out << ']';
            }
        }
        return out << "[]";
    }

    template<class CharT, class Traits, class It>
    inline std::basic_ostream<CharT, Traits> &
    write_map_impl(std::basic_ostream<CharT, Traits> &out, It first, It last) {
        if (first != last) {
            for (out << '{';; out << ',') {
                write_pair(out, *first);
                ++first;
                if (first != last) continue;
                return out << '}';
            }
        }
        return out << "{}";
    }

#if __cplusplus >= 201103L

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &
    write_adapter(std::basic_ostream<CharT, Traits> &out, Adapter &&adapter) {
        return write_collection(out, adapter_helper<
                typename std::remove_reference<Adapter>::type
        >::get_container(std::forward<Adapter>(adapter)));
    }

    template<class CharT, class Traits, class Tp>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &out, Tp &&pair, char sep) {
        return out << std::forward<Tp>(pair).first << sep << std::forward<Tp>(pair).second;
    }

    template<class CharT, class Traits, class Coll>
    inline std::basic_ostream<CharT, Traits> &
    write_collection(std::basic_ostream<CharT, Traits> &out, Coll &&c) {
        // can't forward c here, for it might be an array right reference
        return write_collection_impl(out, std::begin(c), std::end(c));
    }

    template<class CharT, class Traits, class Map>
    inline std::basic_ostream<CharT, Traits> &
    write_map(std::basic_ostream<CharT, Traits> &out, Map &&map) {
        return write_map_impl(out, std::begin(std::forward<Map>(map)), std::end(std::forward<Map>(map)));
    }

    template<size_t Ip, class Tp, size_t Np, class CharT, class Traits>
    inline typename std::enable_if<Ip == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &out, Tp &&) { return out; }

    template<size_t Ip, class Tp, size_t Np, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &out, Tp &&tp) {
        return out << std::get<Ip>(std::forward<Tp>(tp));
    }

    template<size_t Ip, class Tp, size_t Np, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 < Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &out, Tp &&tp) {
        return write_tuple<Ip + 1>(out << std::get<Ip>(std::forward<Tp>(tp)) << ',', std::forward<Tp>(tp));
    }

#if __cpp_lib_variant

    template<class CharT, class Traits, class Variant>
    inline std::basic_ostream<CharT, Traits> &write_variant(std::basic_ostream<CharT, Traits> &out, Variant &&variant) {
        return std::visit([&](auto &&var) -> decltype(auto) {
            return out << std::forward<decltype(var)>(var);
        }, std::forward<Variant>(variant));
    }

#endif

    template<class CharT, class Traits, class ...Args>
    inline std::basic_ostream<CharT, Traits> &
    trace(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out, Args &&...args) {
        return write_tuple(
                out << file << ":" << line << ": " << name << ": ",
                std::forward_as_tuple(std::forward<Args>(args)...)) << std::endl;
    }

    template<class ...Args>
    inline std::ostream &trace(const char *file, int line, const char *name, Args &&...args) {
        return trace(file, line, name, std::clog, std::forward<Args>(args)...);
    }

#else

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &
    write_adapter(std::basic_ostream<CharT, Traits> &out, const Adapter &adapter) {
        return write_collection(out, adapter_helper<Adapter>::get_container(adapter));
    }

    template<class CharT, class Traits, class T1, class T2>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &out, const std::pair<T1, T2> &pair, char sep) {
        return out << pair.first << sep << pair.second;
    }

    template<class CharT, class Traits, class Coll>
    inline std::basic_ostream<CharT, Traits> &
    write_collection(std::basic_ostream<CharT, Traits> &out, const Coll &c) {
        return write_collection_impl(out, c.begin(), c.end());
    }

    template<class CharT, class Traits, class Map>
    inline std::basic_ostream<CharT, Traits> &
    write_map(std::basic_ostream<CharT, Traits> &out, const Map &map) {
        return write_map_impl(out, map.begin(), map.end());
    }

    template<class CharT, class Traits, class Tp>
    inline std::basic_ostream<CharT, Traits> &
    trace(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out, const Tp &val) {
        return out << file << ":" << line << ": " << name << ": " << val << std::endl;
    }

    template<class CharT, class Traits, class Tp, class Up>
    inline std::basic_ostream<CharT, Traits> &
    trace(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out,
          const Tp &val1, const Up &val2) {
        return out << file << ":" << line << ": " << name << ": " << val1 << ", " << val2 << std::endl;
    }

    template<class CharT, class Traits, class Tp, class Up, class Vp>
    inline std::basic_ostream<CharT, Traits> &
    trace(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out,
          const Tp &val1, const Up &val2, const Vp &val3) {
        return out << file << ":" << line << ": " << name << ": " << val1 << ", " << val2 << ", " << val3 << std::endl;
    }

    template<class Tp>
    inline std::ostream &trace(const char *file, int line, const char *name, const Tp &val) {
        return trace(file, line, name, std::clog, val);
    }

    template<class Tp, class Up>
    inline std::ostream &trace(const char *file, int line, const char *name, const Tp &val1, const Up &val2) {
        return trace(file, line, name, std::clog, val1, val2);
    }

    template<class Tp, class Up, class Vp>
    inline std::ostream &
    trace(const char *file, int line, const char *name, const Tp &val1, const Up &val2, const Vp &val3) {
        return trace(file, line, name, std::clog, val1, val2, val3);
    }

#endif
}

#define out(...) debug::trace(__FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)
#endif
