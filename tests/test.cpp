#include <climits>
#include <cstdio>

int main() {
    for (int i = INT_MAX - 10; i != INT_MIN + 10; ++i) {
        printf("%d\n", i);
    }
}
