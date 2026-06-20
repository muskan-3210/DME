#pragma once
#include <cstddef>
#include <stdexcept>

template<typename T>
class DynamicArray {
private:
    T*     data_;
    size_t size_;
    size_t capacity_;

public:
    DynamicArray();
    explicit DynamicArray(size_t initial_capacity);
    ~DynamicArray();

    void push_back(const T& value);
    void pop_back();

    T&       operator[](size_t index);
    const T& operator[](size_t index) const;

    T&       at(size_t index);
    const T& at(size_t index) const;

    size_t size()     const;
    size_t capacity() const;
    bool   empty()    const;

    void clear();
    void reserve(size_t new_capacity);
};

// --- Constructors & Destructor ---

template<typename T>
DynamicArray<T>::DynamicArray()
    : data_(nullptr), size_(0), capacity_(0) {}

template<typename T>
DynamicArray<T>::DynamicArray(size_t initial_capacity)
    : data_(nullptr), size_(0), capacity_(0) {
    if (initial_capacity > 0)
        reserve(initial_capacity);
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data_;
}

// --- Core Operations ---

template<typename T>
void DynamicArray<T>::reserve(size_t new_capacity) {
    if (new_capacity <= capacity_) return;

    T* new_data = new T[new_capacity];

    for (size_t i = 0; i < size_; ++i)
        new_data[i] = data_[i];

    delete[] data_;

    data_     = new_data;
    capacity_ = new_capacity;
}

template<typename T>
void DynamicArray<T>::push_back(const T& value) {
    if (size_ == capacity_)
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    data_[size_++] = value;
}

template<typename T>
void DynamicArray<T>::pop_back() {
    if (size_ == 0) return;
    --size_;
}

template<typename T>
void DynamicArray<T>::clear() {
    size_ = 0;
}

// --- Access ---

template<typename T>
T& DynamicArray<T>::operator[](size_t index) {
    return data_[index];
}

template<typename T>
const T& DynamicArray<T>::operator[](size_t index) const {
    return data_[index];
}

template<typename T>
T& DynamicArray<T>::at(size_t index) {
    if (index >= size_) throw std::out_of_range("DynamicArray::at — index out of range");
    return data_[index];
}

template<typename T>
const T& DynamicArray<T>::at(size_t index) const {
    if (index >= size_) throw std::out_of_range("DynamicArray::at — index out of range");
    return data_[index];
}

// --- Queries ---

template<typename T>
size_t DynamicArray<T>::size()     const { return size_; }

template<typename T>
size_t DynamicArray<T>::capacity() const { return capacity_; }

template<typename T>
bool   DynamicArray<T>::empty()    const { return size_ == 0; }
