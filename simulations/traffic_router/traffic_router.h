#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace DME {

struct Route {
    int         src;
    int         dst;
    int         latency_ms;
    std::string path_description;
};

class TrafficRouter {
public:
    TrafficRouter(int num_nodes);
    void add_link(int from, int to, int latency_ms);
    Route find_best_route(int src, int dst) const;
    size_t node_count() const;

private:
    int                                              num_nodes_;
    std::vector<std::vector<std::pair<int,int>>>     adj_;   // adj[u] = {v, latency}

    std::vector<int> dijkstra_latency(int src) const;
    std::vector<int> reconstruct_path(const std::vector<int>& parent, int src, int dst) const;
};

} // namespace DME
