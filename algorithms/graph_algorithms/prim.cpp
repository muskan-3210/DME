#include "graph_algo.h"
#include <queue>
#include <vector>
#include <unordered_set>

std::vector<std::pair<int,int>> prim(const Graph& g, int src) {
    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    std::unordered_set<int> in_mst;
    std::vector<std::pair<int,int>> mst;
    std::vector<int> parent(g.vertex_count(), -1);

    pq.push({0, src});
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_mst.count(u)) continue;
        in_mst.insert(u);
        if (parent[u] != -1) mst.push_back({parent[u], u});
        for (const auto& e : g.neighbors(u)) {
            if (!in_mst.count(e.to)) {
                parent[e.to] = u;
                pq.push({e.weight, e.to});
            }
        }
    }
    return mst;
}
