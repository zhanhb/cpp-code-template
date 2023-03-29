#pragma once

#include <iostream>
#include <locale>
#include <valarray>
#include "common/is_range.h"
#include "common/string_traits.h"

#if __cplusplus >= 201103L

#include <tuple>

#endif /* 201103L */

#if __cplusplus >= 201703L && defined(__has_include)
#if __has_include(<optional>)

#include <optional>

#endif
#if __has_include(<variant>)

#include <variant>

#endif
#endif

namespace debug {

    namespace detail {

#if __cplusplus >= 201103L

        DEFINE_CONCEPT1_COND(
                is_map, Tp,
                CONCEPT_GET(utility::is_range, Tp) &&
                CONCEPT_GET(
                        extension::same_as,
                        typename extension::remove_cvref_t<Tp>::value_type,
                        std::pair<const typename extension::remove_cvref_t<Tp>::key_type, typename extension::remove_cvref_t<Tp>::mapped_type>
                )
        )
        DEFINE_CONCEPT1_EXP(is_tuple, Tp, std::tuple_size<extension::remove_cvref_t<Tp> >::value)

        template<class Tp> using member_object_pointer_t = typename std::enable_if<std::is_member_object_pointer<Tp>::value, Tp>::type;

        template<class Tp, Tp value>
        struct constant {
            typedef Tp value_type;
            typedef constant type;

            constexpr value_type operator()() const noexcept { return value; }
        };

        template<class Tp,
                bool = std::is_class<Tp>::value || std::is_union<Tp>::value,
                bool = extension::is_final<Tp>::value>
        class member_c_test {

            template<class Up>
            static constant<member_object_pointer_t<decltype(&Up::c)>, &Up::c> test(int) { return {}; }

            template<class>
            static void test(...) {}

        public:
            using result = decltype(member_c_test::test<Tp>(0));
        };

        template<class Tp>
        class member_c_test<Tp, true, false> : Tp {

            template<class Up>
            static constant<member_object_pointer_t<decltype(&Up::c)>, &Up::c> test(int) { return {}; }

            template<class>
            static void test(...) {}

        public:
            using result = decltype(member_c_test::test<member_c_test>(0));
        };

        template<class Tp> using member_c_info = typename member_c_test<typename extension::remove_cvref<Tp>::type>::result::type;

        // @formatter:off
        template<class> struct mem_ptr_traits;
        template<class Tp, class Up> struct mem_ptr_traits<Tp Up::*> { using return_type = Tp; };
        // @formatter:on

        DEFINE_CONCEPT1_COND(
                is_adapter, Tp,
                CONCEPT_GET(
                        extension::same_as,
                        typename extension::remove_cvref_t<Tp>::container_type,
                        typename mem_ptr_traits<typename member_c_info<Tp>::value_type>::return_type
                ) && CONCEPT_GET(
                        utility::is_range,
                        extension::copy_cv_t<
                                typename std::remove_reference<Tp>::type,
                                typename extension::remove_cvref_t<Tp>::container_type
                        >
                )
        )

        template<class Tp>
        struct is_char : extension::disjunction<
                std::is_same<Tp, char>,
                std::is_same<Tp, signed char>,
                std::is_same<Tp, unsigned char>,
                std::is_same<Tp, wchar_t>,
#if __cpp_char8_t
                std::is_same<Tp, char8_t>,
#endif
                std::is_same<Tp, char16_t>,
                std::is_same<Tp, char32_t>
        >::type {
        };

        DEFINE_CONCEPT1_COND(is_string_or_view, Tp, is_char<typename utility::string_traits<Tp>::value_type>::value)
        DEFINE_CONCEPT1_COND(
                is_char_pointer, Tp,
                std::is_pointer<Tp>::value &&
                is_char<typename std::decay<typename std::remove_pointer<Tp>::type>::type>::value
        )
        DEFINE_CONCEPT1_UNSAFE(
                is_string, Tp,
                CONCEPT_GET(is_string_or_view, typename std::decay<Tp>::type) ||
                CONCEPT_GET(is_char_pointer, typename std::decay<Tp>::type))

        struct adapter_helper {
            template<class Tp>
            static inline constexpr auto container(Tp &&adapter)
            noexcept(noexcept(std::forward<Tp>(adapter).*(member_c_info<Tp>{}()))) ->
            decltype(std::forward<Tp>(adapter).*(member_c_info<Tp>{}())) {
                return std::forward<Tp>(adapter).*(member_c_info<Tp>{}());
            }
        };

#else /* 201103L */

        template<class Tp, bool = utility::is_range<Tp>::value>
        class is_map {
            template<class Up>
            static extension::detail::yes_type *test(
                    typename Up::value_type * = 0,
                    typename Up::key_type * = 0,
                    typename Up::mapped_type * = 0,
                    typename extension::enable_if<extension::is_same<
                            typename Up::value_type,
                            std::pair<typename Up::key_type const, typename Up::mapped_type>
                    >::value, int>::type = 0
            ) { return 0; } // NOLINT(modernize-use-nullptr)

            template<class>
            static extension::detail::no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)

        public:
            enum {
                value = extension::detail::test_result<
                        sizeof(*is_map::test<typename extension::remove_cvref<Tp>::type>(0))
                >::value
            };
        };

        template<class Tp>
        class is_map<Tp, false> : public extension::false_type {
        };

        template<class Tp,
                bool = extension::is_class<Tp>::value || extension::is_union<Tp>::value,
                bool = extension::is_final<Tp>::value>
        class adapter_test {

            template<class Up, class Base>
            static int accept(Up Base::*) { return 0; }

            template<class Up>
            static extension::detail::yes_type *test(
                    typename extension::enable_if<utility::is_range<typename Up::container_type>::value, int>::type = 0,
                    SIZEABLE(adapter_test::accept<typename Up::container_type>(&Up::c)) * = 0
            ) { return 0; } // NOLINT(modernize-use-nullptr)

            template<class>
            static extension::detail::no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)

        public:
            typedef typename extension::detail::test_result<sizeof(*adapter_test::test<Tp>(0))>::type type;
        };

        template<class Tp>
        class adapter_test<Tp, true, false> : Tp {

            template<class Up, class Base>
            static int accept(Up Base::*) { return 0; }

            template<class Up>
            static extension::detail::yes_type *test(
                    typename extension::enable_if<utility::is_range<typename Up::container_type>::value, int>::type = 0,
                    SIZEABLE(adapter_test::accept<typename Up::container_type>(&Up::c)) * = 0
            ) { return 0; } // NOLINT(modernize-use-nullptr)

            template<class>
            static extension::detail::no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)

        public:
            typedef typename extension::detail::test_result<sizeof(*adapter_test::test<adapter_test>(0))>::type type;
        };

        template<class Tp>
        struct is_adapter : adapter_test<Tp>::type {
        };

        template<class Tp>
        struct is_char : extension::integral_constant<bool,
                extension::is_same<Tp, char>::value ||
                extension::is_same<Tp, signed char>::value ||
                extension::is_same<Tp, unsigned char>::value ||
                extension::is_same<Tp, wchar_t>::value
        > {
        };

        template<class Tp, class = void>
        struct is_string_or_view : extension::false_type {
        };
        template<class Tp>
        struct is_string_or_view<Tp, typename extension::enable_if<
                is_char<typename utility::string_traits<Tp>::value_type>::value
        >::type> : extension::true_type {
        };

        template<class Tp>
        struct is_char_pointer : extension::integral_constant<bool,
                extension::is_pointer<Tp>::value &&
                is_char<typename extension::decay<typename extension::remove_pointer<Tp>::type>::type>::value
        > {
        };

        template<class Tp>
        struct is_string : extension::integral_constant<bool,
                is_string_or_view<typename extension::remove_cvref<Tp>::type>::value ||
                is_char_pointer<typename extension::decay<Tp>::type>::value
        > {
        };

        template<class Tp, bool = is_adapter<Tp>::value, bool = extension::is_final<Tp>::value>
        struct container_getter {
        };

        template<class Tp>
        struct container_getter<Tp, true, true> {
            static typename Tp::container_type &get(Tp &adapter) { return adapter.*(&Tp::c); }
        };

        template<class Tp>
        struct container_getter<Tp, true, false> : Tp {
            static typename Tp::container_type &get(Tp &adapter) { return adapter.*(&container_getter::c); }
        };

        struct adapter_helper {
            template<class Tp>
            static typename extension::enable_if<is_adapter<Tp>::value, Tp>::type::container_type &
            container(Tp &adapter) { return container_getter<Tp>::get(adapter); }

            template<class Tp>
            static const typename extension::enable_if<is_adapter<const Tp>::value, Tp>::type::container_type &
            container(const Tp &adapter) { return adapter_helper::container(const_cast<Tp &>(adapter)); }

            template<class Tp>
            static volatile typename extension::enable_if<is_adapter<volatile Tp>::value, Tp>::type::container_type &
            container(volatile Tp &adapter) { return adapter_helper::container(const_cast<Tp &>(adapter)); }

            template<class Tp>
            static const volatile typename extension::enable_if<is_adapter<const volatile Tp>::value, Tp>::type::container_type &
            container(const volatile Tp &adapter) { return adapter_helper::container(const_cast<Tp &>(adapter)); }
        };

#endif /* 201103L */

    }

    DEFINE_CONCEPT1_UNSAFE(adapter, Tp, CONCEPT_GET(detail::is_adapter, Tp))
    DEFINE_CONCEPT1_UNSAFE(map, Tp, CONCEPT_GET(detail::is_map, Tp))
    DEFINE_CONCEPT1_UNSAFE(collection, Tp, CONCEPT_GET(utility::is_range, Tp) && !CONCEPT_GET(map, Tp))
    DEFINE_CONCEPT1_UNSAFE(string, Tp, CONCEPT_GET(detail::is_string, Tp))
    DEFINE_CONCEPT1_UNSAFE(collection_minus_string, Tp, CONCEPT_GET(collection, Tp) && !CONCEPT_GET(string, Tp))

#if __cplusplus >= 201103L

    DEFINE_CONCEPT1_UNSAFE(tuple, Tp, CONCEPT_GET(detail::is_tuple, Tp))
    DEFINE_CONCEPT1_UNSAFE(tuple_minus_range, Tp, CONCEPT_GET(tuple, Tp) && !CONCEPT_GET(utility::is_range, Tp))

#if __cpp_lib_variant
    DEFINE_CONCEPT1_EXP(variant, Tp, std::variant_size<typename std::decay<Tp>::type>::value)
#endif

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &
    write_adapter(std::basic_ostream<CharT, Traits> &, Adapter &&);

    template<class CharT, class Traits, class Coll>
    inline std::basic_ostream<CharT, Traits> &
    write_collection(std::basic_ostream<CharT, Traits> &, Coll &&);

    template<class CharT, class Traits, class Map>
    inline std::basic_ostream<CharT, Traits> &
    write_map(std::basic_ostream<CharT, Traits> &, Map &&);

    template<class CharT, class Traits, class Tp>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &, Tp &&, const char * = ":");

    template<std::size_t Ip, class Tp, std::size_t Np = std::tuple_size<extension::remove_cvref_t<Tp> >::value, class CharT, class Traits>
    inline typename std::enable_if<Ip == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &, Tp &&);

    template<std::size_t Ip, class Tp, std::size_t Np = std::tuple_size<extension::remove_cvref_t<Tp> >::value, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &, Tp &&);

    template<std::size_t Ip, class Tp, std::size_t Np = std::tuple_size<extension::remove_cvref_t<Tp> >::value, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 < Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &, Tp &&);

#if __cpp_lib_variant

    template<class CharT, class Traits, class Variant>
    inline std::basic_ostream<CharT, Traits> &
    write_variant(std::basic_ostream<CharT, Traits> &, Variant &&);

#endif

#else /* 201103L */

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &
    write_adapter(std::basic_ostream<CharT, Traits> &, Adapter &);

    template<class CharT, class Traits, class Coll>
    inline std::basic_ostream<CharT, Traits> &
    write_collection(std::basic_ostream<CharT, Traits> &, Coll &);

    template<class CharT, class Traits, class Map>
    inline std::basic_ostream<CharT, Traits> &
    write_map(std::basic_ostream<CharT, Traits> &, Map &);

    template<class CharT, class Traits, class Pair>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &, Pair &, const char * = ":");

#endif /* 201103L */

}

#include "write-wide.h"

#if __cplusplus >= 201103L

template<class CharT, class Traits, TYPE_CONCEPT(Adapter, debug::adapter)>
inline CONCEPT_IF_1(debug::adapter, Adapter, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, Adapter &&adapter) {
    return debug::write_adapter(out, std::forward<Adapter>(adapter));
}

template<class CharT, class Traits, TYPE_CONCEPT(Coll, debug::collection_minus_string)>
inline CONCEPT_IF_1(debug::collection_minus_string, Coll, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, Coll &&c) {
    return debug::write_collection(out, std::forward<Coll>(c));
}

template<class CharT, class Traits, TYPE_CONCEPT(Map, debug::map)>
inline CONCEPT_IF_1(debug::map, Map, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, Map &&map) {
    return debug::write_map(out, std::forward<Map>(map));
}

template<class CharT, class Traits, TYPE_CONCEPT(Tp, debug::tuple_minus_range)>
inline CONCEPT_IF_1(debug::tuple_minus_range, Tp, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, Tp &&tuple) {
    return debug::write_tuple<0>(out << "[", std::forward<Tp>(tuple)) << "]";
}

#if __cpp_lib_variant

template<class CharT, class Traits, TYPE_CONCEPT(Variant, debug::variant)>
inline CONCEPT_IF_1(debug::variant, Variant, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, Variant &&var) {
    return debug::write_variant(out, std::forward<Variant>(var));
}

#endif

#else /* 201103L */

template<class CharT, class Traits, class Adapter>
inline CONCEPT_IF_1(debug::adapter, const Adapter, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, const Adapter &q) {
    return debug::write_adapter(out, q);
}

template<class CharT, class Traits, class Coll>
inline CONCEPT_IF_1(debug::collection_minus_string, const Coll, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, const Coll &c) {
    return debug::write_collection(out, c);
}

template<class CharT, class Traits, class Map>
inline CONCEPT_IF_1(debug::map, const Map, std::basic_ostream<CharT, Traits> &)
operator<<(std::basic_ostream<CharT, Traits> &out, const Map &mp) {
    return debug::write_map(out, mp);
}

template<class CharT, class Traits, class T1, class T2>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::pair<T1, T2> &p) {
    return debug::write_pair(out << "[", p, ",") << "]";
}

#endif /* 201103L */

#if __cpp_lib_optional

template<class CharT, class Traits, class Tp>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, const std::optional<Tp> &o) {
    return o.has_value() ? out << *o : out << "null";
}

#endif

#if __cpp_impl_three_way_comparison

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &out, std::partial_ordering po) {
    if (po == 0) return out << "\"==\""; // NOLINT(modernize-use-nullptr)
    else if (po < 0) return out << "\"<\""; // NOLINT(modernize-use-nullptr)
    else if (po > 0) return out << "\">\""; // NOLINT(modernize-use-nullptr)
    else return out << "\"!=\"";
}

#endif

namespace debug {

    template<class CharT, class Traits, class Bp, class Ep>
    inline std::basic_ostream<CharT, Traits> &
    write_collection_impl(std::basic_ostream<CharT, Traits> &out, Bp first, Ep last) {
        if (first != last) {
            for (out << "[";; out << ",") {
                out << *first;
                ++first;
                if (first != last) continue;
                return out << "]";
            }
        }
        return out << "[]";
    }

    template<class CharT, class Traits, class Bp, class Ep>
    inline std::basic_ostream<CharT, Traits> &
    write_map_impl(std::basic_ostream<CharT, Traits> &out, Bp first, Ep last) {
        if (first != last) {
            for (out << "{";; out << ",") {
                write_pair(out, *first);
                ++first;
                if (first != last) continue;
                return out << "}";
            }
        }
        return out << "{}";
    }

#if __cplusplus >= 201103L

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &
    write_adapter(std::basic_ostream<CharT, Traits> &out, Adapter &&adapter) {
        return write_collection(out, detail::adapter_helper::container(std::forward<Adapter>(adapter)));
    }

    template<class CharT, class Traits, class Tp>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &out, Tp &&pair, const char *sep) {
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

    template<std::size_t Ip, class Tp, std::size_t Np, class CharT, class Traits>
    inline typename std::enable_if<Ip == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &out, Tp &&) { return out; }

    template<std::size_t Ip, class Tp, std::size_t Np, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 == Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &out, Tp &&tp) {
        return out << std::get<Ip>(std::forward<Tp>(tp));
    }

    template<std::size_t Ip, class Tp, std::size_t Np, class CharT, class Traits>
    inline typename std::enable_if<Ip + 1 < Np, std::basic_ostream<CharT, Traits> &>::type
    write_tuple(std::basic_ostream<CharT, Traits> &out, Tp &&tp) {
        return write_tuple<Ip + 1>(out << std::get<Ip>(std::forward<Tp>(tp)) << ",", std::forward<Tp>(tp));
    }

#if __cpp_lib_variant

    template<class CharT, class Traits, class Variant>
    inline std::basic_ostream<CharT, Traits> &
    write_variant(std::basic_ostream<CharT, Traits> &out, Variant &&variant) {
        return std::visit([&](auto &&var) -> decltype(auto) {
            return out << std::forward<decltype(var)>(var);
        }, std::forward<Variant>(variant));
    }

#endif

    template<class CharT, class Traits, class ...Args>
    inline std::basic_ostream<CharT, Traits> &
    dump(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out, Args &&...args) {
        return write_tuple<0>(
                out << file << ":" << line << ": " << name << ": ",
                std::forward_as_tuple(std::forward<Args>(args)...)) << std::endl;
    }

    template<class ...Args>
    inline std::ostream &dump(const char *file, int line, const char *name, Args &&...args) {
        return dump(file, line, name, std::cout, std::forward<Args>(args)...);
    }

#else /* 201103L */

    template<class CharT, class Traits, class Adapter>
    inline std::basic_ostream<CharT, Traits> &
    write_adapter(std::basic_ostream<CharT, Traits> &out, Adapter &adapter) {
        return debug::write_collection(out, debug::detail::adapter_helper::container(adapter));
    }

    template<class CharT, class Traits, class Coll>
    inline std::basic_ostream<CharT, Traits> &
    write_collection(std::basic_ostream<CharT, Traits> &out, Coll &c) {
        return write_collection_impl(out, c.begin(), c.end());
    }

    template<class CharT, class Traits, class Map>
    inline std::basic_ostream<CharT, Traits> &
    write_map(std::basic_ostream<CharT, Traits> &out, Map &map) {
        return write_map_impl(out, map.begin(), map.end());
    }

    template<class CharT, class Traits, class Pair>
    inline std::basic_ostream<CharT, Traits> &
    write_pair(std::basic_ostream<CharT, Traits> &out, Pair &pair, const char *sep) {
        return out << pair.first << sep << pair.second;
    }

    template<class CharT, class Traits, class Tp>
    inline std::basic_ostream<CharT, Traits> &
    dump(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out, const Tp &val) {
        return out << file << ":" << line << ": " << name << ": " << val << std::endl;
    }

    template<class CharT, class Traits, class Tp, class Up>
    inline std::basic_ostream<CharT, Traits> &
    dump(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out,
          const Tp &val1, const Up &val2) {
        return out << file << ":" << line << ": " << name << ": " << val1 << ", " << val2 << std::endl;
    }

    template<class CharT, class Traits, class Tp, class Up, class Vp>
    inline std::basic_ostream<CharT, Traits> &
    dump(const char *file, int line, const char *name, std::basic_ostream<CharT, Traits> &out,
          const Tp &val1, const Up &val2, const Vp &val3) {
        return out << file << ":" << line << ": " << name << ": " << val1 << ", " << val2 << ", " << val3 << std::endl;
    }

    template<class Tp>
    inline std::ostream &dump(const char *file, int line, const char *name, const Tp &val) {
        return dump(file, line, name, std::cout, val);
    }

    template<class Tp, class Up>
    inline std::ostream &dump(const char *file, int line, const char *name, const Tp &val1, const Up &val2) {
        return dump(file, line, name, std::cout, val1, val2);
    }

    template<class Tp, class Up, class Vp>
    inline std::ostream &
    dump(const char *file, int line, const char *name, const Tp &val1, const Up &val2, const Vp &val3) {
        return dump(file, line, name, std::cout, val1, val2, val3);
    }

#endif /* 201103L */

}

#define OUT(...) debug::dump(__FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)
