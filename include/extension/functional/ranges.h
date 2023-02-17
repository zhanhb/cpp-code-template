#pragma once

#if __cplusplus >= 201103L

#include <functional>
#include "../concepts.h"
#include "../config.h"

namespace extension {

#if __cplusplus >= 202002L
    using std::identity;
#else

    struct identity {
        template<class Tp>
        NODISCARD constexpr Tp &&operator()(Tp &&value) const noexcept { return std::forward<Tp>(value); }

        using is_transparent MAYBE_UNUSED = void;
    };

#endif

    namespace ranges {
#if __cplusplus >= 202002L
        using std::ranges::equal_to;
        using std::ranges::not_equal_to;
        using std::ranges::less;
        using std::ranges::less_equal;
        using std::ranges::greater;
        using std::ranges::greater_equal;
#else

        struct equal_to {
            template<class Tp, class Up>
            REQUIRE_CONCEPT(equality_comparable_with, Tp, Up)
            NODISCARD constexpr CONCEPT_IF_2(equality_comparable_with, Tp, Up, bool)
            operator()(Tp &&t, Up &&u) const
            noexcept(noexcept(bool(std::forward<Tp>(t) == std::forward<Up>(u)))) {
                return std::forward<Tp>(t) == std::forward<Up>(u);
            }

            using is_transparent MAYBE_UNUSED = void;
        };

        struct not_equal_to {
            template<class Tp, class Up>
            REQUIRE_CONCEPT(equality_comparable_with, Tp, Up)
            NODISCARD constexpr CONCEPT_IF_2(equality_comparable_with, Tp, Up, bool)
            operator()(Tp &&t, Up &&u) const
            noexcept(noexcept(!equal_to{}(std::forward<Tp>(t), std::forward<Up>(u)))) {
                return !equal_to{}(std::forward<Tp>(t), std::forward<Up>(u));
            }

            using is_transparent MAYBE_UNUSED = void;
        };

        struct less {
            template<class Tp, class Up>
            REQUIRE_CONCEPT(totally_ordered_with, Tp, Up)
            NODISCARD constexpr CONCEPT_IF_2(totally_ordered_with, Tp, Up, bool)
            operator()(Tp &&t, Up &&u) const
            noexcept(noexcept(bool(std::forward<Tp>(t) < std::forward<Up>(u)))) {
                return std::forward<Tp>(t) < std::forward<Up>(u);
            }

            using is_transparent MAYBE_UNUSED = void;
        };

        struct less_equal {
            template<class Tp, class Up>
            REQUIRE_CONCEPT(totally_ordered_with, Tp, Up)
            NODISCARD constexpr CONCEPT_IF_2(totally_ordered_with, Tp, Up, bool)
            operator()(Tp &&t, Up &&u) const
            noexcept(noexcept(!less{}(std::forward<Up>(u), std::forward<Tp>(t)))) {
                return !less{}(std::forward<Up>(u), std::forward<Tp>(t));
            }

            using is_transparent MAYBE_UNUSED = void;
        };

        struct greater {
            template<class Tp, class Up>
            REQUIRE_CONCEPT(totally_ordered_with, Tp, Up)
            NODISCARD constexpr CONCEPT_IF_2(totally_ordered_with, Tp, Up, bool)
            operator()(Tp &&t, Up &&u) const
            noexcept(noexcept(less{}(std::forward<Up>(u), std::forward<Tp>(t)))) {
                return less{}(std::forward<Up>(u), std::forward<Tp>(t));
            }

            using is_transparent MAYBE_UNUSED = void;
        };

        struct greater_equal {
            template<class Tp, class Up>
            REQUIRE_CONCEPT(totally_ordered_with, Tp, Up)
            NODISCARD constexpr CONCEPT_IF_2(totally_ordered_with, Tp, Up, bool)
            operator()(Tp &&t, Up &&u) const
            noexcept(noexcept(!less{}(std::forward<Tp>(t), std::forward<Up>(u)))) {
                return !less{}(std::forward<Tp>(t), std::forward<Up>(u));
            }

            using is_transparent MAYBE_UNUSED = void;
        };

#endif
    }
}

#endif
