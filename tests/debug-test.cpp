#include "debug.h"
#include <bits/stdc++.h>

using namespace std;

template<class T, size_t N>
class delegated_array {
    std::array<T, N> arr;
public:
    const function<int *()> begin, end;

    delegated_array() : begin([&] { return std::begin(this->arr); }), end([&] { return std::end(this->arr); }) {}
};

template<class Tp>
struct as_ptr {

    explicit as_ptr(Tp val) : val(val) {}

    int operator*() const { return val; }

    void operator++() &{ ++val; }

    as_ptr(const as_ptr &) = delete;

    as_ptr(as_ptr &&) noexcept = default;

    as_ptr &operator=(const as_ptr &) = delete;

    as_ptr &operator=(as_ptr &&) = delete;

    friend bool operator!=(as_ptr &a, as_ptr &b) { return a.val != b.val; }

private:
    Tp val;
};

template<class Tp>
struct range : pair<Tp, Tp> {
    static_assert(std::is_integral<Tp>::value, "must be integral");
    using pair<Tp, Tp>::pair;

    [[nodiscard]] as_ptr<Tp> begin() const {
        return as_ptr<Tp>{this->first};
    }

    [[nodiscard]] as_ptr<Tp> end() const {
        return as_ptr<Tp>{this->second};
    }
};

#if __cplusplus >= 201703L
template<typename Tp> range(Tp, Tp) -> range<Tp>;
#endif

struct array_adapter {
    typedef int container_type[10];
protected:
    container_type c;
};

struct incompatible_adapter {
    typedef vector<int> container_type;
protected:
    int c;
};

struct contain_type_only_adapter {
    typedef vector<int> container_type;
};

struct custom_adapter {
    typedef vector<int> container_type;
protected:
    container_type c;
};

struct not_container_adapter {
    typedef int container_type;
protected:
    int c;
};

struct extends_queue : queue<int> {
};

struct final_adapter final : queue<int> {
};

struct final_adapter_exposed_c final : queue<int> {
    using queue::c;
};

struct private_adapter : private queue<int> {
};

struct protected_adapter : protected queue<int> {
};

struct private_c_adapter {
    typedef vector<int> container_type;
private:
    container_type c;
};

struct static_c_adapter {
    typedef vector<int> container_type;
protected:
    static container_type c;
};

class map_protected : private map<int, int> {
    typedef map super;
public:
//    using super::key_type;
//    using super::mapped_type;
    using super::begin;
    using super::end;
};

struct fake_map : vector<pair<int, int> > {
    typedef int key_type, mapped_type;
    using vector::vector;
};

#define do_static_assert(is, ...) static_assert(is<__VA_ARGS__>::value, #is "<" #__VA_ARGS__ ">::value")
#define static_assert_not(is, ...) static_assert(!is<__VA_ARGS__>::value, #is "<" #__VA_ARGS__ ">::value")

int main() {
    out(setlocale(LC_CTYPE, nullptr));
    setlocale(LC_CTYPE, "");
    out(setlocale(LC_CTYPE, nullptr));

    clog << boolalpha;
    out(__cplusplus);

    out(delegated_array<int, 5>{});
    out(array<int, 3>{1, 5});
    out(array_adapter{});
    auto iList = {1, 3, 2};
    out(deque<int>(iList));
    out(fake_map{{1, 2}});
    out(forward_list<int>(iList));
    out(iList);
    out(map<int, int>{
            {1, 2},
            {2, 3},
            {1, 3},
    });
    out(make_pair(2, multiset<int>{3}));
    out(make_tuple(1, 2, unordered_multiset<int>{4, 5, 4}, 4, 5));
    out(multimap<int, int>{
            {1, 2},
            {1, 3},
            {2, 3},
            {3, 5},
    });
    out(map<vector<int>, map<int, int> >{{{2, 4, 5}, {{1, 3}, {3, 4}}}});
    out(priority_queue<int>(less<int>{}, vector<int>{2, 5, 4}));
    out(priority_queue<int, vector<int>, function<bool(int, int)> >([](int x, int y) {
        return x > y;
    }, vector<int>{2, 5, 4}));
    out(queue<int>(deque<int>{1, 2, 3, 4}));
    out(range<int>{0, 5});
#if __cplusplus >= 201703L
    out(range{11LL, 20LL});
#endif
    out(set<int>{2, 3, 4});
    out(stack<int>(deque<int>{1, 2, 3}));
    out(unordered_map<int, int>{
            {1, 2},
            {1, 3},
            {3, 4},
            {5, 6},
            {7, 8},
    });
    out(valarray<int>{iList});
    out(vector<bool>{true, false, true});
    out(vector<map<int, int> >{{{2, 3}}});
    out(vector<pair<int, int> >{{1, 2}});
    out(vector<vector<int> >{{1}});
    {
        char hello[] = "hello";
        out(clog, hello);
        out(wclog, hello);
    }
    {
        const char hello[] = "hello";
        out(clog, hello);
        out(wclog, hello);
    }
    {
        wchar_t hello[] = L"hello";
        out(clog, hello);
        out(wclog, hello);
    }

    out("hello, world!");
    out(string("hello, world!"));
#if __cplusplus >= 201703L
    out("hello, world!"sv);
#endif
    out(L"hello, world!");
    out(wstring(L"hello, world!"));
#if __cplusplus >= 201703L
    out(L"hello, world!"sv);
#endif

    {
        out(make_tuple());
        out(make_tuple(queue<int>{deque<int>{3, 4, 5}}, 1, 2, 3));
    }

    string str = "hello";
    wstring ws = L"hello";
    out(clog, str);
    out(wclog, str);
    out(clog, ws);
    out(wclog, ws);

    str = "\u4f60\u597d";
    ws = L"\u4f60\u597d";

    wclog.imbue(locale(""));
    out(clog, str);
    out(wclog, str);
    out(clog, ws);
    out(wclog, ws);

    do_static_assert(debug::is_adapter, array_adapter);
    static_assert_not(debug::is_adapter, incompatible_adapter);
    static_assert_not(debug::is_adapter, contain_type_only_adapter);
    do_static_assert(debug::is_adapter, custom_adapter);
    static_assert_not(debug::is_adapter, not_container_adapter);
    do_static_assert(debug::is_adapter, extends_queue);
    static_assert_not(debug::is_adapter, final_adapter);
    do_static_assert(debug::is_adapter, final_adapter_exposed_c);
    static_assert_not(debug::is_adapter, private_adapter);
    static_assert_not(debug::is_adapter, protected_adapter);
    static_assert_not(debug::is_adapter, private_c_adapter);
    static_assert_not(debug::is_adapter, static_c_adapter);
    do_static_assert(debug::is_adapter, queue<int>);
    do_static_assert(debug::is_adapter, queue<int> &);
    do_static_assert(debug::is_adapter, const queue<int> &);
    do_static_assert(debug::is_adapter, queue<int> &&);
    do_static_assert(debug::is_collection, string);
#if __cplusplus >= 201703L
    do_static_assert(debug::is_collection, string_view);
#endif
    do_static_assert(debug::is_collection, valarray<int>);
    do_static_assert(debug::is_collection, vector<int>);
    do_static_assert(debug::is_collection, vector<int> &);
    do_static_assert(debug::is_collection, vector<int> &&);
    do_static_assert(debug::is_collection, const vector<int>);
    do_static_assert(debug::is_collection, const vector<int> &);
    do_static_assert(debug::is_map, fake_map);
    do_static_assert(debug::is_map, map<int, int>);
    do_static_assert(debug::is_map, map<int, int> &);
    do_static_assert(debug::is_map, map<int, int> &&);
    do_static_assert(debug::is_map, const map<int, int>);
    static_assert_not(debug::is_map, map_protected);
    do_static_assert(debug::is_map, multimap<int, int>);
    do_static_assert(debug::is_map, unordered_map<int, int> &&);
    do_static_assert(debug::detail::is_specialization_of, tuple<>, tuple);
    do_static_assert(debug::is_tuple, tuple<>);
    do_static_assert(debug::is_tuple, tuple<> &&);
    do_static_assert(debug::is_tuple, const pair<int, int>&);
}
