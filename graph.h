#pragma once

#include <vector>
#include "point.h"

class Graph {
public:
    Graph(const std::vector<Point>& points);

    Graph(std::vector<Point>&& points);

    Graph(const Graph& other);

    Graph(Graph&& other);

    float GetWeight() const;

    int CountEdges() const;

    void AddEdge(size_t v, size_t u);

    void RemoveEdge(size_t v, size_t u);

    std::vector<size_t>& operator[](size_t v);

    const std::vector<size_t>& operator[](size_t v) const;

    bool Compare(Graph& other);

    void RemoveByTreshold(float treshold); 

    const std::vector<Point>& Points() const;

private:
    std::vector<std::vector<size_t>> graph_;
    
    size_t verts_;

    std::vector<Point> points_;

    void SortLists();

};
