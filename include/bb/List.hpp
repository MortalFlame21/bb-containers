#pragma once

#include <initializer_list>

#include "Iterator.hpp"

namespace bb {
template<typename T>
class List {
public:
    // type alias's
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = RandomAccessIterator<T>;
    using const_iterator = iterator const;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    // ctor and dtor
    /// @brief Default constructor
    List() = default;

    /// @brief Initialise List with `count` default `value_type`.
    /// @param count
    List(size_type count)

    { }

    /// @brief Initialise List with count `v` `value_type`.
    List(size_type count, const value_type& v)

    { }

    /// @brief Initialise List with contents of `list`.
    /// @param list
    List(std::initializer_list<value_type> list)

    { }

    // iterators
    /// @brief Begin iterator access.
    /// @return Return begin iterator of the List container.
    iterator begin() { return iterator{beg_}; }

    /// @brief End iterator access.
    /// @return Return end iterator of the List container.
    iterator end() { return iterator{end_}; }

    // size
    /// @brief Return the size of the List.
    /// @return Return size of the List.
    size_type size() { return end() - begin(); }

    /// @brief Returns whether List is empty.
    /// @return Return bool whether List is empty.
    bool empty() { return begin() == end(); }

    // modifiers
    void insert() {}

    void erase() {}

private:
    T* beg_{};
    T* end_{};
};

} // namespace bb