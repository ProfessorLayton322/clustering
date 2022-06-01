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

    void AddEdge(int v, int u);

    void RemoveEdge(int v, int u);

    std::vector<int>& operator[](int v);

    const std::vector<int>& operator[](int v) const;

    bool Compare(Graph& other);

    void RemoveByTreshold(float treshold); 

    const std::vector<Point>& Points() const;

    const std::vector<std::vector<int>>& GetGraph() const;

private:
    std::vector<std::vector<int>> graph_;
    
    size_t verts_;

    std::vector<Point> points_;

    void SortLists();
};

Graph ReadGraph();
