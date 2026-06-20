#pragma once
#include <cstddef>

// Simplified Red-Black Tree — insert + search (no delete for clarity)
template<typename T>
class RedBlackTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        T      data;
        Color  color;
        Node*  left;
        Node*  right;
        Node*  parent;
        Node(const T& d, Color c)
            : data(d), color(c), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node*  root_;
    size_t size_;
    Node*  NIL_;

    void left_rotate(Node* x) {
        Node* y  = x->right;
        x->right = y->left;
        if (y->left != NIL_) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent)          root_      = y;
        else if (x == x->parent->left) x->parent->left  = y;
        else                           x->parent->right = y;
        y->left   = x;
        x->parent = y;
    }

    void right_rotate(Node* y) {
        Node* x  = y->left;
        y->left  = x->right;
        if (x->right != NIL_) x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent)          root_      = x;
        else if (y == y->parent->left) y->parent->left  = x;
        else                           y->parent->right = x;
        x->right  = y;
        y->parent = x;
    }

    void fix_insert(Node* z) {
        while (z->parent && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color          = BLACK;
                    y->color                  = BLACK;
                    z->parent->parent->color  = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) { z = z->parent; left_rotate(z); }
                    z->parent->color         = BLACK;
                    z->parent->parent->color = RED;
                    right_rotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color          = BLACK;
                    y->color                  = BLACK;
                    z->parent->parent->color  = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) { z = z->parent; right_rotate(z); }
                    z->parent->color         = BLACK;
                    z->parent->parent->color = RED;
                    left_rotate(z->parent->parent);
                }
            }
        }
        root_->color = BLACK;
    }

    void destroy(Node* n) {
        if (!n || n == NIL_) return;
        destroy(n->left); destroy(n->right); delete n;
    }

public:
    RedBlackTree() : root_(nullptr), size_(0) {
        NIL_        = new Node(T{}, BLACK);
        root_       = NIL_;
    }
    ~RedBlackTree() { destroy(root_); delete NIL_; }

    void insert(const T& val) {
        Node* z  = new Node(val, RED);
        z->left  = NIL_;
        z->right = NIL_;
        Node* y  = nullptr;
        Node* x  = root_;
        while (x != NIL_) {
            y = x;
            x = (val < x->data) ? x->left : x->right;
        }
        z->parent = y;
        if (!y)               root_     = z;
        else if (val < y->data) y->left   = z;
        else                    y->right  = z;
        ++size_;
        fix_insert(z);
    }

    bool search(const T& val) const {
        Node* x = root_;
        while (x != NIL_) {
            if (val == x->data) return true;
            x = (val < x->data) ? x->left : x->right;
        }
        return false;
    }

    size_t size()  const { return size_; }
    bool   empty() const { return size_ == 0; }
};
