#pragma once
// Compiler directive: include this file only once per translation unit; prevents multiple-definition errors.
#include <cstddef>
// Provides size_t for element counts and index arithmetic.
#include <stdexcept>
// Provides standard exception types; included for completeness even if not directly used here.

template<typename T>
// Declares a template with type parameter T — the value type stored in each node.
struct DNode {
// Doubly-linked node struct (all members public). 'D' prefix distinguishes it from singly-linked Node.
    T      data;
// The payload value stored at this node.
    DNode* prev;
// Pointer to the previous node; nullptr when this is the head node.
    DNode* next;
// Pointer to the next node; nullptr when this is the tail node.
    explicit DNode(const T& val) : data(val), prev(nullptr), next(nullptr) {}
// Constructor: initializes data and sets both link pointers to nullptr (node starts unlinked).
// 'explicit' prevents accidental implicit construction from a T value.
};
// End of DNode struct.

template<typename T>
// Template parameter T for the list class; must match the T used in DNode<T>.
class DoublyLinkedList {
// Manages a bidirectional chain of DNode<T> objects; O(1) insert/delete at both ends.
private:
// Internal state hidden from callers.
    DNode<T>* head_;
// Pointer to the first node; nullptr when the list is empty.
    DNode<T>* tail_;
// Pointer to the last node; nullptr when the list is empty.
    size_t    size_;
// Cached node count; maintained incrementally for O(1) size().

public:
// Public interface: constructors, destructor, and operations.
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
// Inline default constructor: initializes all three members in the initializer list. Empty body.
    ~DoublyLinkedList() {
// Destructor: walks the list from head to tail and frees every node.
        DNode<T>* cur = head_;
// Start traversal at the head.
        while (cur) { DNode<T>* nxt = cur->next; delete cur; cur = nxt; }
// Save next before deleting cur; advance cur after each delete. Compact single-line loop.
    }
// End of destructor.

    void insert_head(const T& val) {
// Inserts a new node at the front of the list in O(1).
        DNode<T>* n = new DNode<T>(val);
// Heap-allocate and construct the new node; n->prev and n->next are nullptr after construction.
        n->next = head_;
// Link the new node's next to the current head (nullptr if list was empty).
        if (head_) head_->prev = n;
// If the list was non-empty, the old head's prev must point back to the new node.
        head_ = n;
// The new node is now the head.
        if (!tail_) tail_ = head_;
// If the list was empty, the new node is also the tail (only element).
        ++size_;
// Increment the cached count.
    }
// End of insert_head().

    void insert_tail(const T& val) {
// Appends a new node at the back of the list in O(1).
        DNode<T>* n = new DNode<T>(val);
// Heap-allocate the new node; both pointers start nullptr.
        n->prev = tail_;
// Link the new node's prev to the current tail.
        if (tail_) tail_->next = n;
// If the list was non-empty, the old tail's next must point to the new node.
        tail_ = n;
// The new node is now the tail.
        if (!head_) head_ = tail_;
// If the list was empty, the new node is also the head.
        ++size_;
// Increment the cached count.
    }
// End of insert_tail().

    void delete_head() {
// Removes and frees the first node in O(1) — doubly-linked lists can do this without traversal.
        if (!head_) return;
// Guard: empty list — nothing to delete.
        DNode<T>* old = head_;
// Save a pointer to the current head so we can free it after updating head_.
        head_ = head_->next;
// Advance head_ to the second node (or nullptr if this was the only node).
        if (head_) head_->prev = nullptr;
// The new head has no predecessor; clear its prev to avoid a dangling pointer.
        else       tail_ = nullptr;
// List became empty; tail_ must also be nulled out.
        delete old;
// Free the removed head node.
        --size_;
// Decrement count.
    }
// End of delete_head().

    void delete_tail() {
// Removes and frees the last node in O(1) — the prev pointer makes this trivial.
        if (!tail_) return;
// Guard: empty list.
        DNode<T>* old = tail_;
// Save the current tail for later deletion.
        tail_ = tail_->prev;
// Retreat tail_ to the second-to-last node (or nullptr if this was the only node).
        if (tail_) tail_->next = nullptr;
// The new tail has no successor; clear its next pointer.
        else       head_ = nullptr;
// List is now empty; head_ must also be nulled.
        delete old;
// Free the removed tail node.
        --size_;
// Decrement count.
    }
// End of delete_tail(). O(1) — contrast with singly-linked list where this is O(n).

    DNode<T>* head() const { return head_; }
// Read-only accessor for the head pointer; allows iteration without exposing the private member.
    DNode<T>* tail() const { return tail_; }
// Read-only accessor for the tail pointer.
    size_t    size() const { return size_; }
// Returns the cached node count in O(1).
    bool      empty()const { return size_ == 0; }
// Returns true iff the list has no nodes.

    void move_to_front(DNode<T>* node) {
// Unlinks 'node' from wherever it is and reattaches it as the new head. Used in LRU caches.
        if (!node || node == head_) return;
// Guard: null node or already at front — nothing to do.
        if (node->prev) node->prev->next = node->next;
// Patch the previous node's next pointer to skip over 'node'.
        if (node->next) node->next->prev = node->prev;
// Patch the next node's prev pointer to skip over 'node'.
        if (node == tail_) tail_ = node->prev;
// If 'node' was the tail, the node before it becomes the new tail.
        node->prev = nullptr;
// 'node' will become the new head, so it has no predecessor.
        node->next = head_;
// Link 'node' before the current head.
        if (head_) head_->prev = node;
// The old head's prev must point back to 'node'.
        head_ = node;
// 'node' is now the head of the list.
    }
// End of move_to_front(). Critical for LRU cache eviction (see cache implementations).
};
// End of DoublyLinkedList class.
