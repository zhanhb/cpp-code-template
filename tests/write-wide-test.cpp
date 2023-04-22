#include "write-wide.h"
#include <clocale>
#include <cstring>
#include <sstream>
#include "static_assert.h"
#include "verify.h"

using namespace std;

template<class Tp, class Up>
void test1(basic_ostringstream<Tp> &out, const basic_string<Up> &str) {
    out << str;
}

template<class Tp, class Up>
void test2(basic_ostringstream<Tp> &out, const basic_string<Up> &str) {
    out << str.data();
}

#if __cpp_lib_string_view

template<class Tp, class Up>
void test3(basic_ostringstream<Tp> &out, const basic_string<Up> &str) {
    typedef typename std::decay<decltype(str)>::type string_t;
    out << std::basic_string_view<typename string_t::value_type, typename string_t::traits_type>(str);
}

#endif

template<class Tp>
struct test4 {
    template<class Up>
    static void do_test(const std::basic_string<Tp> &, const std::basic_string<Up> &, std::locale &) {}
};

template<>
struct test4<char> {
    typedef char Tp;

    template<class Up>
    static void do_test(const std::basic_string<Tp> &expect, const std::basic_string<Up> &str, std::locale &loc) {
        basic_ostringstream<Tp> os;
        os.imbue(loc);
        for (std::size_t i = 0; i != str.length(); ++i) { // NOLINT(modernize-loop-convert)
            os << str[i];
        }
        basic_string<Tp> res = os.str();
        verify(expect, res);
    }
};

template<class Tp, class Up>
void do_test(const basic_string<Tp> &expect, const basic_string<Up> &u, std::locale &loc) {
    typedef void (*Fun)(basic_ostringstream<Tp> &, const basic_string<Up> &);
    Fun fns[] = {
            test1,
            test2,
#if __cpp_lib_string_view
            test3,
#endif
    };
    for (std::size_t i = 0; i != sizeof(fns) / sizeof(fns[0]); ++i) {
        basic_ostringstream<Tp> os;
        os.imbue(loc);
        fns[i](os, u);
        basic_string<Tp> res = os.str();
        verify(expect, res);
    }

    test4<Tp>::do_test(expect, u, loc);
}

STATIC_ASSERT_SAME_AS(char, utility::string_traits<std::string>::value_type);
STATIC_ASSERT_SAME_AS(wchar_t, utility::string_traits<std::wstring>::value_type);
STATIC_ASSERT_CONCEPT(utility::string_of, std::string, char);
STATIC_ASSERT_CONCEPT(utility::string_of, std::wstring, wchar_t);
#if __cplusplus >= 201103L
STATIC_ASSERT_CONCEPT(utility::string_of, std::string &, char);
STATIC_ASSERT_CONCEPT(utility::string_of, std::wstring &, wchar_t);
STATIC_ASSERT_CONCEPT(utility::string_of, std::string &&, char);
STATIC_ASSERT_CONCEPT(utility::string_of, std::wstring &&, wchar_t);
#endif
STATIC_ASSERT_CONCEPT(utility::string_of, const std::string, char);
STATIC_ASSERT_CONCEPT(utility::string_of, const std::wstring, wchar_t);

class lctype_around {
    const char *old;
public:
    explicit lctype_around(const char *loc) : old(std::setlocale(LC_CTYPE, reinterpret_cast<const char *>(0))) {
        std::setlocale(LC_CTYPE, loc);
    }

    ~lctype_around() { std::setlocale(LC_CTYPE, old); }
};

class fallback_codecvt : public std::codecvt<wchar_t, char, std::mbstate_t> {
    const char *loc_;
public:

    explicit fallback_codecvt(const char *loc = "") : loc_(loc) {}

protected:
    virtual result do_out(
            state_type &state,
            const intern_type *from, const intern_type *from_end, const intern_type *&from_next,
            extern_type *to, extern_type *to_end, extern_type *&to_next
    ) const throw() {
        lctype_around guard(loc_);
        char buf[8];
        for (from_next = from, to_next = to; from_next != from_end; ++from_next) {
            wchar_t wch = *from_next;
            std::size_t res = std::wcrtomb(buf, wch, &state);
            if (res == std::size_t(-1)) return error;
            if (to_end - to_next < (std::ptrdiff_t) res) return partial;
            std::memcpy(to_next, buf, res);
            to_next += res;
        }
        return ok;
    }

    virtual result do_in(
            state_type &state,
            const extern_type *from, const extern_type *from_end, const extern_type *&from_next,
            intern_type *to, intern_type *to_end, intern_type *&to_next
    ) const throw() {
        lctype_around guard(loc_);
        for (from_next = from, to_next = to; from_next != from_end;) {
            if (to_next == to_end) return partial;
            wchar_t wch[2];
            std::size_t res = std::mbrtowc(wch, from_next, from_end - from_next, &state);
            switch (res) {
                case std::size_t(-2):
                    return partial;
                case std::size_t(-1):
                    return error;
                case 0:
                    res = 1;
                default:
                    break;
            }
            *to_next++ = *wch;
            from_next += res;
        }
        return ok;
    }

    virtual int do_encoding() const throw() { return -1; }

    virtual bool do_always_noconv() const throw() { return false; }

    virtual int do_max_length() const throw() {
        lctype_around guard(loc_);
        return MB_CUR_MAX;
    }
};

int main() {
    std::locale loc;
    try {
        loc = std::locale("");
    } catch (...) {
        loc = std::locale(std::locale("C"), new fallback_codecvt());
    }
    pair<string, wstring> test_cases[] = {
            make_pair("\u4f60\u597d", L"\u4f60\u597d"),
            make_pair("(-\u221e,+\u221e)", L"(-\u221e,+\u221e)"),
            make_pair("z\u00df\u6c34\U0001f600", L"z\u00df\u6c34\U0001f600"),
    };

    std::cout.imbue(loc);
    for (std::size_t i = 0; i != sizeof(test_cases) / sizeof(test_cases[0]); ++i) { // NOLINT(modernize-loop-convert)
        pair<string, wstring> &pair = test_cases[i];
        const string &a = pair.first;
        const wstring &b = pair.second;
        do_test(a, b, loc);
        do_test(b, a, loc);
    }
}
