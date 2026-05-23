#include <algorithm> // std::copy
#include <initializer_list> // std::initializer_list
#include <stdexcept> // std::out_of_range

#include "Iterator.hpp"

namespace bb {
/// @brief A dynamic array of type `T`, a Vector. Aiming to mimic
/// [`std::vector`](https://en.cppreference.com/cpp/container/vector).
/// @tparam T
template<typename T>
class Vector {
public:
    // typedefs
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = RandomAccessIterator<T>;
    using const_iterator = iterator const;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    // ctor, move and dtor
    /// @brief Initialise default Vector container.
    Vector() = default;

    /// @brief Initialise Vector container to `size`.
    /// @param size
    explicit Vector(size_t size)
        : beg_{new T[size]{}}
        , end_{beg_ + size}
        , cap_{end_} {}

    /// @brief Initialise Vector container with contents of `list`.
    /// @param list
    Vector(std::initializer_list<T> list)
        : beg_{new T[list.size()]{}}
        , end_{beg_ + list.size()}
        , cap_{end_}
    { std::copy(list.begin(), list.end(), beg_); }

    /// @brief Initialise Vector container to copy Vector `other`.
    Vector(const Vector& other)
        : beg_{new T[other.size()]{}}
        , end_{beg_ + other.size()}
        , cap_{end_}
    { std::copy(other.begin(), other.end(), beg_); }

    /// @brief Copy assign Vector container to copy Vector `other`.
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            // preserve elements incase a exception is thrown
            T* container{new T[other.size()]};
            std::copy(other.begin(), other.end(), container);

            delete[] beg_;

            beg_ = container;
            end_ = beg_ + other.size();
            cap_ = end_;
        }
        return *this;
    }

    /// @brief Move contents of Vector `other` to initialise Vector.
    /// @post `other` is invalidated.
    Vector(Vector&& other)
        : beg_{other.beg_}
        , end_{other.end_}
        , cap_{other.cap_}
    {
        other.beg_ = nullptr;
        other.end_ = nullptr;
        other.cap_ = nullptr;
    }

    /// @brief Move assign of Vector `other` to initialise Vector.
    /// @post `other` is invalidated.
    Vector& operator=(Vector&& other) {
        if (this != &other) {
            delete[] beg_;

            beg_ = other.beg_;
            end_ = other.end_;
            cap_ = other.cap_;

            other.beg_ = nullptr;
            other.end_ = nullptr;
            other.cap_ = nullptr;
        }
        return *this;
    }

    ~Vector() { delete[] beg_; }

    // element access
    /// @brief Const reference access via operator[]().
    /// @return `const_reference` of `i`th element in Vector container.
    const_reference operator[](size_type i) const { return beg_[i]; }

    /// @brief Reference access via operator[]().
    /// @return `reference` of `i`th element in Vector container.
    reference operator[](size_type i) { return beg_[i]; }

    /// @brief Const reference access with size check.
    /// @return `const_reference` of `i`th element in Vector container.
    const_reference at(size_type i) const {
        if (i >= size()) throw std::out_of_range{"Vector::at: i >= Vector::size"};
        return beg_[i];
    }

    /// @brief Reference access with size check.
    /// @return `reference` of `i`th element in Vector container.
    reference at(size_type i) {
        if (i >= size()) throw std::out_of_range{"Vector::at: i >= Vector::size"};
        return beg_[i];
    }

    // iterators
    /// @brief Begin iterator access.
    /// @return Begin `iterator`.
    iterator begin() { return iterator(beg_); }

    /// @brief Begin const iterator access.
    /// @return Begin `const_iterator`.
    const_iterator begin() const { return const_iterator(beg_); }

    /// @brief End iterator access.
    /// @return End `iterator`.
    iterator end() { return iterator(end_); }

    /// @brief End const_iterator access.
    /// @return End `const_iterator`.
    const_iterator end() const { return const_iterator(end_); }

    // size and capacity
    /// @brief Returns how many elements Vector container can contain.
    /// @return Return Vector container size.
    size_type size() const { return end_ - beg_; }

    /// @brief Return how many elements Vector container has allocated.
    /// @return Return Vector capacity size.
    size_type capacity() const { return cap_ - beg_; }

    /// @brief Returns if Vector container is empty, i.e `begin() == end()`.
    /// @return `bool` value if Vector is empty.
    bool empty() const { return begin() == end(); }

    /// @brief Resize Vector size() to new_size.
    /// @details
    /// - if `size() > new_size`, decrease to new_size.
    /// - if `size() == new_size`, do nothing.
    /// - if `size() < new_size`, increase to new_size.
    /// - if `capacity() < new_size`, `capacity()` grows to fit new_size.
    /// @param new_size
    /// @post `size() >= new_size`
    void resize(size_type new_size) {
        if (size() > new_size) {
            while(size() != new_size)
                (end_--)->~T();
        }
        else if (size() < new_size) {
            if (capacity() < new_size)
                reserve(new_size);
            while(size() != new_size)
                *(end_++) = T();
        }
    }

    /// @brief Resize Vector `capacity()` to `new_capacity`.
    /// @details
    /// - If `capacity() >= new_capacity`, do nothing.
    /// - If `capacity() < new_capacity`, `capacity()` grows to new_capacity.
    /// @param new_capacity
    /// @post `capacity() >= new_capacity`
    void reserve(size_type new_capacity) {
        if (capacity() < new_capacity) {
            T* container{new T[new_capacity]};
            std::copy(begin(), end(), container);

            cap_ = container + new_capacity;
            end_ = container + size(); // using previous size
            delete[] beg_;
            beg_ = container;
        }
    }

    // modifiers
    /// @brief Pushes element `e` to back of Vector container.
    /// @details if `capacity()` <= `size()`, `reserve()` is called to fit `e`.
    /// @post `size()` is increased by one.
    void push_back(const T& e) {
        if (capacity() <= size())
            reserve(empty() ? 8 : 2 * size());
        beg_[size()] = e;
        ++end_;
    }

    /// @brief Inserts element `e` before `pos` of Vector container.
    /// @param pos
    /// @param e
    void insert(iterator pos, const T& e) {
        insert(pos, 1, e);
    }

    /// @brief Inserts count element `e` in Vector before pos.
    /// @param pos
    /// @param count
    /// @param e
    void insert(iterator pos, size_type count, const T& e) {
        difference_type i{pos - begin()}; // find offset
        if (capacity() <= size())
            reserve(empty() ? count + 1 : 2 * capacity() + count);
        // shift elements after to-be-inserted elements to fit
        if (begin() + i != end())
            std::move_backward(begin() + i, end(), end() + count);
        std::fill(begin() + i, begin() + i + count, e);
        end_ += count;
    }

    /// @brief Pops the last element from the Vector container.
    /// @details if `empty()` a `std::out_of_range` is thrown.
    /// @post `size()` is decreased by one.
    void pop_back() {
        if (empty())
            throw std::out_of_range{"Vector::pop_back: Vector::empty == true"};
        beg_[size() - 1].~T();
        --end_;
    }

    /// @brief Erases element at `pos` at Vector container.
    /// @param pos
    void erase(iterator pos) {
        if (pos + 1 != end())
            std::copy(pos + 1, end(), pos);
        --end_;
    }

    // /// @brief Erases elements in range [`f`, `l`) at Vector container.
    // /// @param f
    // /// @param l
    // void erase(iterator f, iterator l)

    /// @brief clears the entire Vector container.
    /// @post `begin()` is invalidated.
    /// @post `end()` is invalidated.
    void clear() {
        delete[] beg_;
        beg_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
    }

    /// @brief Swaps current Vector with Vector `other`.
    /// @param other
    void swap(Vector& other) { std::swap(*this, other); }

    /// @brief Swaps Vector `a` with Vector `b`.
    /// @param a
    /// @param b
    friend void swap(Vector& a, Vector& b) { std::swap(a, b); }

private:
    T* beg_{};
    T* end_{};
    T* cap_{};
};
} // namespace bb