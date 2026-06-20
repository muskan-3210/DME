#pragma once
#include "../array/dynamic_array.h"
#include <stdexcept>

template<typename T>
class Stack {
private:
    DynamicArray<T> data_;

public:
    void push(const T& val) { data_.push_back(val); }

    void pop() {
        if (empty()) throw std::underflow_error("Stack::pop on empty stack");
        data_.pop_back();
    }

    T& top() {
        if (empty()) throw std::underflow_error("Stack::top on empty stack");
        return data_[data_.size() - 1];
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("Stack::top on empty stack");
        return data_[data_.size() - 1];
    }

    bool   empty()    const { return data_.empty(); }
    size_t size()     const { return data_.size(); }
    void   clear()          { data_.clear(); }
};
