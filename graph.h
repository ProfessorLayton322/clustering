#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

typedef std::vector<std::vector<size_t>> Graph;

void AddEdge(size_t v, size_t u, Graph& g);

#endif
