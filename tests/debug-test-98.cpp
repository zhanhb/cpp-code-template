#include "debug.h"
#include <bits/stdc++.h>
#include "extension/config.h"
#include "extension/nullptr.h"
#include "static_assert.h"

#if __cplusplus >= 201103L
namespace extension {
    using namespace std;
}
#endif

template<class Tp>
struct is_class_or_union : BOOL_CONSTANT(extension::is_class<Tp>::value || extension::is_union<Tp>::value) {
};

template<class>
struct member_info {
};
template<class Tp, class Base>
struct member_info<Tp Base::*> {
    typedef Tp return_type;
    typedef Base owner_type;
};

struct empty_t;

typedef int cq_fun() const;

typedef cq_fun empty_t::* qualified_member_fun;
typedef qualified_member_fun *member_fun_pointer;

STATIC_ASSERT_NOT_V(extension::is_const, cq_fun);
STATIC_ASSERT_NOT_V(extension::is_same, cq_fun, extension::add_const<int()>::type);
STATIC_ASSERT_SAME_AS(cq_fun, member_info<qualified_member_fun>::return_type);
STATIC_ASSERT_SAME_AS(empty_t, member_info<qualified_member_fun>::owner_type);

STATIC_ASSERT_SAME_AS(int*, extension::add_pointer<int>::type);
STATIC_ASSERT_SAME_AS(int*, extension::add_pointer<int &>::type);
STATIC_ASSERT_SAME_AS(int(*)[5], extension::add_pointer<int[5]>::type);
STATIC_ASSERT_SAME_AS(void*, extension::add_pointer<void>::type);
STATIC_ASSERT_SAME_AS(const void*, extension::add_pointer<const void>::type);
STATIC_ASSERT_SAME_AS(volatile void*, extension::add_pointer<volatile void>::type);
STATIC_ASSERT_SAME_AS(int(**)(), extension::add_pointer<int (*)()>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::add_pointer<cq_fun>::type);
STATIC_ASSERT_SAME_AS(member_fun_pointer, extension::add_pointer<qualified_member_fun>::type);

STATIC_ASSERT_SAME_AS(qualified_member_fun, extension::remove_pointer<qualified_member_fun>::type);
STATIC_ASSERT_NOT_SAME_AS(qualified_member_fun, extension::remove_pointer<qualified_member_fun const>::type);
STATIC_ASSERT_SAME_AS(qualified_member_fun const, extension::remove_pointer<qualified_member_fun const>::type);
STATIC_ASSERT_SAME_AS(qualified_member_fun, extension::remove_pointer<member_fun_pointer>::type);
STATIC_ASSERT_SAME_AS(qualified_member_fun, extension::remove_pointer<member_fun_pointer const>::type);

STATIC_ASSERT_SAME_AS(int &, extension::add_const<int &>::type);
STATIC_ASSERT_SAME_AS(const int, extension::add_const<int>::type);
STATIC_ASSERT_SAME_AS(const int[5], extension::add_const<int[5]>::type);
STATIC_ASSERT_NOT_V(extension::is_same, void, extension::add_const<void>::type);
STATIC_ASSERT_SAME_AS(const void, extension::add_const<void>::type);
STATIC_ASSERT_SAME_AS(int(), extension::add_const<int()>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::add_const<cq_fun>::type);
STATIC_ASSERT_NOT_V(extension::is_same, int[5], extension::add_const<int[5]>::type);
STATIC_ASSERT_SAME_AS(const int, extension::remove_extent<extension::add_const<int[5]>::type>::type);

STATIC_ASSERT_SAME_AS(int &, extension::add_volatile<int &>::type);
STATIC_ASSERT_SAME_AS(volatile int, extension::add_volatile<int>::type);
STATIC_ASSERT_SAME_AS(volatile int[5], extension::add_volatile<int[5]>::type);
STATIC_ASSERT_NOT_V(extension::is_same, void, extension::add_volatile<void>::type);
STATIC_ASSERT_SAME_AS(volatile void, extension::add_volatile<void>::type);
STATIC_ASSERT_SAME_AS(int(), extension::add_volatile<int()>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::add_volatile<cq_fun>::type);

STATIC_ASSERT_SAME_AS(int &, extension::add_cv<int &>::type);
STATIC_ASSERT_SAME_AS(const volatile int, extension::add_cv<int>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::add_cv<cq_fun>::type);

STATIC_ASSERT_SAME_AS(int &, extension::add_lvalue_reference<int &>::type);
STATIC_ASSERT_SAME_AS(int &, extension::add_lvalue_reference<int>::type);
STATIC_ASSERT_SAME_AS(int(&)[5], extension::add_lvalue_reference<int[5]>::type);
STATIC_ASSERT_SAME_AS(char(&)[1], extension::add_lvalue_reference<char[1]>::type);
STATIC_ASSERT_SAME_AS(char(&)[2], extension::add_lvalue_reference<char[2]>::type);
STATIC_ASSERT_SAME_AS(char(&)[3], extension::add_lvalue_reference<char[3]>::type);
STATIC_ASSERT_SAME_AS(void, extension::add_lvalue_reference<void>::type);
STATIC_ASSERT_SAME_AS(int(&)(), extension::add_lvalue_reference<int()>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::add_lvalue_reference<cq_fun>::type);

STATIC_ASSERT_SAME_AS(int, extension::remove_reference<int &>::type);
STATIC_ASSERT_SAME_AS(int, extension::remove_reference<int>::type);
STATIC_ASSERT_SAME_AS(int[5], extension::remove_reference<int[5]>::type);
STATIC_ASSERT_SAME_AS(void, extension::remove_reference<void>::type);
STATIC_ASSERT_SAME_AS(int(), extension::remove_reference<int()>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::remove_reference<cq_fun>::type);

STATIC_ASSERT_NOT_V(extension::is_lvalue_reference, int);
STATIC_ASSERT_V(extension::is_lvalue_reference, int &);
STATIC_ASSERT_NOT_V(extension::is_lvalue_reference, int[5]);
STATIC_ASSERT_NOT_V(extension::is_lvalue_reference, void);
STATIC_ASSERT_NOT_V(extension::is_lvalue_reference, int());
STATIC_ASSERT_NOT_V(extension::is_lvalue_reference, cq_fun);

STATIC_ASSERT_NOT_V(extension::is_rvalue_reference, int);
STATIC_ASSERT_NOT_V(extension::is_rvalue_reference, int &);
STATIC_ASSERT_NOT_V(extension::is_rvalue_reference, int[5]);
STATIC_ASSERT_NOT_V(extension::is_rvalue_reference, void);
STATIC_ASSERT_NOT_V(extension::is_rvalue_reference, int());
STATIC_ASSERT_NOT_V(extension::is_rvalue_reference, cq_fun);

STATIC_ASSERT_NOT_V(extension::is_function, int);
STATIC_ASSERT_NOT_V(extension::is_function, int &);
STATIC_ASSERT_NOT_V(extension::is_function, int[5]);
STATIC_ASSERT_NOT_V(extension::is_function, void);
STATIC_ASSERT_V(extension::is_function, int());
STATIC_ASSERT_V(extension::is_function, cq_fun);

STATIC_ASSERT_SAME_AS(int, extension::decay<int>::type);
STATIC_ASSERT_SAME_AS(int, extension::decay<int &>::type);
STATIC_ASSERT_SAME_AS(int*, extension::decay<int[5]>::type);
STATIC_ASSERT_SAME_AS(void, extension::decay<void>::type);
STATIC_ASSERT_SAME_AS(void, extension::decay<const void>::type);
STATIC_ASSERT_SAME_AS(int(*)(), extension::decay<int()>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::decay<cq_fun>::type);

template<class int_r, class, bool = extension::is_rvalue_reference<int_r>::value>
struct static_assert_rvalue_reference : extension::true_type {
};
template<class int_r, class fun_r>
struct static_assert_rvalue_reference<int_r, fun_r, true> : extension::true_type {
    STATIC_ASSERT_SAME_AS(int*, typename extension::add_pointer<int_r>::type);
    STATIC_ASSERT_SAME_AS(int(*)(), typename extension::add_pointer<fun_r>::type);
    STATIC_ASSERT_SAME_AS(int &, typename extension::add_lvalue_reference<int_r>::type);
    STATIC_ASSERT_SAME_AS(int, typename extension::remove_reference<int_r>::type);
    STATIC_ASSERT_NOT_V(extension::is_lvalue_reference, int_r);
    STATIC_ASSERT_V(extension::is_rvalue_reference, int_r);
    STATIC_ASSERT_NOT_V(extension::is_function, int_r);
    STATIC_ASSERT_SAME_AS(int, typename extension::decay<int_r>::type);

    STATIC_ASSERT_SAME_AS(int(&)(), typename extension::add_lvalue_reference<fun_r>::type);
    STATIC_ASSERT_SAME_AS(int(), typename extension::remove_reference<fun_r>::type);
    STATIC_ASSERT_NOT_V(extension::is_lvalue_reference, fun_r);
    STATIC_ASSERT_V(extension::is_rvalue_reference, fun_r);
    STATIC_ASSERT_NOT_V(extension::is_function, fun_r);
    STATIC_ASSERT_SAME_AS(int(*)(), typename extension::decay<fun_r>::type);

    STATIC_ASSERT_SAME_AS(int_r, typename extension::add_rvalue_reference<int_r>::type);
    STATIC_ASSERT_SAME_AS(int_r, extension::add_rvalue_reference<int>::type);
    STATIC_ASSERT_SAME_AS(fun_r, extension::add_rvalue_reference<int()>::type);
};

STATIC_ASSERT_SAME_AS(int &, extension::add_rvalue_reference<int &>::type);
STATIC_ASSERT_SAME_AS(void, extension::add_rvalue_reference<void>::type);
STATIC_ASSERT_SAME_AS(cq_fun, extension::add_rvalue_reference<cq_fun>::type);

STATIC_ASSERT_V(
        static_assert_rvalue_reference,
        extension::add_rvalue_reference<int>::type,
        extension::add_rvalue_reference<int()>::type
);
enum en {
};

class cl {
};

union un {
};
struct incomplete;
union incomplete_un;

STATIC_ASSERT_NOT_V(is_class_or_union, en);
STATIC_ASSERT_NOT_V(is_class_or_union, int);
STATIC_ASSERT_V(extension::is_class, cl);
STATIC_ASSERT_V(extension::is_union, un);
STATIC_ASSERT_V(extension::is_class, incomplete);
STATIC_ASSERT_V(extension::is_union, incomplete_un);

STATIC_ASSERT_NOT_V(is_class_or_union, const en);
STATIC_ASSERT_NOT_V(is_class_or_union, const int);
STATIC_ASSERT_V(extension::is_class, const cl);
STATIC_ASSERT_V(extension::is_union, const un);
STATIC_ASSERT_V(extension::is_class, const incomplete);
STATIC_ASSERT_V(extension::is_union, const incomplete_un);

struct range_like_1 {
    void begin() const {}

    NODISCARD int end() const { return 0; } // NOLINT(readability-convert-member-functions-to-static)
};

struct range_like_2 {
    NODISCARD std::vector<int> begin() const { // NOLINT(readability-convert-member-functions-to-static)
        return std::vector<int>(); // NOLINT(modernize-return-braced-init-list)
    }

    NODISCARD std::vector<int> end() const { // NOLINT(readability-convert-member-functions-to-static)
        return std::vector<int>(); // NOLINT(modernize-return-braced-init-list)
    }
};

struct range_like_3 {
    NODISCARD int *begin() const { return nullptr; } // NOLINT(readability-convert-member-functions-to-static)
    NODISCARD int *end() const { return nullptr; } // NOLINT(readability-convert-member-functions-to-static)
};

struct range_like_4 {
    NODISCARD int *begin() const { return nullptr; } // NOLINT(readability-convert-member-functions-to-static)
    NODISCARD const char *end() const { return nullptr; } // NOLINT(readability-convert-member-functions-to-static)
};

struct range_like_5 {
    NODISCARD char *begin() const { return nullptr; } // NOLINT(readability-convert-member-functions-to-static)
    NODISCARD const char *end() const { return nullptr; } // NOLINT(readability-convert-member-functions-to-static)
};

STATIC_ASSERT_NOT_V(utility::is_range, range_like_1);
STATIC_ASSERT_NOT_V(utility::is_range, range_like_2);
STATIC_ASSERT_V(utility::is_range, range_like_3);
STATIC_ASSERT_NOT_V(utility::is_range, range_like_4);
#if __cpp_range_based_for >= 201603L
STATIC_ASSERT_V(utility::is_range, range_like_5);
#else
STATIC_ASSERT_NOT_V(utility::is_range, range_like_5);
#endif
STATIC_ASSERT_V(utility::is_range, std::vector<int>);
STATIC_ASSERT_NOT_V(utility::is_range, std::ostream);

int main() {
    OUT(std::setlocale(LC_CTYPE, nullptr));
    std::setlocale(LC_CTYPE, "");
    OUT(std::setlocale(LC_CTYPE, nullptr));
    OUT(std::queue<int>(), std::priority_queue<int>(), std::stack<int>());

    OUT(std::vector<std::vector<int> >(), std::vector<std::map<int, int> >(), std::map<int, int>());
    std::cout << std::vector<int>() << std::map<int, int>() << std::endl;
}
