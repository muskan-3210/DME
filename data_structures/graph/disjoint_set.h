#pragma once
#include <vector>
#include <cstddef>

class DisjointSet {
private:
    std::vector<int>    parent_;
    std::vector<int>    rank_;
    size_t              components_;

public:
    explicit DisjointSet(size_t n)
        : parent_(n), rank_(n, 0), components_(n) {
        for (size_t i = 0; i < n; ++i) parent_[i] = static_cast<int>(i);
    }

    int find(int x) {
        if (parent_[x] != x) parent_[x] = find(parent_[x]); // path compression
        return parent_[x];
    }

    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank_[rx] < rank_[ry]) std::swap(rx, ry);
        parent_[ry] = rx;
        if (rank_[rx] == rank_[ry]) ++rank_[rx];
        --components_;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }
    size_t components() const { return components_; }
};
