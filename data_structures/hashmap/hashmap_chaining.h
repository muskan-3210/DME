#pragma once
#include <vector>
#include <list>
#include <functional>
#include <cstddef>
#include <stdexcept>

template<typename K, typename V>
class HashMapChaining {
private:
    struct Entry { K key; V value; };
    std::vector<std::list<Entry>> buckets_;
    size_t size_;
    size_t bucket_count_;

    size_t bucket_index(const K& key) const {
        return std::hash<K>{}(key) % bucket_count_;
    }

public:
    explicit HashMapChaining(size_t bucket_count = 16)
        : buckets_(bucket_count), size_(0), bucket_count_(bucket_count) {}

    void insert(const K& key, const V& value) {
        size_t idx = bucket_index(key);
        for (auto& e : buckets_[idx]) {
            if (e.key == key) { e.value = value; return; }
        }
        buckets_[idx].push_back({key, value});
        ++size_;
    }

    bool get(const K& key, V& out) const {
        size_t idx = bucket_index(key);
        for (const auto& e : buckets_[idx])
            if (e.key == key) { out = e.value; return true; }
        return false;
    }

    bool remove(const K& key) {
        size_t idx = bucket_index(key);
        auto& lst  = buckets_[idx];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it->key == key) { lst.erase(it); --size_; return true; }
        }
        return false;
    }

    bool   contains(const K& key) const { V v; return get(key, v); }
    size_t size()                  const { return size_; }
    bool   empty()                 const { return size_ == 0; }
};
