#include <algorithm>
#include <cassert>
#include <cctype>
#include <queue>
#include <string>

using namespace std;

struct PamNode {
    int cnt{}, len, dep, dif{};
    PamNode *ch[26]{};
    PamNode *fail, *slink{};

    PamNode(int len, PamNode *fail, int dep = 0) : len(len), dep(dep), fail(fail) {}
};

class Pam {
    deque<PamNode> container{};
    string s{};
    PamNode *root, *last;

    template<class ...Args>
    PamNode *node(Args &&...args) {
        return &container.emplace_back(std::forward<Args>(args)...);
    }

    PamNode *getfail(PamNode *x) { // NOLINT(readability-convert-member-functions-to-static)
        int tot = (int) s.length() - 1;
        while (s[tot - x->len - 1] != s[tot]) x = x->fail;
        return x;
    }

public:
    Pam() : s("$"), root(node(0, nullptr)), last(nullptr) {
        root->fail = node(-1, root);
    }

    [[maybe_unused]]
    void insert(char ch) { // NOLINT(misc-no-recursion)
        assert(islower(ch)); // NOLINT(cppcoreguidelines-narrowing-conversions)
        int x = ch - 'a';
        s += ch;
        PamNode *now = getfail(last);
        auto &q = now->ch[x];
        if (!q) {
            auto fail = getfail(now->fail)->ch[x];
            q = node(now->len + 2, fail, fail->dep + 1);
            q->dif = q->len = fail->len;

            if (q->dif == q->fail->dif) {
                q->slink = q->fail->slink;
            } else {
                q->slink = q->fail;
            }
        }
        ++q->cnt;
        last = q;
    }

    [[nodiscard]]
    [[maybe_unused]]
    int64_t solve() const { // NOLINT(readability-convert-member-functions-to-static)
        for (auto p = container.rbegin(), e = container.rend(); p != e; ++p) {
            p->fail->cnt += p->cnt;
        }
        int64_t ans = 0;
        for (int i = 1, size = (int) container.size(); i < size; ++i) {
            auto &&e = container[i];
            ans = max(ans, int64_t(e.len) * e.cnt);
        }
        return ans;
    }
};
