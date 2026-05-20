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
    // type alias
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    // ctor
    RandomAccessIterator(pointer ptr) : ptr_{ptr} {};

    /// @brief Dereference operator.
    /// @return `value_type`
    value_type operator*() { return *ptr_; }

    /// @brief Arrow operator.
    /// @return `pointer`
    pointer operator->() { return ptr_; }

    /// @brief Pre-increment operator.
    /// @return `RandomAccessIterator` reference.
    RandomAccessIterator& operator++() {
        ++ptr_;
        return *this;
    }

    /// @brief Post-increment operator.
    /// @return `RandomAccessIterator` reference to previous value.
    RandomAccessIterator& operator++(int) {
        auto tmp{*this};
        ++ptr_;
        return tmp;
    }

    /// @brief Pre-decrement operator.
    /// @return `RandomAccessIterator` reference.
    RandomAccessIterator& operator--() {
        --ptr_;
        return *this;
    }

    /// @brief Post-decrement operator.
    /// @return `RandomAccessIterator` reference to previous value.
    RandomAccessIterator& operator--(int) {
        auto tmp{*this};
        --ptr_;
        return tmp;
    }

    /// @brief Addition. Increment Iterator by `n`.
    /// @param n
    /// @return `RandomAccessIterator` reference.
    RandomAccessIterator& operator+(size_type n) {
        ptr_ += n;
        return *this;
    }

    /// @brief Addition. Decrement Iterator by `n`.
    /// @param n
    /// @return `RandomAccessIterator` reference.
    RandomAccessIterator& operator-(size_type n) {
        ptr_ -= n;
        return *this;
    }

    /// @brief Equality of a and b.
    /// @param a
    /// @param b
    /// @return `bool`. `true` if underlying pointers of `a` and `b` are the same.
    friend bool operator==(const RandomAccessIterator& a, const RandomAccessIterator& b) {
        return a.ptr_ == b.ptr_;
    }

    /// @brief Not equal a and b.
    /// @param a
    /// @param b
    /// @return `bool`. `true` if underlying pointers of `a` and `b` are the not same.
    friend bool operator!=(const RandomAccessIterator& a, const RandomAccessIterator& b) {
        return !(a == b);
    }
private:
    pointer ptr_{};
};
} // namespace bb