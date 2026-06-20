#include "graph_algo.h"
#include <queue>
#include <unordered_set>

std::vector<int> bfs(const Graph& g, int src) {
    std::vector<int> order;
    std::unordered_set<int> visited;
    std::queue<int> q;
    q.push(src); visited.insert(src);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        order.push_back(v);
        for (const auto& e : g.neighbors(v)) {
            if (!visited.count(e.to)) {
                visited.insert(e.to);
                q.push(e.to);
            }
        }
    }
    return order;
}
