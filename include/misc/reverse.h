#include <iterator>
#include <type_traits>

namespace reverse_wrapper {

#if __cplusplus >= 201402L
    using std::rbegin;
    using std::rend;
#else

    template<class Tp>
    inline auto rbegin(Tp &iterable) -> decltype(iterable.rbegin()) { return iterable.rbegin(); }

    template<class Tp>
    inline auto rend(Tp &iterable) -> decltype(iterable.rend()) { return iterable.rend(); }

    template<class Tp, size_t N>
    inline std::reverse_iterator<Tp *> rbegin(Tp (&arr)[N]) { return std::reverse_iterator<Tp *>(arr + N); }

    template<class Tp, size_t N>
    inline std::reverse_iterator<Tp *> rend(Tp (&arr)[N]) { return std::reverse_iterator<Tp *>(arr); }

    template<class Tp>
    inline std::reverse_iterator<const Tp *> rbegin(std::initializer_list<Tp> list) {
        return std::reverse_iterator<const Tp *>(list.end());
    }

    template<class Tp>
    inline std::reverse_iterator<const Tp *> rend(std::initializer_list<Tp> list) {
        return std::reverse_iterator<const Tp *>(list.begin());
    }

#endif

#if __cplusplus >= 201703L
    using std::void_t;
#else
    template<class...>
    struct void_t_impl {
        typedef void type;
    };
    template<class ...Args> using void_t = typename void_t_impl<Args...>::type;
#endif

    template<class Tp>
    struct mutable_member;
    template<class Tp>
    struct mutable_member<Tp &> {
        Tp &data;
    };
    template<class Tp>
    struct mutable_member<Tp &&> {
        mutable typename std::remove_const<Tp>::type data;
    };

    template<class Tp>
    class wrapper {
        mutable_member<Tp &&> upper;
    public:
        explicit wrapper(Tp &&iterable) noexcept(noexcept(decltype(upper)({std::forward<Tp>(iterable)}))):
                upper({std::forward<Tp>(iterable)}) {}

        [[nodiscard]] auto begin() const -> decltype(rbegin(upper.data)) { return rbegin(upper.data); }

        [[nodiscard]] auto end() const -> decltype(rend(upper.data)) { return rend(upper.data); }
    };

    template<class Tp, class = void>
    struct reversible : std::false_type {
    };
    template<class Tp>
    struct reversible<Tp, void_t<
            typename std::common_type<
                    decltype(rbegin(std::declval<Tp &>())),
                    decltype(rend(std::declval<Tp &>()))
            >::type
    > > : std::true_type {
    };

    template<class Tp>
    inline typename std::enable_if<reversible<Tp>::value, wrapper<Tp> >::type reverse(Tp &&upper) {
        return wrapper<Tp>(std::forward<Tp>(upper));
    }
}

using reverse_wrapper::reverse;
