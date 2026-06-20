#pragma once
#include <cstddef>
#include <stdexcept>

// --- Node ---

template<typename T>
struct Node {
    T     data;
    Node* next;

    explicit Node(const T& value)
        : data(value), next(nullptr) {}
};

// --- SinglyLinkedList ---

template<typename T>
class SinglyLinkedList {
private:
    Node<T>* head_;
    Node<T>* tail_;
    size_t   size_;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();

    void insert_head(const T& value);
    void insert_tail(const T& value);

    void delete_head();
    void delete_tail();
    bool delete_by_value(const T& value);

    bool   search(const T& value) const;
    size_t size()                 const;
    bool   empty()                const;
};

// --- Constructor ---

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList()
    : head_(nullptr), tail_(nullptr), size_(0) {}

// --- Destructor ---

template<typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    Node<T>* current = head_;
    while (current != nullptr) {
        Node<T>* next = current->next;
        delete current;
        current = next;
    }
}

// --- Insert Head ---

template<typename T>
void SinglyLinkedList<T>::insert_head(const T& value) {
    Node<T>* new_node = new Node<T>(value);
    new_node->next    = head_;
    head_             = new_node;
    if (tail_ == nullptr)
        tail_ = head_;
    ++size_;
}

// --- Insert Tail ---

template<typename T>
void SinglyLinkedList<T>::insert_tail(const T& value) {
    Node<T>* new_node = new Node<T>(value);
    if (tail_ == nullptr) {
        head_ = tail_ = new_node;
    } else {
        tail_->next = new_node;
        tail_       = new_node;
    }
    ++size_;
}

// --- Delete Head ---

template<typename T>
void SinglyLinkedList<T>::delete_head() {
    if (head_ == nullptr) return;
    Node<T>* to_delete = head_;
    head_              = head_->next;
    if (head_ == nullptr)
        tail_ = nullptr;
    delete to_delete;
    --size_;
}

// --- Delete Tail ---

template<typename T>
void SinglyLinkedList<T>::delete_tail() {
    if (head_ == nullptr) return;

    if (head_ == tail_) {
        delete head_;
        head_ = tail_ = nullptr;
        --size_;
        return;
    }

    Node<T>* prev    = nullptr;
    Node<T>* current = head_;
    while (current->next != nullptr) {
        prev    = current;
        current = current->next;
    }

    prev->next = nullptr;
    tail_      = prev;
    delete current;
    --size_;
}

// --- Delete By Value ---

template<typename T>
bool SinglyLinkedList<T>::delete_by_value(const T& value) {
    if (head_ == nullptr) return false;

    if (head_->data == value) {
        delete_head();
        return true;
    }

    Node<T>* prev    = nullptr;
    Node<T>* current = head_;

    while (current != nullptr && current->data != value) {
        prev    = current;
        current = current->next;
    }

    if (current == nullptr) return false;

    if (current == tail_)
        tail_ = prev;

    prev->next = current->next;
    delete current;
    --size_;
    return true;
}

// --- Search ---

template<typename T>
bool SinglyLinkedList<T>::search(const T& value) const {
    Node<T>* current = head_;
    while (current != nullptr) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

// --- Queries ---

template<typename T>
size_t SinglyLinkedList<T>::size() const { return size_; }

template<typename T>
bool SinglyLinkedList<T>::empty() const { return size_ == 0; }
