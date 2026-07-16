#pragma once
// Include guard: ensures this header is processed only once per compilation unit.
#include <cstddef>
// Provides size_t for counting and sizing.

template<typename T>
// Template parameter T: the value type stored in each node.
struct CNode {
// Circular-list node (all members public). 'C' prefix distinguishes it from Node and DNode.
    T      data;
// The payload value at this node.
    CNode* next;
// Pointer to the next node; in a circular list this is NEVER nullptr — it wraps back to the head.
    explicit CNode(const T& val) : data(val), next(nullptr) {}
// Constructor: initializes data and temporarily sets next to nullptr (caller wires up the circle).
// 'explicit' prevents implicit conversion from a T value to a CNode<T>.
};
// End of CNode struct.

template<typename T>
// Template parameter T for the list class.
class CircularLinkedList {
// A singly-linked circular list where the tail's next pointer always points back to the head.
// Keeping only a tail_ pointer (not head_) is a common trick: head is always tail_->next.
private:
// Private implementation details.
    CNode<T>* tail_;   // tail->next == head
// The single tracked pointer; tail_->next gives the head in O(1). Comment restates the invariant.
    size_t    size_;
// Cached node count; maintained for O(1) size().

public:
// Public interface.
    CircularLinkedList() : tail_(nullptr), size_(0) {}
// Default constructor: empty list (tail_ == nullptr means no nodes exist).
    ~CircularLinkedList() {
// Destructor: breaks the circle, then deletes each node.
        if (!tail_) return;
// Guard: already empty — nothing to free.
        CNode<T>* head = tail_->next;
// Save the head pointer before we break the circle.
        CNode<T>* cur  = head;
// Start traversal at the head.
        tail_->next = nullptr;
// Break the circle: set tail_->next to nullptr so the while loop below terminates at the old tail.
        while (cur) { CNode<T>* nxt = cur->next; delete cur; cur = nxt; }
// Walk from head to the end (now nullptr) and free each node.
    }
// End of destructor.

    void insert_tail(const T& val) {
// Inserts a new node at the tail of the circle in O(1).
        CNode<T>* n = new CNode<T>(val);
// Allocate the new node; n->next starts as nullptr (constructor).
        if (!tail_) {
// Special case: list was empty — the new node points to itself to form a one-element circle.
            tail_ = n;
// The new node is the tail.
            tail_->next = tail_;
// Point the single node's next back to itself; circle invariant established.
        } else {
// General case: splice the new node in after tail_ and before the current head.
            n->next    = tail_->next;
// New node's next points to the current head (tail_->next).
            tail_->next = n;
// Old tail now points to the new node.
            tail_       = n;
// New node becomes the new tail.
        }
// End of empty vs non-empty branch.
        ++size_;
// Update the cached count.
    }
// End of insert_tail().

    void rotate() {
// Advances the logical head by one position; effectively moves tail_ forward around the circle.
        if (tail_) tail_ = tail_->next; // advance head becomes new tail context
// If non-empty, advance tail_ one step; the new tail_->next is the new head.
// In-line comment restates the semantic: the old head becomes the new tail.
    }
// End of rotate().

    CNode<T>* head() const { return tail_ ? tail_->next : nullptr; }
// Returns the head pointer (tail_->next) or nullptr if the list is empty.
    CNode<T>* tail() const { return tail_; }
// Returns the tail pointer; nullptr when the list is empty.
    size_t    size() const { return size_; }
// Returns the cached node count in O(1).
    bool      empty()const { return size_ == 0; }
// Returns true iff the list has no nodes.

    void delete_head() {
// Removes and frees the head node (tail_->next) in O(1).
        if (!tail_) return;
// Guard: empty list.
        if (tail_->next == tail_) {
// Special case: only one node in the circle — it points to itself.
            delete tail_;
// Free the single node.
            tail_ = nullptr;
// List is now empty.
        } else {
// General case: more than one node.
            CNode<T>* old = tail_->next;
// Save the current head node for deletion.
            tail_->next   = old->next;
// Tail now points to what was the second node (new head).
            delete old;
// Free the old head.
        }
// End of single-node vs multi-node branch.
        --size_;
// Decrement the cached count.
    }
// End of delete_head().
};
// End of CircularLinkedList class.
