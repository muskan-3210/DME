#pragma once
#include <unordered_map>
#include <list>
#include <cstddef>

namespace DME {

template<typename K, typename V>
class LFUCache {
public:
    explicit LFUCache(size_t capacity);

    bool  get(const K& key, V& out_value);
    void  put(const K& key, const V& value);
    bool  contains(const K& key) const;
    size_t size()     const;
    size_t capacity() const;

private:
    struct Entry {
        K key;
        V value;
        int freq;
    };

    size_t capacity_;
    size_t size_;
    int    min_freq_;

    std::unordered_map<K, typename std::list<Entry>::iterator> key_map_;
    std::unordered_map<int, std::list<Entry>>                  freq_map_;

    void increment_freq(typename std::list<Entry>::iterator it);
    void evict();
};

template<typename K, typename V>
LFUCache<K,V>::LFUCache(size_t capacity)
    : capacity_(capacity), size_(0), min_freq_(0) {}

template<typename K, typename V>
bool LFUCache<K,V>::get(const K& key, V& out_value) {
    auto it = key_map_.find(key);
    if (it == key_map_.end()) return false;
    out_value = it->second->value;
    increment_freq(it->second);
    return true;
}

template<typename K, typename V>
void LFUCache<K,V>::put(const K& key, const V& value) {
    if (capacity_ == 0) return;

    auto it = key_map_.find(key);
    if (it != key_map_.end()) {
        it->second->value = value;
        increment_freq(it->second);
        return;
    }

    if (size_ == capacity_) evict();

    freq_map_[1].push_front({key, value, 1});
    key_map_[key] = freq_map_[1].begin();
    min_freq_ = 1;
    ++size_;
}

template<typename K, typename V>
bool LFUCache<K,V>::contains(const K& key) const {
    return key_map_.find(key) != key_map_.end();
}

template<typename K, typename V>
size_t LFUCache<K,V>::size() const { return size_; }

template<typename K, typename V>
size_t LFUCache<K,V>::capacity() const { return capacity_; }

template<typename K, typename V>
void LFUCache<K,V>::increment_freq(typename std::list<Entry>::iterator it) {
    int old_freq = it->freq;
    Entry entry  = *it;
    entry.freq++;

    freq_map_[old_freq].erase(it);
    if (freq_map_[old_freq].empty()) {
        freq_map_.erase(old_freq);
        if (min_freq_ == old_freq) min_freq_++;
    }

    freq_map_[entry.freq].push_front(entry);
    key_map_[entry.key] = freq_map_[entry.freq].begin();
}

template<typename K, typename V>
void LFUCache<K,V>::evict() {
    auto& lst = freq_map_[min_freq_];
    key_map_.erase(lst.back().key);
    lst.pop_back();
    if (lst.empty()) freq_map_.erase(min_freq_);
    --size_;
}

} // namespace DME
