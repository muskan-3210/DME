#pragma once
#include <vector>
#include <stdexcept>
#include <functional>

template<typename T, typename Compare = std::less<T>>
class Heap {
private:
    std::vector<T> data_;
    Compare        cmp_;

    void sift_up(size_t i) {
        while (i > 0) {
            size_t parent = (i - 1) / 2;
            if (cmp_(data_[parent], data_[i])) {
                std::swap(data_[i], data_[parent]);
                i = parent;
            } else break;
        }
    }

    void sift_down(size_t i) {
        size_t n = data_.size();
        while (true) {
            size_t largest = i;
            size_t left    = 2 * i + 1;
            size_t right   = 2 * i + 2;
            if (left  < n && cmp_(data_[largest], data_[left]))  largest = left;
            if (right < n && cmp_(data_[largest], data_[right])) largest = right;
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
    }

public:
    void   push(const T& val) { data_.push_back(val); sift_up(data_.size() - 1); }
    void   pop()  {
        if (empty()) throw std::underflow_error("Heap: empty");
        data_[0] = data_.back(); data_.pop_back();
        if (!empty()) sift_down(0);
    }
    const T& top()  const {
        if (empty()) throw std::underflow_error("Heap: empty");
        return data_[0];
    }
    bool   empty() const { return data_.empty(); }
    size_t size()  const { return data_.size(); }
};

template<typename T> using MaxHeap = Heap<T, std::less<T>>;
template<typename T> using MinHeap = Heap<T, std::greater<T>>;
