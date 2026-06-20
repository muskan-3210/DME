#pragma once
#include <vector>
#include <functional>
#include <optional>
#include <cstddef>

template<typename K, typename V>
class HashMapOpenAddressing {
private:
    enum class State { EMPTY, OCCUPIED, DELETED };
    struct Slot { K key; V value; State state = State::EMPTY; };

    std::vector<Slot> table_;
    size_t size_;
    size_t capacity_;

    size_t probe(const K& key, size_t i) const {
        return (std::hash<K>{}(key) + i) % capacity_;
    }

    void rehash() {
        auto old = table_;
        capacity_ *= 2;
        table_.assign(capacity_, Slot{});
        size_ = 0;
        for (auto& s : old)
            if (s.state == State::OCCUPIED) insert(s.key, s.value);
    }

public:
    explicit HashMapOpenAddressing(size_t cap = 16)
        : table_(cap), size_(0), capacity_(cap) {}

    void insert(const K& key, const V& value) {
        if (size_ * 2 >= capacity_) rehash();
        for (size_t i = 0; i < capacity_; ++i) {
            size_t idx = probe(key, i);
            if (table_[idx].state == State::EMPTY || table_[idx].state == State::DELETED) {
                table_[idx] = {key, value, State::OCCUPIED};
                ++size_;
                return;
            }
            if (table_[idx].key == key) { table_[idx].value = value; return; }
        }
    }

    bool get(const K& key, V& out) const {
        for (size_t i = 0; i < capacity_; ++i) {
            size_t idx = probe(key, i);
            if (table_[idx].state == State::EMPTY) return false;
            if (table_[idx].state == State::OCCUPIED && table_[idx].key == key) {
                out = table_[idx].value;
                return true;
            }
        }
        return false;
    }

    bool remove(const K& key) {
        for (size_t i = 0; i < capacity_; ++i) {
            size_t idx = probe(key, i);
            if (table_[idx].state == State::EMPTY) return false;
            if (table_[idx].state == State::OCCUPIED && table_[idx].key == key) {
                table_[idx].state = State::DELETED;
                --size_;
                return true;
            }
        }
        return false;
    }

    size_t size()  const { return size_; }
    bool   empty() const { return size_ == 0; }
};
