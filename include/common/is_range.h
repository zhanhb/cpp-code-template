#pragma once

#include "extension/type_traits.h"

namespace utility {

#if __cplusplus >= 201103L

    template<class Bp, class Ep, class = void>
    struct is_range_iterator_impl : std::false_type {
    };
    template<class Bp, class Ep>
    struct is_range_iterator_impl<Bp, Ep, extension::void_t<
            typename std::enable_if<
                    std::is_convertible<Bp, typename std::decay<Bp>::type>::value &&
                    std::is_convertible<Ep, typename std::decay<Ep>::type>::value &&
                    !std::is_void<decltype(*std::declval<typename std::decay<Bp>::type &>())>::value
            >::type,
            decltype(++std::declval<typename std::decay<Bp>::type &>()),
            decltype(bool(std::declval<typename std::decay<Bp>::type &>() != std::declval<typename std::decay<Ep>::type &>()))
    > > : std::true_type {
    };

#if __cpp_range_based_for >= 201603L
    template<class Bp, class Ep>
    struct is_range_iterator : is_range_iterator_impl<Bp, Ep> {
    };
#else
    template<class, class>
    struct is_range_iterator : std::false_type {
    };
    template<class Tp>
    struct is_range_iterator<Tp, Tp> : is_range_iterator_impl<Tp, Tp> {
    };
#endif

    template<class, class = void>
    struct is_range : std::false_type {
    };
    template<class Tp>
    struct is_range<Tp, typename std::enable_if<is_range_iterator<
            decltype(std::begin(std::declval<Tp &>())),
            decltype(std::end(std::declval<Tp &>()))
    >::value>::type> : std::true_type {
    };

#else /* 201103L */

    template<class Tp, class Up>
    class is_range_iterator {
        template<class Bp, class Ep>
        static extension::detail::yes_type *test(
                SIZEABLE(*extension::declval<Bp &>()) * = 0,
                typename SIZEABLE(!(void(++extension::declval<Bp &>()), 0))::value_type = 0,
                typename SIZEABLE(bool (extension::declval<Bp &>() != extension::declval<Ep &>()))::value_type = 0
        ) { return 0; } // NOLINT(modernize-use-nullptr)

        template<class, class>
        static extension::detail::no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)

    public:
        enum {
            value = extension::detail::test_result<sizeof(*is_range_iterator::test<Tp, Up>(0))>::value
        };
    };

    template<class Tp>
    class is_range {
        template<class It>
        static typename extension::enable_if<is_range_iterator<It, It>::value, int>::type
        accept(It, It) { return 0; }

        template<class Up>
        static extension::detail::yes_type *test(SIZEABLE(is_range::accept(
                extension::declval<Up &>().begin(),
                extension::declval<Up &>().end()
        )) * = 0) { return 0; } // NOLINT(modernize-use-nullptr)

        template<class>
        static extension::detail::no_type *test(...) { return 0; } // NOLINT(modernize-use-nullptr)

    public:
        enum {
            value = extension::detail::test_result<sizeof(*is_range::test<Tp>(0))>::value
        };
    };

#endif /* 201103L */
}
