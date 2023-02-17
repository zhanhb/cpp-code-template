#pragma once

#include <cstdlib>
#include <locale>
#include <ostream>
#include "common/string_traits.h"

namespace wide_writer {
    typedef std::codecvt<wchar_t, char, std::mbstate_t> Facet;

    template<class Tp, class Base, class InputC, class OutputC, class Output>
    inline void do_convert(
            Tp Base::*impl,
            const InputC *input_begin, const InputC *input_end, Output &output,
            OutputC on_error, const Facet &facet
    ) {
        std::mbstate_t mb = std::mbstate_t();
        for (OutputC *begin = &output[0], *pos = begin, *end = begin + output.length();;) {
            std::codecvt_base::result res = (facet.*impl)(mb, input_begin, input_end, input_begin, pos, end, pos);
            switch (res) {
                case std::codecvt_base::ok:
                    output.resize(pos - begin);
                    return;
                case std::codecvt_base::partial:
                case std::codecvt_base::error:
                    *pos++ = on_error;
                    ++input_begin;
                    continue;
                case std::codecvt_base::noconv:
                default:
                    std::abort();
            }
        }
    }

    template<class Traits>
    inline std::basic_ostream<char, Traits> &
    write_string(std::basic_ostream<char, Traits> &out, const wchar_t *str, std::size_t len) {
        std::locale loc = out.getloc();
        if (!std::has_facet<Facet>(loc)) return out;
        const Facet &facet = std::use_facet<Facet>(loc); // NOLINT(modernize-use-auto)
        std::basic_string<char, Traits> output(len * facet.max_length(), '?');
        do_convert(&Facet::out, str, str + len, output, '?', facet);
        return out << output;
    }

    template<class Traits>
    inline std::basic_ostream<wchar_t, Traits> &
    write_string(std::basic_ostream<wchar_t, Traits> &out, const char *str, std::size_t len) {
        std::locale loc = out.getloc();
        if (!std::has_facet<Facet>(loc)) return out;
        const Facet &facet = std::use_facet<Facet>(loc); // NOLINT(modernize-use-auto)
        std::basic_string<wchar_t, Traits> output(len, wchar_t(0xFFFD));
        do_convert(&Facet::in, str, str + len, output, wchar_t(0xFFFD), facet);
        return out << output;
    }
}

// deleted overloads.
// provides overloads with default char_traits only.
// @see https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt2
inline std::basic_ostream<char> &
operator<<(std::basic_ostream<char> &out, const wchar_t *str) {
    return wide_writer::write_string(out, str, std::char_traits<wchar_t>::length(str));
}

inline std::basic_ostream<wchar_t> &
operator<<(std::basic_ostream<wchar_t> &out, const char *str) {
    return wide_writer::write_string(out, str, std::char_traits<char>::length(str));
}

inline std::basic_ostream<char> &
operator<<(std::basic_ostream<char> &out, wchar_t ch) {
    return wide_writer::write_string(out, &ch, 1);
}

#if __cplusplus >= 201103L

template<class Traits, class String>
inline typename std::enable_if<
        utility::is_string_of<String, wchar_t>::value,
        std::basic_ostream<char, Traits> &>::type
operator<<(std::basic_ostream<char, Traits> &out, const String &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}

template<class Traits, class String>
inline typename std::enable_if<
        utility::is_string_of<String, char>::value,
        std::basic_ostream<wchar_t, Traits> &>::type
operator<<(std::basic_ostream<wchar_t, Traits> &out, const String &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}

#else /* 201103L */

template<class Traits, class String>
inline typename extension::enable_if<
        utility::is_string_of<String, wchar_t>::value,
        std::basic_ostream<char, Traits> &>::type
operator<<(std::basic_ostream<char, Traits> &out, const String &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}

template<class Traits, class String>
inline typename extension::enable_if<
        utility::is_string_of<String, char>::value,
        std::basic_ostream<wchar_t, Traits> &>::type
operator<<(std::basic_ostream<wchar_t, Traits> &out, const String &str) {
    return wide_writer::write_string(out, str.data(), str.length());
}

#endif /* 201103L */
