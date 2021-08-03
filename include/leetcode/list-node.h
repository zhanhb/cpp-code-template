#pragma once

#include <initializer_list>
#include <iostream>

struct ListNode {
    int val;
    ListNode *next{};

    ListNode() : val(0) {}

    explicit ListNode(int x) : val(x) {}

    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &out, const ListNode *root) {
    if (!root) return out << "[]";
    out << '[' << root->val;
    while ((root = root->next)) out << ',' << root->val;
    return out << ']';
}

ListNode *newListNode(const std::initializer_list<int> &list) {
    ListNode *res = nullptr;
    for (auto p = list.end(), b = list.begin(); p != b;) res = new ListNode(*--p, res);
    return res;
}
