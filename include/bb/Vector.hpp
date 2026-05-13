#include <algorithm>
#include <initializer_list>
#include <stdexcept>

namespace bb {
template<typename T>
class Vector {
public:
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
    T operator[](size_t i) const { return beg_[i]; }

    T& operator[](size_t i) { return beg_[i]; }

    T at(size_t i) const {
        if (i >= size()) throw std::out_of_range{"Vector::at: i >= Vector::size"};
        return beg_[i];
    }

    T& at(size_t i) {
        if (i >= size()) throw std::out_of_range{"Vector::at: i >= Vector::size"};
        return beg_[i];
    }

    // iterators
    T* begin() const { return beg_; }

    T* const begin() { return beg_; }

    T* end() const { return end_; }

    T* const end() { return end_; }

    // size and capacity
    size_t size() const { return end_ - beg_; }

    size_t capacity() const { return cap_ - beg_; }

    bool empty() const { return size() == 0; }

    void resize(size_t new_size) {
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

    void reserve(size_t new_capacity) {
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