#pragma once

#include <string>

template<class CharT, class Traits, class Alloc>
std::basic_string<CharT, Traits, Alloc>
operator*(const std::basic_string<CharT, Traits, Alloc> &s, std::size_t n) {
    std::size_t len = s.length();
    if (n <= 1 || !len) {
        return n == 1 ? s : std::basic_string<CharT, Traits, Alloc>();
    }
    if (len == 1) return std::basic_string<CharT, Traits, Alloc>(n, s[0]);
    std::size_t total = len * n;
    if (total / n != len) total = std::size_t(-1);
    std::basic_string<CharT, Traits, Alloc> res(total, CharT(0));
    CharT *p = &res[0], *cur = p + len, *q = p + total;
    for (std::copy(s.begin(), s.end(), p); cur != q;) {
        std::size_t s1 = std::distance(p, cur), s2 = std::distance(cur, q);
        if (s1 < s2) {
            std::copy(p, p + s1, cur);
            cur += s1;
        } else {
            std::copy(p, p + s2, cur);
            break;
        }
    }
    return res;
}

template<class CharT, class Traits, class Alloc>
std::basic_string<CharT, Traits, Alloc>
operator*=(std::basic_string<CharT, Traits, Alloc> &s, std::size_t n) {
    std::size_t len = s.length();
    if (n <= 1 || !len) {
        if (n == 0) s.clear();
        return s;
    }
    if (len == 1) {
        s.resize(n, s[0]);
        return s;
    }
    std::size_t total = len * n;
    if (total / n != len) total = std::size_t(-1);
    s.resize(total);
    for (CharT *p = &s[0], *cur = p + len, *q = p + total;;) {
        std::size_t s1 = std::distance(p, cur), s2 = std::distance(cur, q);
        if (s1 < s2) {
            std::copy(p, p + s1, cur);
            cur += s1;
        } else {
            std::copy(p, p + s2, cur);
            break;
        }
    }
    return s;
}

#if __cplusplus >= 201103L

template<class CharT, class Traits, class Alloc>
std::basic_string<CharT, Traits, Alloc>
operator*(std::basic_string<CharT, Traits, Alloc> &&s, std::size_t n) {
    return std::move(s *= n);
}

#endif
