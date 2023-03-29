#include "debug.h"
#include <bits/stdc++.h>
#include "static_assert.h"

template<class Tp>
class return_it {
    Tp value;
public:
    explicit return_it(Tp val) : value(val) {};

    Tp operator()() const { return value; }
};

template<class Tp, std::size_t Sz>
class delegated_array {
    Tp data[Sz];
public:
    const return_it<Tp *> begin, end;

    delegated_array() : begin(data + 0), end(data + Sz) {}
};

template<class Tp>
struct as_ptr {

    explicit as_ptr(Tp val) noexcept: val(val) {}

    int operator*() const noexcept { return val; }

    void operator++() & noexcept { ++val; }

    as_ptr(const as_ptr &) = delete;

    as_ptr(as_ptr &&) noexcept = default;

    as_ptr &operator=(const as_ptr &) = delete;

    as_ptr &operator=(as_ptr &&) = delete;

    friend bool operator!=(as_ptr &a, as_ptr &b) noexcept { return a.val != b.val; }

private:
    Tp val;
};

template<class Tp>
struct range : std::pair<Tp, Tp> {
    static_assert(std::is_integral<Tp>::value, "must be integral");
    using std::pair<Tp, Tp>::pair;

    NODISCARD as_ptr<Tp> begin() const {
        return as_ptr<Tp>(this->first);
    }

    NODISCARD as_ptr<Tp> end() const {
        return as_ptr<Tp>(this->second);
    }
};

#if __cpp_deduction_guides
template<class Tp> range(Tp, Tp) -> range<Tp>;
#endif

struct array_adapter {
    typedef int container_type[6];
protected:
    container_type c;
};

struct incompatible_adapter {
    typedef std::vector<int> container_type;
protected:
    int c;
};

struct contain_type_only_adapter {
    typedef std::vector<int> container_type;
};

struct c_only_adapter {
protected:
    std::vector<int> c;
};

struct custom_adapter {
    typedef std::vector<int> container_type;
protected:
    container_type c;
};

struct not_container_adapter {
    typedef int container_type;
protected:
    int c;
};

struct extends_queue : std::queue<int> {
};

struct final_adapter final : std::queue<int> {
};

struct final_adapter_exposed_c final : std::queue<int> {
    using queue::c;
};

struct private_adapter : private std::queue<int> {
};

struct protected_adapter : protected std::queue<int> {
};

struct private_c_adapter {
    typedef std::vector<int> container_type;
private:
    container_type c;
};

struct static_c_adapter {
    typedef std::vector<int> container_type;
protected:
    static container_type c;
};

struct private_map : private std::map<int, int> {
};

class smallest_map : private std::map<int, int> {
    typedef map super;
public:
    using super::key_type;
    using super::mapped_type;
    using super::value_type;
    using super::begin;
    using super::end;
};

struct falsy_fake_map : std::vector<std::pair<int, int> > {
    typedef int key_type, mapped_type;
    using vector::vector;
};

struct fake_map : std::vector<std::pair<const int, int> > {
    typedef int key_type, mapped_type;
    using vector::vector;
};

int main() {
    OUT(setlocale(LC_CTYPE, nullptr));
    setlocale(LC_CTYPE, "");
    OUT(setlocale(LC_CTYPE, nullptr));

    std::cout << std::boolalpha;
    OUT(__cplusplus);

    OUT(delegated_array<int, 5>());
    for (auto x: delegated_array<int, 2>()) OUT(x);
    OUT(std::array<int, 3>{1, 5});
    OUT(array_adapter{});
    auto iList = {1, 3, 2};
    OUT(std::deque<int>(iList));
    OUT(fake_map{{1, 2}});
    OUT(std::forward_list<int>(iList));
    OUT(iList);
    OUT(std::map<int, int>{
            {1, 2},
            {2, 3},
            {1, 3},
    });
    OUT(make_pair(2, std::multiset<int>{3}));
    OUT(make_tuple(1, 2, std::unordered_multiset<int>{4, 5, 4}, 4, 5));
    OUT(std::multimap<int, int>{
            {1, 2},
            {1, 3},
            {2, 3},
            {3, 5},
    });
    OUT(std::map<std::vector<int>, std::map<int, int> >{{{2, 4, 5}, {{1, 3}, {3, 4}}}});
    OUT(std::priority_queue<int>(std::less<int>{}, std::vector<int>{2, 5, 4}));
    OUT(std::priority_queue<int, std::vector<int>, std::function<bool(int, int)> >([](int x, int y) {
        return x > y;
    }, std::vector<int>{2, 5, 4}));
    OUT(std::queue<int>(std::deque<int>{1, 2, 3, 4}));
    OUT(range<int>{0, 5});
#if __cpp_deduction_guides
    OUT(range{11LL, 20LL});
#endif
#if __cpp_lib_variant
    OUT(std::variant<int, std::string>{});
#endif
    OUT(std::set<int>{2, 3, 4});
    OUT(std::stack<int>(std::deque<int>{1, 2, 3}));
    OUT(std::unordered_map<int, int>{
            {1, 2},
            {1, 3},
            {3, 4},
            {5, 6},
            {7, 8},
    });
    OUT(std::valarray<int>{iList});
    OUT(std::vector<bool>{true, false, true});
    OUT(std::vector<std::map<int, int> >{{{2, 3}}});
    OUT(std::vector<std::pair<int, int> >{{1, 2}});
    OUT(std::vector<std::vector<int> >{{1}});
    {
        char hello[] = "hello";
        OUT(std::cout, hello);
        OUT(std::wcout, hello);
    }
    {
        const char hello[] = "hello";
        OUT(std::cout, hello);
        OUT(std::wcout, hello);
    }
    {
        wchar_t hello[] = L"hello";
        OUT(std::cout, hello);
        OUT(std::wcout, hello);
    }

    OUT("hello, world!");
    OUT(std::string("hello, world!"));
#if __cpp_lib_string_view
    using namespace std::string_view_literals;
    OUT("hello, world!"sv);
#endif
    OUT(L"hello, world!");
    OUT(std::wstring(L"hello, world!"));
#if __cpp_lib_string_view
    OUT(L"hello, world!"sv);
#endif

    {
        OUT(std::make_tuple());
        OUT(std::make_tuple(std::queue<int>{std::deque<int>{3, 4, 5}}, 1, 2, 3));
    }

    std::string str = "hello";
    std::wstring ws = L"hello";
    OUT(std::cout, str);
    OUT(std::wcout, str);
    OUT(std::cout, ws);
    OUT(std::wcout, ws);

    STATIC_ASSERT_CONCEPT(debug::adapter, array_adapter);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, incompatible_adapter);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, contain_type_only_adapter);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, c_only_adapter);
    STATIC_ASSERT_CONCEPT(debug::adapter, custom_adapter);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, not_container_adapter);
    STATIC_ASSERT_CONCEPT(debug::adapter, extends_queue);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, final_adapter);
    STATIC_ASSERT_CONCEPT(debug::adapter, final_adapter_exposed_c);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, private_adapter);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, protected_adapter);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, private_c_adapter);
    STATIC_ASSERT_NOT_CONCEPT(debug::adapter, static_c_adapter);
    STATIC_ASSERT_CONCEPT(debug::adapter, std::queue<int>);
    STATIC_ASSERT_CONCEPT(debug::adapter, std::queue<int> &);
    STATIC_ASSERT_CONCEPT(debug::adapter, const std::queue<int> &);
    STATIC_ASSERT_CONCEPT(debug::adapter, std::queue<int> &&);
    STATIC_ASSERT_CONCEPT(debug::collection, std::string);
#if __cpp_lib_string_view
    STATIC_ASSERT_CONCEPT(debug::collection, std::string_view);
    STATIC_ASSERT_CONCEPT(utility::string_of, std::string_view, char);
    STATIC_ASSERT_CONCEPT(utility::string_of, std::wstring_view, wchar_t);
    STATIC_ASSERT_CONCEPT(debug::string, std::string_view);
    STATIC_ASSERT_CONCEPT(debug::string, std::string_view &);
    STATIC_ASSERT_CONCEPT(debug::string, std::string_view &&);
    STATIC_ASSERT_CONCEPT(debug::string, std::wstring_view);
    STATIC_ASSERT_CONCEPT(debug::string, std::wstring_view &);
    STATIC_ASSERT_CONCEPT(debug::string, std::wstring_view &&);
#endif
    STATIC_ASSERT_CONCEPT(utility::string_of, std::string, char);
    STATIC_ASSERT_CONCEPT(utility::string_of, std::wstring, wchar_t);
    STATIC_ASSERT_CONCEPT(debug::string, std::string);
    STATIC_ASSERT_CONCEPT(debug::string, std::string &);
    STATIC_ASSERT_CONCEPT(debug::string, std::string &&);
    STATIC_ASSERT_CONCEPT(debug::string, std::wstring);
    STATIC_ASSERT_CONCEPT(debug::string, std::wstring &);
    STATIC_ASSERT_CONCEPT(debug::string, std::wstring &&);
    STATIC_ASSERT_CONCEPT(debug::string, const char*);
    STATIC_ASSERT_CONCEPT(debug::string, const wchar_t[10]);
    STATIC_ASSERT_CONCEPT(debug::collection, std::valarray<int>);
    STATIC_ASSERT_CONCEPT(debug::collection, std::vector<int>);
    STATIC_ASSERT_CONCEPT(debug::collection, std::vector<int> &);
    STATIC_ASSERT_CONCEPT(debug::collection, std::vector<int> &&);
    STATIC_ASSERT_CONCEPT(debug::collection, const std::vector<int>);
    STATIC_ASSERT_CONCEPT(debug::collection, const std::vector<int> &);
    STATIC_ASSERT_NOT_CONCEPT(debug::map, falsy_fake_map);
    STATIC_ASSERT_CONCEPT(debug::map, fake_map);
    STATIC_ASSERT_CONCEPT(debug::map, std::map<int, int>);
    STATIC_ASSERT_CONCEPT(debug::map, std::map<int, int> &);
    STATIC_ASSERT_CONCEPT(debug::map, std::map<int, int> &&);
    STATIC_ASSERT_CONCEPT(debug::map, const std::map<int, int>);
    STATIC_ASSERT_NOT_CONCEPT(debug::map, private_map);
    STATIC_ASSERT_CONCEPT(debug::map, smallest_map);
    STATIC_ASSERT_CONCEPT(debug::map, std::multimap<int, int>);
    STATIC_ASSERT_CONCEPT(debug::map, std::unordered_map<int, int> &&);
    STATIC_ASSERT_CONCEPT(debug::tuple, std::tuple<>);
    STATIC_ASSERT_CONCEPT(debug::tuple, std::tuple<> &&);
    STATIC_ASSERT_CONCEPT(debug::tuple, const std::pair<int, int>&);
}
