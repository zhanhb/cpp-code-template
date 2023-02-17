#pragma once

#if __cplusplus >= 201103L

#include <functional>
#include "unwrap_ref.h"
#include "../config.h"
#include "../detail/complete_check.h"

namespace extension {

#if __cpp_lib_invoke && PREFER_LIB_INVOKE
    using std::invoke;
#else
#define INVOKE_RETURN(...) noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

    namespace detail {
        template<class Tp>
        struct is_reference_wrapper : BOOL_CONSTANT(!std::is_same<unwrap_reference_t<Tp>, Tp>::value) {
        };

        template<class Base>
        struct argument_getter {
            template<class Derived, typename std::enable_if<
                    std::is_base_of<Base, typename std::decay<Derived>::type>::value,
                    int>::type = 0>
            static inline constexpr auto get(Derived &&ref) INVOKE_RETURN(static_cast<Derived &&>(ref))

            template<class RefWrap, typename std::enable_if<
                    !std::is_base_of<Base, typename std::decay<RefWrap>::type>::value &&
                    is_reference_wrapper<typename std::decay<RefWrap>::type>::value,
                    int>::type = 0>
            static inline constexpr auto get(RefWrap &&ref) INVOKE_RETURN(ref.get())

            template<class Pointer, typename std::enable_if<
                    !std::is_base_of<Base, typename std::decay<Pointer>::type>::value &&
                    !is_reference_wrapper<typename std::decay<Pointer>::type>::value,
                    int>::type = 0>
            static inline constexpr auto get(Pointer &&ptr) INVOKE_RETURN(*static_cast<Pointer &&>(ptr))
        };

        template<class Getter, bool>
        struct member_invoker {
            template<class Pmf, class Ref, class ...Args>
            static inline constexpr auto invoke(Pmf pmf, Ref &&ref, Args &&... args)
            INVOKE_RETURN((Getter::get(static_cast<Ref &&>(ref)).*pmf)(static_cast<Args &&>(args)...))
        };

        template<class Getter>
        struct member_invoker<Getter, false> {
            template<class Pmd, class Ref>
            static inline constexpr auto invoke(Pmd pmd, Ref &&ref)
            INVOKE_RETURN(Getter::get(static_cast<Ref &&>(ref)).*pmd)
        };

        template<class>
        struct invoke_impl {
            template<class Fn, class ...Args>
            static inline constexpr auto invoke(Fn &&fn, Args &&... args)
            INVOKE_RETURN(static_cast<Fn &&>(fn)(static_cast<Args &&>(args)...))

            template<class Fn, class ...Args>
            static inline constexpr auto invoke(std::reference_wrapper<Fn> fn, Args &&... args)
            INVOKE_RETURN(invoke_impl<typename std::decay<Fn>::type>::invoke(fn.get(), static_cast<Args &&>(args)...))
        };

        template<class Tp, class Base>
        struct invoke_impl<Tp Base::*> : member_invoker<argument_getter<Base>, std::is_function<Tp>::value> {
        };
    }

    template<class Fn, class ...Args>
    inline constexpr auto invoke(Fn &&fn, Args &&... args)
    INVOKE_RETURN(detail::enable_if_all_t<
            detail::invoke_impl<typename std::decay<Fn>::type>,
            detail::is_complete_or_unbounded<Fn>,
            detail::is_complete_or_unbounded<Args>...
    >::invoke(static_cast<Fn &&>(fn), static_cast<Args &&>(args)...))

#undef INVOKE_RETURN
#endif

#if PREFER_LIB_INVOKE
    using std::ref;
    using std::cref;
    using std::reference_wrapper;
#else

    template<class Tp>
    struct reference_wrapper {
        typedef Tp type;
    private:
        type *data;

        template<class Up>
        static inline Up *addressof(Up &val) noexcept { return std::addressof(val); }

        template<class Up>
        MAYBE_UNUSED static void addressof(Up &&) = delete; // NOLINT(modernize-use-equals-delete)

    public:

        template<
                class Up,
                class = typename std::enable_if<!std::is_same<typename std::decay<Up>::type, reference_wrapper>::value>::type,
                class = decltype(reference_wrapper::addressof<Tp>(std::declval<Up>()))
        >
        inline constexpr reference_wrapper(Up &&ref) // NOLINT(google-explicit-constructor)
        noexcept(noexcept(reference_wrapper::addressof<Tp>(static_cast<Up &&>(ref)))):
                data(reference_wrapper::addressof<Tp>(static_cast<Up &&>(ref))) {}

        inline constexpr type &get() const noexcept { return *data; }

        inline constexpr operator type &() const noexcept { return this->get(); } // NOLINT(google-explicit-constructor)

        template<class ...Args>
        inline constexpr auto operator()(Args &&... args) const
        noexcept(noexcept(extension::invoke(std::declval<type &>(), static_cast<Args &&>(args)...))) ->
        decltype(extension::invoke(std::declval<type &>(), static_cast<Args &&>(args)...)) {
            return extension::invoke(this->get(), static_cast<Args &&>(args)...);
        }
    };

    // @formatter:off
#if __cpp_deduction_guides
    template<class Tp> reference_wrapper(Tp &) -> reference_wrapper<Tp>;
    template<class Tp> reference_wrapper(std::reference_wrapper<Tp>) -> reference_wrapper<Tp>;
#endif

    template<class Tp> inline constexpr reference_wrapper<Tp> ref(Tp &tp) noexcept { return {tp}; }
    template<class Tp> inline constexpr reference_wrapper<Tp> ref(std::reference_wrapper<Tp> tp) noexcept { return {tp.get()}; }
    template<class Tp> inline constexpr reference_wrapper<Tp> ref(reference_wrapper<Tp> tp) noexcept { return tp; }
    template<class Tp> inline constexpr reference_wrapper<const Tp> cref(const Tp &tp) noexcept { return {tp}; }
    template<class Tp> inline constexpr reference_wrapper<const Tp> cref(std::reference_wrapper<Tp> tp) noexcept { return {tp.get()}; }
    template<class Tp> inline constexpr reference_wrapper<const Tp> cref(reference_wrapper<Tp> tp) noexcept { return {tp.get()}; }
    template<class Tp> void ref(const Tp &&) = delete;
    template<class Tp> void cref(const Tp &&) = delete;
    // @formatter:on

#endif

#if __cpp_lib_invoke_r && PREFER_LIB_INVOKE
    using std::invoke_r;
#else
#define INVOKE_RETURN(...) noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

    namespace detail {
        template<class Tp>
        static inline void accept(Tp) noexcept; // NOLINT(readability-redundant-declaration)

        template<bool>
        struct invoke_r_impl {
            template<class Tp, class ...Args>
            static inline constexpr auto invoke_r(Args &&... args)
            INVOKE_RETURN(Tp(extension::invoke(static_cast<Args &&>(args)...)))
        };

        template<>
        struct invoke_r_impl<false> {
            template<class Tp, class ...Args>
            static inline constexpr auto invoke_r(Args &&... args)
            noexcept(noexcept(detail::accept<Tp>(extension::invoke(static_cast<Args &&>(args)...)))) ->
            detail::first_type_t<Tp, decltype(detail::accept<Tp>(extension::invoke(static_cast<Args &&>(args)...)))> {
                return extension::invoke(static_cast<Args &&>(args)...);
            }
        };
    }

    template<class Ret, class Fn, class ...Args>
    inline constexpr auto invoke_r(Fn &&fn, Args &&... args)
    INVOKE_RETURN(detail::enable_if_all_t<
            detail::invoke_r_impl<std::is_void<Ret>::value>,
            detail::is_complete_or_unbounded<Ret>,
            detail::is_complete_or_unbounded<Fn>,
            detail::is_complete_or_unbounded<Args>...
    >::template invoke_r<Ret>(static_cast<Fn &&>(fn), static_cast<Args &&>(args)...))

#undef INVOKE_RETURN
#endif

}

#endif /* 201103L */
