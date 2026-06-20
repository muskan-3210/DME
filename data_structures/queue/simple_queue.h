#pragma once
#include <deque>
#include <stdexcept>
#include <cstddef>

template<typename T>
class SimpleQueue {
private:
    std::deque<T> data_;

public:
    void enqueue(const T& val) { data_.push_back(val); }

    void dequeue() {
        if (empty()) throw std::underflow_error("Queue::dequeue on empty queue");
        data_.pop_front();
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Queue::front on empty queue");
        return data_.front();
    }

    const T& back() const {
        if (empty()) throw std::underflow_error("Queue::back on empty queue");
        return data_.back();
    }

    bool   empty() const { return data_.empty(); }
    size_t size()  const { return data_.size(); }
    void   clear()       { data_.clear(); }
};
