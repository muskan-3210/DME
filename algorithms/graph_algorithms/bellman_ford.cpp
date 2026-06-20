#include "graph_algo.h"

std::vector<int> bellman_ford(const Graph& g, int src, bool& has_neg_cycle) {
    int n = g.vertex_count();
    std::vector<int> dist(n, Graph::INF);
    dist[src] = 0;

    for (int iter = 0; iter < n - 1; ++iter)
        for (int u = 0; u < n; ++u)
            if (dist[u] != Graph::INF)
                for (const auto& e : g.neighbors(u))
                    if (dist[u] + e.weight < dist[e.to])
                        dist[e.to] = dist[u] + e.weight;

    has_neg_cycle = false;
    for (int u = 0; u < n; ++u)
        if (dist[u] != Graph::INF)
            for (const auto& e : g.neighbors(u))
                if (dist[u] + e.weight < dist[e.to])
                    has_neg_cycle = true;

    return dist;
}
