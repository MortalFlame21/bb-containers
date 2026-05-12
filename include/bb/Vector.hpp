#include <vector>

namespace bb {
template<typename T>
class Vector {
public:
    Vector() = default;

    explicit Vector(size_t size)
        : beg_{new T[size]}
        , end_{beg_ + size}
        , cap_{end_} {}

    ~Vector() { delete beg_; }

    size_t size() { return end_ - beg_; }
    size_t capacity() { return cap_ - beg_; }
    bool empty() { return size() == 0; }
private:
    T* beg_{};
    T* end_{};
    T* cap_{};
};
} // namespace bb