#include "traffic_router.h"
#include <queue>
#include <limits>
#include <sstream>

namespace DME {

TrafficRouter::TrafficRouter(int num_nodes)
    : num_nodes_(num_nodes), adj_(num_nodes) {}

void TrafficRouter::add_link(int from, int to, int latency_ms) {
    adj_[from].push_back({to,   latency_ms});
    adj_[to].push_back  ({from, latency_ms});
}

Route TrafficRouter::find_best_route(int src, int dst) const {
    const int INF = std::numeric_limits<int>::max() / 2;
    std::vector<int> dist(num_nodes_, INF);
    std::vector<int> parent(num_nodes_, -1);
    dist[src] = 0;

    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (const auto& [v, w] : adj_[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v]   = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    auto path = reconstruct_path(parent, src, dst);
    std::ostringstream oss;
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) oss << " -> ";
        oss << "Node" << path[i];
    }

    return {src, dst, dist[dst] == INF ? -1 : dist[dst], oss.str()};
}

size_t TrafficRouter::node_count() const {
    return static_cast<size_t>(num_nodes_);
}

std::vector<int> TrafficRouter::reconstruct_path(
    const std::vector<int>& parent, int src, int dst) const {
    std::vector<int> path;
    for (int v = dst; v != -1; v = parent[v]) path.push_back(v);
    std::reverse(path.begin(), path.end());
    if (path.empty() || path[0] != src) return {};
    return path;
}

} // namespace DME
