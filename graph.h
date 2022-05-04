#pragma once

#include <vector>
#include "point.h"

class Graph {
public:
    Graph(const std::vector<std::vector<size_t>>& graph);

    Graph(size_t n);

    Graph(const Graph& other);

    Graph(Graph&& other);

    float GetWeight(const std::vector<Point>& points) const;

    void AddEdge(size_t v, size_t u);

    std::vector<size_t>& operator[](size_t v);

    const std::vector<size_t>& operator[](size_t v) const;

    bool Compare(Graph& other);

private:
    std::vector<std::vector<size_t>> graph_;
    
    size_t verts_;
    
    void SortLists();
};
