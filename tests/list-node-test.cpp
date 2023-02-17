#include "leetcode/list-node.h"
#include "debug.h"

int main() {
    leetcode::List<int> list{2, 3};
    OUT(list);
    list = {1, 2, 3};
    OUT(list);
}
