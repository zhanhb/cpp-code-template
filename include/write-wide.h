#pragma once

#include <locale>

namespace wide_writer {
    namespace detail {
#if __cplusplus >= 201103L
        using std::enable_if;
        template<bool b> using bool_constant = std::integral_constant<bool, b>;
#else
        template<bool, class = void>
        struct enable_if {
        };
        template<class Tp>
        struct enable_if<true, Tp> {
            typedef Tp type;
        };

        template<bool b>
        struct bool_constant {
            enum {
                value = b
            };
            typedef bool_constant type;
        };

#endif

        template<class Tp, class CharT>
        class is_string_view_test {

            template<class Up, Up>
            struct type_check;
            typedef char yes;
            typedef char no[2];

            template<class Up>
            static yes *test(
                    type_check<const CharT *(Tp::*)() const, &Up::data> *,
                    type_check<const typename Up::traits_type::char_type *(Tp::*)() const, &Up::data> *,
                    type_check<const typename Up::value_type *(Tp::*)() const, &Up::data> *,
                    type_check<size_t(Tp::*)() const, &Up::length> *,
                    type_check<typename Up::const_pointer(Tp::*)() const, &Up::data> *,
                    type_check<typename Up::iterator(Tp::*)() const, &Up::begin> *,
                    type_check<typename Up::iterator(Tp::*)() const, &Up::end> *,
                    type_check<typename Up::size_type(Tp::*)() const, &Up::length> *,
                    int
            ) { return 0; } // NOLINT(modernize-use-nullptr)

            template<class>
            static no *test(...) { return 0; } // NOLINT(modernize-use-nullptr)

        public:
            typedef bool_constant<
                    sizeof(*test<Tp>(0, 0, 0, 0, 0, 0, 0, 0, 0)) == sizeof(yes) // NOLINT(modernize-use-nullptr)
            > type;
        };

        template<class>
        struct is_string_view : bool_constant<false> {
        };
        template<template<class, class> class BasicStringView, class CharT, class CharTraits>
        struct is_string_view<BasicStringView<CharT, CharTraits> > : is_string_view_test<
                BasicStringView<CharT, CharTraits>, CharT>::type {
        };
    }

    typedef std::codecvt<wchar_t, char, std::mbstate_t> Facet;

    template<class InputC, class OutputC, class Output>
    inline void do_convert(
            std::codecvt_base::result (Facet::*impl)(
                    std::mbstate_t &,
                    const InputC *, const InputC *, const InputC *&,
                    OutputC *, OutputC *, OutputC *&) const,
            const InputC *input_begin, const InputC *input_end, Output &output,
            OutputC on_error, const Facet &facet
    ) {
        std::mbstate_t mb = std::mbstate_t();
        for (OutputC *begin = &output[0], *pos = begin, *end = begin + output.length();;) {
            std::codecvt_base::result res = (facet.*impl)(mb, input_begin, input_end, input_begin, pos, end, pos);
            switch (res) {
                case std::codecvt_base::ok: {
                    output.resize(pos - begin);
                    return;
                }
                case std::codecvt_base::partial:
                case std::codecvt_base::error:
                    *pos++ = on_error;
                    ++input_begin;
                    continue;
                case std::codecvt_base::noconv:
                default:
                    abort();
            }
        }
    }

    template<class Traits>
    inline std::basic_ostream<char, Traits> &
    write_string(std::basic_ostream<char, Traits> &out, const wchar_t *str, size_t len) {
        std::locale loc = out.getloc();
        if (loc.name() == "C") loc = std::locale("");
        if (!std::has_facet<Facet>(loc)) return out;
        const Facet &facet = std::use_facet<Facet>(loc); // NOLINT(modernize-use-auto)
        std::basic_string<char, Traits> output(len * facet.max_length(), char(0));
        do_convert(&Facet::out, str, str + len, output, char('?'), facet);
        return out << output;
    }

    template<class Traits>
    inline std::basic_ostream<wchar_t, Traits> &
    write_string(std::basic_ostream<wchar_t, Traits> &out, const char *str, size_t len) {
        std::locale loc = out.getloc();
        if (loc.name() == "C") loc = std::locale("");
        if (!std::has_facet<Facet>(loc)) return out;
        const Facet &facet = std::use_facet<Facet>(loc); // NOLINT(modernize-use-auto)
        std::basic_string<wchar_t, Traits> output(len, wchar_t(0));
        do_convert(&Facet::in, str, str + len, output, wchar_t(0xFFFD), facet);
        return out << output;
    }
}

template<class Traits1, class Traits2, class Alloc>
inline std::basic_ostream<char, Traits1> &
operator<<(std::basic_ostream<char, Traits1> &out, const std::basic_string<wchar_t, Traits2, Alloc> &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}

template<class Traits1, class Traits2, class Alloc>
inline std::basic_ostream<wchar_t, Traits1> &
operator<<(std::basic_ostream<wchar_t, Traits1> &out, const std::basic_string<char, Traits2, Alloc> &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}

template<class Traits>
inline std::basic_ostream<char, Traits> &
operator<<(std::basic_ostream<char, Traits> &out, const wchar_t *str) {
    return wide_writer::write_string(out, str, std::char_traits<wchar_t>::length(str));
}

template<class Traits>
inline std::basic_ostream<wchar_t, Traits> &
operator<<(std::basic_ostream<wchar_t, Traits> &out, const char *str) {
    return wide_writer::write_string(out, str, std::char_traits<char>::length(str));
}

template<class Traits>
inline std::basic_ostream<char, Traits> &
operator<<(std::basic_ostream<char, Traits> &out, wchar_t ch) {
    return wide_writer::write_string(out, &ch, 1);
}

template<class Traits1, class Traits2, template<class, class> class BasicStringView>
inline typename wide_writer::detail::enable_if<
        wide_writer::detail::is_string_view<BasicStringView<wchar_t, Traits2> >::value,
        std::basic_ostream<char, Traits1> &
>::type
operator<<(std::basic_ostream<char, Traits1> &out, const BasicStringView<wchar_t, Traits2> &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}

template<class Traits1, class Traits2, template<class, class> class BasicStringView>
inline typename wide_writer::detail::enable_if<
        wide_writer::detail::is_string_view<BasicStringView<char, Traits2> >::value,
        std::basic_ostream<wchar_t, Traits1> &
>::type
operator<<(std::basic_ostream<wchar_t, Traits1> &out, const BasicStringView<char, Traits2> &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}
