#include <bits/stdc++.h>

// <editor-fold defaultstate="collapsed" desc="#define out(...)">
#ifndef out
#define out(...)
#endif
// </editor-fold>

using namespace std;

// <editor-fold defaultstate="collapsed" desc="unknown">

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="85">
// TODO O(N^2)
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="2060">
//typedef variant<string, vector<int> > E;
//
//vector<E> parse(const string &s) {
//    vector<E> v;
//    for (int i = 0, len = s.length(); i < len;) {
//        int j = i;
//        while (j < len && !isdigit(s[j])) ++j;
//        if (i != j) v.emplace_back(string(s, i, j - i));
//        i = j;
//        while (j < len && isdigit(s[j])) ++j;
//        if (i != j) {
//            vector<int> lst;
//            assert(j - i <= 3);
//            if (j - i == 1) {
//                lst = {s[i] - '0'};
//            } else if (j - i == 2) {
//                int a = s[i] - '0', b = s[i + 1] - '0';
//                lst = {a + b, a * 10 + b};
//            } else if (j - i == 3) {
//                int a = s[i] - '0', b = s[i + 1] - '0', c = s[i + 2] - '0';
//                lst = {
//                        a + b + c,
//                        a + b * 10 + c,
//                        a * 10 + b + c,
//                        (a * 10 + b) * 10 + c
//                };
//                sort(lst.begin(), lst.end());
//                lst.erase(unique(lst.begin(), lst.end()), lst.end());
//            }
//            v.emplace_back(std::move(lst));
//        }
//        i = j;
//    }
//    return v;
//}
//
//void dfs(int cur, int sum, const vector<E> &v, vector<int> &buf, map<int, list<vector<int> > > &res) {
//    if (cur == v.size()) return res[sum].push_back(buf);
//    auto &&item = v[cur];
//    if (!item.index()) {
//        auto x = get<0>(item).length();
//        buf.emplace_back(x);
//        dfs(cur + 1, sum + x, v, buf, res);
//        buf.pop_back();
//        return;
//    }
//    for (int x: get<1>(item)) {
//        buf.emplace_back(x);
//        dfs(cur + 1, sum + x, v, buf, res);
//        buf.pop_back();
//    }
//}
//
//vector<vector<int> > to_dp(const vector<E> &vv) {
//    size_t n = vv.size();
//    vector<vector<int> > dp(n + 1);
//    dp[n] = {0};
//    for (int i = n - 1; i >= 0; --i) {
//        auto &&cur = dp[i];
//        auto &&nxt = dp[i + 1];
//        auto &&val = vv[i];
//        if (!val.index()) {
//            auto &&str = get<0>(val);
//            auto len = str.length();
//            for (int x: nxt) cur.push_back(x + len);
//        } else {
//            list<int> lst;
//            for (int x: nxt) {
//                for (int y: get<1>(val)) {
//                    lst.push_back(x + y);
//                }
//            }
//            lst.sort();
//            lst.unique();
//            cur = typename std::remove_reference<decltype(cur)>::type(lst.begin(), lst.end());
//        }
//    }
//    return dp;
//}
//
//template<class ...Ts>
//struct overloaded : Ts ... {
//    using Ts::operator()...;
//};
//template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
//
//bool matches(
//        int len,
//        const vector<E> &a, int indexA, int aOff,
//        const vector<E> &b, int indexB, int bOff,
//        const vector<vector<int> > &dpa,
//        const vector<vector<int> > &dpb) {
//    assert(aOff >= 0);
//    assert(bOff >= 0);
//    if (indexA == a.size() || indexB == b.size()) {
//        assert(!len);
//        return true;
//    }
//    auto t = visit(overloaded{[&](const string &as) -> int {
//        if (aOff >= as.length()) {
//            return matches(len, a, indexA + 1, aOff - as.length(), b, indexB, bOff, dpa, dpb);
//        }
//        return -1;
//    }, [](const vector<int> &) { return -1; }
//    }, a[indexA]);
//    if (t >= 0) return t;
//
//    t = visit(overloaded{[&](const string &bs) -> int {
//        if (bOff >= bs.length()) {
//            return matches(len, a, indexA, aOff, b, indexB + 1, bOff - bs.length(), dpa, dpb);
//        }
//        return -1;
//    }, [&](const vector<int> &bv) -> int {
//        if (bOff) {
//            for (int x: bv) {
//                int bRem = x - bOff;
//                auto &&arr = dpb[indexB + 1];
//                if (bRem <= 0 && binary_search(arr.begin(), arr.end(), len - bRem)) {
//                    if (matches(len, a, indexA, aOff, b, indexB + 1, bOff - x, dpa, dpb)) return true;
//                }
//            }
//        }
//        return -1;
//    }
//    }, b[indexB]);
//
//
//    assert(!aOff || !bOff);
//    return visit(overloaded{
//            [&](const string &as, const string &bs) {
//                int aRem = (int) as.length() - aOff, bRem = (int) bs.length() - bOff;
//                int cnt = min(aRem, bRem);
//                assert(cnt >= 0);
//                for (int k = 0; k < cnt; ++k) {
//                    if (as[aOff + k] != bs[bOff + k]) {
//                        return false;
//                    }
//                }
//                return matches(len - cnt, a, indexA, aOff + cnt, b, indexB, bOff + cnt, dpa, dpb);
//            }, [&](const vector<int> &, const string &bs) {
//                return matches(len, b, indexB, bOff, a, indexA, aOff, dpb, dpa);
//            }, [&](const string &as, const vector<int> &bv) {
//                int aRem = (int) as.length() - aOff;
//                auto &&arr = dpb[indexB + 1];
//                for (int x: bv) {
//                    int bRem = x - bOff;
////            out(len - bRem, arr, binary_search(arr.begin(), arr.end(), len - bRem));
//                    if (bRem > 0 && binary_search(arr.begin(), arr.end(), len - bRem)) {
//                        if (bRem >= aRem) {
//                            if (matches(len - bRem, a, indexA + 1, bRem - aRem, b, indexB + 1, 0, dpa, dpb)) {
//                                return true;
//                            }
//                        } else {
//                            if (matches(len - bRem, a, indexA, aOff + bRem, b, indexB + 1, 0, dpa, dpb)) {
//                                return true;
//                            }
//                        }
//                    }
//                }
//                return false;
//            }, [&](const vector<int> &av, const vector<int> &bv) {
//                assert(!aOff || !bOff);
//                auto &&aArr = dpa[indexA + 1], &&bArr = dpb[indexB + 1];
//                for (int x: av) {
//                    int aRem = x - aOff;
//                    if (binary_search(aArr.begin(), aArr.end(), len - aRem)) {
//                        for (int y: bv) {
//                            int bRem = y - bOff;
//                            if (binary_search(bArr.begin(), bArr.end(), len - bRem)) {
//                                int cnt = max(aRem, bRem);
//                                if (aRem < bRem) {
//                                    if (matches(len - cnt, a, indexA + 1, bRem - aRem, b, indexB + 1, 0, dpa, dpb)) {
//                                        return true;
//                                    }
//                                } else {
//                                    if (matches(len - cnt, a, indexA + 1, 0, b, indexB + 1, aRem - bRem, dpa, dpb)) {
//                                        return true;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//                return false;
//            },
//    }, a[indexA], b[indexB]);
//}
//
//class Solution {
//public:
//    static bool possiblyEquals(const string &s1, const string &s2) {
//        auto a = parse(s1), b = parse(s2);
//        out(a, b);
//        auto dpa = to_dp(a), dpb = to_dp(b);
//        out(dpa[0]);
//        out(dpb[0]);
//        out(dpa, dpb);
//        auto &&dpa0 = dpa[0];
//        auto &&dpb0 = dpb[0];
//        for (int i = 0, j = 0; i < dpa0.size() && j < dpb0.size();) {
//            int expect = dpa0[i];
//            int cmp = expect - dpb0[j];
//            if (cmp < 0) ++i;
//            else if (cmp > 0) ++j;
//            else {
//                if (matches(expect, a, 0, 0, b, 0, 0, dpa, dpb)) return true;
//                ++i, ++j;
//            }
//        }
//        return false;
//    }
//};
//    validate(true, s.possiblyEquals("internationalization", "i18n"));
//    validate(true, s.possiblyEquals("l123e", "44"));
//    validate(false, s.possiblyEquals("a5b", "c5b"));
//    validate(true, s.possiblyEquals("a7a242a", "a2a14"));
//    validate(true, s.possiblyEquals("a312b4a77", "a428a1a8a"));
//
//    validate(false, s.possiblyEquals(
//            "v939v587v615v543v191v491v736v691v838v17",
//            "648v117v725v231v933v689v329v158v617v6"));
//
//    validate(false, s.possiblyEquals("98u8v8v8v89u888u998v88u98v88u9v99u989v8u",
//                                     "9v898u98v888v89v998u98v9v888u9v899v998u9"));
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="2172">
//// Additional library code
///**
// * Author: Benjamin Qi, chilli
// * Date: 2020-04-04
// * License: CC0
// * Source: https://github.com/bqi343/USACO/blob/master/Implementations/content/graphs%20(12)/Matching/Hungarian.h
// * Description: Given a weighted bipartite graph, matches every node on
// * the left with a node on the right such that no
// * nodes are in two matchings and the sum of the edge weights is minimal. Takes
// * cost[N][M], where cost[i][j] = cost for L[i] to be matched with R[j] and
// * returns (min cost, match), where L[i] is matched with
// * R[match[i]]. Negate costs for max cost. Requires $N \le M$.
// * Time: O(N^2M)
// * Status: Tested on kattis:cordonbleu, stress-tested
// */
//pair<int, vector<int> > hungarian(const vector<vector<int> > &mat) {
//    if (mat.empty()) return {0, {}};
//    int n = size(mat) + 1, m = size(mat[0]) + 1;
//    vector<int> u(n), v(m), p(m), ans(n - 1);
//    for (int i = 1; i < n; i++) {
//        p[0] = i;
//        int j0 = 0; // add "dummy" worker 0
//        vector<int> dist(m, INT_MAX), pre(m, -1);
//        vector<bool> done(m + 1);
//        do { // dijkstra
//            done[j0] = true;
//            int i0 = p[j0], j1, delta = INT_MAX;
//            for (int j = 1; j < m; j++) {
//                if (!done[j]) {
//                    auto cur = mat[i0 - 1][j - 1] - u[i0] - v[j];
//                    if (cur < dist[j]) dist[j] = cur, pre[j] = j0;
//                    if (dist[j] < delta) delta = dist[j], j1 = j;
//                }
//            }
//            for (int j = 0; j < m; j++) {
//                if (done[j]) u[p[j]] += delta, v[j] -= delta;
//                else dist[j] -= delta;
//            }
//            j0 = j1;
//        } while (p[j0]);
//        while (j0) { // update alternating path
//            int j1 = pre[j0];
//            p[j0] = p[j1], j0 = j1;
//        }
//    }
//    for (int j = 1; j < m; j++) {
//        if (p[j]) {
//            ans[p[j] - 1] = j - 1;
//        }
//    }
//    return {-v[0], ans}; // min cost
//}
//
//class Solution {
//public:
//    static int maximumANDSum(const vector<int> &a, int s) {
//        int n = a.size();
//        vector<vector<int>> mat(n);
//        for (int i = 1; i <= s; ++i) {
//            for (int j = 0; j < n; j++) {
//                mat[j].push_back(-(a[j] & i));
//                mat[j].push_back(-(a[j] & i));
//            }
//        }
//        return -hungarian(mat).first;
//    }
//};
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="2338">
// TODO optimize O(M*sqrt(M)) => O(M log(M))
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="2493">
//// <editor-fold defaultstate="collapsed" desc="measure([&] { validate(5, s.magnificentSets(38, {...})); });">
//    measure([&] {
//        validate(5, s.magnificentSets(38, {
//                {6,  19},
//                {2,  13},
//                {12, 3},
//                {21, 11},
//                {12, 22},
//                {21, 19},
//                {8,  9},
//                {16, 31},
//                {1,  3},
//                {23, 9},
//                {29, 36},
//                {2,  26},
//                {6,  31},
//                {28, 25},
//                {17, 22},
//                {12, 25},
//                {28, 26},
//                {6,  22},
//                {29, 37},
//                {20, 10},
//                {6,  10},
//                {8,  13},
//                {28, 36},
//                {32, 19},
//                {20, 19},
//                {35, 9},
//                {14, 11},
//                {1,  18},
//                {21, 22},
//                {2,  15},
//                {35, 15},
//                {21, 30},
//                {4,  25},
//                {33, 36},
//                {32, 11},
//                {7,  18},
//                {27, 9},
//                {24, 18},
//                {14, 25},
//                {29, 10},
//                {33, 18},
//                {14, 13},
//                {14, 10},
//                {1,  36},
//                {16, 3},
//                {4,  13},
//                {29, 26},
//                {20, 30},
//                {6,  30},
//                {2,  31},
//                {1,  9},
//                {6,  11},
//                {24, 11},
//                {17, 19},
//                {21, 9},
//                {28, 31},
//                {35, 22},
//                {2,  9},
//                {32, 36},
//                {33, 3},
//                {12, 9},
//                {1,  22},
//                {4,  10},
//                {29, 18},
//                {28, 3},
//                {1,  30},
//                {35, 19},
//                {5,  36},
//                {8,  18},
//                {1,  13},
//                {33, 11},
//                {21, 3},
//                {6,  36},
//                {16, 22},
//                {7,  30},
//                {14, 31},
//                {33, 10},
//                {34, 36},
//                {27, 30},
//                {38, 10},
//                {5,  31},
//                {21, 15},
//                {28, 13},
//                {12, 37},
//                {34, 37},
//                {35, 30},
//                {29, 3},
//                {16, 9},
//                {23, 18},
//                {7,  26},
//                {14, 37},
//                {27, 11},
//                {4,  22},
//                {16, 10},
//                {1,  11},
//                {16, 18},
//                {35, 10},
//                {24, 10},
//                {35, 3},
//                {6,  15},
//                {17, 13},
//                {29, 15},
//                {8,  15},
//                {4,  37},
//                {7,  37},
//                {24, 15},
//                {32, 10},
//                {28, 10},
//                {34, 22},
//                {2,  11},
//                {20, 36},
//                {23, 26},
//                {32, 15},
//                {14, 9},
//                {2,  19},
//                {21, 13},
//                {38, 3},
//                {5,  10},
//                {16, 25},
//                {4,  3},
//                {34, 25},
//                {16, 30},
//                {1,  25},
//                {32, 22},
//                {24, 9},
//                {34, 10},
//                {38, 19},
//                {38, 37},
//                {12, 13},
//        }));
//    });
//// </editor-fold>
//// <editor-fold defaultstate="collapsed" desc="measure([&] { validate(5, s.magnificentSets(66, {...})); });">
//    measure([&] {
//        validate(5, s.magnificentSets(66, {
//                {16, 65},
//                {17, 37},
//                {40, 2},
//                {26, 47},
//                {13, 59},
//                {16, 2},
//                {31, 45},
//                {60, 10},
//                {4,  14},
//                {4,  24},
//                {5,  3},
//                {5,  37},
//                {44, 51},
//                {9,  36},
//                {16, 48},
//                {13, 65},
//                {13, 61},
//                {60, 58},
//                {9,  51},
//                {31, 61},
//                {1,  8},
//                {16, 46},
//                {1,  52},
//                {16, 42},
//                {40, 11},
//                {60, 32},
//                {4,  42},
//                {17, 23},
//                {13, 32},
//                {1,  6},
//                {1,  28},
//                {26, 3},
//                {19, 20},
//                {16, 10},
//                {4,  2},
//                {19, 22},
//                {7,  30},
//                {5,  8},
//                {7,  62},
//                {26, 36},
//                {50, 32},
//                {1,  38},
//                {44, 52},
//                {5,  33},
//                {44, 38},
//                {44, 11},
//                {34, 20},
//                {5,  54},
//                {16, 53},
//                {26, 39},
//                {26, 56},
//                {40, 39},
//                {9,  35},
//                {13, 14},
//                {17, 11},
//                {13, 36},
//                {4,  48},
//                {5,  51},
//                {26, 55},
//                {17, 6},
//                {7,  61},
//                {9,  48},
//                {31, 2},
//                {44, 29},
//                {7,  38},
//                {4,  20},
//                {60, 36},
//                {19, 62},
//                {60, 3},
//                {60, 65},
//                {9,  38},
//                {16, 39},
//                {4,  22},
//                {44, 36},
//                {16, 52},
//                {13, 3},
//                {4,  61},
//                {40, 33},
//                {17, 24},
//                {40, 27},
//                {5,  62},
//                {16, 45},
//                {13, 54},
//                {40, 46},
//                {19, 55},
//                {31, 55},
//                {60, 11},
//                {26, 51},
//                {17, 64},
//                {60, 29},
//                {7,  11},
//                {17, 33},
//                {34, 51},
//                {19, 21},
//                {34, 63},
//                {34, 32},
//                {19, 23},
//                {19, 32},
//                {31, 23},
//                {5,  29},
//                {34, 65},
//                {1,  47},
//                {13, 2},
//                {34, 61},
//                {17, 63},
//                {26, 29},
//                {4,  56},
//                {4,  15},
//                {60, 22},
//                {34, 57},
//                {16, 14},
//                {19, 14},
//                {9,  20},
//                {13, 53},
//                {40, 47},
//                {50, 45},
//                {44, 15},
//                {13, 46},
//                {19, 57},
//                {34, 37},
//                {44, 62},
//                {40, 36},
//                {16, 8},
//                {31, 21},
//                {34, 25},
//                {9,  10},
//                {9,  57},
//                {17, 46},
//                {31, 53},
//                {9,  32},
//                {1,  18},
//                {9,  58},
//                {19, 39},
//                {17, 38},
//                {7,  8},
//                {5,  63},
//                {16, 32},
//                {31, 41},
//                {4,  49},
//                {40, 8},
//                {44, 56},
//                {50, 14},
//                {5,  20},
//                {50, 36},
//                {4,  62},
//                {16, 62},
//                {50, 33},
//                {17, 25},
//                {34, 43},
//                {60, 27},
//                {26, 49},
//                {44, 61},
//                {26, 61},
//                {60, 49},
//                {16, 22},
//                {5,  61},
//                {31, 29},
//                {7,  24},
//                {31, 14},
//                {4,  51},
//                {5,  24},
//                {17, 30},
//                {7,  49},
//                {16, 15},
//                {16, 3},
//                {19, 8},
//                {16, 58},
//                {40, 35},
//                {7,  57},
//                {9,  54},
//                {5,  52},
//                {5,  58},
//                {60, 24},
//                {44, 24},
//                {7,  29},
//                {60, 12},
//                {4,  21},
//                {60, 30},
//                {44, 64},
//                {7,  3},
//                {19, 45},
//                {44, 33},
//                {26, 10},
//                {16, 59},
//                {4,  10},
//                {4,  33},
//                {17, 51},
//                {31, 64},
//                {16, 57},
//                {31, 35},
//                {5,  22},
//                {34, 14},
//                {50, 6},
//                {7,  15},
//                {9,  21},
//                {19, 58},
//                {40, 32},
//                {26, 52},
//                {5,  66},
//                {26, 48},
//                {40, 54},
//                {60, 66},
//                {4,  23},
//                {60, 15},
//                {34, 11},
//                {9,  47},
//                {7,  42},
//                {17, 52},
//                {60, 21},
//                {1,  3},
//                {16, 33},
//                {1,  29},
//                {50, 15},
//                {60, 59},
//                {13, 33},
//                {31, 62},
//                {44, 48},
//                {26, 20},
//                {9,  12},
//                {9,  22},
//                {31, 18},
//                {60, 28},
//                {26, 35},
//                {34, 6},
//                {5,  53},
//                {5,  30},
//                {16, 55},
//                {1,  33},
//                {1,  22},
//                {4,  11},
//                {7,  36},
//                {5,  32},
//                {9,  15},
//                {31, 38},
//                {1,  63},
//                {17, 61},
//                {50, 39},
//                {17, 35},
//                {5,  28},
//                {9,  6},
//                {13, 35},
//                {50, 11},
//                {17, 53},
//                {5,  59},
//                {40, 18},
//                {34, 41},
//                {9,  63},
//                {4,  58},
//                {44, 41},
//                {17, 48},
//                {5,  15},
//                {60, 43},
//                {13, 52},
//                {17, 8},
//                {9,  2},
//                {13, 29},
//                {26, 33},
//                {60, 63},
//                {1,  39},
//                {16, 12},
//                {31, 36},
//                {1,  58},
//                {7,  48},
//                {4,  38},
//                {40, 65},
//                {60, 47},
//                {16, 24},
//                {4,  46},
//                {31, 57},
//                {4,  47},
//                {4,  12},
//                {1,  42},
//                {1,  24},
//                {1,  25},
//                {5,  10},
//                {26, 12},
//                {19, 51},
//                {34, 53},
//                {31, 22},
//                {5,  35},
//                {31, 51},
//                {1,  20},
//                {26, 14},
//                {16, 35},
//                {44, 32},
//                {13, 62},
//                {13, 66},
//                {16, 11},
//                {60, 20},
//                {5,  6},
//                {5,  39},
//                {17, 10},
//                {44, 2},
//                {60, 25},
//                {17, 62},
//                {31, 25},
//                {31, 49},
//                {13, 48},
//                {34, 39},
//                {7,  59},
//                {7,  63},
//                {17, 59},
//                {31, 37},
//                {16, 41},
//                {34, 55},
//                {5,  47},
//                {31, 11},
//                {7,  25},
//                {7,  27},
//                {9,  3},
//                {7,  23},
//                {5,  43},
//                {1,  49},
//                {17, 55},
//                {4,  36},
//                {16, 38},
//                {34, 22},
//                {50, 38},
//                {44, 47},
//                {5,  14},
//                {17, 47},
//                {50, 30},
//                {26, 37},
//                {7,  65},
//                {7,  37},
//                {26, 21},
//                {9,  8},
//                {5,  57},
//                {60, 52},
//                {44, 63},
//                {5,  25},
//                {34, 48},
//                {34, 58},
//                {5,  11},
//                {60, 14},
//                {7,  58},
//                {5,  38},
//                {26, 41},
//                {1,  15},
//                {40, 37},
//                {50, 18},
//                {7,  6},
//                {34, 23},
//                {19, 46},
//                {7,  66},
//                {40, 52},
//                {13, 23},
//                {7,  10},
//                {50, 55},
//                {19, 64},
//                {44, 42},
//                {17, 43},
//                {13, 41},
//                {34, 21},
//        }));
//    });
//// </editor-fold>
//// <editor-fold defaultstate="collapsed" desc="measure([&] { validate(4, s.magnificentSets(26, {...})); });">
//    measure([&] {
//        validate(4, s.magnificentSets(26, {
//                {9,  16},
//                {8,  3},
//                {20, 21},
//                {12, 16},
//                {14, 3},
//                {7,  21},
//                {22, 3},
//                {22, 18},
//                {11, 16},
//                {25, 4},
//                {2,  4},
//                {14, 21},
//                {23, 3},
//                {17, 3},
//                {2,  16},
//                {24, 16},
//                {13, 4},
//                {10, 21},
//                {7,  4},
//                {9,  18},
//                {14, 18},
//                {14, 4},
//                {14, 16},
//                {1,  3},
//                {25, 18},
//                {17, 4},
//                {1,  16},
//                {23, 4},
//                {2,  21},
//                {5,  16},
//                {24, 18},
//                {20, 18},
//                {19, 16},
//                {24, 21},
//                {9,  3},
//                {24, 3},
//                {19, 18},
//                {25, 16},
//                {19, 21},
//                {6,  3},
//                {26, 18},
//                {5,  21},
//                {20, 16},
//                {2,  3},
//                {10, 18},
//                {26, 16},
//                {8,  4},
//                {11, 21},
//                {23, 16},
//                {13, 16},
//                {25, 3},
//                {7,  18},
//                {19, 3},
//                {20, 4},
//                {26, 3},
//                {23, 18},
//                {15, 18},
//                {17, 18},
//                {10, 16},
//                {26, 21},
//                {23, 21},
//                {7,  16},
//                {8,  18},
//                {10, 4},
//                {24, 4},
//                {7,  3},
//                {11, 18},
//                {9,  4},
//                {26, 4},
//                {13, 21},
//                {22, 16},
//                {22, 21},
//                {20, 3},
//                {6,  18},
//                {9,  21},
//                {10, 3},
//                {22, 4},
//                {1,  18},
//                {25, 21},
//                {11, 4},
//                {1,  21},
//                {15, 3},
//                {1,  4},
//                {15, 16},
//                {2,  18},
//                {13, 3},
//                {8,  21},
//                {13, 18},
//                {11, 3},
//                {15, 21},
//                {8,  16},
//                {17, 16},
//                {15, 4},
//                {12, 3},
//                {6,  4},
//                {17, 21},
//                {5,  18},
//                {6,  16},
//                {6,  21},
//                {12, 4},
//                {19, 4},
//                {5,  3},
//                {12, 21},
//                {5,  4},
//        }));
//    });
//// </editor-fold>
//// <editor-fold defaultstate="collapsed" desc="measure([&] { validate(393, s.magnificentSets(430, {...})); });">
//    measure([&] {
//        validate(393, s.magnificentSets(430, {
//                {316, 360},
//                {236, 101},
//                {236, 100},
//                {282, 102},
//                {193, 176},
//                {253, 60},
//                {372, 42},
//                {223, 214},
//                {62,  113},
//                {257, 374},
//                {74,  289},
//                {368, 405},
//                {200, 302},
//                {259, 212},
//                {281, 101},
//                {91,  167},
//                {109, 268},
//                {194, 121},
//                {93,  412},
//                {11,  139},
//                {308, 324},
//                {11,  409},
//                {366, 110},
//                {366, 390},
//                {236, 34},
//                {282, 240},
//                {379, 199},
//                {93,  394},
//                {87,  288},
//                {141, 28},
//                {76,  49},
//                {146, 204},
//                {172, 3},
//                {20,  140},
//                {59,  240},
//                {203, 374},
//                {26,  152},
//                {429, 49},
//                {59,  289},
//                {74,  312},
//                {387, 217},
//                {47,  4},
//                {427, 19},
//                {130, 408},
//                {242, 101},
//                {429, 247},
//                {76,  1},
//                {248, 134},
//                {35,  358},
//                {342, 337},
//                {347, 337},
//                {106, 303},
//                {276, 67},
//                {239, 355},
//                {129, 284},
//                {291, 386},
//                {263, 329},
//                {256, 152},
//                {321, 114},
//                {279, 394},
//                {118, 228},
//                {53,  44},
//                {237, 104},
//                {210, 155},
//                {344, 111},
//                {291, 245},
//                {6,   400},
//                {90,  114},
//                {137, 428},
//                {69,  251},
//                {256, 32},
//                {50,  362},
//                {401, 254},
//                {156, 214},
//                {37,  192},
//                {146, 8},
//                {194, 176},
//                {395, 140},
//                {208, 107},
//                {18,  103},
//                {366, 135},
//                {88,  317},
//                {90,  294},
//                {335, 331},
//                {326, 306},
//                {258, 155},
//                {210, 3},
//                {185, 411},
//                {175, 33},
//                {64,  167},
//                {2,   125},
//                {88,  371},
//                {413, 275},
//                {263, 312},
//                {397, 142},
//                {90,  134},
//                {93,  81},
//                {90,  139},
//                {263, 396},
//                {257, 273},
//                {338, 303},
//                {18,  169},
//                {195, 44},
//                {232, 382},
//                {36,  86},
//                {290, 128},
//                {359, 180},
//                {258, 280},
//                {50,  302},
//                {151, 430},
//                {381, 84},
//                {239, 81},
//                {313, 115},
//                {45,  331},
//                {185, 408},
//                {53,  270},
//                {363, 251},
//                {41,  404},
//                {315, 337},
//                {71,  168},
//                {210, 170},
//                {146, 3},
//                {322, 135},
//                {76,  383},
//                {41,  67},
//                {315, 399},
//                {424, 105},
//                {83,  417},
//                {88,  67},
//                {189, 399},
//                {203, 214},
//                {384, 336},
//                {147, 219},
//                {145, 298},
//                {290, 272},
//                {342, 170},
//                {185, 3},
//                {424, 65},
//                {198, 340},
//                {209, 332},
//                {357, 97},
//                {175, 103},
//                {147, 181},
//                {195, 131},
//                {22,  188},
//                {80,  350},
//                {27,  360},
//                {112, 268},
//                {194, 346},
//                {309, 288},
//                {118, 402},
//                {315, 419},
//                {353, 299},
//                {368, 345},
//                {379, 46},
//                {378, 204},
//                {185, 33},
//                {112, 238},
//                {365, 296},
//                {300, 174},
//                {137, 216},
//                {87,  358},
//                {59,  179},
//                {93,  92},
//                {429, 270},
//                {41,  243},
//                {248, 81},
//                {413, 101},
//                {74,  235},
//                {300, 408},
//                {129, 360},
//        }));
//    });
//// </editor-fold>
//// <editor-fold defaultstate="collapsed" desc="out(s.magnificentSets(n, edges));">
//    {
//        int n = 500;
//        vector<int> v(n);
//        fill(v.begin(), v.begin() + D{1, n - 1}(gen), 1);
//        shuffle(v.begin(), v.end(), gen);
//
//        vector<int> by_value[2];
//        for (int i = 0; i < n; ++i) by_value[v[i]].push_back(i);
//        auto &&a = by_value[0], &&b = by_value[1];
//        auto total = a.size() * b.size();
//        int add = D{0, (int) total}(gen);
//        vector<vector<int> > edges;
//        if (add < total / 2) {
//            set<pair<int, int>> st;
//            while (st.size() < add) st.emplace(a[D{0, (int) a.size() - 1}(gen)], b[D{0, (int) b.size() - 1}(gen)]);
//            for (auto &&[f, t]: st) edges.push_back(vector<int>{f + 1, t + 1});
//        } else {
//            set<pair<int, int>> st;
//            while (st.size() < total - add) {
//                st.emplace(a[D{0, (int) a.size() - 1}(gen)], b[D{0, (int) b.size() - 1}(gen)]);
//            }
//            for (int i: a) for (int j: b) if (!st.count({i, j})) edges.push_back(vector<int>{i + 1, j + 1});
//        }
//        measure([&] {
////            out(n, edges);
//            out(s.magnificentSets(n, edges));
//        });
//    }
//// </editor-fold>
// </editor-fold>
