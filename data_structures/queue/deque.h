#pragma once
#include <vector>
#include <stdexcept>
#include <cstddef>

template<typename T>
class Deque {
private:
    std::vector<T> data_;
    size_t         head_;
    size_t         tail_;
    size_t         size_;
    size_t         capacity_;

    void grow() {
        size_t new_cap = capacity_ * 2 + 1;
        std::vector<T> tmp(new_cap);
        for (size_t i = 0; i < size_; ++i)
            tmp[i] = data_[(head_ + i) % (capacity_ + 1)];
        data_     = std::move(tmp);
        head_     = 0;
        tail_     = size_;
        capacity_ = new_cap - 1;
    }

public:
    explicit Deque(size_t initial_cap = 8)
        : data_(initial_cap + 1), head_(0), tail_(0), size_(0), capacity_(initial_cap) {}

    void push_front(const T& val) {
        if (size_ == capacity_) grow();
        head_ = (head_ + capacity_) % (capacity_ + 1);
        data_[head_] = val;
        ++size_;
    }

    void push_back(const T& val) {
        if (size_ == capacity_) grow();
        data_[tail_] = val;
        tail_ = (tail_ + 1) % (capacity_ + 1);
        ++size_;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("Deque: empty");
        head_ = (head_ + 1) % (capacity_ + 1);
        --size_;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Deque: empty");
        tail_ = (tail_ + capacity_) % (capacity_ + 1);
        --size_;
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Deque: empty");
        return data_[head_];
    }

    const T& back() const {
        if (empty()) throw std::underflow_error("Deque: empty");
        return data_[(tail_ + capacity_) % (capacity_ + 1)];
    }

    bool   empty() const { return size_ == 0; }
    size_t size()  const { return size_; }
};
