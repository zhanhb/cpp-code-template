#pragma once

#include <iostream>
#include <queue>
#include <sstream>

template<class Tp>
struct Tree {
    struct Node {
        Tp val;
        Node *left, *right;

        Node() : val(), left(), right() {}

        explicit Node(Tp val) : val(val), left(), right() {}

        Node(Tp val, Node *left, Node *right) : val(val), left(left), right(right) {}
    };

    template<class CharT, class Traits>
    friend inline std::basic_ostream<CharT, Traits> &
    operator<<(std::basic_ostream<CharT, Traits> &out, const Node *node) {
        std::streamsize width = out.width(0);
        if (width) {
            std::basic_ostringstream<CharT, Traits> ss;
            ss.copyfmt(out);
            ss << node;
            out.width(width);
            return out << ss.str();
        }
        typename std::basic_ostream<CharT, Traits>::sentry sentry(out);
        if (!sentry) return out;
        if (!node) return out << "[]";
        CharT ch = '[';
        std::queue<const Node *> q;
        int cnt = 0;
        for (q.push(node); !q.empty(); q.pop()) {
            const Node *p = q.front();
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

    operator const Node *() const { return root; } // NOLINT(google-explicit-constructor)

    operator Node *() { return root; } // NOLINT(google-explicit-constructor)

private:
    std::deque<Node> container;
    Node *root;

    template<class CharT, class Traits>
    friend inline std::basic_istream<CharT, Traits> &
    operator>>(std::basic_istream<CharT, Traits> &is, Tree &tree) {
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

        Node *result;
        std::deque<Node **> q;

        std::deque<Node> &dq = tree.container;
        dq.clear();
        tree.root = reinterpret_cast<Node *>(0);

        std::size_t offset = 0;
        for (q.push_back(&result);;) {
            int x;
            if (is >> x) {
                if (offset == q.size()) {
                    goto fail;
                }
#if __cplusplus >= 201703L
                Node *new_node = &dq.emplace_back(x);
#else
#if __cplusplus >= 201103L
                dq.emplace_back(x);
#else
                dq.push_back(Node(x));
#endif
                Node *new_node = &dq.back();
#endif
                Node **p = q[offset++];
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
                *q[offset++] = reinterpret_cast<Node *>(0);
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
        for (std::size_t total = q.size(); offset != total; ++offset) *q[offset] = reinterpret_cast<Node *>(0);
        tree.root = result;
        goto finish;
// @formatter:off
fail:
// @formatter:on
        for (; offset; --offset) dq.pop_front();
        state |= std::ios_base::failbit;
// @formatter:off
finish:
// @formatter:on
        is.setstate(state);
        return is;
    }
};

typedef Tree<int>::Node TreeNode;
