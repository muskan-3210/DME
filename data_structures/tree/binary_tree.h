#pragma once
#include <cstddef>
#include <functional>
#include <queue>

template<typename T>
class BinaryTree {
public:
    struct Node {
        T     data;
        Node* left;
        Node* right;
        explicit Node(const T& d) : data(d), left(nullptr), right(nullptr) {}
    };

private:
    Node*  root_;
    size_t size_;

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }

public:
    BinaryTree() : root_(nullptr), size_(0) {}
    ~BinaryTree() { destroy(root_); }

    Node* root() const { return root_; }

    Node* insert_bfs(const T& val) {
        Node* n = new Node(val); ++size_;
        if (!root_) { root_ = n; return n; }
        std::queue<Node*> q;
        q.push(root_);
        while (!q.empty()) {
            Node* cur = q.front(); q.pop();
            if (!cur->left)  { cur->left  = n; return n; }
            if (!cur->right) { cur->right = n; return n; }
            q.push(cur->left); q.push(cur->right);
        }
        return n;
    }

    void inorder(Node* n, std::function<void(const T&)> fn) const {
        if (!n) return;
        inorder(n->left, fn); fn(n->data); inorder(n->right, fn);
    }

    void preorder(Node* n, std::function<void(const T&)> fn) const {
        if (!n) return;
        fn(n->data); preorder(n->left, fn); preorder(n->right, fn);
    }

    size_t size()  const { return size_; }
    bool   empty() const { return size_ == 0; }
};
