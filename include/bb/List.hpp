#pragma once

#include <initializer_list>

#include "Iterator.hpp"

namespace bb {
template<typename T>
class List {
public:
    class Node;

    // type alias's
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = BidirectionIterator<Node>;
    using const_iterator = iterator const;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    class Node {
    public:
        using value_type = T;

        value_type value_{};
        Node* prev_{};
        Node* next_{};

        Node() = default;

        explicit Node(value_type v) : value_{v} { }

        Node(value_type v, Node* prev, Node* next)
            : value_{v}, prev_{prev}, next_{next} { }
    };

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
        std::fill_n(std::back_inserter(*this), count, v);
    }

    /// @brief Initialise List with contents of `list`.
    /// @param list
    List(std::initializer_list<value_type> list)
        : List()
    {
        std::copy(list.begin(), list.end(), std::back_inserter(*this));
    }

    List(const List& other) : List() {
        std::copy(other.begin(), other.end(), std::back_inserter(*this));
    }

    List& operator=(const List& other) {
        if (this != &other) {
            clear();

            std::copy(other.begin(), other.end(), std::back_inserter(*this));
        }
        return *this;
    }

    List(List&& other) noexcept : List() {
        std::swap(sentinel_, other.sentinel_);
        std::swap(sz_, other.sz_);
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();

            std::swap(sentinel_, other.sentinel_);
            std::swap(sz_, other.sz_);
        }
        return *this;
    }

    ~List() {
        clear();
        delete sentinel_;
    }

    // iterators
    /// @brief Begin iterator access.
    /// @return Return begin iterator of the List container.
    iterator begin() { return iterator{sentinel_->next_}; }

    /// @brief Const begin iterator access.
    /// @return Return const begin iterator of the List container.
    const_iterator begin() const { return iterator{sentinel_->next_}; }

    /// @brief End iterator access.
    /// @return Return end iterator of the List container.
    iterator end() { return iterator{sentinel_}; }

    /// @brief Const end iterator access.
    /// @return Return const end iterator of the List container.
    const_iterator end() const { return iterator{sentinel_}; }

    // size
    /// @brief Return the size of the List.
    /// @return Return size of the List.
    size_type size() { return sz_; }

    /// @brief Returns whether List is empty.
    /// @return Return bool whether List is empty.
    bool empty() { return begin() == iterator{sentinel_}; }

    // modifiers
    /// @brief Push element `v` in begining of list.
    /// @param v
    void push_front(const T& v) {
        auto* n{new Node{v, sentinel_, sentinel_->next_}};
        // update begin node prev and begin node to be n
        sentinel_->next_->prev_ = n;
        sentinel_->next_ = n;
        ++sz_;
    }

    /// @brief Push element `v` in back of list.
    /// @param v
    void push_back(const T& v) {
        auto* n{new Node{v, sentinel_->prev_, sentinel_}};
        // update last node next and last node to be n
        sentinel_->prev_->next_ = n;
        sentinel_->prev_ = n;
        ++sz_;
    }

    /// @brief Remove begining element from List.
    void pop_front() {
        if (empty())
            throw std::out_of_range{"List::pop_back: List::empty == true"};

        auto* front{sentinel_->next_};
        sentinel_->next_ = front->next_;
        front->next_->prev_ = sentinel_;
        delete front;
        --sz_;
    }

    /// @brief Remove last element from List.
    void pop_back() {
        if (empty())
            throw std::out_of_range{"List::pop_back: List::empty == true"};

        auto* back{sentinel_->prev_};
        sentinel_->prev_ = back->prev_;
        back->prev_->next_ = sentinel_;
        delete back;
        --sz_;
    }

    /// @brief Insert `v` before `pos` in List container.
    /// @param pos
    /// @param v
    /// @return Iterator pointing inserted v.
    iterator insert(iterator pos, const T& v) {
        // wack, but i need to move on from this
        if (empty()) {
            push_back(v);
            return begin();
        }

        auto* n{new Node(v, pos->prev_, pos.ptr_)};
        pos->prev_->next_ = n;
        pos->prev_ = n;
        ++sz_;

        return iterator{n};
    }

    /// @brief Insert `count` `v` elements before `pos` in List container.
    /// @param pos
    /// @param count
    /// @param v
    /// @return An iterator pointing to first element inserted before pos.
    iterator insert(iterator pos, size_type count, const T& v) {
        for (size_type i{}; i < count; ++i)
            pos = insert(pos, v);
        return pos;
    }

    /// @brief Erase element at `pos` in List container.
    /// @return A iterator to
    iterator erase(iterator pos) {
        if (empty()) return end();

        pos->next_->prev_ = pos->prev_;
        pos->prev_->next_ = pos->next_;
        auto it{iterator{pos->next_}}; // save it, ugh
        delete pos.ptr_;

        --sz_;

        return it;
    }

    /// @brief Erases elements in range [`f`, `l`) at List container.
    /// @param f
    /// @param l
    /// @return A iterator after range [f, l)
    iterator erase(iterator f, iterator l) {
        while (f != l)
            f = erase(f);
        return f;
    }

    /// @brief Clear contents of List, freeing space occupied.
    void clear() {
        for (auto i{sentinel_->next_}; i != sentinel_;) {
            auto next{i->next_};
            delete i;
            i = next;
        }
        sentinel_->prev_ = sentinel_;
        sentinel_->next_ = sentinel_;
        sz_ = 0;
    }

    /// @brief Transfer other into *this before pos.
    /// @param pos
    /// @param other
    /// @post other contents is transfer and thus emptied.
    void splice(iterator pos, List& other) {
        // wacky ahh implementation. ugh.
        pos->prev_->next_ = other.begin().ptr_;
        other.begin()->prev_ = pos->prev_;

        // merge other to *this
        std::prev(other.end())->next_ = pos.ptr_;
        sz_ += other.sz_;

        // clear out other
        other.sentinel_->prev_ = other.sentinel_;
        other.sentinel_->next_ = other.sentinel_;
        other.sz_ = 0;
    }
private:
    Node* sentinel_{};
    size_type sz_{};
};
} // namespace