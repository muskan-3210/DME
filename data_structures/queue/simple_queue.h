#pragma once
// Include guard: prevents this header from being processed more than once per translation unit.
#include <deque>
// Includes std::deque from the C++ standard library; used as the backing store for SimpleQueue.
// std::deque supports O(1) push_back and pop_front — exactly what a queue needs.
#include <stdexcept>
// Provides std::underflow_error, thrown when dequeue/front/back is called on an empty queue.
#include <cstddef>
// Provides size_t for the size() return type.

template<typename T>
// Template parameter T: the value type the queue holds.
class SimpleQueue {
// A FIFO (First-In First-Out) queue backed by std::deque<T>.
// This is a thin wrapper: all the complexity lives inside std::deque.
private:
// Private section: the backing container.
    std::deque<T> data_;
// std::deque allows O(1) insertion at the back and O(1) removal from the front — ideal for FIFO.

public:
// Public FIFO interface.
    void enqueue(const T& val) { data_.push_back(val); }
// Adds val to the back of the queue in O(1). "enqueue" is the domain-specific name for push_back.

    void dequeue() {
// Removes the front element (oldest element); throws if empty.
        if (empty()) throw std::underflow_error("Queue::dequeue on empty queue");
// Guard: throw instead of undefined behavior (deque::pop_front on empty is UB).
        data_.pop_front();
// std::deque::pop_front removes the first element in O(1).
    }
// End of dequeue().

    const T& front() const {
// Returns a const reference to the front (oldest) element without removing it.
        if (empty()) throw std::underflow_error("Queue::front on empty queue");
// Throw on empty to prevent a dangling reference.
        return data_.front();
// std::deque::front() returns a reference to the first element.
    }
// End of front().

    const T& back() const {
// Returns a const reference to the back (newest) element without removing it.
        if (empty()) throw std::underflow_error("Queue::back on empty queue");
// Guard against accessing an empty deque.
        return data_.back();
// std::deque::back() returns a reference to the last element.
    }
// End of back().

    bool   empty() const { return data_.empty(); }
// Returns true if the queue holds no elements; delegates to std::deque::empty().
    size_t size()  const { return data_.size(); }
// Returns the number of elements currently in the queue.
    void   clear()       { data_.clear(); }
// Removes all elements from the queue; the backing deque is reset to empty.
};
// End of SimpleQueue class.
