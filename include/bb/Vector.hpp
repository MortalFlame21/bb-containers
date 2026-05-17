#include <algorithm>
#include <initializer_list>
#include <stdexcept>

namespace bb {
template<typename T>
class Vector {
public:
    // typedefs
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = T* const;
    using diff_type = std::ptrdiff_t;
    using size_type = std::size_t;

    // ctor, move and dtor
    Vector() = default;

    explicit Vector(size_t size)
        : beg_{new T[size]{}}
        , end_{beg_ + size}
        , cap_{end_} {}

    Vector(std::initializer_list<T> list)
        : beg_{new T[list.size()]{}}
        , end_{beg_ + list.size()}
        , cap_{end_}
    { std::copy(list.begin(), list.end(), beg_); }

    Vector(const Vector& other)
        : beg_{new T[other.size()]{}}
        , end_{beg_ + other.size()}
        , cap_{end_}
    { std::copy(other.begin(), other.end(), beg_); }

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

    Vector(Vector&& other)
        : beg_{other.beg_}
        , end_{other.end_}
        , cap_{other.cap_}
    {
        other.beg_ = nullptr;
        other.end_ = nullptr;
        other.cap_ = nullptr;
    }

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
    const_reference operator[](size_type i) const { return beg_[i]; }

    reference operator[](size_type i) { return beg_[i]; }

    const_reference at(size_type i) const {
        if (i >= size()) throw std::out_of_range{"Vector::at: i >= Vector::size"};
        return beg_[i];
    }

    reference at(size_type i) {
        if (i >= size()) throw std::out_of_range{"Vector::at: i >= Vector::size"};
        return beg_[i];
    }

    // iterators
    iterator begin() const { return beg_; }

    const_iterator begin() { return beg_; }

    iterator end() const { return end_; }

    const_iterator end() { return end_; }

    // size and capacity
    size_type size() const { return end_ - beg_; }

    size_type capacity() const { return cap_ - beg_; }

    bool empty() const { return begin() == end(); }

    void resize(size_type new_size) {
        if (size() > new_size) {
            end_ -= size() - new_size;
        }
        else if (size() < new_size) {
            Vector<T> container(new_size);
            std::copy(begin(), end(), container.begin());
            // use swap to simulate a resize
            swap(container);
        }
    }

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
    void push_back(const T& e) {
        if (capacity() <= size())
            reserve(empty() ? 8 : 2 * size());
        beg_[size()] = e;
        ++end_;
    }

    void pop_back() {
        if (empty())
            throw std::out_of_range{"Vector::pop_back: Vector::empty == true"};
        beg_[size() - 1].~T();
        --end_;
    }

    void clear() {
        delete[] beg_;
        beg_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
    }

    void swap(Vector& other) { std::swap(*this, other); }

    friend void swap(Vector& a, Vector& b) { std::swap(a, b); }

private:
    T* beg_{};
    T* end_{};
    T* cap_{};
};
} // namespace bb