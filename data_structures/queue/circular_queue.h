#pragma once
// Include guard: ensures the file is compiled only once per translation unit.
#include <vector>
// Includes std::vector, used as the fixed-size backing array for the circular buffer.
#include <stdexcept>
// Provides std::overflow_error and std::underflow_error for full/empty conditions.
#include <cstddef>
// Provides size_t for indices and counts.

template<typename T>
// Template parameter T: the type of elements stored in the circular queue.
class CircularQueue {
// A fixed-capacity FIFO queue implemented as a circular buffer.
// Circular buffers reuse array slots by wrapping indices with modulo arithmetic — no shifting needed.
private:
// Private implementation details.
    std::vector<T> data_;
// Backing array of size capacity_+1; the +1 extra slot is used to distinguish full from empty.
    size_t         head_;
// Index of the front element (next to be dequeued); wraps around using modulo.
    size_t         tail_;
// Index where the next element will be written; wraps around using modulo.
    size_t         size_;
// Current number of stored elements; maintained for O(1) full/empty checks.
    size_t         capacity_;
// Maximum number of elements the queue can hold (one less than data_.size()).

public:
// Public interface.
    explicit CircularQueue(size_t capacity)
// Constructor requires a capacity; 'explicit' prevents accidental implicit conversion from size_t.
        : data_(capacity + 1), head_(0), tail_(0), size_(0), capacity_(capacity) {}
// Allocate capacity+1 slots (extra slot differentiates full from empty), initialize indices to 0.

    void enqueue(const T& val) {
// Adds val to the back of the queue; throws if the queue is at capacity.
        if (full()) throw std::overflow_error("CircularQueue: full");
// Throw on full to prevent overwriting unread elements; this queue has a fixed capacity.
        data_[tail_] = val;
// Write val into the slot at tail_.
        tail_ = (tail_ + 1) % (capacity_ + 1);
// Advance tail_ and wrap around using modulo; after the last slot the index returns to 0.
        ++size_;
// Increment the element count.
    }
// End of enqueue().

    void dequeue() {
// Removes the front element; throws if the queue is empty.
        if (empty()) throw std::underflow_error("CircularQueue: empty");
// Guard against dequeuing from an empty queue.
        head_ = (head_ + 1) % (capacity_ + 1);
// Advance head_ and wrap around; the slot is logically freed for future enqueues.
        --size_;
// Decrement the element count.
    }
// End of dequeue().

    const T& front() const {
// Returns a const reference to the front element without removing it.
        if (empty()) throw std::underflow_error("CircularQueue: empty");
// Guard: accessing data_[head_] on an empty queue would return garbage.
        return data_[head_];
// head_ always points to the oldest (front) element.
    }
// End of front().

    bool   empty()    const { return size_ == 0; }
// Returns true if no elements are stored. Using size_ avoids the head_==tail_ ambiguity.
    bool   full()     const { return size_ == capacity_; }
// Returns true if size_ has reached the capacity limit; checked before every enqueue.
    size_t size()     const { return size_; }
// Returns the current number of stored elements.
    size_t capacity() const { return capacity_; }
// Returns the fixed maximum capacity set at construction.
};
// End of CircularQueue class.
