#pragma once

#include "extension/concepts.h"

namespace utility {

#if __cplusplus >= 201103L

    DEFINE_CONCEPT2_COND_EXP(
            is_range_iterator_impl, Bp, Ep,
            PARAM_PACK(
                    std::is_convertible<Bp, typename std::decay<Bp>::type>::value &&
                    std::is_convertible<Ep, typename std::decay<Ep>::type>::value &&
                    !std::is_void<decltype(*std::declval<typename std::decay<Bp>::type &>())>::value
            ),
            ++std::declval<typename std::decay<Bp>::type &>(),
            bool (std::declval<typename std::decay<Bp>::type &>() != std::declval<typename std::decay<Ep>::type &>())
    )

#if __cpp_range_based_for >= 201603L
    DEFINE_CONCEPT2_UNSAFE(is_range_iterator, Bp, Ep, CONCEPT_GET(is_range_iterator_impl, Bp, Ep))
#else
    DEFINE_CONCEPT2_UNSAFE(
            is_range_iterator, Bp, Ep,
            CONCEPT_GET(extension::same_as, Bp, Ep) && CONCEPT_GET(is_range_iterator_impl, Bp, Ep))
#endif

    DEFINE_CONCEPT1_COND(
            is_range, Tp,
            CONCEPT_GET(
                    is_range_iterator,
                    decltype(std::begin(std::declval<Tp &>())),
                    decltype(std::end(std::declval<Tp &>()))
            )
    )

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
