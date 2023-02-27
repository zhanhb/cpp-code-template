#include <cstdio>
#include <cstring>
#include <utility>

using namespace std;

char str[1050], res[1050];
int cnt[128];

void swap_at_most_once() {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) ++cnt[str[i]];
    int j = 0;
    for (int i = 1; i < 128; ++i) {
        int c = cnt[i];
        while (c) {
            if (i == str[j]) {
                --c;
                ++j;
                continue;
            }
            for (int k = len - 1; k >= j; --k) {
                if (str[k] == i) {
                    swap(str[k], str[j]);
                    break;
                }
            }
            return;
        }
    }
}

int main() {
    scanf("%s", str);
    swap_at_most_once();
    puts(str);
}
