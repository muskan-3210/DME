#include "graph_algo.h"
#include "../../data_structures/graph/disjoint_set.h"
#include <vector>
#include <algorithm>

std::vector<std::pair<int,int>> kruskal(const Graph& g, int num_vertices) {
    struct WEdge { int u, v, w; };
    std::vector<WEdge> edges;
    for (int u = 0; u < num_vertices; ++u)
        for (const auto& e : g.neighbors(u))
            if (u < e.to) edges.push_back({u, e.to, e.weight});
    std::sort(edges.begin(), edges.end(), [](const WEdge& a, const WEdge& b){ return a.w < b.w; });

    DisjointSet ds(num_vertices);
    std::vector<std::pair<int,int>> mst;
    for (const auto& e : edges) {
        if (ds.unite(e.u, e.v)) mst.push_back({e.u, e.v});
        if ((int)mst.size() == num_vertices - 1) break;
    }
    return mst;
}
