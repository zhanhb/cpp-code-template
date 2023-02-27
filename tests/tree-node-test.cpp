#include "leetcode/tree-node.h"
#include <sstream>
#include "validate.h"

using namespace std;

int main() {
    string test_cases[] = {
            "[1,3,4,2,null,6,5,null,null,null,null,null,7]",
            "[5,8,9,2,1,3,7,4,6]",
            "[]",
    };
    for (auto const &buf: test_cases) {
        istringstream ss(buf);
        TreeNode *p;
        ss >> p;
        assert(ss.good());
        ostringstream os;
        os << p;
        os.str();
        validate(buf, os.str());
    }
    {
        string buf;
        for (auto const &s: test_cases) buf += s;
        istringstream ss(buf);
        TreeNode *p;
        for (int i = 0; ss >> p; ++i) {
            ostringstream os;
            os << p;
            validate(test_cases[i], os.str());
        }
    }
    istringstream ss(" [ 1 , 3 , 4 , 2 , null, 6, 5, null, null, null , null, null, 7]");
    TreeNode *p;
    ss >> p;
    out(p);
}
