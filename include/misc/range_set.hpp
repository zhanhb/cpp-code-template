#include <set>

template<class Cmp>
struct pair_less {
    Cmp cmp;

    template<class Tp, class Up>
    bool operator()(Tp &&a, Up &&b) const {
        return cmp(std::forward<Tp>(a).second, std::forward<Up>(b).first);
    }
};

template<class Tp>
struct cut {
};

template<class Tp>
struct range {
};

template<class Tp, class Cmp = std::less<Tp> >
class range_set {

    std::set<std::pair<Tp, Tp>, pair_less<Cmp> > st;

public:
    range_set() = default;

    void insert() {

    }
};
