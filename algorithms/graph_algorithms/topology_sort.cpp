#include "graph_algo.h"
#include <stack>
#include <unordered_set>
#include <vector>

static void dfs_topo(const Graph& g, int v, std::unordered_set<int>& visited, std::stack<int>& stk) {
    visited.insert(v);
    for (const auto& e : g.neighbors(v))
        if (!visited.count(e.to)) dfs_topo(g, e.to, visited, stk);
    stk.push(v);
}

std::vector<int> topological_sort(const Graph& g) {
    std::unordered_set<int> visited;
    std::stack<int> stk;
    for (int v = 0; v < g.vertex_count(); ++v)
        if (!visited.count(v)) dfs_topo(g, v, visited, stk);
    std::vector<int> order;
    while (!stk.empty()) { order.push_back(stk.top()); stk.pop(); }
    return order;
}
