#pragma once

#include <array>
#include <atomic>
#include <mutex>

const int N = 1e5 + 5;

template<class Tp>
class static_storage { // NOLINT(cppcoreguidelines-pro-type-member-init)
    struct alignas(alignof(Tp)) data {
        [[maybe_unused]] char dummy[sizeof(Tp)]; // NOLINT(bugprone-sizeof-expression)
    };
public:
    volatile std::atomic_size_t offset{};
    std::array<data, N> buf;
};

template<class Tp>
class static_allocator {
    using Delegate = std::array<Tp, N>;
    using storage = static_storage<Tp>;
    [[maybe_unused]] static std::mutex mtx;
    [[maybe_unused]] static std::weak_ptr<storage> weak;

    static std::shared_ptr<storage> to_strong() {
        std::shared_ptr<storage> ptr;
        {
            std::lock_guard<std::mutex> lg{mtx};
            std::weak_ptr<storage> w = weak;
            ptr = w.lock();
            if (!ptr) weak = ptr = std::make_shared<storage>();
        }
        return ptr;
    }

    std::shared_ptr<storage> ptr{to_strong()};
public:

    [[maybe_unused]] typedef typename Delegate::size_type size_type;
    [[maybe_unused]] typedef typename Delegate::difference_type difference_type;
    [[maybe_unused]] typedef typename Delegate::pointer pointer;
    [[maybe_unused]] typedef typename Delegate::const_pointer const_pointer;
    [[maybe_unused]] typedef typename Delegate::reference reference;
    [[maybe_unused]] typedef typename Delegate::const_reference const_reference;
    [[maybe_unused]] typedef typename Delegate::value_type value_type;

    template<class Up>
    struct [[maybe_unused]] rebind {
        [[maybe_unused]] typedef static_allocator<Up> other;
    };

    static_allocator() = default;

    template<class Up>
    [[maybe_unused]] explicit static_allocator(const static_allocator<Up> &) {}

    [[maybe_unused]] pointer allocate(size_type size, pointer = nullptr) {
        size_type original, to;
        do {
            original = ptr->offset;
            to = original + size;
            if (to > N) throw std::out_of_range("out of storage");
        } while (!ptr->offset.compare_exchange_weak(original, to));
        return reinterpret_cast<pointer>(&ptr->buf[original]);
    }

    [[maybe_unused]] void deallocate(pointer, size_type) {}
};

template<class Tp> std::weak_ptr<typename static_allocator<Tp>::storage> static_allocator<Tp>::weak;
template<class Tp> std::mutex static_allocator<Tp>::mtx;
