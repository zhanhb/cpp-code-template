#pragma once

#include <iostream>
#include <queue>
#include <sstream>

struct TreeNode {
    int val;
    TreeNode *left, *right;

    TreeNode() : val(0), left(), right() {}

    explicit TreeNode(int x) : val(x), left(), right() {}

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &out, const TreeNode *root) {
    std::streamsize width = out.width(0);
    if (width) {
        std::basic_ostringstream<CharT, Traits> ss;
        ss.copyfmt(out);
        ss << root;
        out.width(width);
        return out << ss.str();
    }
    typename std::basic_ostream<CharT, Traits>::sentry sentry(out);
    if (!sentry) return out;
    if (!root) return out << "[]";
    CharT ch = '[';
    std::queue<const TreeNode *> q;
    int cnt = 0;
    for (q.push(root); !q.empty(); q.pop()) {
        const TreeNode *p = q.front();
        if (!p) {
            ++cnt;
            continue;
        }
        for (; cnt > 0; --cnt) out << ch << "null", ch = ',';
        out << ch << p->val, ch = ',';
        q.push(p->left);
        q.push(p->right);
    }
    return out << "]";
}

template<class CharT, class Traits>
inline std::basic_istream<CharT, Traits> &operator>>(std::basic_istream<CharT, Traits> &is, TreeNode *&root) {
    typename std::basic_istream<CharT, Traits>::sentry sentry(is);
    if (!sentry) return is;

    class save_flags {
        std::ios_base &stream;
        std::ios_base::fmtflags flags;
    public:

        explicit save_flags(std::ios_base &stream, std::ios_base::fmtflags flags) :
                stream(stream), flags(stream.flags(flags)) {}

        ~save_flags() { stream.flags(flags); }
    };

    save_flags saveFlags(is, std::ios_base::dec | std::ios_base::skipws);

    std::ios_base::iostate state = std::ios_base::goodbit;

    CharT ch;
    is >> ch;
    if (is.fail()) return is;
    if (!Traits::eq(ch, '[')) {
        is.unget();
        is.setstate(state | std::ios_base::failbit);
        return is;
    }

    TreeNode *result;
    std::deque<TreeNode **> q;

    std::size_t offset = 0;
    for (q.push_back(&result);;) {
        int x;
        if (is >> x) {
            if (offset == q.size()) {
                goto fail;
            }
            TreeNode *new_node = new TreeNode(x);
            TreeNode **p = q[offset++];
            *p = new_node;
            q.push_back(&new_node->left);
            q.push_back(&new_node->right);
        } else {
            is.clear();

            if (!(is >> ch)) {
                goto fail;
            }
            if (Traits::eq(ch, ']')) {
                break;
            }
            if (!Traits::eq(ch, 'n')) {
                goto fail;
            }
            std::noskipws(is);
            CharT expected[3] = {'u', 'l', 'l'};
            for (int i = 0; i < 3; ++i) { // NOLINT(modernize-loop-convert)
                if (!Traits::eq_int_type(is.get(), expected[i])) {
                    goto fail;
                }
            }
            if (offset == q.size()) {
                goto fail;
            }
#if __cplusplus >= 201103L || defined(nullptr)
            *q[offset++] = nullptr;
#elif defined(NULL)
            *q[offset++] = NULL;
#else
            *q[offset++] = 0;
#endif
            std::skipws(is);
        }
        if (!(is >> ch)) goto fail;
        if (Traits::eq(ch, ']')) {
            break;
        }
        if (!Traits::eq(ch, ',')) {
            goto fail;
        }
    }
    for (std::size_t total = q.size(); offset != total; ++offset) {
#if __cplusplus >= 201103L || defined(nullptr)
        *q[offset] = nullptr;
#elif defined(NULL)
        *q[offset] = NULL;
#else
        *q[offset] = 0;
#endif
    }
    root = result;
    goto finish;
fail:
    while (offset) delete *q[--offset];
    state |= std::ios_base::failbit;
finish:
    is.setstate(state);
    return is;
}
