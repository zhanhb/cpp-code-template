#include <cstdio>
#include <vector>
#include <string>

using namespace std;

char str[256];

bool cmp(const string &a, const string &b) {
    return a + b > b + a;
}

bool is_zero(const string &s) {
    return s == "0";
}

int main() {
    vector<string> v;
    while (1 == scanf("%s", str)) {
        v.emplace_back(str);
    }
    if (all_of(v.begin(), v.end(), is_zero)) {
        puts("0");
    } else {
        sort(v.begin(), v.end(), cmp);
        for (auto &&s: v) {
            printf("%s", s.c_str());
        }
        puts("");
    }
}
