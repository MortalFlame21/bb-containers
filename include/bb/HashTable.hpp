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
    using iterator = bb::HashTableIterator<value_type>;
    using const_iterator = const bb::HashTableIterator<const value_type>;
    using local_iterator = bb::List<value_type>::iterator;
    using const_local_iterator = bb::List<value_type>::const_iterator;
    using hasher = H;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // ctor and dtor
    HashTable() = default;

    HashTable(std::initializer_list<const value_type> init)
        : buckets_(init.size())
    {
        for (const auto& vt : init) {
            insert(vt);
        }
    };

    // iterators

    /// @brief
    /// @return iterator to first element in bucket
    iterator begin() {
        if (empty()) return iterator(local_iterator{nullptr}, local_iterator{nullptr}, buckets_.begin());
        return iterator((*chains_.begin())->begin(), (*chains_.begin())->end(), buckets_.begin());
    }

    iterator begin() const {
        if (empty()) return iterator(local_iterator{nullptr}, local_iterator{nullptr}, buckets_.begin());
        return iterator((*chains_.begin())->begin(), (*chains_.begin())->end(), buckets_.begin());
    }

    /// @brief
    /// @return iterator past last element in bucket
    iterator end() {
        if (empty()) return iterator(local_iterator{nullptr}, local_iterator{nullptr}, buckets_.end());
        return iterator((*std::prev(chains_.end()))->end(), (*std::prev(chains_.end()))->end(), buckets_.end());
    }

    iterator end() const {
        if (empty()) return iterator(local_iterator{nullptr}, local_iterator{nullptr}, buckets_.end());
        return iterator((*std::prev(chains_.end()))->end(), (*std::prev(chains_.end()))->end(), buckets_.end());
    }

    // capacity

    bool empty() const {
        return size() == 0;
    }

    size_type size() const {
        return value_sz_;
    }

    // modifiers

    /// @brief Inserts `v` into HashTable.
    /// @param v
    /// @return Returns iterator to position of inserted v, bool if insert took place.
    std::pair<iterator, bool> insert(const value_type& v) {
        bool inserted{};
        auto h{hash(v.first)};
        auto& chain{buckets_[h]};
        iterator it{end()};

        for (auto i{chain.begin()}; i != chain.end(); ++i) {
            if ((*i).first == v.first) {
                (*i).second = v.second;
                it = iterator{i, chain.end(), buckets_.begin() + h}; // dawg
                break;
            }
        }

        if (!inserted) {
            // brudda...
            it = {chain.insert(chain.begin(), v), chain.end(), buckets_.begin() + h};
            chains_[h] = &chain; // replace old chain, doesn't matter
            inserted = true;
        }

        ++value_sz_;
        return {it, inserted};
    }

    // lookup

    /// @brief Access element of key_type k with range check.
    /// @param k
    /// @return mapped_type reference.
    // mapped_type at(key_type k) {
    //     if (auto it{find(k)}; it != end())
    //         return *it;
    //     else
    //         throw std::out_of_range{"HashTable::at: key_type k does not exist."};
    // }

    /// @brief Access element of key_type k, or insert if not exist.
    /// @param k
    /// @return mapped_type reference.
    mapped_type operator[](key_type k) {
        if (auto it{find(k)}; it == end())
            return (*insert({k, mapped_type{}}).first).second;
        else
            return (*it).second;
    }

    /// @brief Finds element of key_type to k.
    /// @param k
    /// @return Iterator to value of key k.
    iterator find(const key_type k) {
        iterator it{end()}; // end() means we didn't find it
        auto& chain{buckets_[hash(k)]};
        for (auto i{chain.begin()}; i != chain.end(); ++i) {
            if ((*i).first == k) {
                it = iterator{i, chain.end(), buckets_.begin() + hash(k)};
                break;
            }
        }
        return it;
    }

    /// @brief Checks if element of key_type k exists.
    /// @param k
    /// @return Return bool if element of key_type k exists.
    // bool contains(const key_type k) const {
    //     return find() != end();
    // }

    // bucket interface

    /// @brief Return bucket size of HashTable.
    /// @return Bucket size of HashTable.
    size_type bucket_count() const {
        return buckets_.size();
    }

    // local_iterator begin(size_type i) {
    //     return buckets[i].begin();
    // }

    // local_iterator end(size_type i) {
    //     return buckets[i].end();
    // }
private:

    /// @brief Hash key_type k
    /// @param k
    /// @return A size_type between [0, size()).
    size_type hash(key_type k) {
        return hasher{}(k) % buckets_.size();
    }

    // probably have member variable be an iterator/pointer to first element
    // this could make begin be quicker?

    bb::Vector<bb::List<value_type>> buckets_{1}; // init with 1 bucket
    bb::Vector<bb::List<value_type>*> chains_{1};
    size_type value_sz_{};
};
} // namespace bb