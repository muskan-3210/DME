#pragma once
// Include guard: ensures this file is compiled only once per translation unit.
#include "../array/dynamic_array.h"
// Includes DynamicArray<T> from the sibling directory; Stack is implemented on top of it.
// Cross-file dependency: stack_array.h requires dynamic_array.h to be compiled first.
#include <stdexcept>
// Provides std::underflow_error, thrown when pop() or top() is called on an empty stack.

template<typename T>
// Template parameter T: the value type the stack will hold; matches DynamicArray<T>'s T.
class Stack {
// A LIFO (Last-In First-Out) data structure backed by DynamicArray<T>.
// Using an existing data structure rather than raw pointers is an example of composition.
private:
// Private section: internal storage hidden from callers.
    DynamicArray<T> data_;
// The underlying DynamicArray stores all stack elements; top is always data_[size-1].
// Defined in dynamic_array.h — that's the cross-file dependency noted in the include above.

public:
// Public LIFO interface.
    void push(const T& val) { data_.push_back(val); }
// Pushes val onto the top of the stack by appending to the end of the DynamicArray. O(amortized 1).

    void pop() {
// Removes the top element; throws if the stack is empty.
        if (empty()) throw std::underflow_error("Stack::pop on empty stack");
// Throwing here (instead of silently returning) makes misuse visible immediately.
        data_.pop_back();
// DynamicArray::pop_back() decrements size_ in O(1); no memory is freed.
    }
// End of pop().

    T& top() {
// Returns a mutable reference to the top element without removing it.
        if (empty()) throw std::underflow_error("Stack::top on empty stack");
// Throw on empty to prevent undefined behavior from accessing data_[-1] effectively.
        return data_[data_.size() - 1];
// The top element lives at the last valid index. operator[] on DynamicArray returns a reference.
    }
// End of non-const top().

    const T& top() const {
// Const overload of top(): called on const Stack objects; prevents modification of the top element.
        if (empty()) throw std::underflow_error("Stack::top on empty stack");
// Same guard as the non-const version.
        return data_[data_.size() - 1];
// Returns const T& — caller can read but not modify the top element through this overload.
    }
// End of const top().

    bool   empty()    const { return data_.empty(); }
// Delegates to DynamicArray::empty(); returns true if no elements are on the stack.
    size_t size()     const { return data_.size(); }
// Returns the number of elements currently on the stack via DynamicArray::size().
    void   clear()          { data_.clear(); }
// Resets the stack to empty in O(1) by calling DynamicArray::clear(); heap memory is retained.
};
// End of Stack class.
