#pragma once
#include <unordered_map>
#include <list>
#include <stdexcept>
#include <cstddef>

namespace DME {

template<typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(size_t capacity);

    bool  get(const K& key, V& out_value);
    void  put(const K& key, const V& value);
    bool  contains(const K& key) const;
    size_t size()     const;
    size_t capacity() const;
    void  clear();

private:
    size_t capacity_;

    // list: front = most recent, back = least recent
    std::list<std::pair<K, V>> list_;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> map_;

    void evict();
};

template<typename K, typename V>
LRUCache<K,V>::LRUCache(size_t capacity) : capacity_(capacity) {}

template<typename K, typename V>
bool LRUCache<K,V>::get(const K& key, V& out_value) {
    auto it = map_.find(key);
    if (it == map_.end()) return false;
    list_.splice(list_.begin(), list_, it->second);
    out_value = it->second->second;
    return true;
}

template<typename K, typename V>
void LRUCache<K,V>::put(const K& key, const V& value) {
    auto it = map_.find(key);
    if (it != map_.end()) {
        it->second->second = value;
        list_.splice(list_.begin(), list_, it->second);
        return;
    }
    if (list_.size() == capacity_) evict();
    list_.emplace_front(key, value);
    map_[key] = list_.begin();
}

template<typename K, typename V>
bool LRUCache<K,V>::contains(const K& key) const {
    return map_.find(key) != map_.end();
}

template<typename K, typename V>
size_t LRUCache<K,V>::size() const { return list_.size(); }

template<typename K, typename V>
size_t LRUCache<K,V>::capacity() const { return capacity_; }

template<typename K, typename V>
void LRUCache<K,V>::clear() {
    list_.clear();
    map_.clear();
}

template<typename K, typename V>
void LRUCache<K,V>::evict() {
    if (list_.empty()) return;
    map_.erase(list_.back().first);
    list_.pop_back();
}

} // namespace DME
