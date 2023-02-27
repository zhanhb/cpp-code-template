#pragma once

#if __cplusplus >= 201103L

#include "copy_cv.h"
#include "lib_type_traits.h"
#include "remove_cvref.h"
#include "void_t.h"

namespace extension {
#if __cplusplus >= 202002L
    using std::common_reference;
    using std::common_reference_t;
    using std::basic_common_reference;
#else
    namespace detail {

        // Let COND_RES(X, Y) be:
        template<class Tp, class Up>
        using cond_res = decltype(false ? std::declval<Tp(&)()>()() : std::declval<Up(&)()>()()); // NOLINT

        // Given types A and B, let X be remove_reference_t<A>, let Y be remove_reference_t<B>,
        // and let COMMON-REF(A, B) be:
        template<class Ap, class Bp, class = void>
        struct common_ref;

        template<class Xp, class Yp>
        using cv_cond_res = cond_res<extension::copy_cv_t<Xp, Yp> &, extension::copy_cv_t<Yp, Xp> &>;

        // If A and B are both lvalue reference types, COMMON-REF(A, B) is
        // COND-RES(COPYCV(X, Y)&, COPYCV(Y, X)&) if that type exists and is a reference type.
        template<class Xp, class Yp>
        struct common_ref<Xp &, Yp &, typename std::enable_if<
                std::is_reference<cv_cond_res<Xp, Yp> >::value
        >::type> {
            using type = cv_cond_res<Xp, Yp>;
        };

        // Otherwise, let C be remove_reference_t<COMMON-REF(X&, Y&)>&&. ...
        template<class Xp, class Yp>
        using common_ref_C = typename std::remove_reference<typename common_ref<Xp &, Yp &>::type>::type &&;

        template<class Xp, class Yp>
        struct common_ref<Xp &&, Yp &&, typename std::enable_if<
                std::is_convertible<Xp &&, common_ref_C<Xp, Yp> >::value &&
                std::is_convertible<Yp &&, common_ref_C<Xp, Yp> >::value
        >::type> {
            using type = common_ref_C<Xp, Yp>;
        };

        // Otherwise, let D be COMMON-REF(const X&, Y&). ...
        template<class Xp, class Yp>
        using common_ref_D = typename common_ref<const Xp &, Yp &>::type;

        // ... If A is an rvalue reference and B is an lvalue reference and D is well-formed and
        // is_convertible_v<A, D> is true, then COMMON-REF(A, B) is D.
        template<class Xp, class Yp>
        struct common_ref<Xp &&, Yp &, typename std::enable_if<
                std::is_convertible<Xp &&, common_ref_D<Xp, Yp> >::value
        >::type> {
            using type = common_ref_D<Xp, Yp>;
        };

        // Otherwise, if A is an lvalue reference and B is an rvalue reference, then
        // COMMON-REF(A, B) is COMMON-REF(B, A).
        template<class Xp, class Yp>
        struct common_ref<Xp &, Yp &&, extension::void_t<typename common_ref<Yp &&, Xp &>::type> > {
            using type = typename common_ref<Yp &&, Xp &>::type;
        };

        // Otherwise, COMMON-REF(A, B) is ill-formed.
        template<class, class, class>
        struct common_ref {
        };
    }

    // @formatter:off
    template<class, class, template<class> class, template<class> class>
    // @formatter:on
    struct basic_common_reference {
    };

    template<class...>
    struct common_reference;
    template<class ...Types> using common_reference_t = typename common_reference<Types...>::type;

    // bullet 1 - sizeof...(T) == 0
    template<>
    struct common_reference<> {
    };

    // bullet 2 - sizeof...(T) == 1
    template<class Tp>
    struct common_reference<Tp> {
        typedef Tp type;
    };

    namespace detail {

        // Let `XREF(A)` denote a unary alias template `T` such that `T<U>` denotes the same type as `U`
        // with the addition of `A`'s cv and reference qualifiers, for a non-reference cv-unqualified type
        // `U`.
        // [Note: `XREF(A)` is `xref<A>::template apply`]
        // @formatter:off
        template<class Tp> struct xref { template<class Up> using apply = extension::copy_cv_t<Tp, Up>; };
        template<class Tp> struct xref<Tp &> { template<class Up> using apply = extension::copy_cv_t<Tp, Up> &; };
        template<class Tp> struct xref<Tp &&> { template<class Up> using apply = extension::copy_cv_t<Tp, Up> &&; };
        // @formatter:on

        template<class Tp, class Up> using basic_common_reference_t = typename basic_common_reference<
                extension::remove_cvref_t<Tp>, extension::remove_cvref_t<Up>,
                xref<Tp>::template apply, xref<Up>::template apply
        >::type;

        template<class Tp, class Up, int bullet = 1, class = void>
        struct common_reference_sub_bullet : common_reference_sub_bullet<Tp, Up, bullet + 1> {
        };

        // sub-bullet 1 - If T1 and T2 are reference types and COMMON-REF(T1, T2) is well-formed, then
        // the member typedef `type` denotes that type.
        template<class Tp, class Up>
        struct common_reference_sub_bullet<Tp, Up, 1, extension::void_t<
                typename std::enable_if<std::is_reference<Tp>::value>::type,
                typename std::enable_if<std::is_reference<Up>::value>::type,
                typename common_ref<Tp, Up>::type
        > > {
            using type = typename common_ref<Tp, Up>::type;
        };
        // sub-bullet 2 - Otherwise, if basic_common_reference<remove_cvref_t<T1>, remove_cvref_t<T2>, XREF(T1), XREF(T2)>::type
        // is well-formed, then the member typedef `type` denotes that type.
        template<class Tp, class Up>
        struct common_reference_sub_bullet<Tp, Up, 2, extension::void_t<basic_common_reference_t<Tp, Up> > > {
            using type = basic_common_reference_t<Tp, Up>;
        };
        // sub-bullet 3 - Otherwise, if COND-RES(T1, T2) is well-formed,
        // then the member typedef `type` denotes that type.
        template<class Tp, class Up>
        struct common_reference_sub_bullet<Tp, Up, 3, extension::void_t<cond_res<Tp, Up> > > {
            using type = cond_res<Tp, Up>;
        };
        // sub-bullet 4 - Otherwise, if common_type_t<T1, T2> is well-formed,
        // then the member typedef `type` denotes that type.
        template<class Tp, class Up>
        struct common_reference_sub_bullet<Tp, Up, 4, extension::void_t<typename std::common_type<Tp, Up>::type> > {
            using type = typename std::common_type<Tp, Up>::type;
        };
        // sub-bullet 5 - Otherwise, there shall be no member `type`.
        template<class Tp, class Up>
        struct common_reference_sub_bullet<Tp, Up, 5> {
        };
    }

    // bullet 3 - sizeof...(T) == 2
    template<class Tp, class Up>
    struct common_reference<Tp, Up> : detail::common_reference_sub_bullet<Tp, Up> {
    };

    namespace detail {
        template<class...>
        struct common_reference_bullet4 {
        };
        template<class Tp, class Up, class ...Rest>
        struct common_reference_bullet4<extension::void_t<
                common_reference_t<common_reference_t<Tp, Up>, Rest...>
        >, Tp, Up, Rest...> {
            using type = common_reference_t<common_reference_t<Tp, Up>, Rest...>;
        };
    }

    // bullet 4 - If there is such a type `C`, the member typedef type shall denote the same type, if
    //            any, as `common_reference_t<C, Rest...>`.
    template<class Tp, class Up, class Vp, class ...Rest>
    struct common_reference<Tp, Up, Vp, Rest...> : detail::common_reference_bullet4<void, Tp, Up, Vp, Rest...> {
    };

    // bullet 5 - Otherwise, there shall be no member `type`.
    template<class...>
    struct common_reference {
    };
#endif
}

#endif
