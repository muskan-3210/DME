#pragma once
// Include guard: prevents multiple inclusion of this header.
#include <vector>
// Provides std::vector as the backing store for the circular buffer inside this Deque.
#include <stdexcept>
// Provides std::underflow_error thrown when pop or peek is called on an empty deque.
#include <cstddef>
// Provides size_t for indices and counts.

template<typename T>
// Template parameter T: the type of elements stored in the deque.
class Deque {
// A double-ended queue (deque) implemented as a growable circular buffer.
// Supports O(1) push/pop at both front and back; grows automatically unlike CircularQueue.
private:
// Private implementation details.
    std::vector<T> data_;
// Backing vector of size capacity_+1; the extra slot distinguishes full from empty.
    size_t         head_;
// Index of the front element; decrements (with wrap) on push_front.
    size_t         tail_;
// Index where the next push_back element goes; increments (with wrap) on push_back.
    size_t         size_;
// Current number of elements; used to check full/empty without head_==tail_ ambiguity.
    size_t         capacity_;
// Maximum number of elements before a grow() is needed (one less than data_.size()).

    void grow() {
// Private helper: doubles capacity when the deque is full. Called from push_front/push_back.
        size_t new_cap = capacity_ * 2 + 1;
// New capacity is 2x+1; the +1 ensures we don't reach 0 if capacity_ somehow starts at 0.
        std::vector<T> tmp(new_cap);
// Allocate a fresh contiguous block of new_cap elements.
        for (size_t i = 0; i < size_; ++i)
// Iterate over all live elements in logical order (front to back).
            tmp[i] = data_[(head_ + i) % (capacity_ + 1)];
// Map each logical index i to the circular physical index; this unwraps the circular layout.
        data_     = std::move(tmp);
// Replace data_ with the new, larger vector using move semantics (transfers ownership, no copy).
        head_     = 0;
// After linearization, the front element is at index 0.
        tail_     = size_;
// The next free slot is immediately after the last element.
        capacity_ = new_cap - 1;
// Update capacity_ to reflect the new buffer size (new_cap total slots means capacity_ = new_cap - 1).
    }
// End of grow(). After this, size_ < capacity_ so the caller can insert one more element.

public:
// Public interface.
    explicit Deque(size_t initial_cap = 8)
// Constructor with a default initial capacity of 8; 'explicit' blocks implicit size_t->Deque conversions.
        : data_(initial_cap + 1), head_(0), tail_(0), size_(0), capacity_(initial_cap) {}
// Allocate initial_cap+1 slots; initialize all indices and size to 0.

    void push_front(const T& val) {
// Inserts val at the front of the deque in O(1) amortized (O(n) on grow).
        if (size_ == capacity_) grow();
// Grow before inserting to ensure there is a free slot available.
        head_ = (head_ + capacity_) % (capacity_ + 1);
// Decrement head_ by 1 with wrap-around: adding capacity_ is equivalent to subtracting 1 mod (capacity_+1).
        data_[head_] = val;
// Write val into the newly acquired front slot.
        ++size_;
// Increment element count.
    }
// End of push_front().

    void push_back(const T& val) {
// Appends val to the back of the deque in O(1) amortized.
        if (size_ == capacity_) grow();
// Grow if the buffer is full.
        data_[tail_] = val;
// Write val at the current tail position.
        tail_ = (tail_ + 1) % (capacity_ + 1);
// Advance tail_ with wrap-around.
        ++size_;
// Increment element count.
    }
// End of push_back().

    void pop_front() {
// Removes the front element; throws if the deque is empty.
        if (empty()) throw std::underflow_error("Deque: empty");
// Guard: accessing data_[head_] on an empty deque would return garbage.
        head_ = (head_ + 1) % (capacity_ + 1);
// Advance head_ forward by one slot with wrap-around; the slot is now logically free.
        --size_;
// Decrement the element count.
    }
// End of pop_front().

    void pop_back() {
// Removes the back element; throws if the deque is empty.
        if (empty()) throw std::underflow_error("Deque: empty");
// Guard against popping from an empty deque.
        tail_ = (tail_ + capacity_) % (capacity_ + 1);
// Move tail_ back by one with wrap-around: subtracting 1 mod (capacity_+1) via adding capacity_.
        --size_;
// Decrement the element count.
    }
// End of pop_back().

    const T& front() const {
// Returns a const reference to the front element without removing it.
        if (empty()) throw std::underflow_error("Deque: empty");
// Guard: empty deque has no valid front element.
        return data_[head_];
// head_ is the physical index of the front element.
    }
// End of front().

    const T& back() const {
// Returns a const reference to the back element without removing it.
        if (empty()) throw std::underflow_error("Deque: empty");
// Guard: empty deque has no valid back element.
        return data_[(tail_ + capacity_) % (capacity_ + 1)];
// Back element is one step behind tail_; subtract 1 mod (capacity_+1) by adding capacity_.
    }
// End of back().

    bool   empty() const { return size_ == 0; }
// Returns true if the deque holds no elements.
    size_t size()  const { return size_; }
// Returns the current number of elements in the deque.
};
// End of Deque class.
