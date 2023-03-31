#pragma once

#include <iterator>
#include "extension/concepts.h"

namespace extension {

#if __cplusplus > 201703L
    using std::contiguous_iterator_tag;
#else
    struct contiguous_iterator_tag : public std::random_access_iterator_tag {
    };
#endif

#if __cplusplus >= 202002L

    using std::iter_value_t;
    using std::input_iterator;
    using std::forward_iterator;
    using std::iter_reference_t;

#elif __cplusplus >= 201103L

    namespace detail {
        DEFINE_CONCEPT2_COND(
                category_convertible_to, It, To,
                CONCEPT_GET(extension::convertible_to, typename std::iterator_traits<It>::iterator_category, To)
        )
    }
    DEFINE_CONCEPT1_UNSAFE(
            input_iterator, It, CONCEPT_GET(detail::category_convertible_to, It, std::input_iterator_tag))
    DEFINE_CONCEPT1_UNSAFE(
            forward_iterator, It, CONCEPT_GET(detail::category_convertible_to, It, std::forward_iterator_tag))

#endif
}
