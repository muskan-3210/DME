#pragma once
#include <vector>
#include <unordered_map>
#include <limits>
#include <cstddef>

struct Edge {
    int to;
    int weight;
};

class Graph {
private:
    std::unordered_map<int, std::vector<Edge>> adj_;
    int    vertex_count_;
    bool   directed_;

public:
    explicit Graph(int vertices, bool directed = false)
        : vertex_count_(vertices), directed_(directed) {}

    void add_edge(int from, int to, int weight = 1) {
        adj_[from].push_back({to, weight});
        if (!directed_) adj_[to].push_back({from, weight});
    }

    const std::vector<Edge>& neighbors(int v) const {
        static const std::vector<Edge> empty;
        auto it = adj_.find(v);
        return it != adj_.end() ? it->second : empty;
    }

    int  vertex_count() const { return vertex_count_; }
    bool directed()     const { return directed_; }

    static constexpr int INF = std::numeric_limits<int>::max() / 2;
};
