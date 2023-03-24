#pragma once

#include <cstddef>
#include "config.h"
#include "define_concept.h"
#include "utility/exchange.h"

#if __cplusplus >= 201103L

// @formatter:off
#ifdef __has_include
#if __has_include(<version>)
#include <version>
#endif
#endif

#ifndef LIB_CONCEPTS
#define LIB_CONCEPTS __cpp_lib_concepts
#endif

#if LIB_CONCEPTS && __cpp_lib_concepts
#include <concepts>
#endif
// @formatter:on

namespace extension {
#if LIB_CONCEPTS && __cpp_lib_concepts

    using std::same_as;
    using std::derived_from;
    using std::convertible_to;
    using std::common_reference_with;
    using std::common_with;
    using std::integral;
    using std::signed_integral;
    using std::unsigned_integral;
    using std::floating_point;
    using std::assignable_from;
    using std::swappable;
    using std::swappable_with;
    using std::destructible;
    using std::constructible_from;
    using std::default_initializable;
    using std::move_constructible;
    using std::copy_constructible;
    using std::equality_comparable;
    using std::equality_comparable_with;
    using std::totally_ordered;
    using std::totally_ordered_with;
    using std::movable;
    using std::copyable;
    using std::semiregular;
    using std::regular;
    using std::invocable;
    using std::regular_invocable;
    using std::predicate;
    using std::relation;
    using std::equivalence_relation;
    using std::strict_weak_order;

    namespace ranges {
        using std::ranges::swap;
    }

#else /* __cpp_concepts */

    namespace detail {
        DEFINE_CONCEPT2_UNSAFE(same_as_impl, Tp, Up, std::is_same<Tp, Up>::value)
    }

    DEFINE_CONCEPT2_UNSAFE(
            same_as, Tp, Up,
            CONCEPT_GET(detail::same_as_impl, Tp, Up) && CONCEPT_GET(detail::same_as_impl, Up, Tp)
    )

    DEFINE_CONCEPT2_COND(
            derived_from, Derived, Base,
            std::is_base_of<Base, Derived>::value &&
            std::is_convertible<const volatile Derived *, const volatile Base *>::value
    )

    DEFINE_CONCEPT2_COND_EXP(
            convertible_to, From, To,
            PARAM_PACK(std::is_convertible<From, To>::value),
            static_cast<To>(std::declval<From>())
    )

    DEFINE_CONCEPT2_COND(
            common_reference_with, Tp, Up,
            CONCEPT_GET(same_as, common_reference_t<Tp, Up>, common_reference_t<Up, Tp>) &&
            CONCEPT_GET(convertible_to, Tp, common_reference_t<Tp, Up>) &&
            CONCEPT_GET(convertible_to, Up, common_reference_t<Tp, Up>)
    )

    DEFINE_CONCEPT2_COND_EXP(
            common_with, Tp, Up,
            CONCEPT_GET(same_as, typename std::common_type<Tp, Up>::type, typename std::common_type<Up, Tp>::type) &&
            CONCEPT_GET(
                    common_reference_with,
                    typename std::add_lvalue_reference<const Tp>::type,
                    typename std::add_lvalue_reference<const Up>::type
            ) && CONCEPT_GET(
                    common_reference_with,
                    typename std::add_lvalue_reference<typename std::common_type<Tp, Up>::type>::type,
                    common_reference_t<
                            typename std::add_lvalue_reference<const Tp>::type,
                            typename std::add_lvalue_reference<const Up>::type
                    >
            ),
            static_cast<typename std::common_type<Tp, Up>::type>(std::declval<Tp>()),
            static_cast<typename std::common_type<Tp, Up>::type>(std::declval<Up>())
    )

    DEFINE_CONCEPT1_UNSAFE(integral, Tp, std::is_integral<Tp>::value)
    DEFINE_CONCEPT1_UNSAFE(signed_integral, Tp, CONCEPT_GET(integral, Tp) && std::is_signed<Tp>::value)
    DEFINE_CONCEPT1_UNSAFE(unsigned_integral, Tp, CONCEPT_GET(integral, Tp) && !CONCEPT_GET(signed_integral, Tp))
    DEFINE_CONCEPT1_UNSAFE(floating_point, Tp, std::is_floating_point<Tp>::value)

    namespace detail {
        template<class Tp> using make_clref_t = const typename std::remove_reference<Tp>::type &;
    }

    DEFINE_CONCEPT2_COND(
            assignable_from, Lhs, Rhs,
            std::is_lvalue_reference<Lhs>::value &&
            CONCEPT_GET(
                    common_reference_with,
                    detail::make_clref_t<Lhs>, detail::make_clref_t<Rhs>
            ) && CONCEPT_GET(
                    same_as,
                    decltype(std::declval<Lhs>() = std::declval<Rhs>()),
                    Lhs
            )
    )

    DEFINE_CONCEPT1_COND(destructible, Tp, std::is_nothrow_destructible<Tp>::value)

    DEFINE_CONCEPT_PREPEND_COND(
            PARAM_PACK(class Tp, class ...Args),
            constructible_from,
            PARAM_PACK(Tp, Args...),
            CONCEPT_GET(destructible, Tp) && std::is_constructible<Tp, Args...>::value
    )

    DEFINE_CONCEPT1_COND_EXP(
            default_initializable, Tp,
            CONCEPT_GET(constructible_from, Tp),
            Tp{}, ::new Tp
    )

    DEFINE_CONCEPT1_UNSAFE(
            move_constructible, Tp,
            CONCEPT_GET(constructible_from, Tp, Tp) && CONCEPT_GET(convertible_to, Tp, Tp)
    )

    DEFINE_CONCEPT1_COND(
            copy_constructible, Tp,
            CONCEPT_GET(move_constructible, Tp) &&
            CONCEPT_GET(constructible_from, Tp, Tp & ) && CONCEPT_GET(convertible_to, Tp & , Tp) &&
            CONCEPT_GET(constructible_from, Tp, const Tp &) && CONCEPT_GET(convertible_to, const Tp &, Tp) &&
                    CONCEPT_GET(constructible_from, Tp, const Tp) && CONCEPT_GET(convertible_to, const Tp, Tp)
    )

    namespace ranges {

        namespace impl_swap {

            template<class Tp>
            void swap(Tp &, Tp &) = delete; // NOLINT(readability-redundant-declaration)

            DEFINE_CONCEPT1_UNSAFE(
                    class_or_enum, Tp,
                    std::is_class<Tp>::value || std::is_union<Tp>::value || std::is_enum<Tp>::value)

            DEFINE_CONCEPT2_COND_EXP(
                    unqualified_swappable, Tp, Up,
                    CONCEPT_GET(class_or_enum, extension::remove_cvref_t<Tp>) ||
                    CONCEPT_GET(class_or_enum, extension::remove_cvref_t<Up>),
                    swap(std::declval<Tp>(), std::declval<Up>())
            )

            struct fn;

            DEFINE_CONCEPT_COND_EXP(
                    PARAM_PACK(class Tp, class Up, std::size_t Size),
                    arrays_swappable,
                    PARAM_PACK(Tp, Up, Size),
                    !CONCEPT_GET(unqualified_swappable, Tp(&)[Size], Up(&)[Size]) &&
                    std::extent<Tp>::value == std::extent<Up>::value,
                    std::declval<const fn &>()(std::declval<Tp(&)[Size]>()[0], std::declval<Up(&)[Size]>()[0])
            )

            DEFINE_CONCEPT1_COND(
                    exchangeable, Tp,
                    !CONCEPT_GET(unqualified_swappable, Tp & , Tp & ) &&
                    CONCEPT_GET(move_constructible, Tp) &&
                    CONCEPT_GET(assignable_from, Tp & , Tp)
            )

            struct fn final {
                // 2.1   `S` is `(void)swap(E1, E2)`* if `E1` or `E2` has class or enumeration type and...
                // *The name `swap` is used here unqualified.
                template<class Tp, class Up>
                REQUIRE_CONCEPT(unqualified_swappable, Tp, Up)
                constexpr CONCEPT_IF_2(unqualified_swappable, Tp, Up, void)
                operator()(Tp &&t, Up &&u) const
                noexcept(noexcept(swap(std::forward<Tp>(t), std::forward<Up>(u)))) {
                    return void(swap(std::forward<Tp>(t), std::forward<Up>(u)));
                }

                // 2.2   Otherwise, if `E1` and `E2` are lvalues of array types with equal extent and...
                template<class Tp, class Up, std::size_t Size>
                REQUIRE_CONCEPT(arrays_swappable, Tp, Up, Size)
                inline CONSTEXPR_14 CONCEPT_IF_3(arrays_swappable, Tp, Up, Size, void)
                operator()(Tp(&tp)[Size], Up(&up)[Size]) const
                noexcept(noexcept(std::declval<const fn &>()(tp[0], up[0]))) {
                    for (std::size_t i = 0; i != Size; ++i) (*this)(tp[i], up[i]);
                }

                // 2.3   Otherwise, if `E1` and `E2` are lvalues of the same type `T` that models...
                template<TYPE_CONCEPT(Tp, exchangeable)>
                constexpr CONCEPT_IF_1(exchangeable, Tp, void)
                operator()(Tp &x, Tp &y) const
                noexcept(std::is_nothrow_move_constructible<Tp>::value && std::is_nothrow_move_assignable<Tp>::value) {
                    return void(y = extension::exchange(x, std::move(y)));
                }
            };
        }

        inline namespace impl_exports {
#if __cpp_inline_variables >= 201606L
            inline constexpr impl_swap::fn swap{};
#else
            constexpr impl_swap::fn swap{};
#endif
        }
    }

    DEFINE_CONCEPT1_EXP(swappable, Tp, ranges::swap(std::declval<Tp &>(), std::declval<Tp &>()))

    DEFINE_CONCEPT2_COND_EXP(
            swappable_with, Tp, Up,
            CONCEPT_GET(common_reference_with, Tp, Up),
            ranges::swap(std::declval<Tp &&>(), std::declval<Tp &&>()),
            ranges::swap(std::declval<Up &&>(), std::declval<Up &&>()),
            ranges::swap(std::declval<Tp &&>(), std::declval<Up &&>()),
            ranges::swap(std::declval<Up &&>(), std::declval<Tp &&>())
    )

    namespace detail {

        DEFINE_CONCEPT1_UNSAFE(
                boolean_testable, Tp,
                CONCEPT_GET(convertible_to, Tp, bool) &&
                CONCEPT_GET(convertible_to, decltype(!std::declval<Tp &&>()), bool)
        )
        DEFINE_CONCEPT2_EXP_IS(
                weakly_equality_comparable_eq, Tp, Up,
                std::declval<detail::make_clref_t<Tp> >() == std::declval<detail::make_clref_t<Up> >(),
                boolean_testable
        )
        DEFINE_CONCEPT2_EXP_IS(
                weakly_equality_comparable_ne, Tp, Up,
                std::declval<detail::make_clref_t<Tp> >() != std::declval<detail::make_clref_t<Up> >(),
                boolean_testable
        )
        DEFINE_CONCEPT2_UNSAFE(
                weakly_equality_comparable_with, Tp, Up,
                CONCEPT_GET(weakly_equality_comparable_eq, Tp, Up) &&
                CONCEPT_GET(weakly_equality_comparable_ne, Tp, Up) &&
                CONCEPT_GET(weakly_equality_comparable_eq, Up, Tp) &&
                CONCEPT_GET(weakly_equality_comparable_ne, Up, Tp)
        )
    }

    DEFINE_CONCEPT1_UNSAFE(equality_comparable, Tp, CONCEPT_GET(detail::weakly_equality_comparable_with, Tp, Tp))

    DEFINE_CONCEPT2_COND(
            equality_comparable_with, Tp, Up,
            CONCEPT_GET(equality_comparable, Tp) &&
            CONCEPT_GET(equality_comparable, Up) &&
            CONCEPT_GET(
                    common_reference_with,
                    detail::make_clref_t<Tp>, detail::make_clref_t<Up>
            ) && CONCEPT_GET(
                    equality_comparable,
                    common_reference_t<detail::make_clref_t<Tp>, detail::make_clref_t<Up> >
            ) && CONCEPT_GET(
                    detail::weakly_equality_comparable_with, Tp, Up
            )
    )

    namespace detail {
        DEFINE_CONCEPT2_EXP_IS(
                partially_ordered_lt, Tp, Up,
                std::declval<detail::make_clref_t<Tp> >() < std::declval<detail::make_clref_t<Up> >(),
                boolean_testable
        )
        DEFINE_CONCEPT2_EXP_IS(
                partially_ordered_gt, Tp, Up,
                std::declval<detail::make_clref_t<Tp> >() > std::declval<detail::make_clref_t<Up> >(),
                boolean_testable
        )
        DEFINE_CONCEPT2_EXP_IS(
                partially_ordered_le, Tp, Up,
                std::declval<detail::make_clref_t<Tp> >() <= std::declval<detail::make_clref_t<Up> >(),
                boolean_testable
        )
        DEFINE_CONCEPT2_EXP_IS(
                partially_ordered_ge, Tp, Up,
                std::declval<detail::make_clref_t<Tp> >() >= std::declval<detail::make_clref_t<Up> >(),
                boolean_testable
        )
        DEFINE_CONCEPT2_UNSAFE(
                partially_ordered_with, Tp, Up,
                CONCEPT_GET(partially_ordered_lt, Tp, Up) &&
                CONCEPT_GET(partially_ordered_gt, Tp, Up) &&
                CONCEPT_GET(partially_ordered_le, Tp, Up) &&
                CONCEPT_GET(partially_ordered_ge, Tp, Up) &&
                CONCEPT_GET(partially_ordered_lt, Up, Tp) &&
                CONCEPT_GET(partially_ordered_gt, Up, Tp) &&
                CONCEPT_GET(partially_ordered_le, Up, Tp) &&
                CONCEPT_GET(partially_ordered_ge, Up, Tp)
        )
    }

    DEFINE_CONCEPT1_UNSAFE(
            totally_ordered, Tp,
            CONCEPT_GET(equality_comparable, Tp) &&
            CONCEPT_GET(detail::partially_ordered_with, Tp, Tp)
    )
    DEFINE_CONCEPT2_COND(
            totally_ordered_with, Tp, Up,
            CONCEPT_GET(totally_ordered, Tp) &&
            CONCEPT_GET(totally_ordered, Up) &&
            CONCEPT_GET(equality_comparable_with, Tp, Up) &&
            CONCEPT_GET(
                    totally_ordered,
                    common_reference_t<detail::make_clref_t<Tp>, detail::make_clref_t<Up> >
            ) && CONCEPT_GET(detail::partially_ordered_with, Tp, Up)
    )

    DEFINE_CONCEPT1_COND(
            movable, Tp,
            std::is_object<Tp>::value &&
                    CONCEPT_GET(move_constructible, Tp) && CONCEPT_GET(assignable_from, Tp & , Tp) &&
                    CONCEPT_GET(swappable, Tp)
    )

    DEFINE_CONCEPT1_COND(
            copyable, Tp,
            CONCEPT_GET(copy_constructible, Tp) &&
            CONCEPT_GET(movable, Tp) &&
            CONCEPT_GET(assignable_from, Tp & , Tp & ) &&
            CONCEPT_GET(assignable_from, Tp & , const Tp&) &&
                    CONCEPT_GET(assignable_from, Tp &, const Tp)
    )

    DEFINE_CONCEPT1_UNSAFE(semiregular, Tp, CONCEPT_GET(copyable, Tp) && CONCEPT_GET(default_initializable, Tp))

    DEFINE_CONCEPT1_UNSAFE(regular, Tp, CONCEPT_GET(semiregular, Tp) && CONCEPT_GET(equality_comparable, Tp))

    DEFINE_CONCEPT_PREPEND_EXP(
            PARAM_PACK(class Fn, class ...Args),
            invocable,
            PARAM_PACK(Fn, Args...),
            extension::invoke(std::declval<Fn>(), std::declval<Args>()...)
    )

    DEFINE_CONCEPT2_UNSAFE(regular_invocable, Fn, ...Args, CONCEPT_GET(invocable, Fn, Args...))

    DEFINE_CONCEPT_PREPEND_COND(
            PARAM_PACK(class Fn, class ...Args),
            predicate,
            PARAM_PACK(Fn, Args...),
            CONCEPT_GET(regular_invocable, Fn, Args...) &&
            CONCEPT_GET(detail::boolean_testable, typename invoke_result<Fn, Args...>::type)
    )

    DEFINE_CONCEPT3_UNSAFE(
            relation, Rp, Tp, Up,
            CONCEPT_GET(predicate, Rp, Tp, Tp) && CONCEPT_GET(predicate, Rp, Up, Up) &&
            CONCEPT_GET(predicate, Rp, Tp, Up) && CONCEPT_GET(predicate, Rp, Up, Tp)
    )

    DEFINE_CONCEPT3_UNSAFE(equivalence_relation, Rp, Tp, Up, CONCEPT_GET(relation, Rp, Tp, Up))
    DEFINE_CONCEPT3_UNSAFE(strict_weak_order, Rp, Tp, Up, CONCEPT_GET(relation, Rp, Tp, Up))

#endif /* __cpp_concepts */

}

#endif /* 201103L */
