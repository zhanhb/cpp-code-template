#include "write-wide.h"
#include "debug.h"
#include "validate.h"

using namespace std;

#include <sstream>

template<class>
struct member_const_function_check {
};

template<class Tp, class Up>
struct member_const_function_check<Up (Tp::*)() const> {
    typedef Up type;
};

template<class Tp>
struct is_string_view_test4 {

    template<class>
    static void const_iterator(...) {}

    typedef decltype(const_iterator<Tp>(0))
    type;
};

template<class T, class U>
void test1(basic_ostringstream<T> &out, const basic_string<U> &str) {
    out << str;
}

template<class T, class U>
void test2(basic_ostringstream<T> &out, const basic_string<U> &str) {
    out << str.data();
}

template<class T, class U>
void test3(basic_ostringstream<T> &out, const basic_string<U> &str) {
    typedef typename std::remove_cv<typename std::remove_reference<decltype(str)>::type>::type string_t;
    out << std::basic_string_view<typename string_t::value_type, typename string_t::traits_type>(str);
}

template<class T, class U>
void test4(basic_ostringstream<T> &out, const basic_string<U> &str) {
    for (int i = 0; i < str.length(); ++i) {
        out << str[i];
    }
}

template<class T, class U>
void do_test(const basic_string<T> &expect, const basic_string<U> &u) {
    typedef void (*Fun)(basic_ostringstream<T> &, const basic_string<U> &);
    Fun fns[] = {test1, test2, test3};
    for (int i = 0; i < sizeof(fns) / sizeof(fns[0]); ++i) {
        basic_ostringstream<T> os;
        fns[i](os, u);
        basic_string<T> res = os.str();
        validate(expect, res);
    }

    if (!std::is_same<U, char>::value) {
        basic_ostringstream<T> os;
        test4(os, u);
        basic_string<T> res = os.str();
        validate(expect, res);
    }
}

template<class U, U u>
struct type_check {
    static U value() { return u; }
};

int main() {
    pair<string, wstring> test_cases[] = {
            make_pair("\u221e\u4f60\u597d", L"\u221e\u4f60\u597d"),
            make_pair("(-\u221e,+\u221e)", L"(-\u221e,+\u221e)"),
    };

    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); ++i) { // NOLINT(modernize-loop-convert)
        pair<string, wstring> &pair = test_cases[i];
        const string &a = pair.first;
        const wstring &b = pair.second;
        do_test(a, b);
        do_test(b, a);
    }
    typedef type_check<const char *(string_view::*)() const, &string_view::begin> BEGIN;
    auto t = BEGIN::value();
    out(t);

//    using cond = wide_writer::is_string_view_test<string_view, char>::type;
//    using It = wide_writer::is_string_view_test4<string_view>::type;
//
//    std::is_member_pointer<int> a;
//    auto t = &string_view::begin;
//    typedef wide_writer::member_function_check<decltype(t)>::type tmp_type;
//    tmp_type tt = t;
//    static_assert(is_same<It, const char *>::value, "");

//    mbstate_t mb{};
//    wchar_t arr[200] = {0};
//    out(mbrlen(str, strlen(str), &mb));
//    size_t n = mbrtowc(arr, str, strlen(str), &mb);
//    out(n);
//    out(arr);
    wcout.imbue(std::locale(""));

//    cout << wcout;
//    out(cout, wcout);

}
