#include "leetcode/tree-node.h"
#include <sstream>
#include "verify.h"
#include <cassert>

using namespace std;

void test1() {
    string test_cases[] = {
            "[1,3,4,2,null,6,5,null,null,null,null,null,7]",
            "[5,8,9,2,1,3,7,4,6]",
            "[]",
    };
    Tree<int> tree;
    const std::size_t size = sizeof(test_cases) / sizeof test_cases[0];
    for (std::size_t i = 0; i != size; ++i) { // NOLINT(modernize-loop-convert)
        const string &buf = test_cases[i];
        istringstream ss(buf);
        ss >> tree;
        assert(ss.good());
        ostringstream os;
        os << tree;
        verify(buf, os.str());
    }
    {
        string buf;
        for (std::size_t i = 0; i != size; ++i) { // NOLINT(modernize-loop-convert)
            buf += test_cases[i];
        }
        istringstream ss(buf);
        for (int i = 0; ss >> tree; ++i) {
            ostringstream os;
            os << tree;
            verify(test_cases[i], os.str());
        }
    }
}

void test2() {
    Tree<int> tree;
    const char *test_cases[] = {
            "[1,2,n ull]",
            "[1,2,nu ll]",
            "[1,2,nul l]",
    };
    const std::size_t size = sizeof(test_cases) / sizeof test_cases[0];
    for (std::size_t i = 0; i != size; ++i) { // NOLINT(modernize-loop-convert)
        istringstream ss(test_cases[i]);
        verify(false, bool(ss >> tree));
        verify(reinterpret_cast<TreeNode *>(0), tree);
    }
}

void test3() {
    istringstream ss(" [ 1 , 3 , 4 , 2 , null, 6, 5, null, null, null , null, null, 7]");
    Tree<int> tree;
    ss >> tree;
    OUT(tree);
}

void test_wide() {
    wstring test_cases[] = {
            L"[1,3,4,2,null,6,5,null,null,null,null,null,7]",
            L"[5,8,9,2,1,3,7,4,6]",
            L"[]",
    };
    Tree<int> tree;
    const std::size_t size = sizeof(test_cases) / sizeof test_cases[0];
    for (std::size_t i = 0; i != size; ++i) { // NOLINT(modernize-loop-convert)
        const wstring &buf = test_cases[i];
        wistringstream ss(buf);
        ss >> tree;
        assert(ss.good());
        wostringstream os;
        os << tree;
        verify(buf, os.str());
    }
    {
        wstring buf;
        for (std::size_t i = 0; i != size; ++i) { // NOLINT(modernize-loop-convert)
            buf += test_cases[i];
        }
        wistringstream ss(buf);
        for (int i = 0; ss >> tree; ++i) {
            wostringstream os;
            os << tree;
            verify(test_cases[i], os.str());
        }
    }
}

int main() {
    test1();
    test2();
    test3();
    test_wide();
}
