#include "debug.h"
#include <bits/stdc++.h>
#include "string-repeat.h"

using namespace std;

int main() {
    string expando = string("stable-sort-expando") * 32;

    string expect = string(count_if(expando.begin(), expando.end(), [](char ch) { return ch == 'o'; }), 'o');
    regex_replace(back_inserter(expect), expando.begin(), expando.end(), regex("o"), "");
    sort(expando.begin(), expando.end(), [](char x, char y) { return (x != 'o') < (y != 'o'); });

    out(expando == expect ? "stable" : "unstable");
}
