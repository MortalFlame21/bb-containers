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
    using iterator = BidirectionIterator<T>;
    using const_iterator = iterator const;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    // ctor and dtor
    /// @brief Default constructor
    List()
        : sentinel_{new Node{}}
    {
        sentinel_->prev_ = sentinel_;
        sentinel_->next_ = sentinel_;
    };

    /// @brief Initialise List with `count` default `value_type`.
    /// @param count
    explicit List(size_type count) : List(count, value_type{}) { }

    /// @brief Initialise List with count `v` `value_type`.
    List(size_type count, const value_type& v)
        : List()
    {
        for (; count != 0; --count)
            push_back(v);
    }

    /// @brief Initialise List with contents of `list`.
    /// @param list
    List(std::initializer_list<value_type> list)
        : List()
    {
        for (const auto& e : list)
            push_back(e);
    }

    // iterators
    /// @brief Begin iterator access.
    /// @return Return begin iterator of the List container.
    iterator begin() { return iterator{sentinel_->next_}; }

    /// @brief End iterator access.
    /// @return Return end iterator of the List container.
    iterator end() { return iterator{sentinel_}; }

    // size
    /// @brief Return the size of the List.
    /// @return Return size of the List.
    size_type size() { return sz_; }

    /// @brief Returns whether List is empty.
    /// @return Return bool whether List is empty.
    bool empty() { return sz_ == 0; }

    // modifiers
    void push_front() {
    //     Node* n{new Node{v, sentinel_->prev_, sentinel_}};
    //     // update begin node prev and begin node to be n
    //     sentinel_->next_->prev_ = n;
    //     sentinel_->next_ = n;
    //     ++sz_;
    }

    void push_back(const T& v) {
        Node* n{new Node{v, sentinel_->prev_, sentinel_}};
        // update last node next and last node to be n
        sentinel_->prev_->next_ = n;
        sentinel_->prev_ = n;
        ++sz_;
    }

    void pop_front() {}

    void pop_back() {}

    void insert() {}

    void erase() {}

    void clear() {}

    // temp for now
    std::string str() {
        std::string s{};
        for (auto i{sentinel_->next_}; i != sentinel_; i = i->next_) {
            s += std::to_string(i->value_) + " ";
        }
        return s;
    }

private:
    class Node {
    public:
        value_type value_{};
        Node* prev_{};
        Node* next_{};

        Node() = default;

        explicit Node(value_type v) : value_{v} { }

        Node(value_type v, Node* prev, Node* next)
            : value_{v}, prev_{prev}, next_{next} { }
    };

    Node* sentinel_{};
    size_type sz_{};
};

} // namespace bb