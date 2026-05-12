#include <algorithm>
#include <initializer_list>

namespace bb {
template<typename T>
class Vector {
public:
    // ctor, move and destruction
    Vector() = default;

    explicit Vector(size_t size)
        : beg_{new T[size]}
        , end_{beg_ + size}
        , cap_{end_} {}

    Vector(std::initializer_list<T> list)
        : beg_{new T[list.size()]}
        , end_{beg_ + list.size()}
        , cap_{end_}
    { std::copy(list.begin(), list.end(), beg_); }

    Vector(const Vector& other)
        : beg_{new T[other.size()]}
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
        return this*;
    }

    ~Vector() { delete[] beg_; }

    // iterators
    T* begin() const { return beg_; }

    T* end() const { return end_; }

    // element access

    // size and capacity
    size_t size() { return end_ - beg_; }
    size_t capacity() { return cap_ - beg_; }
    bool empty() { return size() == 0; }
private:
    T* beg_{};
    T* end_{};
    T* cap_{};
};
} // namespace bb