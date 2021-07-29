#include <cstdint>
#include <type_traits>

template<template<class...> class, class Tp>
Tp make_collection(const Tp &tp) { return tp; }

template<template<class...> class Coll, class Tp, class ...Rem>
auto make_collection(const Tp &tp, size_t n, Rem &&...rem) {
    return Coll<decltype(make_collection<Coll, Tp>(tp, std::forward<Rem>(rem)...))>(
            n, make_collection<Coll, Tp>(tp, std::forward<Rem>(rem)...));
}
