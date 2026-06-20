#pragma once
#include <cstddef>
#include <algorithm>

template<typename T>
class AVLTree {
private:
    struct Node {
        T     data;
        Node* left;
        Node* right;
        int   height;
        explicit Node(const T& d) : data(d), left(nullptr), right(nullptr), height(1) {}
    };

    Node*  root_;
    size_t size_;

    int height(Node* n) const { return n ? n->height : 0; }
    int balance(Node* n) const { return n ? height(n->left) - height(n->right) : 0; }

    void update_height(Node* n) {
        if (n) n->height = 1 + std::max(height(n->left), height(n->right));
    }

    Node* rotate_right(Node* y) {
        Node* x = y->left; y->left = x->right; x->right = y;
        update_height(y); update_height(x); return x;
    }

    Node* rotate_left(Node* x) {
        Node* y = x->right; x->right = y->left; y->left = x;
        update_height(x); update_height(y); return y;
    }

    Node* rebalance(Node* n) {
        update_height(n);
        int b = balance(n);
        if (b > 1) {
            if (balance(n->left) < 0) n->left = rotate_left(n->left);
            return rotate_right(n);
        }
        if (b < -1) {
            if (balance(n->right) > 0) n->right = rotate_right(n->right);
            return rotate_left(n);
        }
        return n;
    }

    Node* insert(Node* n, const T& val) {
        if (!n) { ++size_; return new Node(val); }
        if (val < n->data)      n->left  = insert(n->left,  val);
        else if (val > n->data) n->right = insert(n->right, val);
        return rebalance(n);
    }

    bool search(Node* n, const T& val) const {
        if (!n) return false;
        if (val == n->data) return true;
        return val < n->data ? search(n->left, val) : search(n->right, val);
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }

public:
    AVLTree() : root_(nullptr), size_(0) {}
    ~AVLTree() { destroy(root_); }

    void insert(const T& val) { root_ = insert(root_, val); }
    bool search(const T& val) const { return search(root_, val); }
    size_t size()  const { return size_; }
    bool   empty() const { return size_ == 0; }
};
