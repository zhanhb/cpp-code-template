#include <bits/stdc++.h>
#include "debug.h"
#include "measure.h"

using namespace std;

int main() {
    measure([&] {
        ifstream ifs("../data.txt");
//        ofstream ofs("../ojdata.txt");
        vector<string> operators;
        vector<vector<int>> data;

        for (int n, m; ifs >> n >> m;) {
            operators.clear();
            data.clear();
            operators.emplace_back("BookMyShow");
            data.emplace_back(vector<int>{n, m});
            BookMyShow bms(n, m);
            out("BookMyShow(", n, m, ")");
            int cas;
            ifs >> cas;
            assert(ifs);
            scoped_measure();
            auto st = std::chrono::high_resolution_clock::now();
            while (cas-- > 0) {
                char ch;
                ifs >> ch >> n >> m;
                assert(ifs);

                switch (ch) {
                    case 'g': {
                        operators.emplace_back("gather");
                        out("gather", n, m);
                        auto t = bms.gather(n, m);
                        out(t);
                        break;
                    }
                    case 's': {
                        operators.emplace_back("scatter");
                        out("scatter", n, m);
                        auto t = bms.scatter(n, m);
                        out(t);
                        break;
                    }
                    default:
                        assert(false);
                        abort();
                }

                if (cas % 10000 == 0) {
                    auto t = std::chrono::high_resolution_clock::now();
                    out(std::chrono::duration<long double>(t - st).count());
                    st = t;
                }

                data.emplace_back(vector<int>{n, m});
            }
            vector<common_type_t<decltype(quoted(declval<string &>()))>> v;
            v.reserve(operators.size());
            for (auto &&s: operators) v.push_back(quoted(s));
//            ofs << v << endl << data << endl;
        }
    });
}