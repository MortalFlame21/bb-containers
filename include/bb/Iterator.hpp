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

    // operations
    /// @brief Dereference operator.
    /// @return `value_type`
    value_type operator*() const { return *ptr_; }

    /// @brief Const dereference operator.
    /// @return `reference`
    reference operator*() const { return *ptr_; }

    /// @brief Arrow operator.
    /// @return `pointer`
    pointer operator->() const { return ptr_; }

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
    /// @return `RandomAccessIterator` incremented by n.
    RandomAccessIterator operator+(difference_type n) const {
        return RandomAccessIterator(ptr_ + n);
    }

    /// @brief Subtract. Decrement Iterator by `n`.
    /// @param n
    /// @return `RandomAccessIterator` decremented by n.
    RandomAccessIterator operator-(difference_type n) const {
        return RandomAccessIterator(ptr_ - n);
    }

    /// @brief Addition. Sum of RandomAccessIterator of a and b.
    /// @param a
    /// @param b
    /// @return `RandomAccessIterator` sum of a and b.
    friend difference_type
        operator+(RandomAccessIterator a, RandomAccessIterator b) {
        return a.ptr_ + b.ptr_;
    }

    /// @brief Subtract. Difference of RandomAccessIterator of a and b.
    /// @param a
    /// @param b
    /// @return `RandomAccessIterator` difference of a and b.
    friend difference_type
        operator-(RandomAccessIterator a, RandomAccessIterator b) {
        return a.ptr_ - b.ptr_;
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