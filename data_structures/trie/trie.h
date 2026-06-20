#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <cstddef>

class Trie {
private:
    struct Node {
        std::unordered_map<char, Node*> children;
        bool is_end = false;
        ~Node() { for (auto& p : children) delete p.second; }
    };

    Node*  root_;
    size_t word_count_;

public:
    Trie() : root_(new Node()), word_count_(0) {}
    ~Trie() { delete root_; }

    void insert(const std::string& word) {
        Node* cur = root_;
        for (char c : word) {
            if (!cur->children.count(c))
                cur->children[c] = new Node();
            cur = cur->children[c];
        }
        if (!cur->is_end) { cur->is_end = true; ++word_count_; }
    }

    bool search(const std::string& word) const {
        Node* cur = root_;
        for (char c : word) {
            if (!cur->children.count(c)) return false;
            cur = cur->children.at(c);
        }
        return cur->is_end;
    }

    bool starts_with(const std::string& prefix) const {
        Node* cur = root_;
        for (char c : prefix) {
            if (!cur->children.count(c)) return false;
            cur = cur->children.at(c);
        }
        return true;
    }

    std::vector<std::string> autocomplete(const std::string& prefix) const {
        std::vector<std::string> results;
        Node* cur = root_;
        for (char c : prefix) {
            if (!cur->children.count(c)) return results;
            cur = cur->children.at(c);
        }
        collect(cur, prefix, results);
        return results;
    }

    size_t word_count() const { return word_count_; }

private:
    void collect(Node* node, const std::string& current, std::vector<std::string>& results) const {
        if (node->is_end) results.push_back(current);
        for (auto& p : node->children)
            collect(p.second, current + p.first, results);
    }
};
