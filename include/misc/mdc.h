#pragma once
#include <vector>

template<template<class...> class Coll, class Tp, size_t n>
struct mdc {
    typedef Coll<typename mdc<Coll, Tp, n - 1>::type> type;
};
template<template<class...> class Coll, class Tp>
struct mdc<Coll, Tp, 0> {
    typedef Tp type;
};
template<template<class...> class Coll, class Tp, size_t n> using mdc_t = typename mdc<Coll, Tp, n>::type;

typedef mdc_t<std::vector, int, 1> vi;
typedef mdc_t<std::vector, int, 2> v2i;
typedef mdc_t<std::vector, int, 3> v3i;
