#include "graph.h"

void AddEdge(size_t v, size_t u, Graph& g) {
    g[v].push_back(u);
    g[u].push_back(v);
}
