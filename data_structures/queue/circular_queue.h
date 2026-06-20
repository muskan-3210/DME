#pragma once
#include <vector>
#include <stdexcept>
#include <cstddef>

template<typename T>
class CircularQueue {
private:
    std::vector<T> data_;
    size_t         head_;
    size_t         tail_;
    size_t         size_;
    size_t         capacity_;

public:
    explicit CircularQueue(size_t capacity)
        : data_(capacity + 1), head_(0), tail_(0), size_(0), capacity_(capacity) {}

    void enqueue(const T& val) {
        if (full()) throw std::overflow_error("CircularQueue: full");
        data_[tail_] = val;
        tail_ = (tail_ + 1) % (capacity_ + 1);
        ++size_;
    }

    void dequeue() {
        if (empty()) throw std::underflow_error("CircularQueue: empty");
        head_ = (head_ + 1) % (capacity_ + 1);
        --size_;
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("CircularQueue: empty");
        return data_[head_];
    }

    bool   empty()    const { return size_ == 0; }
    bool   full()     const { return size_ == capacity_; }
    size_t size()     const { return size_; }
    size_t capacity() const { return capacity_; }
};
