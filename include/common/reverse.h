#include <iterator>
#include <type_traits>
#include "is_range.h"

namespace reverse_wrapper {

#if __cplusplus >= 201402L
    using std::rbegin;
    using std::rend;
#else

    template<class Tp>
    inline auto rbegin(Tp &iterable) -> decltype(iterable.rbegin()) { return iterable.rbegin(); }

    template<class Tp>
    inline auto rend(Tp &iterable) -> decltype(iterable.rend()) { return iterable.rend(); }

    template<class Tp, std::size_t Sz>
    inline std::reverse_iterator<Tp *> rbegin(Tp (&arr)[Sz]) { return std::reverse_iterator<Tp *>(arr + Sz); }

    template<class Tp, std::size_t Sz>
    inline std::reverse_iterator<Tp *> rend(Tp (&arr)[Sz]) { return std::reverse_iterator<Tp *>(arr); }

    template<class Tp>
    inline std::reverse_iterator<const Tp *> rbegin(std::initializer_list<Tp> list) {
        return std::reverse_iterator<const Tp *>(list.end());
    }

    template<class Tp>
    inline std::reverse_iterator<const Tp *> rend(std::initializer_list<Tp> list) {
        return std::reverse_iterator<const Tp *>(list.begin());
    }

#endif

    template<class>
    struct mutable_member;
    template<class Tp>
    struct mutable_member<Tp &> {
        Tp &data;
    };
    template<class Tp>
    struct mutable_member<Tp &&> {
        mutable typename std::remove_cv<Tp>::type data;
    };

    template<class Tp>
    class wrapper {
        mutable_member<Tp &&> upper;
    public:
        explicit wrapper(Tp &&iterable)
        noexcept(noexcept(decltype(upper)({std::forward<Tp>(iterable)}))):
                upper({std::forward<Tp>(iterable)}) {}

        NODISCARD auto begin() const -> decltype(reverse_wrapper::rbegin(upper.data)) {
            return reverse_wrapper::rbegin(upper.data);
        }

        NODISCARD auto end() const -> decltype(reverse_wrapper::rend(upper.data)) {
            return reverse_wrapper::rend(upper.data);
        }
    };

    template<class, class = void>
    struct reversible : std::false_type {
    };
    template<class Tp>
    struct reversible<Tp, typename std::enable_if<utility::is_range_iterator<
            decltype(reverse_wrapper::rbegin(std::declval<Tp &>())),
            decltype(reverse_wrapper::rend(std::declval<Tp &>()))
    >::value>::type> : std::true_type {
    };

    template<class Tp>
    inline typename std::enable_if<
            reversible<decltype(std::declval<mutable_member<Tp &&>>().data)>::value,
            wrapper<Tp>
    >::type reverse(Tp &&upper) {
        return wrapper<Tp>(std::forward<Tp>(upper));
    }
}

using reverse_wrapper::reverse;
