#pragma once

#if __cplusplus >= 201103L

#include <deque>
#include <iostream>
#include "common/iterator.h"

namespace leetcode {
    template<class Tp>
    struct List {
        struct Node {
            Tp val;
            Node *next;

            Node() : val(), next() {}

            explicit Node(Tp val) : val(val), next() {}

            Node(Tp val, Node *next) : val(val), next(next) {}
        };

        template<class CharT, class Traits>
        friend inline std::basic_ostream<CharT, Traits> &
        operator<<(std::basic_ostream<CharT, Traits> &out, const Node *node) {
            if (!node) return out << "[]";
            out << "[" << node->val;
            while ((node = node->next)) out << "," << node->val;
            return out << "]";
        }

    private:
        std::deque<Node> container{};
    public:

        operator const Node *() const { // NOLINT(google-explicit-constructor)
            return container.empty() ? nullptr : &container.front();
        }

        operator Node *() { // NOLINT(google-explicit-constructor)
            return container.empty() ? nullptr : &container.front();
        }

        template<TYPE_CONCEPT(It, extension::forward_iterator)>
        CONCEPT_IF_1(extension::forward_iterator, It, void)
        assign(It first, It last) {
            container.clear();
            Node *ret = nullptr;
            Node **u = &ret;
            for (; first != last; ++first) {
#if __cplusplus >= 201703L
                Node *p = &container.emplace_back(*first);
#else
                container.emplace_back(*first);
                Node *p = &container.back();
#endif
                *u = p, u = &p->next;
            }
        }

        List() = default;

        List(std::initializer_list<Tp> list) {
            assign(list.begin(), list.end());
        }

        List &operator=(std::initializer_list<Tp> list) &{
            assign(list.begin(), list.end());
            return *this;
        }
    };
}

typedef leetcode::List<int>::Node ListNode;

#endif
