#pragma once
#include <vector>
#include <cstddef>

class FenwickTree {
private:
    std::vector<long long> tree_;
    size_t                 n_;

public:
    explicit FenwickTree(size_t n) : tree_(n + 1, 0LL), n_(n) {}

    void update(size_t i, long long delta) {
        for (++i; i <= n_; i += i & (-static_cast<long long>(i)))
            tree_[i] += delta;
    }

    long long prefix_sum(size_t i) const {
        long long sum = 0;
        for (++i; i > 0; i -= i & (-static_cast<long long>(i)))
            sum += tree_[i];
        return sum;
    }

    long long range_sum(size_t l, size_t r) const {
        return prefix_sum(r) - (l > 0 ? prefix_sum(l - 1) : 0LL);
    }

    size_t size() const { return n_; }
};
