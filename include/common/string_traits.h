#pragma once

#include "extension/concepts.h"

namespace utility {

#if __cplusplus >= 201103L

    template<class, class = void>
    struct string_traits {
    };
    template<class Tp>
    struct string_traits<Tp, extension::void_t<
            typename Tp::traits_type,
            typename Tp::value_type,
            typename Tp::const_pointer,
            typename std::enable_if<
                    std::is_same<typename Tp::value_type, typename Tp::traits_type::char_type>::value &&
                    std::is_same<typename Tp::const_pointer, const typename Tp::value_type *>::value &&
                    std::is_same<typename Tp::size_type, std::size_t>::value &&
                    std::is_constructible<typename Tp::const_pointer, decltype(std::declval<Tp &>().data())>::value &&
                    std::is_constructible<typename Tp::size_type, decltype(std::declval<Tp &>().length())>::value &&
                    std::is_same<
                            std::integral_constant<std::size_t, Tp::npos>,
                            std::integral_constant<std::size_t, std::size_t(-1)>
                    >::value
            >::type
    > > {
        typedef typename Tp::value_type value_type;
    };

    DEFINE_CONCEPT2_COND(
            string_of, Tp, CharT,
            CONCEPT_GET(
                    extension::same_as,
                    CharT,
                    typename string_traits<typename std::remove_reference<Tp>::type>::value_type
            )
    )

#else /* 201103L */

    namespace detail {
        template<class SV>
        class string_traits_test {
            template<class Tp, class Up>
            static int accept(Tp (Up::*)() const) { return 0; }

            template<class Tp>
            static extension::detail::yes_type *test(
                    typename Tp::traits_type * = 0,
                    typename Tp::value_type * = 0,
                    typename Tp::const_pointer * = 0,
                    typename extension::enable_if<
                            extension::is_same<typename Tp::value_type, typename Tp::traits_type::char_type>::value &&
                            extension::is_same<typename Tp::const_pointer, const typename Tp::value_type *>::value &&
                            extension::is_same<typename Tp::size_type, std::size_t>::value
                    >::type * = 0,
                    SIZEABLE(string_traits_test::accept<typename Tp::const_pointer>(&Tp::data)) * = 0,
                    SIZEABLE(string_traits_test::accept<typename Tp::size_type>(&Tp::length)) * = 0,
                    typename extension::enable_if<extension::is_same<
                            extension::integral_constant<std::size_t, Tp::npos>,
                            extension::integral_constant<std::size_t, std::size_t(-1)>
                    >::value>::type * = 0
            ) { return 0; } // NOLINT(modernize-use-nullptr)

            template<class>
            static extension::detail::no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)

        public:
            enum {
                value = extension::detail::test_result<sizeof(*string_traits_test::test<SV>(0))>::value
            };
        };
    }
    template<class Tp, bool = detail::string_traits_test<Tp>::value>
    struct string_traits {
    };
    template<class Tp>
    struct string_traits<Tp, true> {
        typedef typename Tp::value_type value_type;
    };
    template<class Tp, class CharT>
    struct string_of : extension::false_type {
    };
    template<class Tp>
    struct string_of<Tp, typename string_traits<
            typename extension::remove_reference<Tp>::type
    >::value_type> : extension::true_type {
    };

#endif /* 201103L */

}
