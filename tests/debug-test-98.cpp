#include "debug.h"
#include <bits/stdc++.h>

using namespace std;

#if __cplusplus >= 201103L
#undef NULL
#define NULL nullptr
#endif

int main() {
    out(setlocale(LC_CTYPE, NULL)); // NOLINT(modernize-use-nullptr)
    setlocale(LC_CTYPE, "");
    out(setlocale(LC_CTYPE, NULL)); // NOLINT(modernize-use-nullptr)
    out(queue<int>(), priority_queue<int>(), stack<int>());

    out(vector<vector<int> >(), vector<map<int, int> >());

    string str = "\u4f60\u597d";
    wstring ws = L"\u4f60\u597d";

    wclog.imbue(locale(""));
    out(clog, str);
    out(wclog, str);
    out(clog, ws);
    out(wclog, ws);
}
