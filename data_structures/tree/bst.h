#pragma once
#include <cstddef>

template<typename T>
class BST {
private:
    struct Node {
        T     data;
        Node* left;
        Node* right;
        explicit Node(const T& d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node*  root_;
    size_t size_;

    Node* insert(Node* node, const T& val) {
        if (!node) { ++size_; return new Node(val); }
        if (val < node->data)      node->left  = insert(node->left,  val);
        else if (val > node->data) node->right = insert(node->right, val);
        return node;
    }

    bool search(Node* node, const T& val) const {
        if (!node) return false;
        if (val == node->data) return true;
        return val < node->data ? search(node->left, val) : search(node->right, val);
    }

    Node* min_node(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* remove(Node* node, const T& val) {
        if (!node) return nullptr;
        if (val < node->data) node->left  = remove(node->left,  val);
        else if (val > node->data) node->right = remove(node->right, val);
        else {
            if (!node->left)  { Node* r = node->right; delete node; --size_; return r; }
            if (!node->right) { Node* l = node->left;  delete node; --size_; return l; }
            Node* successor = min_node(node->right);
            node->data  = successor->data;
            node->right = remove(node->right, successor->data);
        }
        return node;
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() : root_(nullptr), size_(0) {}
    ~BST() { destroy(root_); }

    void insert(const T& val) { root_ = insert(root_, val); }
    bool search(const T& val) const { return search(root_, val); }
    void remove(const T& val) { root_ = remove(root_, val); }

    size_t size()  const { return size_; }
    bool   empty() const { return size_ == 0; }
};
