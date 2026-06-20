#include "graph_algo.h"

std::vector<std::vector<int>> floyd_warshall(const Graph& g) {
    int n = g.vertex_count();
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, Graph::INF));
    for (int i = 0; i < n; ++i) dist[i][i] = 0;
    for (int u = 0; u < n; ++u)
        for (const auto& e : g.neighbors(u))
            dist[u][e.to] = std::min(dist[u][e.to], e.weight);
    for (int k=0;k<n;++k)
        for (int i=0;i<n;++i)
            for (int j=0;j<n;++j)
                if (dist[i][k]!=Graph::INF && dist[k][j]!=Graph::INF)
                    dist[i][j]=std::min(dist[i][j], dist[i][k]+dist[k][j]);
    return dist;
}
