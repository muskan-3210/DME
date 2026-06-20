#include "graph_algo.h"
#include <unordered_set>
#include <stack>

std::vector<int> dfs(const Graph& g, int src) {
    std::vector<int> order;
    std::unordered_set<int> visited;
    std::stack<int> s;
    s.push(src);
    while (!s.empty()) {
        int v = s.top(); s.pop();
        if (visited.count(v)) continue;
        visited.insert(v);
        order.push_back(v);
        for (const auto& e : g.neighbors(v))
            if (!visited.count(e.to)) s.push(e.to);
    }
    return order;
}
