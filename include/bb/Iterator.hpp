#pragma once

#include <cstddef> // std::ptrdiff_t
#include <iterator> // std::random_access_iterator_tag

namespace bb {

// TODO:
template<typename T>
class InputIterator { };

// TODO:
template<typename T>
class OutputIterator { };

// TODO:
template<typename T>
class BidirectionIterator { };

template<typename T>
class RandomAccessIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    // ctor
    RandomAccessIterator(pointer ptr) : ptr_{ptr} {};

    // operations
    value_type operator*() { return *ptr_; }

    pointer operator->() { return ptr_; }

    RandomAccessIterator& operator++() {
        ++ptr_;
        return *this;
    }

    RandomAccessIterator& operator++(int) {
        auto tmp{*this};
        ++ptr_;
        return tmp;
    }

    RandomAccessIterator& operator--() {
        --ptr_;
        return *this;
    }

    RandomAccessIterator& operator--(int) {
        auto tmp{*this};
        --ptr_;
        return tmp;
    }

    RandomAccessIterator& operator+(size_type n) {
        ptr_ += n;
        return *this;
    }

    RandomAccessIterator& operator-(size_type n) {
        ptr_ -= n;
        return *this;
    }

    friend bool operator==(const RandomAccessIterator& a, const RandomAccessIterator& b) {
        return a.ptr_ == b.ptr_;
    }

    friend bool operator!=(const RandomAccessIterator& a, const RandomAccessIterator& b) {
        return !(a == b);
    }
private:
    pointer ptr_{};
};
} // namespace bb