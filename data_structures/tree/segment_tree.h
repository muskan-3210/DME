#pragma once
#include <vector>
#include <functional>
#include <cstddef>
#include <stdexcept>

template<typename T>
class SegmentTree {
private:
    std::vector<T>        tree_;
    size_t                n_;
    std::function<T(T,T)> combine_;
    T                     identity_;

    void build(const std::vector<T>& arr, size_t node, size_t start, size_t end) {
        if (start == end) { tree_[node] = arr[start]; return; }
        size_t mid = (start + end) / 2;
        build(arr, 2*node, start, mid);
        build(arr, 2*node+1, mid+1, end);
        tree_[node] = combine_(tree_[2*node], tree_[2*node+1]);
    }

    T query(size_t node, size_t start, size_t end, size_t l, size_t r) {
        if (r < start || end < l) return identity_;
        if (l <= start && end <= r) return tree_[node];
        size_t mid = (start + end) / 2;
        return combine_(query(2*node, start, mid, l, r),
                        query(2*node+1, mid+1, end, l, r));
    }

    void update(size_t node, size_t start, size_t end, size_t idx, const T& val) {
        if (start == end) { tree_[node] = val; return; }
        size_t mid = (start + end) / 2;
        if (idx <= mid) update(2*node, start, mid, idx, val);
        else            update(2*node+1, mid+1, end, idx, val);
        tree_[node] = combine_(tree_[2*node], tree_[2*node+1]);
    }

public:
    SegmentTree(const std::vector<T>& arr, std::function<T(T,T)> combine, T identity)
        : n_(arr.size()), combine_(combine), identity_(identity) {
        tree_.resize(4 * n_);
        if (n_ > 0) build(arr, 1, 0, n_ - 1);
    }

    T query(size_t l, size_t r) {
        if (l > r || r >= n_) throw std::out_of_range("SegmentTree: range out of bounds");
        return query(1, 0, n_ - 1, l, r);
    }

    void update(size_t idx, const T& val) {
        if (idx >= n_) throw std::out_of_range("SegmentTree: index out of bounds");
        update(1, 0, n_ - 1, idx, val);
    }

    size_t size() const { return n_; }
};
