#pragma once
#include <cstddef>
#include <stdexcept>

template<typename T>
struct DNode {
    T      data;
    DNode* prev;
    DNode* next;
    explicit DNode(const T& val) : data(val), prev(nullptr), next(nullptr) {}
};

template<typename T>
class DoublyLinkedList {
private:
    DNode<T>* head_;
    DNode<T>* tail_;
    size_t    size_;

public:
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~DoublyLinkedList() {
        DNode<T>* cur = head_;
        while (cur) { DNode<T>* nxt = cur->next; delete cur; cur = nxt; }
    }

    void insert_head(const T& val) {
        DNode<T>* n = new DNode<T>(val);
        n->next = head_;
        if (head_) head_->prev = n;
        head_ = n;
        if (!tail_) tail_ = head_;
        ++size_;
    }

    void insert_tail(const T& val) {
        DNode<T>* n = new DNode<T>(val);
        n->prev = tail_;
        if (tail_) tail_->next = n;
        tail_ = n;
        if (!head_) head_ = tail_;
        ++size_;
    }

    void delete_head() {
        if (!head_) return;
        DNode<T>* old = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr;
        else       tail_ = nullptr;
        delete old;
        --size_;
    }

    void delete_tail() {
        if (!tail_) return;
        DNode<T>* old = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr;
        else       head_ = nullptr;
        delete old;
        --size_;
    }

    DNode<T>* head() const { return head_; }
    DNode<T>* tail() const { return tail_; }
    size_t    size() const { return size_; }
    bool      empty()const { return size_ == 0; }

    void move_to_front(DNode<T>* node) {
        if (!node || node == head_) return;
        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
        if (node == tail_) tail_ = node->prev;
        node->prev = nullptr;
        node->next = head_;
        if (head_) head_->prev = node;
        head_ = node;
    }
};
