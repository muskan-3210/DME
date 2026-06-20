#pragma once
#include "../../data_structures/graph/graph.h"
#include <vector>
#include <unordered_map>

std::vector<int> bfs            (const Graph& g, int src);
std::vector<int> dfs            (const Graph& g, int src);
std::vector<int> dijkstra       (const Graph& g, int src);
std::vector<int> bellman_ford   (const Graph& g, int src, bool& has_neg_cycle);
std::vector<std::vector<int>> floyd_warshall(const Graph& g);
std::vector<std::pair<int,int>> kruskal (const Graph& g, int num_vertices);
std::vector<std::pair<int,int>> prim    (const Graph& g, int src);
std::vector<int> topological_sort(const Graph& g);
