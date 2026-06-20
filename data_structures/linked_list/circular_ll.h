#pragma once
#include <cstddef>

template<typename T>
struct CNode {
    T      data;
    CNode* next;
    explicit CNode(const T& val) : data(val), next(nullptr) {}
};

template<typename T>
class CircularLinkedList {
private:
    CNode<T>* tail_;   // tail->next == head
    size_t    size_;

public:
    CircularLinkedList() : tail_(nullptr), size_(0) {}
    ~CircularLinkedList() {
        if (!tail_) return;
        CNode<T>* head = tail_->next;
        CNode<T>* cur  = head;
        tail_->next = nullptr;
        while (cur) { CNode<T>* nxt = cur->next; delete cur; cur = nxt; }
    }

    void insert_tail(const T& val) {
        CNode<T>* n = new CNode<T>(val);
        if (!tail_) {
            tail_ = n;
            tail_->next = tail_;
        } else {
            n->next    = tail_->next;
            tail_->next = n;
            tail_       = n;
        }
        ++size_;
    }

    void rotate() {
        if (tail_) tail_ = tail_->next; // advance head becomes new tail context
    }

    CNode<T>* head() const { return tail_ ? tail_->next : nullptr; }
    CNode<T>* tail() const { return tail_; }
    size_t    size() const { return size_; }
    bool      empty()const { return size_ == 0; }

    void delete_head() {
        if (!tail_) return;
        if (tail_->next == tail_) {
            delete tail_;
            tail_ = nullptr;
        } else {
            CNode<T>* old = tail_->next;
            tail_->next   = old->next;
            delete old;
        }
        --size_;
    }
};
