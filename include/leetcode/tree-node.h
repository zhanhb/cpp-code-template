#pragma once

#include <iostream>
#include <queue>

struct TreeNode {
    int val;
    TreeNode *left, *right;

    TreeNode() : val(0), left(), right() {}

    explicit TreeNode(int x) : val(x), left(), right() {}

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &out, const TreeNode *root) {
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
