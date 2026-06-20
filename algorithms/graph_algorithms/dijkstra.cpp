#include "graph_algo.h"
#include <queue>
#include <vector>

std::vector<int> dijkstra(const Graph& g, int src) {
    int n = g.vertex_count();
    std::vector<int> dist(n, Graph::INF);
    dist[src] = 0;
    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (const auto& e : g.neighbors(u)) {
            if (dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                pq.push({dist[e.to], e.to});
            }
        }
    }
    return dist;
}
