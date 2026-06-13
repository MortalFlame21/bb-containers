#include <cstddef> // std::size_t, std::ptrdiff_t
#include <initializer_list> // std::initializer_list
#include <memory> // std::hash

#include "Iterator.hpp"
#include "List.hpp"
#include "Vector.hpp"

namespace bb {

template<typename K, typename T, typename H = std::hash<K>>
class HashTable {
public:
    // member types
    using key_type = K;
    using mapped_type = T;
    using value_type = std::pair<const key_type, T>;
    using reference = value_type&;
    using const_reference = const value_type&;
    // using iterator = ForwardIterator<value_type>;
    // using const_iterator = const ForwardIterator<const value_type>;
    using hasher = H;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // ctor and dtor
    HashTable() = default;

    HashTable(std::initializer_list<value_type> init)
        : buckets_(init.size())
    {
        for (const auto& vt : init) {
            auto i{hasher()(vt.first) % buckets_.size()};
            buckets_[i] = vt.second;
        }
    };

    // iterators
    // capacity
    // modifiers

    // lookup

    mapped_type& operator[](key_type k) {
        auto i{hasher()(k) % buckets_.size()};
        return buckets_[i];
    }

    // bucket interface
private:
    bb::Vector<mapped_type> buckets_{};
};
} // namespace bb