#pragma once

#include <iostream>
#include <queue>
#include <sstream>

struct TreeNode {
    int val;
    TreeNode *left{}, *right{};

    TreeNode() : val(0) {}

    explicit TreeNode(int x) : val(x) {}

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &out, const TreeNode *root) {
    std::streamsize width = out.width(0);
    if (width) {
        std::stringstream ss;
        ss.copyfmt(out);
        ss << root;
        out.width(width);
        return out << ss.str();
    }
    typename std::basic_ostream<CharT, Traits>::sentry sentry(out);
    if (!sentry) return out;
    if (!root) return out << "[]";
    char ch = '[';
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
    return out << ']';
}

template<class CharT, class Traits>
inline std::basic_istream<CharT, Traits> &operator>>(std::basic_istream<CharT, Traits> &is, TreeNode *&root) {
    typename std::basic_istream<CharT, Traits>::sentry sentry(is);
    if (!sentry) return is;

    class save_flags {
        std::basic_ios<CharT, Traits> &stream;
        typename std::basic_ios<CharT, Traits>::fmtflags flags;
    public:
        explicit save_flags(std::basic_ios<CharT, Traits> &stream) : stream(stream), flags(stream.flags()) {}

        ~save_flags() { stream.flags(flags); }
    };

    save_flags saveFlags(is);
    std::skipws(is);

    std::ios_base::iostate state = std::ios_base::goodbit;

    TreeNode *old = root;
    root = nullptr;

    CharT ch;
    is >> ch;
    if (is.fail()) return is;
    if (!Traits::eq(ch, '[')) {
        is.unget();
        is.setstate(state |= std::ios_base::failbit);
        return is;
    }

    std::deque<TreeNode **> q;

    size_t offset = 0;
    for (q.push_back(&root);;) {
        int x;
        if (is >> x) {
            if (offset == q.size()) {
                goto fail;
            }
            TreeNode **p = q[offset++];
            auto new_node = new TreeNode(x);
            *p = new_node;
            q.push_back(&new_node->left);
            q.push_back(&new_node->right);
        } else {
            if (!(is.rdstate() & std::ios_base::failbit)) {
                goto fail;;
            }
            is.clear(~std::ios_base::goodbit);
            is.clear(std::ios_base::goodbit);

            if (!(is >> ch)) {
                goto fail;
            }
            if (Traits::eq(ch, ']')) {
                goto finish;
            }
            if (!Traits::eq(ch, 'n')) {
                goto fail;
            }
            std::noskipws(is);
            CharT expected[3] = {'u', 'l', 'l'};
            for (CharT chart: expected) {
                if (!(is >> ch) || !Traits::eq(ch, chart)) {
                    goto fail;
                }
            }
            if (q.empty()) {
                goto fail;
            }
            ++offset;
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
    goto finish;
fail:
    for (q.resize(offset); !q.empty(); q.pop_back()) delete *q.back();
    root = old;
    state |= std::ios_base::failbit;
finish:
    is.setstate(state);
    return is;
}
